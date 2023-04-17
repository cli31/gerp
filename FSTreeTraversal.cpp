/*
 * made by Cheng Li [cli31]
 * Date: 4/17/2023
 * 
 * When executed, this program take the highest directory as a command line 
 * argument, and then print the full paths of all the files accessible from 
 * that directory
 * usage: ./ treeTraversal Directory
 * arbitrary order of the file paths being print
 */

#include "FSTree.h"
#include "DirNode.h"

#include <iostream>
#include <vector>

class FSTreeTraversal {
public:
    FSTreeTraversal(const std::string filename) :
    tree(filename) {
        root = tree.getRoot();
    }

    void run() {
        BFT(root, "");
    }

private:
    FSTree tree;
    DirNode *root;

    // depth first traverse to print all the files under root
    // since order not matter, both DFT and BFT work
    // choose BFT bc numSubDir and getSubDir offered
    void BFT(DirNode *curr, std::string path) { 
        // in the first place, update path
        path += curr->getName() + "/";
        print(curr, path); // first print files under current node
        // stop traversing when numSubDir == 0
        for (int i = 0; i < curr->numSubDirs(); i++) {
            BFT(curr->getSubDir(i), path);
        }
    }

    // print all the files under current dirNode
    // if no files, no print
    void print(DirNode *curr, std::string path) {
        for (int j = 0; j < curr->numFiles(); j++)
            std::cout << path << curr->getFile(j) << std::endl;
    }
};

int main(int argc, char *argv[]) {
    if (not (argc == 2)) {
        std::cerr << "Usage: ./treeTraversal Directory\n";
        exit(EXIT_FAILURE);
    }
    FSTreeTraversal fst(argv[1]);
    fst.run();
}