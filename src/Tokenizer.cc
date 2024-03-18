#include <cctype>
#include "Tokenizer.h"

std::optional<std::string> Tokenizer::next()
{
    if (_cursor == _end)
    {
        return std::nullopt;
    }

    while (!std::isalpha(*_cursor))
    {
        _cursor++;
        if (_cursor == _end)
        {
            return std::nullopt;
        }
    }

    auto start = _cursor;

    while (_cursor != _end && std::isalpha(*_cursor))
    {
        _cursor++;
    }

    auto token = std::string(start, _cursor);
    for (auto &c : token)
    {
        c = std::tolower(c);
    }

    return token;
}
