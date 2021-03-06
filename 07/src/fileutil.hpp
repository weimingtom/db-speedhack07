#ifndef FILEUTIL_HPP
#define FILEUTIL_HPP

#include <string>
#include <vector>

std::string loadFile(const std::string &fileName, bool fullpath = false);

std::vector<std::string> tokenize(const std::string &input, const std::string separator, bool ingoreEmpty = false);

#endif