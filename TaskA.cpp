#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

// Class to represent a Student Record
class StudentRecord {
public:
    string SID;
    string name;
    vector<string> enrollments;
    vector<double> grades;
    string phone;

    void displayRecord() const {
        cout << "SID: " << SID << endl;
        cout << "NAME: " << name << endl;
        cout << "ENROLLMENTS: ";
        for (const auto& course : enrollments) {
            cout << course << " ";
        }
        cout << endl;
        cout << "GRADES: ";
        for (const auto& grade : grades) {
            cout << grade << " ";
        }
        cout << endl;
        cout << "PHONE: " << phone << endl;
        cout << endl;
    }

    void displayField(char flag) const {
        switch (flag) {
            case 'n':
                cout << "NAME: " << name << endl;
                break;
            case 'g':
                cout << "GRADES: ";
                for (const auto& grade : grades) {
                    cout << grade << " ";
                }
                cout << endl;
                break;
            case 'p':
                cout << "PHONE: " << phone << endl;
                break;
            default:
                cout << "Unknown flag: " << flag << endl;
        }
    }
};

// Function to parse the database file and return a list of Student Records
vector<StudentRecord> parseDatabase(const string& filename) {
    vector<StudentRecord> records;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return records;
    }

    StudentRecord record;
    string line;
    while (getline(file, line)) {
        if (line.find("SID:") != string::npos) {
            if (!record.SID.empty()) {
                records.push_back(record);
                record = StudentRecord();
            }
            record.SID = line.substr(line.find(':') + 1);
        } else if (line.find("NAME:") != string::npos) {
            record.name = line.substr(line.find(':') + 1);
        } else if (line.find("ENROLLMENTS:") != string::npos) {
            stringstream ss(line.substr(line.find(':') + 1));
            string course;
            while (ss >> course) {
                record.enrollments.push_back(course);
            }
        } else if (line.find("GRADES:") != string::npos) {
            stringstream ss(line.substr(line.find(':') + 1));
            double grade;
            while (ss >> grade) {
                record.grades.push_back(grade);
            }
        } else if (line.find("PHONE:") != string::npos) {
            record.phone = line.substr(line.find(':') + 1);
        }
    }

    if (!record.SID.empty()) {
        records.push_back(record);
    }

    file.close();
    return records;
}

// Function to find and display a record by SID
bool findRecordBySID(const vector<StudentRecord>& records, const string& sid, char flag = '\0') {
    for (const auto& record : records) {
        if (record.SID.find(sid) != string::npos) {
            if (flag == '\0') {
                record.displayRecord();
            } else {
                record.displayField(flag);
            }
            return true;
        }
    }
    return false;
}

int main(int argc, char* argv[]) {
    // Ensure proper usage of the program
    if (argc < 4 || string(argv[1]) != "-db" || string(argv[3]) != "-sid") {
        cerr << "Usage: TaskA.exe -db <filename> -sid <SID> [-n|-g|-p]" << endl;
        return EXIT_FAILURE;
    }

    string filename = argv[2];
    string sid = argv[4]; // Extract the SID directly
    char flag = '\0';

    if (argc == 6) {
        if (string(argv[5]) == "-n") {
            flag = 'n';
        } else if (string(argv[5]) == "-g") {
            flag = 'g';
        } else if (string(argv[5]) == "-p") {
            flag = 'p';
        } else {
            cerr << "Invalid flag provided. Valid flags are -n, -g, -p." << endl;
            return EXIT_FAILURE;
        }
    }

    vector<StudentRecord> records = parseDatabase(filename);

    // Find and display the record with the given SID and optional flag
    if (!findRecordBySID(records, sid, flag)) {
        cout << "Record with SID " << sid << " not found." << endl;
    }

    return EXIT_SUCCESS;
}

