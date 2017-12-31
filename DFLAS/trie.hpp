//
//  trie.hpp
//  DFLAS
//
//  Created by Yang Yunfei on 2017/12/13.
//  Copyright © 2017年 Yang Yunfei. All rights reserved.
//

#ifndef trie_hpp
#define trie_hpp

#include <cassert>
#include <algorithm>
#include <map>
#include <memory>
#include <string>
#include <vector>

//An implementation of Trie 
class Trie {
    struct trie_node_t_ {
        std::map<char, trie_node_t_> children;
        bool is_word = false;
    };
    trie_node_t_ root_;
public:
    Trie()  = default;
    ~Trie() = default;
    
    void insert(const std::string& word) {
        auto first = &root_;
        for (char c : word) {
            first = &first->children[c]; // create or pull the next node
        }
        first->is_word = true;
    }
    
    bool find(const std::string& word) const {
        auto first = &root_;
        for (char c : word) {
            auto found = first->children.find(c);
            
            if (found == std::end(first->children)) {
                return false;
            }
            first = &found->second;
        }
        
        return first->is_word;
    }
    
    bool prefix_match(const std::string& prefix, std::string& matching_word) const {
        // first we need to find where this node is
        auto first = &root_;
        for (char c : prefix) {
            auto found = first->children.find(c);
            
            if (found == std::end(first->children)) {
                return false;
            }
            first = &found->second;
        }
        
        // matching word is at least our prefix
        matching_word = prefix;
        
        if (first->is_word) {
            return true;
        }
        
        // get the first child and depth first search find a word node
        while (!first->is_word) {
            if (first->children.empty()) {
                assert(false && "prog error");
                return false;
            }
            auto next = std::begin(first->children);
            first = &next->second;
            matching_word += next->first;
        }
        
        return true;
    }
    
    std::vector<std::string> get_words() const {
        std::vector<std::string> ret;
        
        for (const auto& child : root_.children) {
            get_words_impl_(ret, std::string(1, child.first), child.second);
        }
        
        return ret;
    }
    
private:
    void get_words_impl_(std::vector<std::string>& words, std::string prefix, const trie_node_t_& node) const {
        if (node.is_word) words.push_back(prefix);
        if (node.children.empty()) return;
        
        for (const auto& child : node.children) {
            get_words_impl_(words, prefix + child.first, child.second);
        }
    }
};


#endif /* trie_hpp */
