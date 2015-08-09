#ifndef RENTRAK_OPERATION
#define RENTRAK_OPERATION

#include "Record.h"
#include "FileManager.h"
#include <string>
#include <vector>

class Operation {
protected:
    std::string  m_sCommands;
    FileManager &m_fileMgr;

public:
    Operation(std::string const &cmd, FileManager &fileMgr);
    virtual ~Operation();

    virtual void Run(std::vector<Record> &vRecords) const = 0;
};

#endif //RENTRAK_OPERATION
