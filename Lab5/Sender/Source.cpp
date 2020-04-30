#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

int getQuantity(string nameBinFile) {
	ifstream i(nameBinFile, ios::binary);
	string out;
	int count = 0;
	while (!i.eof()) {
		getline(i, out);
		if (out == "") {
			i.close();
			return count;
		}
		count++;
	}
	i.close();
	return count;
}

int main(int args, char* argv[]) {
	//system("Pause");
	if (args != 3) {
		cout << "Invalid arguments." << endl;
		return 1;
	}
	bool work = true;
	
	HANDLE	hMutex, hStartEvent;
	hMutex = OpenMutex(SYNCHRONIZE, FALSE, "Mutex");
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		return GetLastError();
	}
	hStartEvent = OpenEvent(EVENT_MODIFY_STATE, FALSE, argv[1]);
	if (hMutex == NULL)
	{
		cout << "Open mutex failed." << endl;
		return GetLastError();
	}
	HANDLE hWrite = OpenEvent(EVENT_MODIFY_STATE, FALSE, "Write");
	if (hWrite == NULL)
	{
		return GetLastError();
	}
	SetEvent(hStartEvent);
	int choice;
	while (true) {
		if (work) {
			WaitForSingleObject(hMutex, INFINITE);
			cout << "Enter 1 to write to the file or 0 to exit" << endl;
			cin >> choice;
			if (choice == 0) {
				break;
			}
			if (getQuantity(argv[0]) < atoi(argv[2])) {
				ofstream fin(argv[0], ios::binary | ios::app);
				while (true) {
					string text;
					cout << "Enter the string:" << endl;
					cin >> text;
					if (text.length() < 20) {
						fin << text << endl;
						fin.close();
						break;
					}
					else {
						cout << "Message length is less than 20 characters." << endl;
					}
				}
			}
			else {
				work = false;
			}
			SetEvent(hStartEvent);
			ResetEvent(hWrite);
			ReleaseMutex(hMutex);
		}
		else {
			cout << "The binary file is already filled with messages." << endl;
			WaitForSingleObject(hWrite, INFINITE);
			work = true;
		}
	}
	SetEvent(hStartEvent);
	CloseHandle(hMutex);
	CloseHandle(hStartEvent);
	CloseHandle(hWrite);
	return 0;
}