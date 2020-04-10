
#include <windows.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

CRITICAL_SECTION cr_sec;
int massSize;
int* mass;
HANDLE *hFinishEv, hStartEv, *hThreadsForEnd, hNextStepEvent, hWaitMain;

void marker(int indexMass)
{
	int numThread = indexMass + 1;
	int numberStarred = 0;
	srand(numThread);
	hNextStepEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	SetEvent(hNextStepEvent);
	while (true) {
		WaitForSingleObject(hNextStepEvent, INFINITE);
		int randomNumber = rand() % massSize;
		EnterCriticalSection(&cr_sec); 
		if (mass[randomNumber] == 0) {
			Sleep(5);
			mass[randomNumber] = numThread;
			Sleep(5);
			LeaveCriticalSection(&cr_sec);
			numberStarred++;
		}
		else
		{
			cout << "Serial number: " << numThread << endl;
			cout << "Number of marked items: " << numberStarred << endl;
			cout << "Index of an array element that cannot be marked: " << randomNumber << endl;
			LeaveCriticalSection(&cr_sec);
			SetEvent(hFinishEv[indexMass]);
			WaitForSingleObject(hStartEv, INFINITE);
			ResetEvent(hNextStepEvent);
			if (WaitForSingleObject(hThreadsForEnd[indexMass], 0) == WAIT_OBJECT_0) {
				for (int i = 0; i < massSize; i++) {
					if (mass[i] == numThread) {
						mass[i] = 0;
					}
				}
				SetEvent(hWaitMain);
				break;
			}
		}
		
	}
}

int main()
{
	int theadsCount;
	cout << "Enter the dimension of the array:" << endl;
	cin >> massSize;
	mass = new int[massSize]();
	cout << "Enter the number of threads you want to run:" << endl;
	cin >> theadsCount;
	HANDLE* hThreads = new HANDLE[theadsCount];
	DWORD* IDThread = new DWORD[theadsCount];
	hFinishEv = new HANDLE[theadsCount];
	hThreadsForEnd = new HANDLE[theadsCount];
	InitializeCriticalSection(&cr_sec);
	hStartEv = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hStartEv == NULL)
		return GetLastError();
	hWaitMain = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (hWaitMain == NULL)
		return GetLastError();
	for (int i = 0; i < theadsCount; i++) {
		hFinishEv[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hFinishEv == NULL)
			return GetLastError();
		hThreadsForEnd[i] = CreateEvent(NULL, TRUE, FALSE, NULL);
		if (hThreadsForEnd == NULL)
			return GetLastError();
		hThreads[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)marker, (LPVOID)i, 0, &IDThread[i]);
		if (hThreads[i] == NULL)
			return GetLastError();
	}
	int size = theadsCount;
	bool *notLiveThread = new bool[theadsCount]();
	while (size!=0) {
		if (WaitForMultipleObjects(theadsCount, hFinishEv, TRUE, INFINITE) == WAIT_FAILED)
		{
			cout << "Wait for multiple objects failed." << endl;
			return 1;
		}
		cout << "Array contnts: ";
		for (int i = 0; i < massSize; i++) {
			cout << mass[i] << " ";
		}
		cout << endl << "Enter the serial number of the marker stream, to which a signal will be sent to complete its work: " << endl;
		int numberFinishThread;
		cin >> numberFinishThread;
		numberFinishThread -= 1;
		notLiveThread[numberFinishThread] = true;
		SetEvent(hThreadsForEnd[numberFinishThread]);
		SetEvent(hStartEv);
		WaitForSingleObject(hWaitMain, INFINITE);
		cout << "Array contnts: ";
		for (int i = 0; i < massSize; i++) {
			cout << mass[i] << " ";
		}
		cout << endl;
		for (int i = 0; i < theadsCount; i++) {
			if (!notLiveThread[i])
				ResetEvent(hFinishEv[i]);
		}
		ResetEvent(hStartEv);
		ResetEvent(hWaitMain);
		SetEvent(hNextStepEvent);
		size--;
	}
	DeleteCriticalSection(&cr_sec);
	CloseHandle(hStartEv);
	CloseHandle(hNextStepEvent);
	CloseHandle(hWaitMain);
	for (int i = 0; i < theadsCount; i++) {
		CloseHandle(hFinishEv[i]);
		CloseHandle(hThreadsForEnd[i]);
		CloseHandle(hThreads[i]);
	}
	return 0;
}