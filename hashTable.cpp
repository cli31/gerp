/*
 * hashTable.cpp
 * Cheng Li and Andrej Djokic
 * 
 */

#include "hashTable.h"

/*
 * constructor
 * 
 * Explanation of member variables:
 * word_map is the map of words. It has 3D data structure, e.g.:
 * vector:              We          are         the         ...         Wordn
 * in each bucket:      We          we          wE          WE
 * with vertices:  fileA, line1 fileB, line2    ...
 * 
 * map_reference is to record the real hash_value of some string when the
 * hash function gives the same hash value and caused collision, e.g.:
 *      we: hashed to 1th
 *      are: hashed also 1th, but we took the bucket and bucket 2, 3 also taken,
 *           so it ends up to 4th
 * when the client is going to look for 'are', it will receive hash value 1,
 * however by looking for map_reference, the real bucket of 'are' can be located
 */
hashTable::hashTable() : capacity(1024), keyNum(0) {
    // start with setting the table size to 2^10
    word_map = new std::vector<Word>[capacity];
    map_reference = new std::vector<std::pair<std::string, size_t>>[capacity];
}

/*
 * destructor
 * 
 */
hashTable::~hashTable() {
    delete word_map;
    delete map_reference;
}

/*
 * push (the public method, pack hashing process under the hood)
 * parameter: a word object by value 
 *            the original object would be delete when the loop reach its end
 * return: void
 * explanation: when collision happened, first search through reference to see
 *              if there is a bucket for this word, if none, search forward to
 *              find a empty bucket, update hash info in map_reference
 */
void hashTable::push(const Word w) {
    // step1: check if need to expand the table
    if (capacity - keyNum < 2)
        expand();
    // step2: insensitive the wordString
    std::string key = insensitize(w.wordString);
    // step3: get the temporary hash_value
    size_t hash_value = std::hash<std::string>{}(key) % capacity;
    // step4: push into correct bucket
    // case1: check if bucket is empty -> if it is, just fill it in
    if (word_map[hash_value].empty()) {
        word_map[hash_value].push_back(w);
        // update reference
        map_reference[hash_value].push_back(std::make_pair(key, hash_value));
        keyNum++;
    }
    else { // case2: if not empty
        // check if it appeared before (iterate through the reference)
        for (const auto &itr : map_reference[hash_value]) {
            if (key == itr.first) {
                // in that bucket, iterate through to examine if
                // case I: sensitive string appeared before
                for (Word &i : word_map[itr.second]) {
                    if (w.wordString == i.wordString) {
                        i.add(w.vertex.at(0));
                        return; // finished add
                    }
                }
                // case II: did not find sensitive string -> insensitive case
                word_map[itr.second].push_back(w);
                return; // finish add
                // no need to increment keyNum since no new bucket occupied
            }
        }
        // if return before never executed, then this key was never recorded
        hash_value++; // skip this bucket
        // march forward until find a new empty bucket
        while (not word_map[hash_value].empty()) {
            hash_value++;
        }
        // repeat case1
        // fill in the empty bucket, update reference, and increment keyNum
        word_map[hash_value].push_back(w);
        map_reference[hash_value].push_back(std::make_pair(key, hash_value));
        keyNum++;
    }
}

/*
 * get
 * parameter: a string of the word to retrieve (assume it has been stripped)
 *            a bool indicates if looking for a sensitive wordstring
 * return: a vector of Word that has the same wordstring as input
 *         if it is sensitive, only return the elem in the bucket that has the
 *         exact string, which means the returned vector has elem num 1
 *         else if it is not sensitive, return the whole bucket
 *         if did not find, return null
 * purpose: to find a word in the hashTable
 */
std::vector<Word> hashTable::get(const std::string wordString, 
                                 const bool if_sensitive) {
    // step1: transform the wordString to key so that can find the right bucket
    std::string key = insensitize(wordString);
    size_t hash_v = std::hash<std::string>{}(key) % capacity;
    // step2: search in map_reference
    for (const auto &itr :  map_reference[hash_v]) {
        // if found
        if (key == itr.first) {
            // update hash_value
            hash_v = itr.second;
            // go to the right bucket
            // if case sensitive -> need to find the exact same elem
            if (if_sensitive) {
                // the vector will only have one elems
                for (const auto &i : word_map[hash_v]) {
                    if (i.wordString == wordString) {
                        return std::vector<Word>{i};
                    }
                }
            }  
            else { // if case not sensitive -> return everything in this bucket
                return word_map[hash_v];
            }
        }
    }
    // if not found
    return std::vector<Word>{}; // query no found, return empty Word
}

/*
 * insensitize
 * parameter: the wordString in a word
 * return: the 'insensitized' wordString
 * purpose: in the hashTable, case-insensitive wordStrings are grouped together
 *          e.g. we and We are put in the same bucket
 *          therefore, to get the same hash_value, must 'unify' them
 *          by turning every char into lower case
 *          it's unfortunately O(n) but cannot come up with a better method
 */
std::string hashTable::insensitize(std::string _wordString) {
    std::string r;
    for (const char &itr : _wordString) {
        r += tolower(itr);
    }
    return r;
}

/*
 * expand
 * parameter: none
 * return: void
 * purpose: to expand the hashTable
 *          by create a new map with larger capacity, recalculate the hash of
 *          each word and add them one by one
 * 
 * N.P: this expansion is inefficient (I just iterate through each Word and 
 *      push them to the new map), feel free to change it!
 */
void hashTable::expand() {
    // update capacity (also prevent overflow of size_t)
    capacity = (capacity > (2 ^ 32 - 1) / 2) ? (2 ^ 32 - 1) : capacity * 2;
    // assign old data
    std::vector<Word> *old_word_map = word_map;
    // when iterate through every data, the map_reference is in no use
    // delete to avoid overlap
    delete map_reference;
    
    // create new map and references in current capacity
    word_map = new std::vector<Word>[capacity];
    map_reference = new std::vector<std::pair<std::string, size_t>>[capacity];
    
    // iterate through every dimension and implement into new map and reference
    for (size_t i = 0; i < capacity / 2; i++) {
        for (const Word &j : old_word_map[i]) {
            for (const std::pair<File*, int> &k : j.vertex) {
                Word temp(j.wordString , k);
                push(temp);
            }
        }
    }

    delete old_word_map;
}