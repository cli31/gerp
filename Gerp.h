/*
 * gerp.h
 * Cheng Li and Andrej Djokic
 * 
 *
 */

#ifndef _GERP_H
#define _GERP_H

#include <fstream>
#include <sstream>
#include <set>

#include "DirNode.h"
#include "FSTree.h"
#include "hashTable.h"
#include "Word.h"
#include "File.h"

class Gerp {
public:
    Gerp(std::string DirName, std::string outFileName);
    ~Gerp();
    void run();

private:
    FSTree fileSysTree;
    std::vector<std::string> filePaths;
    std::vector<File*> fileData;
    hashTable map;
    std::ofstream outFile;

    void treeTraversal(DirNode *curr, std::string path);
    void readFile(std::string filename);
    std::string stringProcess(std::string input);
    void print(const std::string &query, const bool &if_sensitive);
    void resetFile();
};

#endif