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

public: //classes
    class const_iterator {
    private:
        DataStore const &m_data;
        unsigned long    m_uIndex;
        unsigned long    m_uMaxIndex;

    public:
        const_iterator(DataStore const &ds, size_t const index);
        ~const_iterator();

        bool operator==(const_iterator const &that) const;
        bool operator!=(const_iterator const &that) const;
        Record operator*() const;
        const_iterator &operator++();
    };

public: //methods
    DataStore(std::string const &file);
    ~DataStore();

    void          Open() const;
    void          Close() const;
    bool          IsOpen() const;
    unsigned long GetNumRecords() const;
    Record        FetchRecord(unsigned long index) const;
    void          UpdateRecord(Record const &rec, unsigned long index);
    void          AddRecord(Record const &rec);

    void ImportDataFromFile(std::string const &file);

    //Iterators
    const_iterator begin() const;
    const_iterator end()   const;
};

#endif //RENTRAK_DATA_STORE

