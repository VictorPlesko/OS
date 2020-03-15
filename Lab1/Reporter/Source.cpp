#include <stdio.h>
#include <iostream>
#include <Windows.h>
#include "..\Main\Header.h"

using namespace std;



int main(int args, char* argv[]) {
	if (args != 3) {
		cout << "Invalid arguments. Enter the file name and number of lines." << endl;
		system("Pause");
		return 1;
	}
	LPCTSTR nameFileBin = argv[0];
	LPCTSTR nameFileReport = argv[1];
	char buffer[256];
	double payPerHour = atof(argv[2]);
	HANDLE binFileHandle = CreateFile(nameFileBin, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (binFileHandle == INVALID_HANDLE_VALUE) {
		cout << "Invalid handle." << endl;
		CloseHandle(binFileHandle);
		system("Pause");
		return 2;
	}
	HANDLE reportFileHandle = CreateFile(nameFileReport, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	if (reportFileHandle == INVALID_HANDLE_VALUE) {
		cout << "Invalid handle." << endl;
		CloseHandle(reportFileHandle);
		system("Pause");
		return 3;
	}
	sprintf_s(buffer, "File Report: %s \n", nameFileBin);
	DWORD dwBytesToWrite = (DWORD)strlen(buffer);
	DWORD dwBytesWritten = 0;
	WriteFile(reportFileHandle, buffer, dwBytesToWrite, &dwBytesWritten, NULL);
	employee emp;
	DWORD dwBytesRead = 0;
	ReadFile(binFileHandle, &emp, sizeof(employee), &dwBytesRead, NULL);
	do {
		double salary = emp.hours * payPerHour;
		sprintf_s(buffer, "\t %d \t %s \t %f \t %f \n", emp.num, emp.name, emp.hours, salary);
		dwBytesToWrite = (DWORD)strlen(buffer);
		WriteFile(reportFileHandle, buffer, dwBytesToWrite, &dwBytesWritten, NULL);
		ReadFile(binFileHandle, &emp, sizeof(employee), &dwBytesRead, NULL);
	} while (dwBytesRead == sizeof(employee));
	CloseHandle(binFileHandle);
	CloseHandle(reportFileHandle);
	return 0;
}