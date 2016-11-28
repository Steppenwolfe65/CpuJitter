#include <cstdlib>
#include <iostream>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <stdio.h>
#include "ConsoleUtils.h"
#include "../CpuJitter/Config.h"
#include "../CpuJitter/CJP.h"
#include "../CpuJitter/FileStream.h"

#if defined(CEX_OS_WINDOWS)
#	include <direct.h>
#	define GetCurrentDir _getcwd
#else
#	include <unistd.h>
#	define GetCurrentDir getcwd
#endif

std::string GetCurrentDirectory()
{
	char path[FILENAME_MAX];

	if (!GetCurrentDir(path, sizeof(path)))
		return "";

	return std::string(path);
}

std::string GetResponse()
{
	std::string resp;
	std::getline(std::cin, resp);

	return resp;
}

bool CanTest(std::string Message)
{
	ConsoleUtils::WriteLine(Message);
	std::string resp = GetResponse();
	std::transform(resp.begin(), resp.end(), resp.begin(), ::toupper);

	const std::string CONFIRM = "Y";
	if (resp.find(CONFIRM) != std::string::npos)
		return true;

	return false;
}

template <typename T>
static inline T Min(T A, T B)
{
	return ((A) < (B) ? (A) : (B));
}

void PrintHeader(std::string Data, std::string Decoration = "***")
{
	ConsoleUtils::WriteLine(Decoration + Data + Decoration);
}

void PrintTitle()
{
	ConsoleUtils::WriteLine("*** This file is part of the CEX Cryptographic Library ***");
	ConsoleUtils::WriteLine("");
	ConsoleUtils::WriteLine("**********************************************");
	ConsoleUtils::WriteLine("* CEX++ Version 1.1: CEX Library in C++      *");
	ConsoleUtils::WriteLine("*                                            *");
	ConsoleUtils::WriteLine("* Release:   v1.1m                           *");
	ConsoleUtils::WriteLine("* Date:      November 27, 2016				  *");
	ConsoleUtils::WriteLine("* Contact:   develop@vtdev.com               *");
	ConsoleUtils::WriteLine("**********************************************");
	ConsoleUtils::WriteLine("");
}

void CloseApp()
{
	PrintHeader("An error has occurred! Press any key to close..", "");
	GetResponse();
	exit(0);
}

void CJPGenerateFile(std::string FilePath, size_t FileSize)
{
	CpuJitter::CJP* pvd = new CpuJitter::CJP();
	std::vector<byte> output(1024);
	size_t prcLen = FileSize;
	CpuJitter::FileStream fs(FilePath, CpuJitter::FileStream::FileAccess::Write);
	pvd->EnableDebias() = false;
	pvd->EnableAccess() = false;

	do
	{
		pvd->GetBytes(output);
		size_t rmd = Min(output.size(), prcLen);
		fs.Write(output, 0, rmd);
		prcLen -= rmd;
	} 
	while (prcLen != 0);

	fs.Flush();
	fs.Close();

	delete pvd;
}

int main()
{
	PrintTitle();

	std::string path = GetCurrentDirectory();

	if (path.size() == 0)
	{
		PrintHeader("Could not locate the current directory! Press any key to close..", "");
	}
	else
	{
		path = path + "\\cjp_sample3.txt";
		PrintHeader("Write 10mb of random to a file:", "");
		PrintHeader("Path: " + path, "");

		if (CanTest("Write random to file? Press Y to proceed, any other key to abort"))
		{
			const size_t FILESIZE = 1024 * 1000 * 10;
			CJPGenerateFile(path, FILESIZE);
			PrintHeader("Test completed. Press any key to close..", "");
		}
		else
		{
			PrintHeader("Test aborted. Press any key to close..", "");
		}
		GetResponse();
	}

    return 0;
}

