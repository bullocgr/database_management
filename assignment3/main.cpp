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

FILE* filePointer;

struct Emp {
	string id; //8 bytes
	string name; // <= 200 bytes
	string bio; // <= 500 bytes
	string manager; //8 bytes
};

struct Bucket {
	int id; //same as line of file
	int pFile; //pointer to block in file
	int bytes; //initialize to 4096 bytes
};

vector<Emp> readFromFile(string );
string stringToBinary(string );
string leastSigBits(int , string );
// string binaryToDec(string );
string bitFlip(string );

int main(){

	//done:
	// get the employee id
	// convert id to binary
	// get LSB
	// check to see which bucket it goes into
	// flip bits if necessary
	// store which bucket in local variable
	// convert id back to decimal *might be able to just grab original id value
	// place in bucket with stored local variable by writing id to file
	// calculate how many bits were used up in the bucket 
	// store that to local variable and check that it's less than 4096


	//to do:
	// make dynamic 
	// overflow
	// lookup functionality 
		// print out bucket and employee


	int i = 1;
	int n = 1;
	filePointer = fopen("EmployeeIndex.txt", "wb");
	vector<Emp> data = readFromFile("Employees.csv");

	vector<Bucket> bucketArray;
	Bucket bucket;
	
	for(int i = 0; i < 3; i++) {
		bucket.id = i;
		bucket.bytes = 0;
		bucket.pFile = fseek(filePointer, 4096*i, SEEK_SET);
		bucketArray.push_back(bucket);

		string binaryId = stringToBinary(data[i].id);
		string LSB = leastSigBits(i, binaryId);

		//check to see if bucket is full before inputting next emp
		string bitFlipString;
		//might need to be more iterable
		if(to_string(bucketArray[i].id) != LSB) {
			// cout << "NEED TO BIT FLIP" << endl;
			bitFlipString = bitFlip(LSB);
		}

		if(to_string(bucketArray[i].id) == bitFlipString) {
			fwrite(data[i].id.c_str(), sizeof(char), sizeof(data[i].id), filePointer);
		}


		if(bucket.bytes + sizeof(data[i]) > 4096) {
			//overflow
		} else {
			bucket.bytes += sizeof(data[i]);
		}
	}

	
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
 * Take in the LSB and return a flipped bit string
 */
string bitFlip(string leastSigBits) {
	string flippedBits;
	string firstBit = "1";
	size_t found = leastSigBits.find(firstBit); //index of first bit
	flippedBits = leastSigBits;
	flippedBits[found] = '0';
	
	return flippedBits;
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
string leastSigBits(int i, string binary) {
	string leastBits = binary.substr(binary.size() - i, i);
	return leastBits;
}




/*
 * Converts a binary string to its decimal value
 * Ex: Input 1001 -> Output 9
 */
// int binaryToDec(string n) {
// 	string num = n;
// 	int dec_value = 0;

// 	// Initializing base value to 1, i.e 2^0
// 	int base = 1;

// 	int len = num.length();
// 	for (int i = len - 1; i >= 0; i--) {
// 		if (num[i] == '1')
// 		dec_value += base;
// 		base = base * 2;
// 	}

// 	return dec_value;
// }




/* WHAT WE HAVE
 * read in the file
 * store in struct
 * access the id
 * convert id to binary
 * get the least significant bits for bit flipping using n
 * conver the binary id back to decimal value
*/



/*TO DO:
 * bucket vector<id(index), 
 * place in bucket
 * overflow / splitting bucket
 * size calculation > 80%
*/





