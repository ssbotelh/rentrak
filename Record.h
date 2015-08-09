#ifndef RENTRAK_RECORD
#define RENTRAK_RECORD

#include "Field.h"
#include <map>
#include <vector>
#include <string>

class Record {
private:
    std::map<Field::Name, Field::Value> m_Fields;

public:
    Record();
    Record(std::vector<std::string> const &vValues);
    ~Record();

    bool operator==(Record const &that) const;

    void         AddField(Field::Name const &name, Field::Value const &value);
    bool         HasField(Field::Name const &name) const;
    Field::Value GetFieldValue(Field::Name const &name) const;
    void         Print() const;
};

#endif //RENTRAK_RECORD
