#pragma once

#include <filesystem>

class File
{
public:
	static std::filesystem::path getExeDirectory();
};
