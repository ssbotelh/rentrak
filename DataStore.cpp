#include "DataStore.h"
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

std::string FormatString(unsigned const left, unsigned const right, char const sep)
{
    std::string ret(std::to_string(left));
    ret += sep;
    ret += (right < 10 ? "0" : "");
    ret += std::to_string(right);

    return ret;
}

/////////////////////////////////////////////////

Record FixedSizeRecord::GetRecord() const
{
    Record r;

    r.AddField(Field::STB,       std::string(stb));
    r.AddField(Field::TITLE,     std::string(title));
    r.AddField(Field::PROVIDER,  std::string(provider));
    r.AddField(Field::DATE,      std::string(date));
    r.AddField(Field::REV,       FormatString(dollars, cents, '.'));
    r.AddField(Field::VIEW_TIME, FormatString(hours, mins, ':'));

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

void DataStore::Open()
{
    if (!m_GenDataStore.Open())
        throw std::runtime_error("Failed to open datastore.");
}

void DataStore::Close()
{
    m_GenDataStore.Close();
}

unsigned long DataStore::GetNumRecords()
{
    return m_GenDataStore.GetRecordCount();
}

Record DataStore::FetchRecord(unsigned long index)
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

