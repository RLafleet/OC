// utils.cpp
#include "utils.h"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <cctype>

namespace utils {

    std::vector<std::string> SplitIntoWords(const std::string& text) {
        std::istringstream iss(text);
        std::vector<std::string> words;
        std::string word;

        while (iss >> word) {
            // Приводим к нижнему регистру и убираем неалфавитные символы
            word.erase(std::remove_if(word.begin(), word.end(), [](char c) { return !std::isalpha(c); }), word.end());
            std::transform(word.begin(), word.end(), word.begin(), ::tolower);
            if (!word.empty()) {
                words.push_back(word);
            }
        }

        return words;
    }

    std::vector<std::filesystem::path> GetAllFilesInDirectory(const std::filesystem::path& dir, bool recursive) {
        std::vector<std::filesystem::path> files;

        if (!std::filesystem::exists(dir)) {
            throw std::runtime_error("Directory does not exist: " + dir.string());
        }

        auto iterator = recursive ? std::filesystem::recursive_directory_iterator(dir)
            : std::filesy
