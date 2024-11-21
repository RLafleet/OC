#include "Utils.h"
#include "Indexer.h"
#include "SearchEngine.h"
#include "ThreadPool.h"
#include "InvertedIndex.h"

void PrintResults(const std::vector<std::pair<uint64_t, double>>& results) {
    for (const auto& [docId, relevance] : results) {
        std::cout << "Document ID: " << docId << " Relevance: " << relevance << "\n";
    }
}

int main() {
    InvertedIndex index;
    Indexer indexer(index);
    SearchEngine searchEngine(index);
    ThreadPool threadPool(4); // Пул потоков

    std::string command;
    while (std::getline(std::cin, command)) {
        try {
            if (command.starts_with("add_file ")) {
                std::string path = command.substr(9);
                threadPool.Dispatch([&] { indexer.IndexFile(path); });
            }
            else if (command.starts_with("add_dir ")) {
                std::string path = command.substr(8);
                threadPool.Dispatch([&] { indexer.IndexDirectory(path, false); });
            }
            else if (command.starts_with("add_dir_recursive ")) {
                std::string path = command.substr(18);
                threadPool.Dispatch([&] { indexer.IndexDirectory(path, true); });
            }
            else if (command.starts_with("find ")) {
                std::string query = command.substr(5);
                auto results = searchEngine.Find(SplitWords(query));

                std::vector<std::pair<uint64_t, double>> formattedResults;
                for (const auto& [docInfo, relevance] : results) {
                    formattedResults.emplace_back(docInfo.documentId, relevance);
                }
                PrintResults(formattedResults);
            }
            else if (command.starts_with("remove_file ")) {
                std::string path = command.substr(12);
                index.RemoveFile(path);
            }
            else if (command.starts_with("remove_dir ")) {
                std::string path = command.substr(11);
                index.RemoveDirectory(path, false);
            }
            else if (command.starts_with("remove_dir_recursive ")) {
                std::string path = command.substr(22);
                index.RemoveDirectory(path, true);
            }
            else {
                std::cout << "Unknown command\n";
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << '\n';
        }
    }

    threadPool.Wait(); // Ожидание завершения всех задач перед выходом
}
