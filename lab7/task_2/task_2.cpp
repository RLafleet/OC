#include "DirectoryHandle.h"

int main(int argc, wchar_t* argv[]) 
{
    if (argc != 2) 
    {
        std::wcerr << L"Usage: fsmonw PATH\n";
        return 1;
    }

    std::wstring path = argv[1];
    MonitorDirectory(path);

    return 0;
}
