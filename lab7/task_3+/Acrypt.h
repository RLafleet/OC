#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <windows.h>
 
constexpr size_t BUFFER_SIZE = 4096;

class FileHandle {
    HANDLE m_handle;

public:
    explicit FileHandle(HANDLE handle) : m_handle(handle) {
        if (m_handle == INVALID_HANDLE_VALUE) {
            throw std::runtime_error("Failed to open file.");
        }
    }

    ~FileHandle() {
        if (m_handle != INVALID_HANDLE_VALUE) {
            CloseHandle(m_handle);
        }
    }

    HANDLE get() const {
        return m_handle;
    }
};

void encryptBuffer(std::vector<char>& buffer) {
    for (auto& byte : buffer) {
        byte = ~byte;
    }
}

void acryptw(const std::wstring& inputFile, const std::wstring& outputFile) {
    FileHandle inFile(CreateFile(inputFile.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, nullptr));
    FileHandle outFile(CreateFile(outputFile.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_FLAG_OVERLAPPED, nullptr));

    OVERLAPPED readOverlapped = {};
    OVERLAPPED writeOverlapped = {};
    std::vector<char> buffer(BUFFER_SIZE);
    DWORD bytesRead = 0;

    while (true) {
        // Initiate asynchronous read
        if (!ReadFile(inFile.get(), buffer.data(), BUFFER_SIZE, nullptr, &readOverlapped)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                throw std::runtime_error("ReadFile failed.");
            }
        }

        // Wait for the read operation to complete
        if (!GetOverlappedResult(inFile.get(), &readOverlapped, &bytesRead, TRUE)) {
            throw std::runtime_error("GetOverlappedResult failed.");
        }

        if (bytesRead == 0) break; // End of file

        // Encrypt the data
        buffer.resize(bytesRead);
        encryptBuffer(buffer);

        // Initiate asynchronous write
        if (!WriteFile(outFile.get(), buffer.data(), bytesRead, nullptr, &writeOverlapped)) {
            if (GetLastError() != ERROR_IO_PENDING) {
                throw std::runtime_error("WriteFile failed.");
            }
        }

        // Wait for the write operation to complete
        DWORD bytesWritten;
        if (!GetOverlappedResult(outFile.get(), &writeOverlapped, &bytesWritten, TRUE)) {
            throw std::runtime_error("GetOverlappedResult failed.");
        }

        if (bytesWritten != bytesRead) {
            throw std::runtime_error("Mismatch in bytes written.");
        }

        readOverlapped.Offset += bytesRead;
        writeOverlapped.Offset += bytesRead;
    }
}