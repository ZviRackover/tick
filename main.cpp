#include <windows.h>
#include <iostream>

struct ProcStats {
	double Elpased;
	double User;
	double Kernel;
	DWORD ExitCode;
}; 

template <typename T>
static T convertFILETIME(FILETIME FT) {
	ULARGE_INTEGER TI;
	TI.LowPart = FT.dwLowDateTime;
	TI.HighPart = FT.dwHighDateTime;
	return T(TI.QuadPart) / 10000000;
}

bool ExecuteAndMeasure(const std::string &Command, ProcStats& PS) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	LARGE_INTEGER StartCount, FinishCount;
	QueryPerformanceCounter(&StartCount);

	// Start the child process. 
	if (!CreateProcess(NULL,   // No module name (use command line)
		const_cast<LPSTR>(Command.c_str()),        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)            // Pointer to PROCESS_INFORMATION structure
		)
	{
		std::cerr << "CreateProcess failed (" << GetLastError() << ").\n";
		return false;
	}

	// Wait until child process exits.
	WaitForSingleObject(pi.hProcess, INFINITE);
	
	if (!GetExitCodeProcess(
		pi.hProcess,
		&PS.ExitCode)) {
		std::cerr << "Failed to obtain process exit code.\n";
		return false;
	}

	QueryPerformanceCounter(&FinishCount);

	FILETIME ProcCreate, ProcExit, KernelTime, UserTime;
	if (GetProcessTimes(pi.hProcess, &ProcCreate, &ProcExit, &KernelTime,
		&UserTime) == 0)
		return false;
	// Close process and thread handles.
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	double PCFreq = double(li.QuadPart);

	PS.Elpased = double(FinishCount.QuadPart - StartCount.QuadPart) / PCFreq;
	PS.User = convertFILETIME<double>(UserTime);
	PS.Kernel = convertFILETIME<double>(KernelTime);

	return true;
}


int main(int argc, char* argv[]) {

	if (argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " <command>\n";
		return 1;
	}

	std::string Command;
	for (int I = 1; I != argc; ++I) {
		Command += argv[I];
		Command += " ";
	}

	ProcStats PS;
	if (!ExecuteAndMeasure(Command, PS)) {
		std::cerr << "Failed to execute\n";
		return 1;
	}

	std::cout << "user: " << PS.User << "s\n";
	std::cout << "kernel: " << PS.Kernel << "s\n";
	std::cout << "Elapsed: " << PS.Elpased << "s\n";

	return PS.ExitCode;
}
