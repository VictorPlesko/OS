#include <iostream>
#include<Windows.h>
#include<stdio.h>
#include "..\Main\Header.h"

using namespace std;


bool addNote(HANDLE fileHandle, employee emp) {
	DWORD dwBytesWritten = 0;
	if (!WriteFile(fileHandle, &emp, sizeof(employee), &dwBytesWritten, NULL)) {
		return false;
	}
	return true;
}

int main(int args, char* argv[]) {
	if (args != 2) {
		cout << "Invalid arguments. Enter the file name and number of lines." << endl;
		return 1;
	}
	LPCTSTR nameFile = argv[0];
	int count = atoi(argv[1]);
	HANDLE fileHandle = CreateFile(nameFile,GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE) {
		cout << "Invalid handle." << endl;
		CloseHandle(fileHandle);
		system("Pause");
		return 2;
	}
	int employeeCount = 1;
	for (int i = 0; i < count; i++) {
		cout << "Employee #" << employeeCount++ << endl;
		employee emp;
		cout << "Enter the employee ID:" << endl; 
		cin >> emp.num;
		cout << "Enter employee name:" << endl;
		cin >> emp.name;
		cout << "Enter the number of hours worked:" << endl;
		cin >> emp.hours;
		if (!addNote(fileHandle, emp)) {
			cout << "Error writing data to file." << endl;
			CloseHandle(fileHandle);
			system("Pause");
			return 3;
		}
	}
	CloseHandle(fileHandle);
	return 0;
}
