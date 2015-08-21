#include "Utility.h"
#include <algorithm>

// Base on: http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
std::vector<std::string> Utility::Tokenize(std::string const        &input, 
                                           std::string const        &delim,
                                           bool        const         bAllowEmptyTokens)
{
    const static std::string sQUOTE_CHARACTERS = "\'\"";

    size_t start = 0;
    size_t end   = 0;
    const size_t length = input.size();

    std::vector<std::string> tokens;

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

    return tokens;
}

/*
std::string Utility::FormatString(unsigned const left, unsigned const right, char const sep)
{
    std::string ret(std::to_string(left));
    ret += sep;
    ret += (right < 10 ? "0" : "");
    ret += std::to_string(right);

    return ret;
}
*/

unsigned Utility::LeftPart(std::string const &word, char const sep)
{
    unsigned left(0);

    size_t const pos(word.find(sep));
    if (pos != std::string::npos)
        left = std::stol(word.substr(0, pos));

    return left;
}

unsigned Utility::RightPart(std::string const &word, char const sep)
{
    unsigned right(0);

    size_t const pos(word.find(sep));
    if (pos != std::string::npos)
        right = std::stol(word.substr(pos + 1));

    return right;
}

std::string Utility::ToUpper(std::string const &s)
{
    std::string str(s);
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}

