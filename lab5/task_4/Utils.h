// utils.h
#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace utils {
    // Разделяет строку на слова
    std::vector<std::string> SplitIntoWords(const std::string& text);

    // Рекурсивно получает пути файлов в директории
    std::vector<std::filesystem::path>  (const std::filesystem::path& dir, bool recursive);

    // Читает текстовый файл
    std::string ReadFile(const std::string& filePath);
}
