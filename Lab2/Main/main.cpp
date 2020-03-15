#include <iostream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdio>
#include "Header.h"

using namespace std;

void printBinFile(char nameBinFile[256]) {
    ifstream i(nameBinFile, ios::binary);
    employee emp;
    string out;
    while(!i.eof()){
        getline(i, out);
        cout << out << endl;
    }
    i.close();
}

void printReportFile(char nameReportFile[256]) {
    ifstream i(nameReportFile);
    string out;
    while (!i.eof()) {
        getline(i, out);
        cout << out << endl;
    }
    i.close();
}

int main() {
    char nameBinFile[256], nameReportFile[256];
    cout << "Enter the name of the binary file and the number of entries in it." << endl;
    int a = 0;
    cin >> nameBinFile >> a;
    pid_t pid = fork();
    int status;
    if (pid < 0) {
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        do {
            pid_t w = wait(&status);
            if (w == -1) {
                exit(EXIT_FAILURE);
            }
        } while (!WIFEXITED(status));
        cout << "Creator work is completed." << endl << endl;
    } else {
        char arr[2] = {char(a+'0'),'\0'};
        execlp("/home/victor/CLionProjects/creator/a.out", "creator", nameBinFile,arr,NULL);
        return 0;
    }
        printBinFile(nameBinFile);

        char payPerHour[256];
        cout << "Enter the name of the report file and pay per hour." << endl;
        cin >> nameReportFile >> payPerHour;
        pid_t pid11 = fork();
        if (pid11 < 0){
            exit(EXIT_FAILURE);
        } else if (pid11 > 0){
            do {
                pid_t w = waitpid(pid11, &status, 0);
                if (w == -1) {
                    exit(EXIT_FAILURE);
                }
            } while (!WIFEXITED(status));
            cout << "Reporter work is completed." << endl << endl;
        } else {
            execlp("/home/victor/CLionProjects/reporter/a.out", "reporter", nameBinFile, nameReportFile,payPerHour,NULL);
            return 0;
        }
        printReportFile(nameReportFile);
        return 0;
}