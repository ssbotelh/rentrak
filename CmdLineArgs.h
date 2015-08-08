#ifndef RENTRAK_CMD_LINE_ARGS
#define RENTRAK_CMD_LINE_ARGS

#include <string>
#include <map>

class CmdLineArgs {
private: //fields
    std::map<std::string, std::string>  m_mapNameValue;
    std::string                         m_sShortPrefix;
    std::string                         m_sLongPrefix;

private: //methods
    void ParseCommandLine(int const argc, char *argv[]);
    bool IsPrefixed(std::string const &name) const;
    std::string RemovePrefix(std::string const &name) const;

public:
    CmdLineArgs(int const argc, char *argv[]);
    ~CmdLineArgs();

    CmdLineArgs           (CmdLineArgs const &) = delete;
    CmdLineArgs &operator=(CmdLineArgs const &) = delete;

    void Print() const;

    //Iterators
//    using iterator       = std::map<std::string, std::string>::iterator;
    using const_iterator = std::map<std::string, std::string>::const_iterator;

    const_iterator begin() const { return m_mapNameValue.cbegin(); }
    const_iterator end()   const { return m_mapNameValue.cend();   }
};

#endif //RENTRAK_CMD_LINE_ARGS
