#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <string.h> 

using namespace std;

struct Emp {
	string id; //8 bytes
	string name; // <= 200 bytes
	string bio; // <= 500 bytes
	string manager; //8 bytes
};

struct Bucket {

};

vector<Emp> readFromFile(string );
string stringToBinary(string );
int leastSigBits(int , string );
int binaryToDec(string );

int main(){

	vector<Emp> data = readFromFile("Employees.csv");
	string temp = stringToBinary(data[0].id);
	leastSigBits(2, temp);
	

	return 0;
}


/*
 * Open the file in the same directory ("Employees.csv")
 * and store the values of each line in Emp struct
 * then push those values to a vector
 */
vector<Emp> readFromFile(string fileName) {
	struct Emp emp;
	vector<Emp> data;

	ifstream file(fileName.c_str());

	while(getline(file, emp.id, ',')) {
		getline(file, emp.name, ',');
		getline(file, emp.bio, ',');
		getline(file, emp.manager, '\n');

		data.push_back(emp);
	}

	return data;
}


/*
 * Convert the string given to binary
 * This is the hash function that will be used
 * The return value is the hash key
 */
string stringToBinary(string a) {
	int empId = 0;
	istringstream ss(a);
	ss >> empId;

	uint b = (uint)empId;
	string binary = "";
	uint mask = 0x80000000u;
	while (mask > 0) {
		binary += ((b & mask) == 0) ? '0' : '1';
		mask >>= 1;
	}
	return binary;
}



/*
 * Gets the least significant bits of a binary string
 * then converts the least significant bits to the decimal value
 * Ex: Input (string)100011 -> (string)11 -> (int)3
 * i is the amount of bits taken from the original binary string 
 */
int leastSigBits(int i, string binary) {
	string leastBits = binary.substr(binary.size() - i, i);
	int leastBitsInt = binaryToDec(leastBits);

	return leastBitsInt;
}


/*
 * Converts a binary string to its decimal value
 * Ex: Input 1001 -> Output 9
 */
int binaryToDec(string n) {
	string num = n;
	int dec_value = 0;

	// Initializing base value to 1, i.e 2^0
	int base = 1;

	int len = num.length();
	for (int i = len - 1; i >= 0; i--) {
		if (num[i] == '1')
		dec_value += base;
		base = base * 2;
	}

	return dec_value;
}




/* WHAT WE HAVE
 * read in the file
 * store in struct
 * access the id
 * convert id to binary
 * get the least significant bits for bit flipping using n
*/

/*TO DO:
 * place in bucket
 * overflow / splitting bucket
 * size calculation > 80%
*/





