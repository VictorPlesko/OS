#include <iostream>
#include <Windows.h>
#include <string>
#include <fstream>

using namespace std;

void clearFile(string nameBinFile) {
	ofstream clear(nameBinFile, ios::binary);
	clear.close();
}

bool readAndClearFile(string nameBinFile) {
	ifstream i(nameBinFile, ios::binary);
	string out;
	bool firstIter = false;
	while (!i.eof()) {
		getline(i, out);
		if (out == "" && !firstIter) {
			i.close();
			clearFile(nameBinFile);
			return false;
		}
		firstIter = true;
		cout << out << endl;
	}
	i.close();
	clearFile(nameBinFile);
	return true;
}

int main() {
	STARTUPINFO cif;
	ZeroMemory(&cif, sizeof(STARTUPINFO));
	cif.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION* pi;
	bool work = true;

	string nameBinFile;
	int numberRecords;
	cout << "Enter the name of the binary file" << endl;
	cin >> nameBinFile;
	cout << "Enter the number of entries in the binary file" << endl;
	cin >> numberRecords;
	ofstream bin(nameBinFile, ios::binary);
	bin.close();

	int numberProcess;
	cout << "Enter the number of Sender processes" << endl;
	cin >> numberProcess;

	HANDLE	hMutex;
	hMutex = CreateMutex(NULL, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Create mutex failed." << endl;
		return GetLastError();
	}

	HANDLE hWrite = CreateEvent(NULL, TRUE, FALSE, "Write");
	if (hWrite == NULL) {
		cout << "Invalid handle." << endl;
		CloseHandle(hMutex);
		CloseHandle(hWrite);
		return GetLastError();
	}

	pi = new PROCESS_INFORMATION[numberProcess];
	HANDLE* hEventForStart = new HANDLE[numberProcess];

	for (int i = 0; i < numberProcess; i++) {
		string buf = nameBinFile;
		buf.append(" ");
		CreateProcess("E:\\Учёба\\2 курс\\4 семестр\\Операционные системы\\Lab4Pobeg(Lab5)\\Debug\\Sender.exe"
			,(LPSTR)buf.append(to_string(i)).append(" ").append(to_string(numberRecords)).c_str(),
			NULL, NULL, FALSE, NULL, NULL, NULL, &cif, &pi[i]);
		if (pi[0].hProcess == INVALID_HANDLE_VALUE) {
			cout << "Invalid handle." << endl;
			for (int j = 0; j < i; j++) {
				CloseHandle(pi[j].hProcess);
				CloseHandle(hEventForStart[j]);
			}
			CloseHandle(hMutex);
			CloseHandle(hWrite);
			return GetLastError();
		}
		hEventForStart[i] = CreateEvent(NULL, TRUE, FALSE, to_string(i).c_str());
		if (hEventForStart == NULL) {
			cout << "Invalid handle." << endl;
			for (int j = 0; j < i; j++) {
				CloseHandle(pi[j].hProcess);
				CloseHandle(hEventForStart[j]);
			}
			CloseHandle(hWrite);
			CloseHandle(hMutex);
			return GetLastError();
		}
	}

	WaitForMultipleObjects(numberProcess, hEventForStart, TRUE, INFINITE);

	while (true) {
		if (work) {
			WaitForSingleObject(hMutex,INFINITE);
			int choice;
			cout << "Enter 1 to continue or 0 to complete shut down." << endl;
			cin >> choice;
			if (choice == 0) {
				break;
			}
			if (!readAndClearFile(nameBinFile)) {
				work = false;
			}
			SetEvent(hWrite);
			for (int i = 0; i < numberProcess; i++) {
				if (WaitForSingleObject(pi[i].hProcess, 0) != WAIT_OBJECT_0) {
					ResetEvent(hEventForStart[i]);
				}
			}
			ReleaseMutex(hMutex);
		}
		else {
			cout << "No records!" << endl;
			WaitForMultipleObjects(numberProcess, hEventForStart, TRUE, INFINITE);
			work = true;
		}
	}
	CloseHandle(hMutex);
	CloseHandle(hWrite);
	for (int i = 0; i < numberProcess; i++) {
		CloseHandle(pi[i].hProcess);
		CloseHandle(hEventForStart[i]);
	}
	return 0;
}