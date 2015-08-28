#include "DataStore.h"
#include "Utility.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

/////////////////////////////////////////////////
// Helper functions
//
void CopyString(char *dest, std::string const &src, size_t const size)
{
    size_t const actual_size(std::min(size - 1, src.size()));
    ::strncpy(dest, src.c_str(), actual_size);
    dest[actual_size] = '\0';
}

void CopyValue(unsigned &left, unsigned &right, std::string const &value, char const sep)
{
    left  = 0;
    right = 0;

    size_t const pos(value.find(sep));
    if (pos != std::string::npos) {
        left  = std::stol(value.substr(0, pos));
        right = std::stol(value.substr(pos+1));
    }
}

/////////////////////////////////////////////////
// struct FixedSizeRecord
//
Record FixedSizeRecord::GetRecord() const
{
    Record r;

    r.AddField(Field(Field::STB,       std::string(stb)));
    r.AddField(Field(Field::TITLE,     std::string(title)));
    r.AddField(Field(Field::PROVIDER,  std::string(provider)));
    r.AddField(Field(Field::DATE,      std::string(date)));
    r.AddField(Field(Field::REV,       dollars, cents));
    r.AddField(Field(Field::VIEW_TIME, hours, mins));

    return r;
}

/////////////////////////////////////////////////
// class DataStore::const_iterator
//
DataStore::const_iterator::const_iterator(DataStore const &ds, size_t const index)
    : m_data(ds)
    , m_uIndex(index)
    , m_uMaxIndex(ds.GetNumRecords())
{}

DataStore::const_iterator::~const_iterator()
{}

bool DataStore::const_iterator::operator==(DataStore::const_iterator const &that) const
{
    return ((&m_data == &that.m_data) && // same DataStore...
            (m_uIndex == that.m_uIndex));  // ... and same index
}

bool DataStore::const_iterator::operator!=(DataStore::const_iterator const &that) const
{
    return !(*this == that);
}

Record DataStore::const_iterator::operator*() const
{
    if (!m_data.IsOpen())
        throw std::runtime_error("Must open datastore before using iterators");

    assert(m_uIndex < m_uMaxIndex);
    return m_data.FetchRecord(m_uIndex); 
}

DataStore::const_iterator &DataStore::const_iterator::operator++()
{
    if (!m_data.IsOpen())
        throw std::runtime_error("Must open datastore before using iterators");

    ++m_uIndex;
    return *this;
}

/////////////////////////////////////////////////
// class DataStore
//
DataStore::DataStore(std::string const &file)
    : m_GenDataStore(file.c_str())
{
    assert(!file.empty());
}

DataStore::~DataStore()
{
    Close();
}

FixedSizeRecord DataStore::ConvertToFixedSize(Record const &rec) const
{
    FixedSizeRecord fsr;

    CopyString(fsr.stb,                 rec.GetFieldValue(Field::STB),        64);
    CopyString(fsr.title,               rec.GetFieldValue(Field::TITLE),      64);
    CopyString(fsr.provider,            rec.GetFieldValue(Field::PROVIDER),   64);
    CopyString(fsr.date,                rec.GetFieldValue(Field::DATE),       11);
    CopyValue (fsr.dollars, fsr.cents,  rec.GetFieldValue(Field::REV),       '.');
    CopyValue (fsr.hours, fsr.mins,     rec.GetFieldValue(Field::VIEW_TIME), ':');

    return fsr;
}

void DataStore::Open() const
{
    if (!m_GenDataStore.Open())
        throw std::runtime_error("Failed to open datastore.");
}

void DataStore::Close() const
{
    m_GenDataStore.Close();
}

bool DataStore::IsOpen() const
{
    return m_GenDataStore.IsOpen();
}

unsigned long DataStore::GetNumRecords() const
{
    return m_GenDataStore.GetRecordCount();
}

Record DataStore::FetchRecord(unsigned long index) const
{
    FixedSizeRecord const fsr(m_GenDataStore.FindRecord(index));
    Record          const rec(fsr.GetRecord());

    return rec;
}

void DataStore::UpdateRecord(Record const &rec, unsigned long index)
{
    FixedSizeRecord const fsr(ConvertToFixedSize(rec));
    if (!m_GenDataStore.ModifyRecord(fsr, index))
        throw std::runtime_error("Failed to update record in datastore");
}

void DataStore::AddRecord(Record const &rec)
{
    FixedSizeRecord const fsr(ConvertToFixedSize(rec));
    if (!m_GenDataStore.AddRecord(fsr))
        throw std::runtime_error("Failed to add record in datastore");
}

void DataStore::ImportDataFromFile(std::string const &file)
{
    std::ifstream infile(file);
    std::string line;
    std::getline(infile, line); //skip first line (assume it's always a header)

    std::vector<std::pair<Record, bool>> vRecordsToAdd; //boolean keeps track if Record already exists in datastore

    //Add all Records from input file to temporary storage
    while (std::getline(infile, line)) {
        std::vector<std::string> const tokens(Utility::Tokenize(line, "|"));

        if (tokens.size() != 6)
            throw std::runtime_error("Bad record in file (found " + std::to_string(tokens.size()) + " fields, expected 6)");

        Record const rec(tokens);
        vRecordsToAdd.emplace_back(std::make_pair(rec, false));
    }

    std::cout << vRecordsToAdd.size() << " record(s) read from file" << std::endl;

    //Read data store (from file) and update existing records
    size_t numUpdated(0);
    Open();
    size_t const size(GetNumRecords());
    for (size_t ii = 0; ii < size; ++ii) {
        Record const oldRec(FetchRecord(ii));

        for (auto &pp : vRecordsToAdd) {
            Record const &rec  (pp.first);
            bool         &added(pp.second);

            if (added)
                continue;

            if (rec == oldRec) {
                UpdateRecord(rec, ii);
                added = true;
                ++numUpdated;
            }
        }
    }

    if (numUpdated > 0)
        std::cout << numUpdated << " record(s) updated in datastore" << std::endl;

    //Finally, add brand new records to datastore
    size_t numAdded(0);
    for (auto const &pp : vRecordsToAdd) {
        Record const &rec(pp.first);
        bool   const &added(pp.second);

        if (!added) {
            AddRecord(rec);
            ++numAdded;
        }
    }

    if (numAdded > 0)
        std::cout << numAdded << " record(s) added to datastore" << std::endl;

    Close();
}

DataStore::const_iterator DataStore::begin() const
{
    if (!IsOpen())
        throw std::runtime_error("Must open datastore before calling begin()");

    return DataStore::const_iterator(*this, 0);
}

DataStore::const_iterator DataStore::end() const
{
    if (!IsOpen())
        throw std::runtime_error("Must open datastore before calling end()");

    return DataStore::const_iterator(*this, GetNumRecords());
}

