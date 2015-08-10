#include "DataStore.h"
#include "Utility.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <cassert>

////////////////////////////////////////
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

Record FixedSizeRecord::GetRecord() const
{
    Record r;

    r.AddField(Field::STB,       std::string(stb));
    r.AddField(Field::TITLE,     std::string(title));
    r.AddField(Field::PROVIDER,  std::string(provider));
    r.AddField(Field::DATE,      std::string(date));
    r.AddField(Field::REV,       Utility::FormatString(dollars, cents, '.'));
    r.AddField(Field::VIEW_TIME, Utility::FormatString(hours, mins, ':'));

    return r;
}

////////////////////////////////////////////////////////

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

    CopyString(fsr.stb,                 rec.GetFieldValue(Field::STB), 64);
    CopyString(fsr.title,               rec.GetFieldValue(Field::TITLE), 64);
    CopyString(fsr.provider,            rec.GetFieldValue(Field::PROVIDER), 64);
    CopyString(fsr.date,                rec.GetFieldValue(Field::DATE), 11);
    CopyValue (fsr.dollars, fsr.cents,  rec.GetFieldValue(Field::REV), '.');
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
            Record &rec  (pp.first);
            bool   &added(pp.second);

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

        if (added)
            continue;

        AddRecord(rec);
        ++numAdded;
    }

    if (numAdded > 0)
        std::cout << numAdded << " record(s) added to datastore" << std::endl;

    Close();

/*
    std::cout << std::endl;
    Open();
    size_t const num(GetRecordCount());
    std::cout << "Num = " << num << std::endl;
    for (size_t ii = 0; ii < num; ++ii) {
        BareRecord const br(FindRecord(ii));
        Record const r(br.ConvertFromBare());
        r.Print();
    }
*/
}

void DataStore::FetchRecords(std::vector<Field::Name> const &vFieldNames, std::vector<Record> &vRecords) const
{
    Open();

    size_t const size(GetNumRecords());
    for (size_t ii = 0; ii < size; ++ii) {
        Record const rec(FetchRecord(ii));

        Record newRec;
        if (vFieldNames.empty()) {
            newRec = rec;
        } else {
            for (Field::Name const &name : vFieldNames)
                newRec.AddField(name, rec.GetFieldValue(name));
        }

        vRecords.emplace_back(newRec);
    }

    Close();
}

