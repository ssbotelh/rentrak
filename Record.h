#ifndef RENTRAK_RECORD
#define RENTRAK_RECORD

#include "Field.h"
#include <map>

class Record {
private:
    std::map<Field::Name, Field::Value> m_Fields;

public:
    Record();
    ~Record();

    void         AddField(Field::Name const &name, Field::Value const &value);
    bool         HasField(Field::Name const &name) const;
    Field::Value GetFieldValue(Field::Name const &name) const;
    void         Print() const;
};

#endif //RENTRAK_RECORD
