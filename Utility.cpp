#include "Utility.h"

// Base on: http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
void Utility::Tokenize(std::string const        &input, 
                       std::vector<std::string> &tokens, 
                       std::string const        &delim,
                       bool                      bAllowEmptyTokens)
{
    const static std::string sQUOTE_CHARACTERS = "\'\"";

    size_t start = 0;
    size_t end   = 0;
    const size_t length = input.size();

    while (end != std::string::npos && start < length) {
        end = input.find_first_of(delim, start);

        // If at end use length=remainder, else length=end-start.
        const size_t token_length = end == std::string::npos ? length - start : end - start;

        if (token_length > 0 || bAllowEmptyTokens) {
            const std::string sToken(input.data() + start, token_length);
            tokens.emplace_back(std::move(sToken));
        }

        start = end == std::string::npos ? std::string::npos : end + 1;
    }            

    // Handle case where delimiter is the last character, eg "a,b," -> { "a", "b", "" }
    if (end != std::string::npos && start == length && bAllowEmptyTokens)
        tokens.emplace_back("");
}

