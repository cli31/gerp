/*
 * hashTable.h
 * Cheng Li and Andrej Djokic
 */

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <iostream>
#include <functional>
#include <cctype>

#include "Word.h"


class hashTable {
public:
    hashTable();
    ~hashTable();
    void push(const std::string wordString,
              const std::pair<File*, int> vertex);
    std::vector<Word> get(const std::string &wordString, 
                          const bool &if_sensitive);
private:
    size_t capacity;
    size_t keyNum;
    std::vector<Word> *word_map; // insensitive
    std::vector<std::pair<std::string, size_t>> *map_reference;

    std::string insensitize(std::string _wordString);
    void expand();
    void populate(std::vector<Word> bucket);
};

#endif