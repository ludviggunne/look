#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <algorithm>
#include <unordered_set>
#include "walkdir.h"
#include "Indexer.h"
#include "Trie.h"

void index(const char *root)
{
    StringBuffer sb;
    std::cout << "Enumerating files...\n";
    walkdir(root, sb);

    Indexer indexer;
    for (std::size_t i = 0; i < sb.count(); i++)
    {
        auto name = sb.get(i);
        auto percent = (int) ((float) (i * 100) / (float) sb.count());
        std::cout << "[" << percent << "%] Indexing " << name << std::endl;
        std::ifstream ifs(name);
        std::string content((std::istreambuf_iterator<char>(ifs)),
                            (std::istreambuf_iterator<char>()));
        indexer.index(i, content);
    }

    std::cout << "Normalizing..." << std::endl;
    indexer.normalize(sb.count());

    std::cout << "Emitting..." << std::endl;
    std::ofstream ofs("index.txt");
    indexer.emit(ofs, sb);
}

struct Data {
    std::unique_ptr<Trie<std::unordered_map<std::size_t, unsigned short>>> index;
    std::vector<std::string> files;
};

Data load_data()
{
    if (!std::filesystem::exists("index.txt"))
    {
        std::cout << "No index.txt found: run ./look index <root>" << std::endl;
        std::exit(1);
    }

    std::ifstream ifs("index.txt");
    std::string line;

    std::getline(ifs, line);
    std::stringstream ss(line);
    std::size_t count;
    ss >> count;

    std::vector<std::string> files;
    for (std::size_t i = 0; i < count; i++)
    {
        std::getline(ifs, line);
        files.push_back(line);
    }

    auto index = std::make_unique<Trie<std::unordered_map<std::size_t, unsigned short>>>();

    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        std::string token;
        std::size_t file_id;
        unsigned short freq;

        ss >> token;

        auto *t = index->find_or_reserve(token);

        while (ss >> file_id)
        {
            ss >> freq;

            if (freq > 0)
            {
                (*t)[file_id] = freq;
            }
        }
    }

    return { std::move(index), std::move(files) };
}

void single(std::string query, const Data &data)
{
    std::transform(query.begin(), query.end(), query.begin(),
        [](unsigned char c){ return std::tolower(c); });

    std::vector<std::pair<std::size_t, unsigned short>> results;
    std::unordered_set<std::size_t> added_files;

    auto it = data.index->prefix_iterator(query);

    if (it)
    {
        auto *t = it.value().next();
        while (t)
        {
            for (auto &f: *t)
            {
                if (added_files.find(f.first) != added_files.end())
                {
                    continue;
                }
                results.push_back(std::make_pair(f.second, f.first));
                added_files.insert(f.first);
            }

            t = it.value().next();
        }

        std::sort(results.begin(), results.end(), std::greater<>());
        std::cout << results.size() << std::endl;
        for (auto &r: results)
        {
            std::cout << data.files[r.second] << std::endl;
        }
    }
    else
    {
        std::cout << "0" << std::endl;
    }
}

void server(const Data &data) {

    std::cout << "ready\n" << std::flush;
    while (true)
    {
        std::string query;
        std::cin >> query;
        if (query.empty())
        {
            break;
        }

        single(query, data);
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    if (argc < 2)
    {
        auto data = load_data();
        server(data);
    }
    else
    {
        if (strcmp(argv[1], "--index") == 0 || strcmp(argv[1], "-i") == 0)
        {
            if (argc < 3)
            {
                std::cout << "ERROR: Missing root directory\n" << std::endl;
                std::exit(1);
            }
            if (!std::filesystem::exists(argv[2]))
            {
                std::cout << "ERROR: Root directory not found: " << argv[2] << std::endl;
                std::exit(1);
            }
            index(argv[2]);
        }
        else if (strcmp(argv[1], "--query") == 0 || strcmp(argv[1], "-q") == 0)
        {
            if (argc < 3)
            {
                std::cout << "ERROR: Missing single query\n" << std::endl;
                std::exit(1);
            }
            auto data = load_data();
            single(argv[2], data);
        }
        else
        {
            std::cout << "ERROR: Unknown command: " << argv[1] << std::endl;
            std::exit(1);
        }
    }

    return 0;
}
