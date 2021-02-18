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

vector<Emp> readEmployees(string );
string stringToBinary(string );
string leastSigBits(int , string );
string bitFlip(string );
void allocateBlock(Bucket );
bool checkToFlipBits(Emp , vector<Bucket> , int );
void storeRecord(Emp , vector<Bucket>& , bool , int );
void newBucket(vector<Bucket>& , FILE* , int );
void writeToBlock(Emp , vector<Bucket>& , int );
void creation();
int incrementIndex(vector<Bucket>& , int , int );
void lookup(string );
string hashId(string , int );


int main(int argc, char* argv[]){

	switch (*argv[1]) {
        case 'C':
            creation();
            break;
        
        case 'L':
            lookup(argv[2]);
            break;
        
        default:
            cout << "Incorrect Parameters" << endl;
            break;
    }

	return 0;
}


/*
 * Open the file in the same directory ("Employees.csv")
 * and store the values of each line in Emp struct
 * then push those values to a vector
 */
vector<Emp> readEmployees(string fileName) {
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

	file.close();
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
 * This is the hashId function that will be used
 * The return value is the hashId key
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
	fseek(bucket.pFile, 0, SEEK_END);

	for(int i = 0; i < 4096; i++) {
		fputs("#", bucket.pFile);
	}
	fputs("\n", bucket.pFile);
	fseek(bucket.pFile, 0, SEEK_SET);
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

void newBucket(vector<Bucket> &bucketArray, FILE* fp, int index) {
		//allocate new bucket
		Bucket bucket;
		bucket.id = bucketArray.size();
		bucket.pFile = fp;
		bucket.numEmps = 0;
		bucketArray.push_back(bucket);
		allocateBlock(bucketArray[index]);
}

void writeToBlock(Emp emp, vector<Bucket> &bucketArray, int index) {
	// cout << "BUCKET ID TO WRITE TO: " << bucketArray[index].id << endl;

	//4096 bytes + newline 
	//when file is open it is set to eof
	//this sets fp to 4097*bucketArray[index].id bytes from beginning
	fseek(bucketArray[index].pFile, 4097*bucketArray[index].id, SEEK_SET);
	string block(4096, '\n');

	//set fp to 4096 past 4097*bucketArray[index].id
	fread(&block[0], sizeof(char), 4096, bucketArray[index].pFile);
	// if(index == 5) {
	// 	cout << "===========block BEFORE============\n" << block << endl; 
	// }

	//need to overwrite string
	//string.replace
	string employeeRecord;
	employeeRecord.append(emp.id);
	employeeRecord.append(emp.name);
	employeeRecord.append(emp.bio);
	employeeRecord.append(emp.manager);
	employeeRecord.append(", ");
	while(employeeRecord.length() != 716) {
		employeeRecord.append(" ");
	}
	
	// cout << employeeRecord << endl;
	block.replace(bucketArray[index].numEmps * 716, 716, employeeRecord);

	fseek(bucketArray[index].pFile, 4097*bucketArray[index].id, SEEK_SET);
	fwrite(block.c_str(), sizeof(char), 4096, bucketArray[index].pFile);
	fflush(bucketArray[index].pFile);

	bucketArray[index].numEmps++;
	
	
	
}

int incrementIndex(vector<Bucket> &bucketArray, int index, int j) {
	int id = bucketArray[j].id;
	// if(id )
	return id;
}


void creation() {
	FILE* fp = fopen("EmployeeIndex.txt", "w+");
	vector<Emp> employees = readEmployees("Employees.csv");

	//create initial empty bucket
	Bucket initialBucket;
	initialBucket.pFile = fp;
	initialBucket.id = 0;
	initialBucket.numEmps = 0;

	vector<Bucket> bucketArray;
	bucketArray.push_back(initialBucket);
	allocateBlock(bucketArray[0]);



	bool writing = true;
	int index = 4;
	int bucketArrayIndex = 0;
	bool flippedBits = false;

	for(int i = 0; i < 10; i++) {
		newBucket(bucketArray, bucketArray[bucketArrayIndex].pFile, i);
	}

	int i = 0;
	for(int j = 0; j < employees.size(); j++){
		for(int i = 0; i < bucketArray.size(); i++) {
			string binaryEmpId = stringToBinary(employees[j].id);
			string lsbEmpId = leastSigBits(index, binaryEmpId);
			string binaryBucketId = stringToBinary(to_string(bucketArray[i].id));
			string lsbBucketId = leastSigBits(index, binaryBucketId);
			string bitsflipped = bitFlip(lsbEmpId);
			string lsbBitsFlipped = leastSigBits(index, bitsflipped);

			// cout << "BITS FLIPPED LSB: " << bitsflipped << endl;

			// cout <<  "LSB EMP ID: " << lsbEmpId << endl;
			// cout <<  "LSB BUCKET ID: " << lsbBucketId << endl;

			if(lsbEmpId == lsbBucketId && bucketArray[i].numEmps < 5) {
					// cout << "WILL WRITE TO BLOCK" << endl;
				writeToBlock(employees[j], bucketArray, i);
			} else if(lsbBitsFlipped == lsbBucketId && bucketArray[i].numEmps < 5) {
				// cout << "BITS FLIPPED" << endl;
				// bucketArrayIndex++;
				writeToBlock(employees[j], bucketArray, i);
				// i++;
			} else {
				
				// writeToBlock(employees[j], bucketArray, i);
				// cout << "DIDN'T FIND BLOCK TO WRITE TO" << endl;
			}
		}
	}

	fclose(fp);
}

void lookup(string key) {
	string block;

	ifstream file("EmployeeIndex.txt");
	
	while (getline(file, block, '\n')) {
		// block contains key
		// size_t found = block.find(key);
		if (block.find(key) != std::string::npos) {
    		cout << "===========block===========\n" << block << endl;
			break;
		}
	}
	cout << "employee id not found" << endl;

}

string hashId(string id, int i) {
	string bin = stringToBinary(id);
	string leastSigId = leastSigBits(i, bin);

	return leastSigId;
}

