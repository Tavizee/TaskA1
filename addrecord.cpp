#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // Check for minimum arguments
    if (argc < 9) {
        cerr << "Usage: addrecord.exe -db <filename> -sid <SID> -name <Name> -enrollments <Enrollments> -grades <Grades> -phone <Phone>" << endl;
        return EXIT_FAILURE;
    }

    string filename;
    string sid;
    string name;
    string enrollments;
    string grades;
    string phone;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        if (string(argv[i]) == "-db") {
            filename = argv[++i];
        } else if (string(argv[i]) == "-sid") {
            sid = argv[++i];
        } else if (string(argv[i]) == "-name") {
            name = argv[++i];
        } else if (string(argv[i]) == "-enrollments") {
            enrollments = argv[++i];
        } else if (string(argv[i]) == "-grades") {
            grades = argv[++i];
        } else if (string(argv[i]) == "-phone") {
            phone = argv[++i];
        } else {
            cerr << "Unknown argument: " << argv[i] << endl;
            return EXIT_FAILURE;
        }
    }

    // Check if file exists
    ifstream infile(filename);
    if (!infile) {
        cerr << "Error: Cannot open file " << filename << endl;
        return EXIT_FAILURE;
    }
    infile.close();

    // Append the new record to the file
    ofstream outfile(filename, ios::app);
    if (!outfile) {
        cerr << "Error: Could not write to file " << filename << endl;
        return EXIT_FAILURE;
    }

    outfile << "SID: " << sid << endl;
    outfile << "NAME: " << name << endl;
    outfile << "ENROLLMENTS: " << enrollments << endl;
    outfile << "GRADES: " << grades << endl;
    outfile << "PHONE: " << phone << endl;
    outfile << endl;

    outfile.close();

    cout << "Record added successfully." << endl;

    return EXIT_SUCCESS;
}
