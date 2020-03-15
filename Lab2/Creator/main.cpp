#include <iostream>
#include<stdio.h>
#include "../untitled/Header.h"
#include <fstream>

using namespace std;


int main(int args, char* argv[]) {
	string  nameFile = argv[1];
	int count = atoi(argv[2]);
    ofstream out(nameFile, ios::binary);
    for (int i = 0; i < count; i++) {
        cout << "Employee #" << i + 1 << endl;
        employee emp;
        cout << "Enter the employee ID:" << endl;
        cin >> emp.num;
        cout << "Enter employee name:" << endl;
        cin >> emp.name;
        cout << "Enter the number of hours worked:" << endl;
        cin >> emp.hours;
        out << emp.num<< " " << emp.name << " " <<  emp.hours << "\n";
    }
    out.close();
    return 0;
}
