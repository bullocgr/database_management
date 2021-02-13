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
	FILE* pFile; //pointer to block in file
};

vector<Emp> readFromFile(string );
string stringToBinary(string );
string leastSigBits(int , string );
// string binaryToDec(string );
string bitFlip(string );
void allocateBucket(Bucket );
bool checkToFlipBits(Emp , vector<Bucket> , int );
void storeRecord(Emp , vector<Bucket> , bool , int );

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
	// finish function storeRecord()
	// track bytes input after bucket is filled
	// overflow shit good luck

	int i = 1;
	int index = 0;
	FILE* pFile = fopen("EmployeeIndex.txt", "wb");
	vector<Bucket> bucketArray;
	Bucket bucket;
	bucket.id = 0;
	bucket.pFile = pFile;
	bucketArray.push_back(bucket);
	allocateBucket(bucketArray[index]);


	//check the size of bucket to see if we can write emp to that bucket
		//if less than 4096 then write to the bucket
		//otherwise flag for overflow

	vector<Emp> employees = readFromFile("Employees.csv");


	for(index = 0; index < 5; index++) {
		cout << "========== " << index << " ==========" << endl;
		cout << "EMPLOYEES: " << employees[index].id << endl;

		bool flipBitsBool = checkToFlipBits(employees[index], bucketArray, i);
		storeRecord(employees[index], bucketArray, flipBitsBool, i);
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
 * Allocate space in a file for 4096 bytes
 * and then 10 more bytes for overflow
 * as well as a '#' for deliminating
 */
void allocateBucket(Bucket bucket) {
	for(int i = 0; i < 4096; i++) {
		fputs(" ", bucket.pFile);
	}
	fseek(bucket.pFile, 4096, SEEK_CUR);

	fputs("#", bucket.pFile);
}


/*
 * Check if the bits need to be flipped
 * return false if bits don't need to be flipped
 * return true if bits need to be flipped
 */
bool checkToFlipBits(Emp emp, vector<Bucket> bucketArray, int i) {
	string binaryEmpId = stringToBinary(emp.id);
	string leastSigEmpBits = leastSigBits(i, binaryEmpId);

	for(int j = 0; j < bucketArray.size(); j++) {
		string binaryBucketId = stringToBinary(to_string(bucketArray[j].id));
		string leastSigBucketBits = leastSigBits(i, binaryBucketId);

		if(leastSigBucketBits == leastSigEmpBits) {
			// cout <<"NO BITS FLIPPED" << endl;
			cout << "LEAST SIG EMP BITS: " << leastSigEmpBits << endl;
			return false;
		}

		//Issue: bit flipping happens more than it should
		//Need to do a full loop and then flip bits and then check again
	}

	return true;
}


/*
 * If bits need to be flipped then flip the bits
 * and use that value to check bucket id for indexing
 * otherwise skip bit flipping and just check for the bucket id
 */
void storeRecord(Emp emp, vector<Bucket> bucketArray, bool flipBitsBool, int i) {
	string binaryEmpId = stringToBinary(emp.id);
	string leastSigEmpId = leastSigBits(i, binaryEmpId);
	string binaryEmpIdCpy = binaryEmpId;
	string block;

	if(flipBitsBool == true) {
		cout << "bits have been flipped" << endl;
		binaryEmpIdCpy = bitFlip(leastSigEmpId);
	}

	cout << "least sig emp id: " << leastSigEmpId << endl;
	for(int j = 0; j < bucketArray.size(); j++) {
		string binaryBucketId = stringToBinary(to_string(bucketArray[j].id));
		string leastSigBucketBits = leastSigBits(i, binaryBucketId);

		//if we flip the bits use the new flipped bit value
		//otherwise use the original value
		if((leastSigBucketBits == binaryEmpIdCpy && flipBitsBool == true) || (leastSigBucketBits == leastSigEmpId && flipBitsBool == false)) {
			cout << "BUCKET ID TO WRITE TO: " << bucketArray[j].id << endl;
			ifstream file;
			file.open("EmployeeIndex.txt");

			//get the line of the file and add the employee to it
			getline(file, block, '#');
			cout << "BLOCK: " << block << endl;
			block.append(emp.id);
			// fseek(bucketArray[j].pFile, 0, SEEK_SET);
			fputs(block.c_str(), bucketArray[j].pFile);
//THIS IS WHERE I LEFT OFF
			//will need to check for byte size too
			break;
		}
	}
}


//find which bucket to insert to
//read in block to memory
//add the id and check for overflow





