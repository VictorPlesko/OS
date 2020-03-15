#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include "..\Main\Header.h"

using namespace std;


bool closeAllHandle(PROCESS_INFORMATION pi) {
	if (!CloseHandle(pi.hProcess) | !CloseHandle(pi.hThread)) {
		return false;
	}
	return true;
}

bool printBinFile(char nameBinFile[256]) {
	HANDLE binFileHandle = CreateFile(nameBinFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (binFileHandle == INVALID_HANDLE_VALUE) {
		cout << "Invalid handle." << endl;
		CloseHandle(binFileHandle);
		system("Pause");
		return false;
	}
	employee emp;
	DWORD dwBytesRead = 0;
	ReadFile(binFileHandle, &emp, sizeof(employee), &dwBytesRead, NULL);
	cout << "\tID\t\tName\t\tHours" << endl;
	do {

		cout << "\t" << emp.num << "\t\t" << emp.name << "\t\t" << emp.hours << endl;
		ReadFile(binFileHandle, &emp, sizeof(employee), &dwBytesRead, NULL);
	} while (dwBytesRead == sizeof(employee));
	CloseHandle(binFileHandle);
	return true;
}

void printReportFile(char nameReportFile[256]) {
	ifstream i(nameReportFile);
	string out;
	while (!i.eof()) {
		getline(i, out);
		cout << out << endl;
	}
	i.close();
}

int main() {
	char nameBinFile[256],nameReportFile[256], buffer[256];
	cout << "Enter the name of the binary file and the number of entries in it." << endl;
	int a = 0;
	cin >> nameBinFile >> a;
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	cif.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;
	sprintf_s(buffer, "\"%s\" %d 3", nameBinFile, a);
	CreateProcess("E:\\Учёба\\2 курс\\4 семестр\\Операционные системы\\Lab2\\Debug\\Creator.exe", buffer, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
    if(pi.hProcess == INVALID_HANDLE_VALUE) {
		cout << "Invalid handle." << endl;
		closeAllHandle(pi);
		system("Pause");
		return 1;
	}
	WaitForSingleObject(pi.hProcess, INFINITE);
	//int out;
	//GetExitCodeProcess(pi.hProcess, (LPDWORD)(&out));
	//cout << out << "Error" << endl;
	cout << "Creator work is completed." << endl << endl;
	if (!printBinFile(nameBinFile)) {
		closeAllHandle(pi);
		return 2;
	}
	closeAllHandle(pi);
	double payPerHour = 0;
	cout << "Enter the name of the report file and pay per hour." << endl;
	cin >> nameReportFile >> payPerHour;
	sprintf_s(buffer, "\"%s\" \"%s\" %f", nameBinFile,nameReportFile,payPerHour);
	CreateProcess("E:\\Учёба\\2 курс\\4 семестр\\Операционные системы\\Lab2\\Debug\\Reporter.exe", buffer, NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi);
	WaitForSingleObject(pi.hProcess, INFINITE);
	cout << "Reporter work is completed." << endl << endl;
	printReportFile(nameReportFile);
	closeAllHandle(pi);
	system("Pause");
	return 0;
}