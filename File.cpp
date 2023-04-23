#include "File.h"

File::File(std::string _fileNameWithPath) : fileNameWithPath(_fileNameWithPath) 
{}

void File::add_line(std::string _lineString) {
    lineString.push_back(_lineString);
}