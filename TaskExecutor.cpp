#include "TaskExecutor.h"
#include "CmdLineArgs.h"
#include "Operation.h"
#include "SelectOperation.h"
#include <iostream>
#include <cassert>

TaskExecutor::TaskExecutor(CmdLineArgs const &args)
    : m_cmdArgs(args)
    , m_vRecords()
    , m_vpOperations()
{}

TaskExecutor::~TaskExecutor()
{
    for (Operation const *pOper : m_vpOperations)
        delete pOper;

    m_vpOperations.clear();
    m_vRecords.clear();
}

Operation *TaskExecutor::CreateNewOperation(std::string const &prefix, std::string const &cmd) const
{
    Operation *pOper(nullptr);

    if (prefix == "s") {
        pOper = new SelectOperation(cmd);
        assert(pOper != nullptr);
    }

    return pOper;
}

void TaskExecutor::Run()
{
    for (CmdLineArgs::const_iterator it = m_cmdArgs.begin(); it != m_cmdArgs.end(); ++it)
        m_vpOperations.emplace_back(CreateNewOperation(it->first, it->second));
}

