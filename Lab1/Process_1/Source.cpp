#include <iostream>
#include <Windows.h>

using namespace std;
int main() {
	STARTUPINFO c;
	ZeroMemory(&c, sizeof(STARTUPINFO));
	PROCESS_INFORMATION pi;
	BOOL start = CreateProcess("E:\\Учёба\\2 курс\\4 семестр\\Операционные системы\\FirstLab\\Debug\\FirstLab2.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &c, &pi);
	if (start) {
		cout << "Work" << endl;
		system("Pause");
	}
	return 0;
}