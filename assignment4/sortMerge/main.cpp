#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

using namespace std;

struct EmpBlock {
	int eid;
	string ename;
	int age;
	double salary;
};

struct DeptBlock {
	int did;
	string dname;
	double budget;
	int managerid;
};

EmpBlock grabEmp(fstream& );
DeptBlock grabDept(fstream& );
void printJoin(EmpBlock , DeptBlock , fstream& );
void sortMerge(vector<EmpBlock>& , vector<DeptBlock>& );
bool compareEmpIds(const EmpBlock& , const EmpBlock& );
bool compareDeptIds(const DeptBlock& , const DeptBlock& );
bool advanceLeft(vector<DeptBlock>& , int );
bool advanceRight(vector<EmpBlock>& , int );
int getNumLines(string );
void sort(DeptBlock* , int );
void sort(EmpBlock* , int );

int main() {
	fstream empin;
	empin.open("Emp.csv", ios::in);
	int empNumLines = getNumLines("Emp.csv");

	fstream deptin;
	deptin.open("Dept.csv", ios::in);
	int deptNumLines = getNumLines("Dept.csv");

	EmpBlock empRes;
	EmpBlock empBuffer[21];
	DeptBlock deptRes;
	DeptBlock deptBuffer[21];

	int index = 0;
	int deptRunFileCount = 0;
	while(!deptin.eof()) {
		deptBuffer[index++] = grabDept(deptin);		
		if(index >= 21) {
			sort(deptBuffer, 21);
			fstream f("deptRun" + to_string(deptRunFileCount), ios::trunc | ios::out);
			for(int i = 0; i < 21; i++) {
				f << deptBuffer[i].did << "," << deptBuffer[i].dname << "," << deptBuffer[i].budget << "," << deptBuffer[i].managerid << endl;
			}
			f.close();

			index = 0;

			deptRunFileCount++;
		}
	}

	if(index > 0) {
		sort(deptBuffer, index - 1);
		fstream f("deptRun" + to_string(deptRunFileCount), ios::trunc | ios::out);
		for(int i = 0; i < index - 1; i++) {
			f << deptBuffer[i].did << "," << deptBuffer[i].dname << "," << deptBuffer[i].budget << "," << deptBuffer[i].managerid << endl;
		}
		f.close();

		index = 0;

		deptRunFileCount++;
	}



	index = 0;
	int empRunFileCount = 0;
	while(!empin.eof()) {
		empBuffer[index++] = grabEmp(empin);		
		if(index >= 21) {
			sort(empBuffer, 21);
			fstream f("empRun" + to_string(empRunFileCount), ios::trunc | ios::out);
			for(int i = 0; i < 21; i++) {
				f << empBuffer[i].eid << "," << empBuffer[i].ename << "," << empBuffer[i].age << "," << empBuffer[i].salary << endl;
			}
			f.close();

			index = 0;

			empRunFileCount++;
		}
	}

	if(index > 0) {
		sort(empBuffer, index - 1);
		fstream f("empRun" + to_string(empRunFileCount), ios::trunc | ios::out);
		for(int i = 0; i < index - 1; i++) {
			f << empBuffer[i].eid << "," << empBuffer[i].ename << "," << empBuffer[i].age << "," << empBuffer[i].salary << endl;
		}
		f.close();

		index = 0;

		empRunFileCount++;
	}


	empin.clear();
	empin.seekg(0);
	deptin.clear();
	deptin.seekg(0);



	vector<EmpBlock> empVec;
	while((empRes = grabEmp(empin)).eid != -1) {
		empVec.push_back(empRes);
	}

	vector<DeptBlock> deptVec;
	while((deptRes = grabDept(deptin)).did != -1) {
		deptVec.push_back(deptRes);
	}
	sortMerge(empVec, deptVec);

	return 0;
}


// Grab a single block from the emp.csv file, in theory if a block was larger than
// one tuple, this function would return an array of EmpBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
EmpBlock grabEmp(fstream &empin) {
    string line, word;
    EmpBlock emp;
    // grab entire line
    if (getline(empin, line, '\n')) {
        // turn line into a stream
        stringstream s(line);
        // gets everything in stream up to comma
        getline(s, word,',');
        emp.eid = stoi(word);
        getline(s, word, ',');
        emp.ename = word;
        getline(s, word, ',');
        emp.age = stoi(word);
        getline(s, word, ',');
        emp.salary = stod(word);
        numEmployees += 1;
        return emp;
    } else {
        emp.eid = -1;
        return emp;
    }
}

// Grab a single block from the dept.csv file, in theory if a block was larger than
// one tuple, this function would return an array of DeptBlocks and the entire if
// and else statement would be wrapped in a loop for x times based on block size
DeptBlock grabDept(fstream &deptin) {
	string line, word;
	DeptBlock dept;
	if (getline(deptin, line, '\n')) {
		stringstream s(line);
		getline(s, word,',');
		dept.did = stoi(word);
		getline(s, word, ',');
		dept.dname = word;
		getline(s, word, ',');
		dept.budget = stod(word);
		getline(s, word, ',');
		dept.managerid = stoi(word);
		return dept;
	} else {
		dept.did = -1;
		return dept;
	}
}

//Print out the attributes from emp and dept when a join condition is met
void printJoin(EmpBlock emp, DeptBlock dept, fstream &fout) {
	fout << dept.did << ',' << dept.dname << ','
		<< dept.budget << ',' << emp.eid << ',' << emp.ename << ',' << emp.age << ','
		<< emp.salary <<   "\n";
}


//emp buffer //dept buffer
void sortMerge(vector<EmpBlock>& emp, vector<DeptBlock>& dept) {

	fstream outfile;
	outfile.open("Join.csv", ios::out | ios::trunc);

	sort(emp.begin(), emp.end(), compareEmpIds);
	sort(dept.begin(), dept.end(), compareDeptIds); //sort dept based off did (left)

	int empIterator = 0;
	int deptIterator = 0;
	int mark = -1;

	//when emp size and dept
	while(deptIterator < dept.size() && empIterator < emp.size()) {
		if(mark == -1) {
			while(emp[empIterator].eid < dept[deptIterator].managerid) {
				//overwrite
				empIterator++;
			}
			while (emp[empIterator].eid > dept[deptIterator].managerid) {
				//overwrite at empiterator
				deptIterator++;
			}

			mark = deptIterator;
		}

		if (emp[empIterator].eid == dept[deptIterator].managerid) {
			printJoin(emp[empIterator], dept[deptIterator], outfile);
			deptIterator++;
		} else {
			deptIterator = mark;
			empIterator++;
			mark = -1;
		}
	}
}

bool compareEmpIds(const EmpBlock& a, const EmpBlock& b) {
	return a.eid < b.eid;
}

bool compareDeptIds(const DeptBlock& a, const DeptBlock& b) {
	return a.managerid < b.managerid;
}

bool advanceLeft(vector<DeptBlock>& a, int position) {
	if(position == a.size() - 1 || position == -1) {
		position = -1;
		return false;
	} else {
		position++;
		return true;
	}
}

bool advanceRight(vector<EmpBlock>& a, int position) {
	if(position == a.size() - 1 || position == -1) {
		position = -1;
		return false;
	} else {
		position++;
		return true;
	}
}

int getNumLines(string fileName) {
	int numLines = 0;
	string line;
	ifstream inFile;
	inFile.open(fileName);

	while(getline(inFile, line)) {
		++numLines;
	}
	return numLines;
}



//we read in 22 blocks
//sort and merge those 

void sort(DeptBlock* buf, int len) {
	for (int i = 0; i < len - 1; i++) {
		int min_index = i;
		for (int j = i + 1; j < len; j++) {
			if (buf[j].managerid < buf[min_index].managerid) {
				min_index = j;
			}
		}
		// swap
		if (min_index != i) {
			DeptBlock tmp = buf[i];
			buf[i] = buf[min_index];
			buf[min_index] = tmp;
		}
	}
}

void sort(EmpBlock* buf, int len) {
	for (int i = 0; i < len - 1; i++) {
		int min_index = i;
		for (int j = i + 1; j < len; j++) {
			if (buf[j].eid < buf[min_index].eid) {
				min_index = j;
			}
		}
		// swap
		if (min_index != i) {
			EmpBlock tmp = buf[i];
			buf[i] = buf[min_index];
			buf[min_index] = tmp;
		}
	}
}
