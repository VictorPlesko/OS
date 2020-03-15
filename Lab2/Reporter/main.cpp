#include <stdio.h>
#include <iostream>
#include <fstream>
#include "../untitled/Header.h"

using namespace std;



int main(int args, char* argv[]) {
    string nameFileBin = argv[1];
    string nameFileReport = argv[2];
    double payPerHour = atof(argv[3]);
    ofstream out (nameFileReport);
    ifstream in (nameFileBin, ios::binary);
    out << "File Report: " << nameFileBin << "\n";
    employee emp;
    while(in.peek() != EOF) {
        in >> emp.num >> emp.name >> emp.hours;
        double salary = emp.hours * payPerHour;
        out << emp.num << " "<< emp.name<< " " << emp.hours << " " << salary << "\n";
        in.ignore();
    }
    in.close();
    out.close();
    return 0;
}
