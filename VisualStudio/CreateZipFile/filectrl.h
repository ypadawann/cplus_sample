#pragma once
#include <string>
#include <vector>
#include <windows.h>

class FileCtrl
{
public:
	static bool GetFileList(const char *dir_path, std::vector<std::string> &list, const char *relative_prefix = NULL);
	static byte* ReadFile(const char *path, size_t &size);
	static int GetFileDosDateTime(const char *path);
};