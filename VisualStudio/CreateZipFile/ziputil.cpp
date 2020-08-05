#include <vector>
#include <string>
#include <fstream>

#include <stdlib.h>

#include "zip.h"

#include "ziputil.h"
#include "filectrl.h"

// プリプロセッサをビルド時に合わせる
#pragma comment(lib, "zlibstat.lib")


ZipUtil::ZipUtil()
{}

ZipUtil::~ZipUtil()
{}

bool ZipUtil::CompressDirectory(const char *dir_path, const char *output_path, const char *password)
{
	/* Default Config */
	int compress_level = Z_DEFAULT_COMPRESSION;
	int is_raw = 0; // 1: write raw file
	int window_bits = MAX_WBITS;
	int mem_level = DEF_MEM_LEVEL;
	int strategy = Z_DEFAULT_STRATEGY;

	std::vector<std::string> file_list;
	FileCtrl::GetFileList(dir_path, file_list);

	zipFile zf;
	zf = zipOpen(output_path, 0);

	for (int i = 0; i < file_list.size(); i++) {
		const char *filename = file_list[i].c_str();
		char abs_file_path[MAX_PATH] = { 0 };
		sprintf_s(abs_file_path, "%s\\%s", dir_path, filename);
		size_t buf_size = 0;
		byte *buf = FileCtrl::ReadFile(abs_file_path, buf_size);

		zip_fileinfo zi;
		zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =	zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
		zi.dosDate = FileCtrl::GetFileDosDateTime(abs_file_path);
		zi.internal_fa = 0;
		zi.external_fa = 0;
		unsigned long crc = 0;
		if (password != NULL) {
			crc = crc32(0, buf, buf_size);
		}
		zipOpenNewFileInZip3(
			zf, filename, &zi,
			NULL, 0, NULL, 0, NULL, Z_DEFLATED, compress_level, is_raw,
			window_bits, mem_level, strategy,
			password, crc);

		zipWriteInFileInZip(zf, buf, buf_size);

		zipCloseFileInZip(zf);
	}

	zipClose(zf, NULL);
	return true;
}

bool ZipUtil::CompressDirectory(const wchar_t *dir_path, const wchar_t *output_path, const wchar_t *password)
{
	char dir_path_c[MAX_PATH] = { 0 };
	char output_path_c[MAX_PATH] = { 0 };
	char password_c[MAX_PASSWD_LEN] = { 0 };
	size_t s1,s2,s3;
	wcstombs_s(&s1, dir_path_c, MAX_PATH, dir_path, MAX_PATH);
	wcstombs_s(&s2, output_path_c, MAX_PATH, output_path, MAX_PATH);
	wcstombs_s(&s3, password_c, MAX_PASSWD_LEN, password, MAX_PASSWD_LEN);
	bool ret = CompressDirectory(dir_path_c, output_path_c, password_c);
	return ret;
}