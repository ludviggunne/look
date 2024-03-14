#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <optional>

class Tokenizer
{
public:
    Tokenizer(const std::string &text) : _cursor(text.begin()),
        _end(text.end()) {}
    std::optional<std::string> next();

private:
    std::string::const_iterator _cursor;
    std::string::const_iterator _end;
};

#endif
