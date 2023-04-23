/*
 * gerp.cpp
 * Cheng Li and Andrej Djokic
 * 
 */

#include "gerp.h"

Gerp::Gerp(std::string DirName, std::string outFileName) : 
    fileSysTree(DirName), outFile(outFileName) {
    // * Step1: traverse through the tree to get path of all files
    DirNode *root = fileSysTree.getRoot();
    treeTraversal(root, ""); 
    // * Step2: open every file and read them
    // due to memory limit, handle each file individually
    for (const std::string &itr : filePaths) {
        readFile(itr);
    }
}

Gerp::~Gerp() {
    outFile.close();
}

void Gerp::treeTraversal(DirNode *curr, std::string path) {
    // in the first place, update path
    path += curr->getName() + "/";
    for (int i = 0; i < curr->numFiles(); i++) {
        // store the path of each file in vector
        std::string p = path + curr->getFile(i);
        filePaths.push_back(p);
    }
    // stop traversing when numSubDir == 0
    for (int i = 0; i < curr->numSubDirs(); i++) {
        treeTraversal(curr->getSubDir(i), path);
    }
}

void Gerp::readFile(std::string filename) {
    // * Step3: open file and check parameter
    std::ifstream infile(filename);
    if (not infile.is_open()) {
        std::cerr << "Error: Unable to open " << filename << std::endl;
        std::cerr << "Program terminated\n";
        exit(EXIT_FAILURE);
    }
    // * Step4: read in each file line by line
    // build a File object
    File f(filename);
    int countLine = 0; // represent index in vector lineString, 0-based
    while (not infile.eof()) {
        std::string s; // each line
        std::string inl; // words in the line
        std::set<std::string> filter; // filter set, to avoid duplication
        std::getline(infile, s);
        std::stringstream sstr(s);
        f.add_line(s);
        // * Step5: read in each word within the line
        while (sstr >> inl) {
            // first process string to word, insert to set to avoid duplication
            filter.insert(stringProcess(inl)); // note: make it a word first
        }
        // * Step6: iterate through the set, add words into hashTable
        while (not filter.empty()) {
            // build one word object
            std::string t = *filter.begin();
            Word w(t, std::make_pair(&f, countLine));
            filter.erase(filter.begin());

            // note: group case-insensitive words in the same bucket
            map.push(w);
        }
        countLine++;
    }
    infile.close();
}

void Gerp::run() {
    std::cout << "Query? ";
    std::string query;
    while (std::cin >> query) {
        if (query == "@q" or query == "@quit") {
            break;
        }
        else if (query == "@i" or query == "@insensitive") {
            std::cin >> query;
            print(stringProcess(query), true);
        }
        else if (query == "@f") {
            std::cin >> query;
            outFile.close();
            outFile.open(query);
        }
        else { // AnyString case
            print(stringProcess(query), false);
        }
        std::cout << "Query? ";
    }
    std::cout << "Goodbye! Thank you and have a nice day.\n";
}

/*
 * stringProcess
 * parameter: a unprocessed string
 * return: a word string (no non-alnum prefix or suffix)
 * purpose: transform an arbitrary string into the form of word
 */
std::string Gerp::stringProcess(std::string input) {
    // Find the index of the first alphanumeric character
    int first = 0;
    while (first < int(input.size()) and (not isalnum(input[first]))) {
        first++;
    }

    // Find the index of the last alphanumeric character
    int last = input.size() - 1;
    while (last >= 0 and (not isalnum(input[last]))) {
        last--;
    }

    // Return the substring between the first and last alphanumeric characters
    return input.substr(first, last - first + 1);
}

void Gerp::print(std::string query, bool if_sensitive) {
    std::vector<Word> getWord = map.get(query, true);
    if (getWord.empty()) {
        outFile << query << " Not Found." 
                << ((if_sensitive) ? "\n" : " Try with @insensitive or @i.\n");
    }
    else {
        for (const Word &i : getWord) {
            for (const std::pair<File*, int> &j : i.vertex) {
                outFile << j.first->fileNameWithPath << ":"
                        << j.second << ": "
                        << j.first->lineString.at(j.second)
                        << std::endl;
            }
        }
    }
}