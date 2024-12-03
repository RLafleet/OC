#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>

class Indentation 
{
    int level;
public:
    explicit Indentation(int lvl) : level(lvl) {}

    Indentation(const Indentation&) = delete;
    Indentation& operator=(const Indentation&) = delete;

    void Print() const 
    {
        for (int i = 0; i < level; ++i) 
        {
            std::cout << "|   ";
        }
    }
};
// подогнать поф форматирование

void PrintDirectoryTree(const std::filesystem::path& dir_path, int indent = 0) 
{
    try 
    {
        if (!std::filesystem::exists(dir_path)) 
        {
            std::cerr << "Directory does not exist: " << dir_path << '\n';
            return;
        }

        Indentation indentation(indent);
        std::vector<std::filesystem::directory_entry> entries;

        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) 
        {
            entries.push_back(entry);
        }

        std::sort(entries.begin(), entries.end(), [](const auto& a, const auto& b) 
            {
                return a.is_directory() > b.is_directory(); 
            });


        // разобараться с вопросами для самоконтроля
        for (const auto& entry : entries) 
        {
            indentation.Print();
            if (entry.is_directory()) 
            {
                std::cout << "+---[D] " << entry.path().filename().string() << '\n';
                PrintDirectoryTree(entry.path(), indent + 1);
            }
            else 
            {
                std::cout << "[F] " << entry.path().filename().string() << '\n';
            }
        }
    }

    // лучше бы исключения отлавливать в другом месте, иначе постоянно будем заваливать
    catch (const std::filesystem::filesystem_error& e) 
    {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }
    catch (const std::exception& e) 
{
        std::cerr << "Error: " << e.what() << '\n';
    }
}