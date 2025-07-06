#ifndef RANGEPARSER_HPP
#define RANGEPARSER_HPP

#include <string>
#include <vector>

namespace drogon
{
struct FileRange
{
    size_t start;
    size_t end;
};

enum FileRangeParseResult
{
    InvalidRange = 0,
    NotSatisfiable,
    SinglePart,
    MultiPart
};

FileRangeParseResult parseRangeHeader(const std::string &rangeStr,
                                     size_t contentLength,
                                     std::vector<FileRange> &ranges);
}  // namespace drogon

#endif  // RANGEPARSER_HPP