#ifndef RENTRAK_DATA_STORE
#define RENTRAK_DATA_STORE

#include "GenericDataStore.h"
#include "Record.h"
#include <string>
#include <vector>

struct FixedSizeRecord {
    char     stb[64];
    char     title[64];
    char     provider[64];
    char     date[11];
    unsigned dollars;
    unsigned cents;
    unsigned hours;
    unsigned mins;

    Record GetRecord() const;
};

//////////////////////////////////////////////////////

class DataStore {
private: //fields
    GenericDataStore<FixedSizeRecord> m_GenDataStore;

private: //methods
    FixedSizeRecord ConvertToFixedSize(Record const &rec) const;

public:
    DataStore(std::string const &file);
    ~DataStore();

    void          Open() const;
    void          Close() const;
    unsigned long GetNumRecords() const;
    Record        FetchRecord(unsigned long index) const;
    void          UpdateRecord(Record const &rec, unsigned long index);
    void          AddRecord(Record const &rec);

    void ImportDataFromFile(std::string const &file);
    void FetchRecords(std::vector<Field::Name> const &vFieldNames,
                      std::vector<Record>            &vRecords) const;
};

#endif //RENTRAK_DATA_STORE

