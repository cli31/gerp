#ifndef _FILE_H
#define _FILE_H

#include <string>
#include <vector>
#include <utility>

/*
 * File struct
 * The output format is: FileNameWithPath : LineNum : Line
 * to same memory, every word in this file share the same path, therefore, 
 * instead of storing the path string in each word struct, I save them in the
 * file struct and set a pointer to access
 * to handle the index of line, I push the index (countLine in the program) into
 * the word struct, meanwhile refer to the random access index in vector so
 * after searched a word, the program can access all the output info from the
 * word
 */
struct File
{
    std::string fileNameWithPath;
    std::vector<std::pair<std::string, bool>> Line;

    File(std::string _fileNameWithPath);

    void add_line(std::string _lineString);
};

#endif