#ifndef _WORD_H
#define _WORD_H

#include "File.h"

/*
 * Word struct
 * it is sensitive, which means, e.g. 'we' and 'We' are saved in two separate
 * struct.
 * In hashTable class though, they are pushed in the same bucket, chained with
 * vector
 * 
 * N.P: always use vector instead of linked list bc random access iterator
 */
struct Word
{
    std::string wordString;
    std::vector<std::pair<File*, int>> vertex;

    Word(std::string _wordString, std::pair<File*, int> _vertex);

    void add(std::pair<File*, int> _vertex);
};

#endif