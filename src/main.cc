#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstring>
#include <algorithm>
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

void search() {
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

    Trie<std::unordered_map<std::size_t, unsigned short>> freqs;

    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        std::string token;
        std::size_t file_id;
        unsigned short freq;

        ss >> token;

        auto *t = freqs.find_or_reserve(token);

        while (ss >> file_id)
        {
            ss >> freq;

            if (freq > 0)
            {
                (*t)[file_id] = freq;
            }
        }
    }

    std::cout << "ready\n" << std::flush;
    while (true)
    {
        std::string query;
        std::cin >> query;
        if (query.empty())
        {
            break;
        }

        std::transform(query.begin(), query.end(), query.begin(),
            [](unsigned char c){ return std::tolower(c); });

        std::vector<std::pair<std::size_t, unsigned short>> results;
        auto it = freqs.prefix_iterator(query);

        if (it)
        {
            auto *t = it.value().next();
            while (t)
            {
                for (auto &f: *t)
                {
                    results.push_back(std::make_pair(f.second, f.first));
                }

                t = it.value().next();
            }

            std::sort(results.begin(), results.end(), std::greater<>());
            std::cout << results.size() << std::endl;
            for (auto &r: results)
            {
                std::cout << files[r.second] << std::endl;
            }
        }
        else
        {
            std::cout << "0" << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    (void) argc; (void) argv;

    if (argc < 2)
    {
        if (std::filesystem::exists("index.txt"))
        {
            search();
        }
        else
        {
            std::cout << "ERROR: No index.txt found\n" << std::endl;
            std::exit(1);
        }
    }
    else
    {
        if (strcmp(argv[1], "index") == 0)
        {
            if (argc < 3)
            {
                std::cout << "ERROR: Missing root directory\n" << std::endl;
                std::exit(1);
            }
            index(argv[2]);
        }
        else
        {
            std::cout << "ERROR: Invalid argument " << argv[1] << "\n" << std::endl;
            std::exit(1);
        }
    }

    return 0;
}
