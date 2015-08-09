#ifndef RENTRAK_UTILITY
#define RENTRAK_UTILITY

#include <string>
#include <vector>

namespace Utility
{
    void Tokenize(std::string const &input, std::vector<std::string> &tokens, std::string const &delim, bool bAllowEmptyTokens = false);
    
}

#endif //RENTRAK_UTILITY
