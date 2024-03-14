#include "Indexer.h"
#include <Tokenizer.h>
#include <unordered_set>
#include <cmath>

void Indexer::index(std::size_t file_id, const std::string &text)
{
    Tokenizer tokenizer(text);
    std::unordered_set<std::string> local_df;
    std::size_t count = 0;

    while (auto token = tokenizer.next())
    {
        _tf[token.value()][file_id]++;
        if (local_df.find(token.value()) == local_df.end())
        {
            _df[token.value()]++;
            local_df.insert(token.value());
        }

        count++;
    }

    _tc[file_id] = count;
}

void Indexer::normalize(std::size_t file_count)
{
    const float base = 1.3f;

    for (auto &df : _df)
    {
        df.second = std::log(file_count / df.second);
    }

    for (auto &t : _tf)
    {
        for (auto &f: t.second)
        {
            f.second = 1.0f - std::pow(base, -_df[t.first] * f.second / _tc[f.first]);
        }
    }
}

void Indexer::emit(std::ostream &os, const StringBuffer &sb) const
{
    os << sb.count() << "\n";
    for (std::size_t i = 0; i < sb.count(); i++)
    {
        os << sb.get(i) << "\n";
    }

    for (auto &t : _tf)
    {
        os << t.first;
        for (auto &f: t.second)
        {
            const unsigned short fi = (unsigned short) (f.second * ((2 << sizeof (fi) * 8) - 1));
            if (fi == 0)
            {
                continue;
            }
            os << " " << f.first << " " << fi;
        }
        os << "\n";
    }
}
