#include "Trie.h"
#include "utest.h"

UTEST(Trie,)
{
    Trie<int> trie;

    *trie.find_or_reserve("hello") = 0;
    *trie.find_or_reserve("hel") = 1;

    ASSERT_EQ(0, *trie.find("hello"));
    ASSERT_EQ(1, *trie.find("hel"));
    ASSERT_FALSE(!!trie.find("hello!"));
    ASSERT_TRUE(!!trie.find("hello"));
    ASSERT_FALSE(!!trie.find("hell"));
    ASSERT_TRUE(!!trie.find("hel"));
    ASSERT_FALSE(!!trie.find("h"));
}
