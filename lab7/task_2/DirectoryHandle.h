#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

class DirectoryHandle 
{
    //
    HANDLE m_hDirectory;
public:
    DirectoryHandle(const DirectoryHandle&) = delete;
    DirectoryHandle& operator=(const DirectoryHandle&) = delete;

    explicit DirectoryHandle(const std::wstring& path) 
    {
        m_hDirectory = CreateFileW
        (
            path.c_str(),
            FILE_LIST_DIRECTORY,
            FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
            nullptr,
            OPEN_EXISTING,
            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
            nullptr
        );

        if (m_hDirectory == INVALID_HANDLE_VALUE) 
        {
            throw std::runtime_error("Failed to open directory handle.");
        }
    }

    ~DirectoryHandle() 
    {
        if (m_hDirectory != INVALID_HANDLE_VALUE) 
        {
            CloseHandle(m_hDirectory);
        }
    }

    HANDLE get() const 
    { 
        return m_hDirectory; 
    }
};

void MonitorDirectory(const std::wstring& path) 
{
    try 
    {
        DirectoryHandle dirHandle(path);
        // align_as по dword
        char buffer[1024];
        DWORD bytesReturned;

        while (true) 
        {
            if (ReadDirectoryChangesW
            (
                dirHandle.get(),
                buffer,
                sizeof(buffer),
                TRUE,
                FILE_NOTIFY_CHANGE_FILE_NAME |
                FILE_NOTIFY_CHANGE_DIR_NAME |
                FILE_NOTIFY_CHANGE_SIZE |
                FILE_NOTIFY_CHANGE_ATTRIBUTES,
                &bytesReturned,
                nullptr,
                nullptr
            )) 
            {
                FILE_NOTIFY_INFORMATION* fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

                do 
                {
                    std::wstring filename(fni->FileName, fni->FileNameLength / sizeof(WCHAR));
                    std::wcout << L"File: " << filename << L" - ";

                    switch (fni->Action) {
                    case FILE_ACTION_ADDED:
                        std::wcout << L"Added\n";
                        break;
                    case FILE_ACTION_REMOVED:
                        std::wcout << L"Removed\n";
                        break;
                    case FILE_ACTION_MODIFIED:
                        std::wcout << L"Modified\n";
                        break;
                    case FILE_ACTION_RENAMED_OLD_NAME:
                        std::wcout << L"Renamed from\n";
                        break;
                    case FILE_ACTION_RENAMED_NEW_NAME:
                        std::wcout << L"Renamed to\n";
                        break;
                    default:
                        std::wcout << L"Unknown action\n";
                        break;
                    }

                    if (fni->NextEntryOffset == 0) 
                    {
                        break;
                    }

                    // выяснить для чего преобразование
                    fni = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                        reinterpret_cast<BYTE*>(fni) + fni->NextEntryOffset
                        );

                } while (true);
            }
            else 
            {
                std::cerr << "ReadDirectoryChangesW failed.\n";
                break;
            }
        }
    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << '\n';
    }
}
