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
    , m_fileMgr("_dataStore.db")
{}

TaskExecutor::~TaskExecutor()
{
    for (Operation const *pOper : m_vpOperations)
        delete pOper;

    m_vpOperations.clear();
    m_vRecords.clear();
}

Operation *TaskExecutor::CreateNewOperation(std::string const &prefix, std::string const &cmd)
{
    Operation *pOper(nullptr);

    if (prefix == "s") {
        pOper = new SelectOperation(cmd, m_fileMgr);
        assert(pOper != nullptr);
    }

    return pOper;
}

void TaskExecutor::Run()
{
    // If "-i" found, only parse/import data and exit
    if (m_cmdArgs.Exists("i")) {
        m_fileMgr.ImportDataFromFile(m_cmdArgs.GetValue("i"));
        return;
    }

    for (CmdLineArgs::const_iterator it = m_cmdArgs.begin(); it != m_cmdArgs.end(); ++it)
        m_vpOperations.emplace_back(CreateNewOperation(it->first, it->second));

    //Prepare operations...
    //...

    for (Operation *pOper : m_vpOperations)
        pOper->Run(m_vRecords);

    //Print results
    for (Record const &rec : m_vRecords)
        rec.Print();
}

