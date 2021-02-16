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
//outfile.open("test.txt", ios::out);


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
void allocateBlock(Bucket );
bool checkToFlipBits(Emp , vector<Bucket> , int );
void storeRecord(Emp , vector<Bucket> , bool , int);

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
	// ofstream outfile ("test.txt");
	vector<Bucket> bucketArray;
	Bucket bucket;
	bucket.id = 0;
	bucket.pFile = pFile;
	bucketArray.push_back(bucket);
	allocateBlock(bucketArray[index]);


	//check the size of bucket to see if we can write emp to that bucket
		//if less than 4096 then write to the bucket
		//otherwise flag for overflow

	vector<Emp> employees = readFromFile("Employees.csv");


	for(index = 0; index < 10; index++) {
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
	string manager_newline;
	while(getline(file, emp.id, ',')) {
		getline(file, emp.name, ',');
		getline(file, emp.bio, ',');
		getline(file, manager_newline);

		emp.manager = manager_newline.erase(manager_newline.length() - 1);

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
void allocateBlock(Bucket bucket) {
	// cout << "****************** WE HAVE ALLOCATED A NEW BLOCK" << endl;
	for(int i = 0; i < 4096; i++) {
		fputs(" ", bucket.pFile);
		// outfile << " ";
	}

	fseek(bucket.pFile, 4096 * bucket.id, SEEK_CUR);

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

	unsigned long long int leastSigEmpBitsDec = stoull(leastSigEmpBits, nullptr, 2);

	for(int j = 0; j < bucketArray.size(); j++) {
		string binaryBucketId = stringToBinary(to_string(bucketArray[j].id));
		string leastSigBucketBits = leastSigBits(i, binaryBucketId);

		unsigned long long int leastSigBucketBitsDec = stoull(leastSigBucketBits, nullptr, 2);

		if(leastSigBucketBitsDec == leastSigEmpBitsDec) {
			// cout <<"NO BITS FLIPPED" << endl;
			cout << "LEAST SIG EMP BITS: " << leastSigEmpBitsDec << endl;
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

	unsigned long long int leastSigEmpIdDec = stoull(leastSigEmpId, nullptr, 2);
	unsigned long long int binaryEmpIdDec = stoull(binaryEmpId, nullptr, 2);


	if(flipBitsBool == true) {
		cout << "bits have been flipped" << endl;
		binaryEmpIdCpy = bitFlip(leastSigEmpId);
	}

	unsigned long long int binaryEmpIdCpyDec = stoull(binaryEmpIdCpy, nullptr, 2);
	

	cout << "least sig emp id: " << leastSigEmpId << endl;
	for(int j = 0; j < bucketArray.size(); j++) {
		string binaryBucketId = stringToBinary(to_string(bucketArray[j].id));
		string leastSigBucketBits = leastSigBits(i, binaryBucketId);

		unsigned long long int leastSigBucketBitsDec = stoull(leastSigBucketBits, nullptr, 2);

		//if we flip the bits use the new flipped bit value
		//otherwise use the original value

		ofstream outfile("test.txt");
		FILE* tempFP = fopen("test.txt", "wb");
		// int ch = getc(tempFP);

		if((leastSigBucketBitsDec == binaryEmpIdCpyDec && flipBitsBool == true) || (leastSigBucketBitsDec == leastSigEmpIdDec && flipBitsBool == false)) {
			cout << "BUCKET ID TO WRITE TO: " << bucketArray[j].id << endl;
			ifstream fileIn;
			fileIn.open("EmployeeIndex.txt");

			//get the line of the file and add the employee to it
			getline(fileIn, block, '#');

			string employeeRecord;

			employeeRecord.append(emp.id + " ");
			employeeRecord.append(emp.name);
			employeeRecord.append(emp.bio);
			employeeRecord.append(emp.manager);


			// erase part of a copy and examine to see if the current block copy plus another 
			// employee record would put it over the edge, 4097 (w/ delimeter). If not, then actually
			// erase the block and add another employee. If it does, output block with new line
			// and another employee record on that new line/bucket.
			//Check how many spaces are left
			int remainingSpaces = 0;

            for(int space_counter = 0; block[space_counter] != '1' && space_counter < block.size(); space_counter++){
                remainingSpaces++;
            }

			cout << "remaining Spaces: " << remainingSpaces << endl; 
			cout << "size of employee record: " << employeeRecord.size() << endl;


			// check if we need to go to next line for new block
			if (remainingSpaces < employeeRecord.size()) {

				Bucket bucket;
				bucket.id = bucketArray.size() + 1;
				bucket.pFile = bucketArray[j].pFile;
				bucketArray.push_back(bucket);
				allocateBlock(bucketArray[j+1]);

				block.erase(0, employeeRecord.size());
				block.append(employeeRecord);
				block.append("\n");

				outfile << block;
			  // next employee record needs to go in newly alloced bucket
				outfile << "#";	 			  // delimeter
			} else { // erase
				block.erase(0, employeeRecord.size());
				block.append(employeeRecord);
				block.append("#");
				cout << "original block: " << block << endl;

				outfile << block;
			}

			


			// cout << "current BLOCK: " << block << endl;
			
			cout << "BLOCK size**: " << block.size() << endl;

			ifstream fileInCopy;

		
			
			outfile.close();
			break;
		}
	}
	// go from test.txt to employeeindex.txt, update it
	ifstream  src("test.txt", std::ios::binary);
    ofstream  dst("EmployeeIndex.txt",   std::ios::binary);

    dst << src.rdbuf();

	src.close();
	dst.close();


}


//find which bucket to insert to
//read in block to memory
//add the id and check for overflow





