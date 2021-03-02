#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string> 
#include <cstdint>

using namespace std;

int numEmployees;
int M = 22;

struct Emp {
	string id; // <= 4 bytes
	string ename; // <= 50 bytes
	string age; // <= 4 bytes
	string salary; // <= 8 bytes

	Emp(string id, string ename, string age, string salary) {
		this->id = id;
		this->ename = ename;
		this->age = age;
		this->salary = salary;
	}

};

struct less_than_key
{
    inline bool operator() (const Emp& struct1, const Emp& struct2)
    {
        return (stoi(struct1.id) < stoi(struct2.id));
    }
};

vector<Emp> readEmployees(string);
void doMultipass(vector<Emp>);


int main(int argc, char* argv[]){

	//FILE* fp = fopen("Emp.txt", "w+");
	vector<Emp> employees = readEmployees("Emp.csv");
	doMultipass(employees);
	return 0;
}

/*
 * Open the file in the same directory ("Employees.csv")
 * and store the values of each line in Emp struct
 * then push those values to a vector
 */
vector<Emp> readEmployees(string fileName) {
	vector<Emp> data;

	string ename;
	string age;
	string salary;
	string id;

	//cout << "filname"
	ifstream file(fileName.c_str());
	string salary_newline;
	while(getline(file, id, ',')) {
		numEmployees += 1;

		getline(file, ename, ',');
		getline(file, age, ',');
		getline(file, salary_newline);

		salary = salary_newline.erase(salary_newline.length() - 1);

		Emp emp = Emp(id, ename, age, salary);

		data.push_back(emp);
	}

	file.close();
	return data;
}

void doMultipass(vector<Emp> employees) {


	ofstream myfile;

	// sort by blocks
	int count = 0;

	while (count < 1) {
		// read file again
		vector<Emp> employeesBlock = readEmployees("EmpSorted.csv");
		vector<Emp> employeesToBeSorted;
		myfile.open ("EmpSorted.csv");
		for (int i = 0; i < employeesBlock.size(); i++) {

			int blockSize = M * i;
			employeesToBeSorted.push_back(employeesBlock[i]);
			if (employeesToBeSorted.size() == ((M - 2) * (count + 1))) {
				sort(employeesToBeSorted.begin(), employeesToBeSorted.end(), less_than_key());
				for (int k = 0; k < employeesToBeSorted.size(); k++) {
					
					myfile << employeesToBeSorted[k].id << ", " << employeesToBeSorted[k].ename << ", " << employeesToBeSorted[k].age << ", " << employeesToBeSorted[k].salary << endl;
				}
				employeesToBeSorted.clear();
			}

		}
		count += 1;
		myfile.close();
	}
}