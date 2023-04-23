#include "Word.h"

Word::Word(std::string _wordString, std::pair<File*, int> _vertex) :
wordString(_wordString), vertex(std::vector<std::pair<File*, int>>{_vertex}) {}

void Word::add(std::pair<File*, int> _vertex) {
    vertex.push_back(_vertex);
}