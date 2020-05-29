#pragma once

#include <string>

typedef struct employee
{
	int num;
	char name[10];
	double hours;
} employee;

typedef struct record {
	employee emp;
	int recordKey;
	bool readOrWrite;
} record;
