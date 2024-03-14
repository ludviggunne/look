#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <unordered_map>
#include <iostream>

// DEBUG =========
#include <iostream>
#include "StringBuffer.h"

class Indexer
{
public:
    Indexer() = default;
    void index(std::size_t file_id, const std::string &text);
    void normalize(std::size_t file_count);
    void emit(std::ostream &os, const StringBuffer &sb) const;

private:
    std::unordered_map<std::string,
        std::unordered_map<std::size_t, float>> _tf;
    std::unordered_map<std::string, float> _df;
    std::unordered_map<std::size_t, std::size_t> _tc;
};

#endif
