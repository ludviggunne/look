#include "StringBuffer.h"

std::size_t StringBuffer::append(const char *str)
{
    std::size_t offset = _buffer.size();
    std::size_t index = _offsets.size();
    _offsets.push_back(offset);

    while (*str)
    {
        _buffer.push_back(*str++);
    }
    _buffer.push_back('\0');

    return index;
}

const char *StringBuffer::get(std::size_t index) const
{
    return &_buffer[_offsets[index]];
}

std::size_t StringBuffer::count() const
{
    return _offsets.size();
}
