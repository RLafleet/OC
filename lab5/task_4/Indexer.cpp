#include "Indexer.h"
#include <filesystem> // C++17 or later for filesystem operations
#include <iostream>

void Indexer::IndexDirectory(const std::string& path, bool recursive) {
    if (recursive) {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(path)) {
            if (entry.is_regular_file()) {
                // Process each file
                std::cout << "Indexing file: " << entry.path() << std::endl;
                // Call a method to index the file, e.g., IndexFile(entry.path());
            }
        }
    }
    else {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                // Process each file
                std::cout << "Indexing file: " << entry.path() << std::endl;
                // Call a method to index the file, e.g., IndexFile(entry.path());
            }
        }
    }
}

void Indexer::IndexFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    auto wordFrequencies = CalculateWordFrequencies(buffer.str());

    std::lock_guard<std::mutex> lock(m_idMutex);
    m_index.AddDocument(m_nextId++, path, wordFrequencies);
}

std::unordered_map<std::string, double> Indexer::CalculateWordFrequencies(const std::string& content) {
    std::unordered_map<std::string, double> frequencies;
    std::istringstream stream(content);
    std::string word;

    size_t totalWords = 0;
    while (stream >> word) {
        for (auto& c : word) c = std::tolower(c); // Приведение к нижнему регистру
        if (!std::isalpha(word[0])) continue;    // Игнорирование некорректных слов
        frequencies[word]++;
        totalWords++;
    }

    for (auto& [key, value] : frequencies) {
        value /= totalWords; // Нормализация TF
    }
    return frequencies;
}
