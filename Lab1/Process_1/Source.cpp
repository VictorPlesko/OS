#include <iostream>
#include <Windows.h>

using namespace std;
int main() {
	STARTUPINFO c;
	ZeroMemory(&c, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	BOOL start = CreateProcess("E:\\�����\\2 ����\\4 �������\\������������ �������\\FirstLab\\Debug\\FirstLab2.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &c, &pi);
	if (start) {
		cout << "Work" << endl;
		system("Pause");
	}
	return 0;
}