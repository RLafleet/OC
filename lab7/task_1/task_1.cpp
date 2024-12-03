#include "Indentation.h"

int main(int argc, char* argv[]) 
{
    std::filesystem::path dir = (argc > 1) ? argv[1] : std::filesystem::current_path();

    std::cout << dir.filename().string() << '\n';
    PrintDirectoryTree(dir);
    return 0;
}
