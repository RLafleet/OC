#include <windows.h>
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <stdexcept>

// Глобальная гистограмма
std::vector<std::atomic<size_t>> globalHistogram(256);
std::mutex printMutex;

// Функция для печати отладочных сообщений
void Log(const std::string& message) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cerr << message << std::endl;
}

// Функция для обновления гистограммы
void UpdateHistogram(const char* data, size_t length) {
    std::vector<size_t> localHistogram(256, 0);

    for (size_t i = 0; i < length; ++i) {
        ++localHistogram[static_cast<unsigned char>(data[i])];
    }

    // Обновляем глобальную гистограмму
    for (size_t i = 0; i < 256; ++i) {
        globalHistogram[i] += localHistogram[i];
    }
}

// Потоковая функция
void ProcessFilePart(HANDLE fileHandle, HANDLE mappingHandle, size_t offset, size_t size) {
    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    size_t pageSize = systemInfo.dwAllocationGranularity;

    // Выровнять offset вниз
    size_t alignedOffset = (offset / pageSize) * pageSize;
    size_t offsetAdjustment = offset - alignedOffset;

    Log("Starting to process file part at offset: " + std::to_string(offset) +
        " (aligned: " + std::to_string(alignedOffset) + ") with size: " + std::to_string(size));

    char* view = static_cast<char*>(
        MapViewOfFile(mappingHandle, FILE_MAP_READ, static_cast<DWORD>(alignedOffset >> 32),
            static_cast<DWORD>(alignedOffset), size + offsetAdjustment));
    if (!view) {
        Log("Failed to map view of file at aligned offset: " + std::to_string(alignedOffset));
        throw std::runtime_error("Failed to map view of file");
    }

    // Смещаем указатель view на величину коррекции
    UpdateHistogram(view + offsetAdjustment, size);

    // Освобождаем отображение
    UnmapViewOfFile(view);
    Log("Finished processing file part at offset: " + std::to_string(offset));
}


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: task_2 <file_path> <num_threads>\n";
        return 1;
    }

    const char* filePath = argv[1];
    int numThreads = std::stoi(argv[2]);

    Log("Opening file: " + std::string(filePath));
    HANDLE fileHandle = CreateFile(filePath, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (fileHandle == INVALID_HANDLE_VALUE) {
        Log("Failed to open file");
        return 1;
    }

    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(fileHandle, &fileSize)) {
        Log("Failed to get file size");
        CloseHandle(fileHandle);
        return 1;
    }

    Log("File size: " + std::to_string(fileSize.QuadPart));

    HANDLE mappingHandle = CreateFileMapping(fileHandle, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!mappingHandle) {
        Log("Failed to create file mapping");
        CloseHandle(fileHandle);
        return 1;
    }

    size_t totalSize = static_cast<size_t>(fileSize.QuadPart);
    size_t partSize = totalSize / numThreads;
    size_t remainingSize = totalSize % numThreads;

    Log("Total size: " + std::to_string(totalSize) + ", Part size: " + std::to_string(partSize));

    std::vector<std::thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        size_t offset = i * partSize;
        size_t size = (i == numThreads - 1) ? partSize + remainingSize : partSize;

        Log("Creating thread for part at offset: " + std::to_string(offset) + ", size: " + std::to_string(size));
        threads.emplace_back(ProcessFilePart, fileHandle, mappingHandle, offset, size);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    Log("All threads finished processing");

    for (size_t i = 0; i < 256; ++i) {
        std::cout << globalHistogram[i] << '\n';
    }

    CloseHandle(mappingHandle);
    CloseHandle(fileHandle);

    Log("Program finished successfully");
    return 0;
}
