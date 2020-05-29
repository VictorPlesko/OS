#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>
#include <map>
#include "StructHeader.h"

using namespace std; 

CRITICAL_SECTION cs;
map<int, int> dataIndexFile;
bool *read, *write;
bool clientRead, clientWrite;
string fileName;

void server(LPVOID pipe) {
	HANDLE hPipe = (((HANDLE*)pipe));
	ConnectNamedPipe(hPipe, NULL);

	while (true) {
		record rec;
		ReadFile(hPipe, &rec, sizeof(record), NULL, NULL);

		int indexFileRecord = dataIndexFile.find(rec.recordKey)->second;

		if (rec.readOrWrite == true) {
			EnterCriticalSection(&cs);
			if (!write[indexFileRecord]) {
				read[indexFileRecord] = true;
				LeaveCriticalSection(&cs);
			}
			else {
				LeaveCriticalSection(&cs);
				while (write[indexFileRecord]) { }
				read[indexFileRecord] = true;
			}
			employee emp;
			ifstream fout(fileName, ios::binary);
			fout.seekg(indexFileRecord * sizeof(employee));
			fout.read((char*)&emp, sizeof(employee));
			fout.close();

			rec.emp = emp;
			WriteFile(hPipe, &rec, sizeof(record), NULL, NULL);
			EnterCriticalSection(&cs);
			read[indexFileRecord] = false;
			LeaveCriticalSection(&cs);
		} else if (rec.readOrWrite == false) {
			EnterCriticalSection(&cs);
			if (!write[indexFileRecord] && !read[indexFileRecord]) {
				write[indexFileRecord] = true;
				LeaveCriticalSection(&cs);
			}
			else {
				LeaveCriticalSection(&cs);
				while (write[indexFileRecord] || read[indexFileRecord]) {}
				write[indexFileRecord] = true;
			}
			employee emp;
			ifstream fout(fileName, ios::binary);
			fout.seekg(indexFileRecord * sizeof(employee));
			fout.read((char*)&emp, sizeof(employee));
			fout.close();

			rec.emp = emp;
			WriteFile(hPipe, &rec, sizeof(record), NULL, NULL);
			ReadFile(hPipe, &rec, sizeof(record), NULL, NULL);
			
			emp = rec.emp;
			ofstream fin(fileName, ios::binary || ios::app);
			fin.seekp(indexFileRecord * sizeof(employee));
			fin.write((char*)&emp, sizeof(employee));
			fin.close();
			
			EnterCriticalSection(&cs);
			write[indexFileRecord] = false;
			LeaveCriticalSection(&cs);
		} else{
			return;
		}
	}

}

void fillDataFile(string fileName,int numberRecords) {
	read = new bool[numberRecords];
	write = new bool[numberRecords];
	ofstream fin(fileName, ios::binary);
	for (int i = 0; i < numberRecords; i++) {
		cout << "For " << i + 1 << endl;
		employee emp;
		cout << "Enter id employee: " << endl;
		cin >> emp.num;
		cout << "Enter employee name: " << endl;
		cin >> emp.name;
		cout << "Enter the number of hours worked: " << endl;
		cin >> emp.hours;
		fin.write((char*)&emp, sizeof(employee));
		dataIndexFile[emp.num] = i;
		read[i] = write[i] = false;
	}
	fin.close();
}

void printDataFile(string fileName) {
	ifstream fout(fileName, ios::binary);
	employee emp;
	int count = 1;
	while (fout.read((char*)&emp, sizeof(employee))) {
		cout << "For " << count << ":" <<  endl;
		cout << "Id employee: " << emp.num << endl;
		cout << "Name employee: " << emp.name << endl;
		cout << "Hours employee: " << emp.hours << endl;
		count++;
	}
	cout << endl;
	fout.close();
}

int main() {
	int numberProcess, numberRecords;
	cout << "Enter file name: " << endl;
	getline(cin, fileName);
	cout << "Enter the number of entries" << endl;
	cin >> numberRecords;
	fillDataFile(fileName, numberRecords);
	printDataFile(fileName);

	cout << "Enter the number of processes: " << endl;
	cin >> numberProcess;

	HANDLE *hProcess = new HANDLE[numberProcess];
	HANDLE *hPipeForProcess = new HANDLE[numberProcess];
	HANDLE *hThreadForProcess = new HANDLE[numberProcess];
	
	InitializeCriticalSection(&cs);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	for (int i = 0; i < numberProcess; i++) {
		if (!CreateProcess( "E:\\Учёба\\2 курс\\4 семестр\\Операционные системы\\Lab5_Pobeg(Lab6)\\Debug\\Client.exe" , NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE | PROCESS_SUSPEND_RESUME, NULL,
			NULL, &si, &pi)) {
			CloseHandle(pi.hProcess);
		}
		
		hPipeForProcess[i] = CreateNamedPipe(("\\\\.\\pipe\\PipeForProcess" + to_string(pi.dwProcessId)).c_str(),
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			1,
			0,
			0,
			NMPWAIT_USE_DEFAULT_WAIT,
			NULL);
		hProcess[i] = pi.hProcess;
		hThreadForProcess[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)server, (void*)hPipeForProcess[i], 0, NULL);
	}
	WaitForMultipleObjects(numberProcess, hThreadForProcess, TRUE, INFINITE);

	for (int i = 0; i < numberProcess; i++) {
		CloseHandle(hProcess[i]);
		CloseHandle(hThreadForProcess[i]);
		CloseHandle(hPipeForProcess[i]);
	}

	return 0;
}