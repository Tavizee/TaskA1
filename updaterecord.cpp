#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

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

// Function to save the updated database to the file
void saveDatabase(const vector<StudentRecord>& records, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not write to file " << filename << endl;
        return;
    }

    for (const auto& record : records) {
        file << "SID: " << record.SID << endl;
        file << "NAME: " << record.name << endl;
        file << "ENROLLMENTS: ";
        for (const auto& course : record.enrollments) {
            file << course << " ";
        }
        file << endl;
        file << "GRADES: ";
        for (const auto& grade : record.grades) {
            file << grade << " ";
        }
        file << endl;
        file << "PHONE: " << record.phone << endl;
        file << endl;
    }

    file.close();
}

int main(int argc, char* argv[]) {
    // Check for minimum arguments
    if (argc < 7) {
        cerr << "Usage: updaterecord.exe -db <filename> -sid <SID> [-name <New Name>] [-enrollments <New Enrollments>]" << endl;
        return EXIT_FAILURE;
    }

    string filename;
    string sid;
    string newName;
    string newEnrollments;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-db") {
            filename = argv[++i];
        } else if (string(argv[i]) == "-sid") {
            sid = argv[++i];
        } else if (string(argv[i]) == "-name") {
            newName = argv[++i];
        } else if (string(argv[i]) == "-enrollments") {
            newEnrollments = argv[++i];
        } else {
            cerr << "Unknown or unsupported argument: " << argv[i] << endl;
            return EXIT_FAILURE;
        }
    }

    vector<StudentRecord> records = parseDatabase(filename);

    // Check if file exists
    if (records.empty()) {
        cerr << "Error: Cannot open database file " << filename << endl;
        return EXIT_FAILURE;
    }

    // Update the record with the given SID
    bool updated = false;
    for (auto& record : records) {
        if (record.SID == sid) {
            if (!newName.empty()) {
                record.name = newName;
            }
            if (!newEnrollments.empty()) {
                stringstream ss(newEnrollments);
                string course;
                record.enrollments.clear();
                while (ss >> course) {
                    record.enrollments.push_back(course);
                }
            }
            updated = true;
            break;
        }
    }

    if (!updated) {
        cout << "No record found with SID " << sid << endl;
        return EXIT_FAILURE;
    }

    saveDatabase(records, filename);

    cout << "Record updated successfully." << endl;

    return EXIT_SUCCESS;
}
