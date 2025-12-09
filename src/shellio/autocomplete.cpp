/**
 * @file autocomplete.cpp
 * @brief [Brief description of the file]
 *
 * Detailed description of the file purpose and contents.
 *
 * @date 08/12/2025 A
 * @author princesharma
 */
 #include "autocomplete.h"

namespace autocomplete {
    TrieNode::~TrieNode() {
        for(const auto &node: next) {
            delete node;
        }
    }

    void Autocomplete::insert(const std::string &s) const {
        TrieNode *cur = root;
        for (const unsigned char c : s) {
            if (cur->next[c] == nullptr) cur->next[c] = new TrieNode();
            cur = cur->next[c];
        }
        cur->end = true;
        cur->word = s;
    }

    std::vector<std::string> Autocomplete::suggest(const std::string &prefix) const {
        TrieNode *cur = root;
        for (const unsigned char c : prefix) {
            if (cur->next[c] == nullptr) return {};
            cur = cur->next[c];
        }

        std::vector<std::string> results;
        std::function<void(TrieNode*)> dfs = [&](TrieNode *node) {
            if (!node) return;
            if (node->end) results.emplace_back(node->word);
            for (const auto &p : node->next) dfs(p);
        };
        dfs(cur);

        std::ranges::sort(results);
        return results;
    }
}
