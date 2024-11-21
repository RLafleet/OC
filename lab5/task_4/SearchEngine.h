#include <cmath>
#include <unordered_map>
#include <vector>
#include <algorithm>

class SearchEngine {
public:
    explicit SearchEngine(InvertedIndex& index) : m_index(index) {}

    std::vector<std::pair<InvertedIndex::DocumentInfo, double>> Find(const std::vector<std::string>& query);

private:
    InvertedIndex& m_index;

    double CalculateRelevance(const InvertedIndex::DocumentInfo& doc, const std::vector<std::string>& query);
};

std::vector<std::pair<InvertedIndex::DocumentInfo, double>> SearchEngine::Find(const std::vector<std::string>& query) {
    auto docs = m_index.FindDocuments(query);
    std::vector<std::pair<InvertedIndex::DocumentInfo, double>> results;

    for (const auto& doc : docs) {
        double relevance = CalculateRelevance(doc, query);
        results.emplace_back(doc, relevance);
    }

    std::sort(results.begin(), results.end(), [](auto& a, auto& b) {
        return a.second > b.second;
        });

    if (results.size() > 10) results.resize(10);
    return results;
}

double SearchEngine::CalculateRelevance(const InvertedIndex::DocumentInfo& doc, const std::vector<std::string>& query) {
    double relevance = 0.0;
    for (const auto& word : query) {
        if (doc.wordFrequencies.count(word)) {
            relevance += doc.wordFrequencies.at(word); // Простой вариант без IDF
        }
    }
    return relevance;
}
