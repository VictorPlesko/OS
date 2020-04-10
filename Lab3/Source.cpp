
#include <windows.h>
#include <iostream>
using namespace std;
int _size;
int max = INT_MIN, min = INT_MAX;
int averageElement = 0;
CRITICAL_SECTION cr_sect;
void min_max(int *mass) {
	for (int i = 0; i < _size; i++) {
		if (mass[i] < min) {
			min = mass[i];
		}
		Sleep(7);
		if (mass[i] > max) {
			max = mass[i];
		}
		Sleep(7);
	}
	EnterCriticalSection(&cr_sect);
	cout << "Minimum element: " << min << endl;
	cout << "Max element: " << max << endl;
	LeaveCriticalSection(&cr_sect);
}

void average(int *mass) {
	for (int i = 0; i < _size; i++) {
		averageElement += mass[i];
		Sleep(12);
	}
	EnterCriticalSection(&cr_sect);
	cout << "Mean: " << (double)averageElement / _size << endl;
	LeaveCriticalSection(&cr_sect);
	averageElement /= _size;
}

int main() {
	cout << "Enter the size of the array:" << endl;
	cin >> _size;
	int *mass = new int[_size];
	int elm;
	for (int i = 0; i < _size; i++) {
		cout << "Enter array element number " << i + 1 << ":"<< endl;
		cin >> elm;
		mass[i] = elm;
	}
	HANDLE hThreadMinMax, hThreadAverage;
	DWORD IDThreadMinMax, IDThreadAverage;
	InitializeCriticalSection(&cr_sect);
	hThreadMinMax = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)min_max, mass, 0, &IDThreadMinMax);
	hThreadAverage = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)average, mass, 0, &IDThreadAverage);
	if (hThreadMinMax == NULL || hThreadAverage == NULL) {
		return GetLastError();
	}
	WaitForSingleObject(hThreadMinMax, INFINITE);
	WaitForSingleObject(hThreadAverage, INFINITE);
	DeleteCriticalSection(&cr_sect);
	CloseHandle(hThreadMinMax);
	CloseHandle(hThreadAverage);
	cout << "Mass: ";
	for (int i = 0; i < _size; i++) {
		if (mass[i] == max || mass[i] == min) {
			mass[i] = averageElement;
		}
		cout << mass[i] << " ";
	}
	cout << endl;
	delete[] mass;
	system("Pause");
	return 0;
}