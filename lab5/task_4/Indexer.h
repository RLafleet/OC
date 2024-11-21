#include <fstream>
#include <sstream>
#include <unordered_map>
#include <cctype>
#include "InvertedIndex.h"

class Indexer {
public:
    explicit Indexer(InvertedIndex& index) : m_index(index) {}

    void IndexFile(const std::string& path);
    void IndexDirectory(const std::string& dirPath, bool recursive);

private:
    uint64_t m_nextId = 1;
    InvertedIndex& m_index;
    std::mutex m_idMutex;

    std::unordered_map<std::string, double> CalculateWordFrequencies(const std::string& content);
};