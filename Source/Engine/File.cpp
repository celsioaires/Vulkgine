#include "File.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

std::filesystem::path File::getExeDirectory()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);

    return std::filesystem::path(buffer).parent_path();
}
