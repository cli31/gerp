/*
 * main.cpp
 * the drive program of Gerp
 * Cheng Li and Andrej Djokic
 */

#include <iostream>

#include "gerp.h"

int main(int argc, char *argv[]) {
    if (not (argc == 3)) {
        std::cerr << "Usage : ./ gerp inputDirectory outputFile\n";
        exit(EXIT_FAILURE);
    }
    Gerp g(argv[1], argv[2]);
    g.run();
}