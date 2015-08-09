#include "SelectOperation.h"
#include "Field.h"
#include "Utility.h"

SelectOperation::SelectOperation(std::string const &cmd, FileManager &fileMgr)
    : Operation(cmd, fileMgr)
{}

SelectOperation::~SelectOperation()
{}

void SelectOperation::Run(std::vector<Record> &vRecords) const
{
    std::vector<std::string> tokens;
    Utility::Tokenize(m_sCommands, tokens, ",");

    std::vector<Field::Name> vFieldNames;
    for (std::string const &token : tokens)
        vFieldNames.emplace_back(Field::ToEnum(token));

    m_fileMgr.FetchRecords(vFieldNames, vRecords); 
}

