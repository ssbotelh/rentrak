#include "Operation.h"

Operation::Operation(std::string const &cmd, FileManager &fileMgr)
    : m_sCommands(cmd)
    , m_fileMgr(fileMgr)
{}

Operation::~Operation()
{}

