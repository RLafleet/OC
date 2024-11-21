// InvertedIndex.cpp
#include "InvertedIndex.h"
#include <cmath>
#include <algorithm>
#include <iterator>

void InvertedIndex::AddDocument(uint64_t docId, const std::string& path, const std::unordered_map<std::string, double>& wordFrequencies) {
    std::lock_guard<std::mutex> lock(m_mutex);  // Lock the mutex
    if (m_documents.count(docId)) {
        throw std::runtime_error("Document already exists in the index.");
    }

    m_documents[docId] = DocumentInfo(docId, {});  // Add document info with an empty word frequency map
    m_documentCount++;  // Increment document count

    // Calculate term frequency (TF) and update the index
    for (const auto& [word, count] : wordFrequencies) {
        double tf = count;  // You can adjust the TF calculation as needed
        m_index[word][docId] = tf;  // Add word and TF to the index
        m_wordDocCount[word]++;  // Increment document count for the word
    }
}

void InvertedIndex::RemoveDocument(uint64_t docId) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (!m_documents.count(docId)) {
        throw std::runtime_error("Document not found in the index.");
    }

    // Удалить документ из всех записей
    for (auto& [word, docMap] : m_index) {
        docMap.erase(docId);
        if (docMap.empty()) {
            m_wordDocCount.erase(word);
        }
    }

    m_documents.erase(docId);
    m_documentCount--;
}

std::vector<std::pair<uint64_t, double>> InvertedIndex::Search(const std::vector<std::string>& queryWords) const {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::unordered_map<uint64_t, double> documentScores;

    for (const auto& word : queryWords) {
        if (m_index.count(word) == 0) {
            continue;
        }

        double idf = ComputeIDF(word);
        for (const auto& [docId, tf] : m_index.at(word)) {
            documentScores[docId] += tf * idf;
        }
    }

    std::vector<std::pair<uint64_t, double>> results(documentScores.begin(), documentScores.end());
    std::sort(results.begin(), results.end(),
        [](const auto& a, const auto& b) { return b.second < a.second; }); // Сортировка по релевантности

    return results;
}

void InvertedIndex::Clear() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_index.clear();
    m_documents.clear();
    m_wordDocCount.clear();
    m_documentCount = 0;
}

double InvertedIndex::ComputeIDF(const std::string& word) const {
    if (m_wordDocCount.count(word) == 0) {
        return 0.0;
    }
    return std::log(static_cast<double>(m_documentCount) / m_wordDocCount.at(word));
}
