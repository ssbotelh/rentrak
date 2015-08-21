#ifndef RENTRAK_RECORD
#define RENTRAK_RECORD

#include "Field.h"
#include <vector>
#include <string>

class Record {
private:
    std::vector<Field> m_Fields;

public:
    Record();
    Record(std::vector<std::string> const &vValues);
    ~Record();

    bool operator==(Record const &that) const;

    void  AddField(Field const &field);
    bool  HasField(Field::Name const &name) const;
    Field GetField(Field::Name const &name) const;
    void  Print() const;
};

#endif //RENTRAK_RECORD
