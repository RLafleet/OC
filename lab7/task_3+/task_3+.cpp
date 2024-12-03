#include "Acrypt.h"

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: acryptw <input_file> <output_file>\n";
        return 1;
    }

    // Преобразование аргументов в широкие строки
    std::wstring inputFile = std::wstring(argv[1], argv[1] + strlen(argv[1]));
    std::wstring outputFile = std::wstring(argv[2], argv[2] + strlen(argv[2]));

    try {
        // Начало замера времени
        auto start = std::chrono::high_resolution_clock::now();

        // Выполнение шифрования с асинхронным вводом-выводом
        acryptw(inputFile, outputFile);

        // Окончание замера времени
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        std::cout << "File successfully encrypted and copied in " << duration.count() << " seconds.\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
