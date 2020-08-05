#include <fstream>
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include "filectrl.h"

bool FileCtrl::GetFileList(const char *dir_path, std::vector<std::string> &list, const char *relative_prefix)
{
	char searched_name[MAX_PATH + 1] = { 0 };
	WIN32_FIND_DATAA find_file_data;

	sprintf_s(searched_name, MAX_PATH + 1, "%s\\*", dir_path);
	HANDLE handle = FindFirstFileA(searched_name, &find_file_data);
	do {
		char *file_name = find_file_data.cFileName;
		if (strcmp(file_name, ".") == 0 || strcmp(file_name, "..") == 0) {
			continue;
		}
		char relative_file_path[MAX_PATH + 1] = { 0 };
		if (relative_prefix != NULL) {
			sprintf_s(relative_file_path, MAX_PATH, "%s\\%s", relative_prefix, file_name);
		}
		else {
			sprintf_s(relative_file_path, MAX_PATH, "%s", file_name);
		}

		char abs_file_path[MAX_PATH + 1] = { 0 };
		sprintf_s(abs_file_path, MAX_PATH + 1, "%s\\%s", dir_path, file_name);
		if (PathIsDirectoryA(abs_file_path)) {
			FileCtrl::GetFileList(abs_file_path, list, relative_file_path);
		}
		else {
			list.push_back(relative_file_path);
		}
	} while (FindNextFileA(handle, &find_file_data));
	FindClose(handle);

	return true;
}

byte* FileCtrl::ReadFile(const char *path, size_t &size)
{
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs) {
		return NULL;
	}
	ifs.seekg(0, std::ios::end);
	size = ifs.tellg();
	ifs.seekg(0, std::ios::beg);
	byte *data = (byte*)calloc(size + 1, sizeof(byte));
	ifs.read((char*)data, size);
	return data;
}

int FileCtrl::GetFileDosDateTime(const char *path)
{
	int dos_date_time = 0;
	WORD fat_date = 0, fat_time = 0;
	FILETIME file_time;
	WIN32_FIND_DATAA find_data;
	HANDLE hFile = FindFirstFileA(path, &find_data);
	if (hFile == INVALID_HANDLE_VALUE) return 0;
	FileTimeToLocalFileTime(&(find_data.ftLastWriteTime), &file_time);
	FileTimeToDosDateTime(&file_time, ((LPWORD)&dos_date_time) + 1, ((LPWORD)&dos_date_time) + 0);
	FindClose(hFile);
	return dos_date_time;
}