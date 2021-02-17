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

struct Emp {
	string id; //8 bytes
	string name; // <= 200 bytes
	string bio; // <= 500 bytes
	string manager; //8 bytes
};

struct Bucket {
	int id; //same as line of file
	FILE* pFile; //pointer to block in file
	int numEmps;
};

vector<Emp> readFromFile(string );
string stringToBinary(string );
string leastSigBits(int , string );
// string binaryToDec(string );
string bitFlip(string );
void allocateBlock(Bucket );
bool checkToFlipBits(Emp , vector<Bucket> , int );
void storeRecord(Emp , vector<Bucket>& , bool , int );
void newBucket(vector<Bucket>& , FILE* );
void writeToBlock(Emp , vector<Bucket>& , int );

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
	FILE* pFile = fopen("EmployeeIndex.txt", "w+");
	vector<Bucket> bucketArray;
	Bucket bucket;
	bucket.id = 0;
	bucket.pFile = pFile;
	bucket.numEmps = 0;
	bucketArray.push_back(bucket);
	allocateBlock(bucketArray[index]);

	
	vector<Emp> employees = readFromFile("Employees.csv");



	for(index = 0; index < 10; index++) {
		cout << "========== " << index << " ==========" << endl;
		cout << "EMPLOYEES: " << employees[index].id << endl;

		bool flipBitsBool = checkToFlipBits(employees[index], bucketArray, i);
		storeRecord(employees[index], bucketArray, flipBitsBool, i);
	}


	fclose(pFile);
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
	string manager_newline;
	while(getline(file, emp.id, ',')) {
		getline(file, emp.name, ',');
		getline(file, emp.bio, ',');
		getline(file, manager_newline);

		emp.manager = manager_newline.erase(manager_newline.length() - 1);

		data.push_back(emp);
		// data.push_back(emp);
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
void allocateBlock(Bucket bucket) {
	cout << "EMPLOYEE INDEX ::::::::: " << bucket.pFile << endl;
	for(int i = 0; i < 4096; i++) {
		fputs("#", bucket.pFile);
	}
	fputs("\n", bucket.pFile);
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
void storeRecord(Emp emp, vector<Bucket> &bucketArray, bool flipBitsBool, int i) {
	string binaryEmpId = stringToBinary(emp.id);
	string leastSigEmpId = leastSigBits(i, binaryEmpId);
	string binaryEmpIdCpy = binaryEmpId;
	// string block;

	if(flipBitsBool == true) {
		cout << "bits have been flipped" << endl;
		binaryEmpIdCpy = bitFlip(leastSigEmpId);
	}

	cout << "least sig emp id: " << leastSigEmpId << endl;
	cout << "BUCKET SIZE: " << bucketArray.size() << endl;


	for(int j = 0; j < bucketArray.size(); j++) {
		string binaryBucketId = stringToBinary(to_string(bucketArray[j].id));
		string leastSigBucketBits = leastSigBits(i, binaryBucketId);

		//if we flip the bits use the new flipped bit value
		//otherwise use the original value
		if((leastSigBucketBits == binaryEmpIdCpy && flipBitsBool == true) || (leastSigBucketBits == leastSigEmpId && flipBitsBool == false)) {
			cout << "NUM EMPS: " << bucketArray[j].numEmps << endl;
			if(bucketArray[j].numEmps <= 5) {
				writeToBlock(emp, bucketArray, j);
			} else {
				cout << "NEW BUCKET" << endl;
				newBucket(bucketArray, bucketArray[j].pFile);
				//then write to new bucket
			}
		} else {
			cout << "Bucket not found" << endl;
		}
	}

}

void newBucket(vector<Bucket> &bucketArray, FILE* fp) {
		//allocate new bucket
		Bucket bucket;
		bucket.id = bucketArray.size()-1;
		bucket.pFile = fp;
		bucket.numEmps = 0;
		bucketArray.push_back(bucket);
		allocateBlock(bucketArray[bucketArray.size()-1]);
		// index++;
		// allocateBlock(bucketArray[index]);
		// writeToBlock(emp, bucketArray, index);
		//increment index
		//write to that

}

void writeToBlock(Emp emp, vector<Bucket> &bucketArray, int index) {
	cout << "BUCKET ID TO WRITE TO: " << bucketArray[index].id << endl;
	cout << "FILE POINTER: " << bucketArray[index].pFile << endl;

	//4096 bytes + newline 
	//when file is open it is set to eof
	//this sets fp to 4097*bucketArray[index].id bytes from beginning
	fseek(bucketArray[index].pFile, 4097*bucketArray[index].id, SEEK_SET);
	string block(4096, '\n');

	//set fp to 4096 past 4097*bucketArray[index].id
	fread(&block[0], sizeof(char), 4096, bucketArray[index].pFile);
	// cout << "===========block BEFORE============\n" << block << endl;

	//need to overwrite string
	//string.replace
	string employeeRecord;
	employeeRecord.append(emp.id);
	employeeRecord.append(emp.name);
	employeeRecord.append(emp.bio);
	employeeRecord.append(emp.manager);
	employeeRecord.append(", ");
	cout << "LENGHT: " << employeeRecord.length() << endl;
	while(employeeRecord.length() != 716) {
		employeeRecord.append(" ");
	}
	

	// cout << "================EMP RECORD=============\n";
	// cout << (emp.id).length() << endl;
	// cout << (emp.name).length() << endl;
	// cout << (emp.bio).length() << endl;
	// cout << (emp.manager).length() << endl;

	cout << employeeRecord << endl;

	block.replace(bucketArray[index].numEmps * 716, 716, employeeRecord);

	fseek(bucketArray[index].pFile, 4097*bucketArray[index].id, SEEK_SET);
	fwrite(block.c_str(), sizeof(char), 4096, bucketArray[index].pFile);
	fflush(bucketArray[index].pFile);

	bucketArray[index].numEmps++;
	
	// cout << "===========block============\n" << block << endl; 
	
}


//find which bucket to insert to
//read in block to memory
//add the id and check for overflow
//overwrite 


