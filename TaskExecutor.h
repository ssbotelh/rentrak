#ifndef RENTRAK_TASK_EXECUTOR
#define RENTRAK_TASK_EXECUTOR

#include "Record.h"
#include "FileManager.h"
#include <vector>

class CmdLineArgs;
class Operation;

////////////////////////////////////

class TaskExecutor {
private: //fields
    CmdLineArgs const          &m_cmdArgs;
    std::vector<Record>         m_vRecords;
    std::vector<Operation *>    m_vpOperations;
    FileManager                 m_fileMgr;
    
private: //methods
    Operation *CreateNewOperation(std::string const &prefix, std::string const &cmd);

public:
    TaskExecutor(CmdLineArgs const &args);
    ~TaskExecutor();

    void Run();
};

#endif //RENTRAK_TASK_EXECUTOR

