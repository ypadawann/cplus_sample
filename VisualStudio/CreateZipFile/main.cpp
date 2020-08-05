/*
include directory: zlib; minilib
Library directory: $(ProjectDir)zlib\$(PlatformTarget)
*/

#include "ziputil.h"

int main()
{
	ZipUtil ziputil;
	const wchar_t *dir_path = L"files";
	const wchar_t *output_path = L"test.zip";
	const wchar_t *password = L"password";
	ziputil.CompressDirectory(dir_path, output_path, password);
	return 0;
}