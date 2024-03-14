#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#include <cstddef>
#include <vector>
#include <string>

class StringBuffer
{
public:
    StringBuffer() = default;
    StringBuffer(const StringBuffer &) = delete;
    StringBuffer &operator=(const StringBuffer &) = delete;

    std::size_t append(const char *str);
    const char *get(std::size_t index) const;
    std::size_t count() const;

private:
    std::vector<char> _buffer;
    std::vector<std::size_t> _offsets;
};

#endif
