#include <iostream>
#include <string>
#include <Windows.h>
#include "..\\Lab5_Pobeg(Lab6)\StructHeader.h"

using namespace std;

void printRecord(record rec) {
	cout << "Information about the employee: " << endl;
	cout << "Id employee: " << rec.emp.num << endl;
	cout << "Name employee: " << rec.emp.name << endl;
	cout << "Hours employee: " << rec.emp.hours << endl;
}

record getRecordKey(bool readOrWrite) {
	cout << "Enter record key" << endl;
	int recordKey;
	cin >> recordKey;

	record rec;
	rec.recordKey = recordKey;
	rec.readOrWrite = readOrWrite;
	return rec;
}

int main() {
	int processID = GetCurrentProcessId();
	HANDLE hNamedPipe = CreateFile(("\\\\.\\pipe\\PipeForProcess" + to_string(processID)).c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	while (true) {
		int choice;
		cout << "What do you want to do?" << endl;
		cout << "\t* 1 - to modify the file" << endl;
		cout << "\t* 2 - to read a file" << endl;
		cout << "\t* 0 - to exit" << endl;
		cout << "Your choice: ";
		cin >> choice;

		switch (choice)
		{
		case 0:
			record recExit = getRecordKey(NULL);
			WriteFile(hNamedPipe, &recExit, sizeof(record), NULL, NULL);
			CloseHandle(hNamedPipe);
			return 0;
		case 1:
			record recWrite = getRecordKey(false);

			WriteFile(hNamedPipe, &recWrite, sizeof(record), NULL, NULL);
			ReadFile(hNamedPipe, &recWrite, sizeof(record), NULL, NULL);

			printRecord(recWrite);

			employee emp;
			emp.num = recWrite.emp.num;
			cout << "Enter employee name: " << endl;
			cin >> emp.name;
			cout << "Enter the number of hours worked: " << endl;
			cin >> emp.hours;

			recWrite.emp = emp;
			WriteFile(hNamedPipe, &recWrite, sizeof(record), NULL, NULL);
			break;
		case 2:
			record recRead = getRecordKey(true);

			WriteFile(hNamedPipe, &recRead, sizeof(record), NULL, NULL);
			ReadFile(hNamedPipe, &recRead, sizeof(record), NULL, NULL);

			printRecord(recRead);

			break;
		default:
			cout << "Invalid command. Try again!" << endl;
			break;
		}
	}
}