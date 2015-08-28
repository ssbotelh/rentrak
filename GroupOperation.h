#ifndef RENTRAK_GROUP_OPERATION
#define RENTRAK_GROUP_OPERATION

#include "Operation.h"
#include <string>
#include <vector>

class GroupOperation : public Operation {
private: //enum
    enum class Aggregate { NONE, MIN, MAX, SUM, COUNT, COLLECT };

private: //fields
    std::string                                    m_sGroupParam;
    std::vector<std::pair<Field::Name, Aggregate>> m_vFieldToAggregate;

private: //methods
    Aggregate GetAggregate(std::string const &token) const;
    Field     PerformAggregation(std::vector<Record> const &vRecords,
                                 Field::Name         const  name,
                                 Aggregate           const aggr) const;

public:
    GroupOperation(std::string const &selectFlds,
                   size_t      const priority,
                   DataStore   const &dataStore);
    ~GroupOperation();

    void Run(std::vector<Record> &vRecords);
    void SetExtraParam(std::string const &param) override { m_sGroupParam = param; }
};

#endif //RENTRAK_GROUP_OPERATION
