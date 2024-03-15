#ifndef TRIE_H
#define TRIE_H

#include <memory>
#include <optional>
#include <string_view>
#include <iterator>

#include <iostream>

template<typename T>
class Trie
{
public:
    Trie() : _value(std::nullopt) {}
    ~Trie() = default;

    class Iterator {
    public:
        Iterator(Trie<T> *root) : _stack({root}) {}
        ~Iterator() = default;

        T* next()
        {
            if (_stack.empty())
            {
                return nullptr;
            }

            Trie <T> *current;
            do {
                current = _stack.back();
                _stack.pop_back();
                for (auto &child : current->_children)
                {
                    if (child)
                    {
                        _stack.push_back(child.get());
                    }
                }
            } while (!current->_value.has_value() && !_stack.empty());

            return current->_value.has_value() ? &current->_value.value() : nullptr;
        }

    private:
        std::vector<Trie<T>*> _stack;
    };

    std::optional<Iterator> prefix_iterator(std::string_view prefix)
    {
        Trie<T> *current = this;

        while (prefix.length() > 0)
        {
            std::size_t index = prefix[0] - 'a';
            if (current->_children[index] == nullptr)
            {
                return std::nullopt;
            }
            current = current->_children[index].get();
            prefix = prefix.substr(1);
        }

        return Iterator(current);
    }

    T *find(std::string_view key)
    {
        Trie<T> *current = this;

        while (key.length() > 0)
        {
            std::size_t index = key[0] - 'a';
            if (current->_children[index] == nullptr)
            {
                return nullptr;
            }
            current = current->_children[index].get();
            key = key.substr(1);
        }

        if (current->_value.has_value())
        {
            return &current->_value.value();
        }
        return nullptr;
    }

    T *find_or_reserve(std::string_view key)
    {
        Trie<T> *current = this;

        while (key.length() > 0)
        {
            std::size_t index = key[0] - 'a';
            if (current->_children[index] == nullptr)
            {
                current->_children[index] = std::make_unique<Trie<T>>();
            }
            current = current->_children[index].get();
            key = key.substr(1);
        }

        current->_value = T{};
        return &current->_value.value();
    }

private:
    std::optional<T> _value;
    std::unique_ptr<Trie<T>> _children[26];
};

#endif
