#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Emp {
	string id, name, bio, manager;
};

vector<Emp> readFromFile(string );

int main(){


	vector<Emp> data = readFromFile("Employees.csv");
	for(int i = 0; i < 50; i++) {
		cout << i << ": " << data[i].id << endl;
	}
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







