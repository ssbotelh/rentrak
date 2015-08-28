#ifndef RENTRAK_UTILITY
#define RENTRAK_UTILITY

#include <string>
#include <vector>

namespace Utility
{
    std::vector<std::string> Tokenize(std::string const &input,
                                      std::string const &delim,
                                      bool        const  bAllowEmptyTokens = false);

    unsigned LeftPart (std::string const &word, char const sep);
    unsigned RightPart(std::string const &word, char const sep);

    std::string ToUpper(std::string const &s);
}

#endif //RENTRAK_UTILITY
