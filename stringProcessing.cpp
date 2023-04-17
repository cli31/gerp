#include "stringProcessing.h"

std::string stripNonAlphaNum(std::string input) {
    // Find the index of the first alphanumeric character
    int first = 0;
    while (first < input.size() and (not isalnum(input[first]))) {
        first++;
    }

    // Find the index of the last alphanumeric character
    int last = input.size() - 1;
    while (last >= 0 && !isalnum(input[last])) {
        last--;
    }

    // Return the substring between the first and last alphanumeric characters
    return input.substr(first, last - first + 1);
}