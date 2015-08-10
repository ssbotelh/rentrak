#ifndef RENTRAK_GROUP_OPERATION
#define RENTRAK_GROUP_OPERATION

#include "Operation.h"
#include <string>
#include <vector>

class GroupOperation : public Operation {
private: //enum
    enum class Aggregate { NONE, MIN, MAX, SUM, COUNT, COLLECT };

private: //fields
    std::vector<std::pair<Field::Name, Aggregate>> m_vFieldToAggregate;

private: //methods
    Aggregate   GetAggregate(std::string const &sToken) const;
    std::string PerformAggregation(std::vector<Record> const &vRecords,
                                   Field::Name         const  field,
                                   Aggregate           const aggr) const;

public:
    GroupOperation(std::vector<std::string> const &cmd, size_t const priority, DataStore const &dataStore);
    ~GroupOperation();

    void Run(std::vector<Record> &vRecords);
};

#endif //RENTRAK_GROUP_OPERATION
