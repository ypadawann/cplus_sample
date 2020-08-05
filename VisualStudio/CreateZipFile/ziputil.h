#pragma once

class ZipUtil
{
#define MAX_PASSWD_LEN 128
public:
	ZipUtil();
	~ZipUtil();

	bool CompressDirectory(const char *dir_path, const char *output_path, const char *password);
	bool CompressDirectory(const wchar_t *dir_path, const wchar_t *output_path, const wchar_t *password);
};