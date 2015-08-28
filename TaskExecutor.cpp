#include "TaskExecutor.h"
#include "CmdLineArgs.h"
#include "Operation.h"
#include "SelectOperation.h"
#include "SortOperation.h"
#include "FilterOperation.h"
#include "GroupOperation.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <cassert>

TaskExecutor::TaskExecutor(CmdLineArgs const &args)
    : m_cmdArgs(args)
    , m_vRecords()
    , m_vpOperations()
    , m_dataStore("_dataStore.db")
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

    if (prefix == "s")
        pOper = new SelectOperation(m_cmdArgs.GetValue("s"), 0, m_dataStore);
    else if (prefix == "f")
        pOper = new FilterOperation(m_cmdArgs.GetValue("s"), 1, m_dataStore);
    else if (prefix == "o")
        pOper = new SortOperation  (m_cmdArgs.GetValue("s"), 2, m_dataStore);
    else if (prefix == "g")
        pOper = new GroupOperation (m_cmdArgs.GetValue("s"), 3, m_dataStore);

    if (pOper == nullptr)
        throw std::runtime_error("Unknown cmd line prefix: " + prefix);

    pOper->SetExtraParam(cmd);
    return pOper;
}

void TaskExecutor::PrepareOperations()
{
    //Sort operations in the order they must be executed
    std::sort(m_vpOperations.begin(), m_vpOperations.end(),
              [](Operation const *p1, Operation const *p2) { return p1->GetPriority() < p2->GetPriority(); });
}

void TaskExecutor::Run()
{
    // If "-i" found, only parse/import data and exit
    if (m_cmdArgs.Exists("i")) {
        m_dataStore.ImportDataFromFile(m_cmdArgs.GetValue("i"));
        return;
    }

    if (!m_cmdArgs.Exists("s"))
        throw std::runtime_error("Missing select parameter \"-s\"");

    //Collect operations from cmd line
    for (CmdLineArgs::const_iterator it = m_cmdArgs.begin(); it != m_cmdArgs.end(); ++it)
        m_vpOperations.emplace_back(CreateNewOperation(it->first, it->second));

    PrepareOperations();

    //Execute operations
    for (Operation *pOper : m_vpOperations)
        pOper->Run(m_vRecords);

    //Print results
    for (Record const &rec : m_vRecords)
        rec.Print();
}

