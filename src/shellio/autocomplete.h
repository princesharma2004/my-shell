/**
 * @file autocomplete.h
 * @brief [Brief description of the file]
 *
 * Detailed description of the file purpose and contents.
 *
 * @date 08/12/2025 A
 * @author princesharma
 */
 #ifndef AUTOCOMPLETE_H
#define AUTOCOMPLETE_H

#include <unordered_map>
#include <string>
#include <array>
#include <functional>
#include <algorithm>
#include <ranges>
#include <vector>

namespace  autocomplete {
    struct TrieNode {
        bool end = false;
        std::array<TrieNode*, 256> next{};
        std::string word;

        ~TrieNode();
    };

    class Autocomplete {
    public:
        Autocomplete() : root(new TrieNode()) {}
        Autocomplete(const Autocomplete&) = delete;
        Autocomplete& operator=(const Autocomplete&) = delete;
        ~Autocomplete() { delete root; }

        void insert(const std::string &s) const;
        [[nodiscard]] std::vector<std::string> suggest(const std::string &prefix) const;
    private:
        TrieNode *root;
    };
}



#endif //AUTOCOMPLETE_H
