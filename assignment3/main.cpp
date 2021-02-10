#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> readFromFile(string );

int main(){

	vector<string> data = readFromFile("Employees.csv");
	
	return 0;
}

vector<string> readFromFile(string fileName) {
	vector<string> data;
	ifstream file;

	file.open(fileName);

	string id, name, bio, manager;
	int i = 0;

	while(getline(file, id, ',')) {
		getline(file, name, ',');
		getline(file, bio, ',');
		getline(file, manager, '\n');

		data.push_back(id);
		data.push_back(name);
		data.push_back(bio);
		data.push_back(manager);
		// cout << i << ": " << data[i] << endl;
		// i++;
	}

	return data;
}


