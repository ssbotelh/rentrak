#include "FileManager.h"
#include <fstream>
#include <iostream>

FileManager::FileManager(std::string const &dbFileName)
    : m_dataStore(dbFileName)
{}

FileManager::~FileManager()
{}

// Base on: http://stackoverflow.com/questions/53849/how-do-i-tokenize-a-string-in-c
void FileManager::Tokenize(std::string const        &input, 
                           std::vector<std::string> &tokens, 
                           std::string const        &delim,
                           bool                      bAllowEmptyTokens)
{
    const static std::string sQUOTE_CHARACTERS = "\'\"";

    size_t start = 0;
    size_t end   = 0;
    const size_t length = input.size();

    while (end != std::string::npos && start < length) {
        end = input.find_first_of(delim, start);

        // If at end use length=remainder, else length=end-start.
        const size_t token_length = end == std::string::npos ? length - start : end - start;

        if (token_length > 0 || bAllowEmptyTokens) {
            const std::string sToken(input.data() + start, token_length);
            tokens.emplace_back(std::move(sToken));
        }

        start = end == std::string::npos ? std::string::npos : end + 1;
    }            

    // Handle case where delimiter is the last character, eg "a,b," -> { "a", "b", "" }
    if (end != std::string::npos && start == length && bAllowEmptyTokens)
        tokens.emplace_back("");
}

void FileManager::ImportDataFromFile(std::string const &file)
{
    std::ifstream infile(file);
    std::string line;
    std::getline(infile, line); //skip first line (assume it's always a header)

    std::vector<std::pair<Record, bool>> vRecordsToAdd; //boolean to keep track if Record already exists in datastore

    //Add all Records from input file to temporary storage
    while (std::getline(infile, line)) {
        std::vector<std::string> tokens;
        Tokenize(line, tokens, "|");

        if (tokens.size() != 6)
            throw std::runtime_error("Bad record in file (found " + std::to_string(tokens.size()) + " fields, expected 6)");

        Record const rec(tokens);
        vRecordsToAdd.emplace_back(std::make_pair(rec, false));
    }

    std::cout << vRecordsToAdd.size() << " record(s) read from file" << std::endl;

    //Read data store (from file) and update existing records
    size_t numUpdated(0);
    m_dataStore.Open();
    size_t const size(m_dataStore.GetNumRecords());
    for (size_t ii = 0; ii < size; ++ii) {
        Record const oldRec(m_dataStore.FetchRecord(ii));

        for (auto &pp : vRecordsToAdd) {
            Record &rec  (pp.first);
            bool   &added(pp.second);

            if (added)
                continue;

            if (rec == oldRec) {
                m_dataStore.UpdateRecord(rec, ii);
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

        m_dataStore.AddRecord(rec);
        ++numAdded;
    }

    if (numAdded > 0)
        std::cout << numAdded << " record(s) added to datastore" << std::endl;

    m_dataStore.Close();

/*
    std::cout << std::endl;
    m_dataStore.Open();
    size_t const num(m_dataStore.GetRecordCount());
    std::cout << "Num = " << num << std::endl;
    for (size_t ii = 0; ii < num; ++ii) {
        BareRecord const br(m_dataStore.FindRecord(ii));
        Record const r(br.ConvertFromBare());
        r.Print();
    }
*/
}

