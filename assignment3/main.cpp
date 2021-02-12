#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <bits/stdc++.h> 

using namespace std;

struct Emp {
	string id; //8 bytes
	string name; // <= 200 bytes
	string bio; // <= 500 bytes
	string manager; //8 bytes
	//will always be < 1016 bytes tho
};

vector<Emp> readFromFile(string );
void hash(string );
string intToBytes(int );


int main(){


	vector<Emp> data = readFromFile("Employees.csv");
	hash(data[0].id);
	
	return 0;
}

vector<Emp> readFromFile(string fileName) {
	struct Emp emp;
	vector<Emp> data;

	ifstream file(fileName.c_str());

	string id, name, bio, manager;
	int i = 0;

	while(getline(file, emp.id, ',')) {
		getline(file, emp.name, ',');
		getline(file, emp.bio, ',');
		getline(file, emp.manager, '\n');

		data.push_back(emp);
	}

	return data;
}

void hash(string id) {
	int empId = 0;
	istringstream ss(id);
	ss >> empId;

	int temp;

	temp = intToBytes(empId);

	for(int i = 0; i < 2; i++) {
		cout << temp[i];
	}


}

int intToBytes(int paramInt) {
	int num = n;
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int temp = num;
	while (temp) {
		int last_digit = temp % 10;
		temp = temp / 10;

		dec_value += last_digit * base;

		base = base * 2;
	}

	return dec_value;
}







