#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

// RAII обёртка для вывода (необязательно, но для примера)
class Indentation {
    int level;
public:
    explicit Indentation(int lvl) : level(lvl) {}
    void print() const {
        for (int i = 0; i < level; ++i) {
            std::cout << "|   ";
        }
    }
};

void print_directory_tree(const std::filesystem::path& dir_path, int indent = 0) {
    try {
        if (!std::filesystem::exists(dir_path)) {
            std::cerr << "Directory does not exist: " << dir_path << '\n';
            return;
        }

        Indentation indentation(indent);
        std::vector<std::filesystem::directory_entry> entries;

        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            entries.push_back(entry);
        }

        // Сортировка: сначала файлы, затем директории
        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) {
            return a.is_directory() > b.is_directory(); // Директории идут после файлов
        });

        for (const auto& entry : entries) {
            indentation.print();
            if (entry.is_directory()) {
                std::cout << "+---[D] " << entry.path().filename().string() << '\n';
                print_directory_tree(entry.path(), indent + 1);
            } else {
                std::cout << "[F] " << entry.path().filename().string() << '\n';
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
    }
}

int main(int argc, char* argv[]) {
    std::filesystem::path dir = (argc > 1) ? argv[1] : std::filesystem::current_path();

    std::cout << dir.filename().string() << '\n';
    print_directory_tree(dir);
    return 0;
}
