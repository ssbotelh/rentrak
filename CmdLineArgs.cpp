#include "CmdLineArgs.h"
#include <cassert>
#include <iostream>

CmdLineArgs::CmdLineArgs(int const argc, char *argv[])
    : m_mapNameValue()
    , m_sShortPrefix("-")
    , m_sLongPrefix("--")
{
    ParseCommandLine(argc, argv);
}

CmdLineArgs::~CmdLineArgs()
{}

void CmdLineArgs::ParseCommandLine(int const argc, char *argv[])
{
    for (int counter = 1; counter < argc; ++counter) {
        if (IsPrefixed(argv[counter])) {
            auto pp =
                m_mapNameValue.insert(std::make_pair(RemovePrefix(argv[counter]), std::string()));

            std::string &value(pp.first->second);

            if ((counter < argc - 1) && !IsPrefixed(argv[counter + 1])) {
                value = argv[counter + 1];
                ++counter;
            }
        }
    }
}

bool CmdLineArgs::IsPrefixed(std::string const &name) const
{
    if ((name.substr(0, m_sShortPrefix.length()) == m_sShortPrefix) ||
        (name.substr(0, m_sLongPrefix.length())  == m_sLongPrefix)) {
        return true;
    }

    return false;
}

std::string CmdLineArgs::RemovePrefix(std::string const &name) const
{
    std::string result(name);

    if (name.substr(0, m_sLongPrefix.length()) == m_sLongPrefix)
        result = name.substr(m_sLongPrefix.length());
    else if (name.substr(0, m_sShortPrefix.length()) == m_sShortPrefix)
        result = name.substr(m_sShortPrefix.length());

    return result;
}

void CmdLineArgs::Print() const
{
    for (auto const &p : m_mapNameValue)
        std::cout << p.first << " => " << p.second << std::endl;
}

