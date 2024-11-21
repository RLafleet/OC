// InvertedIndex.h
#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <mutex>
#include <map>   // for std::map
#include <unordered_map>

class InvertedIndex {
public:
    struct DocumentInfo {
        uint64_t documentId;
        std::map<std::string, size_t> wordFrequencies;

        DocumentInfo(uint64_t id, std::map<std::string, size_t> frequencies)
            : documentId(id), wordFrequencies(std::move(frequencies)) {}
    };

    // Добавляет документ в индекс
    // InvertedIndex.h
    void AddDocument(uint64_t docId, const std::string& path, const std::unordered_map<std::string, double>& wordFrequencies);


    // Удаляет документ из индекса
    void RemoveDocument(uint64_t docId);

    // Выполняет поиск по запросу, возвращая релевантные документы
    std::vector<std::pair<uint64_t, double>> Search(const std::vector<std::string>& queryWords) const;

    // Очищает весь индекс
    void Clear();

private:
    mutable std::mutex m_mutex;
    std::unordered_map<std::string, std::unordered_map<uint64_t, double>> m_index; // слово -> (документ -> TF)
    std::unordered_map<uint64_t, DocumentInfo> m_documents; // id -> информация о документе
    std::unordered_map<std::string, size_t> m_wordDocCount; // слово -> кол-во документов, где оно встречается
    size_t m_documentCount = 0;

    double ComputeIDF(const std::string& word) const;
};
