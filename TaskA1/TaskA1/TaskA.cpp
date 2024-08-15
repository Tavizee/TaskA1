#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <regex>
#include <map>
#include <string>

using namespace std;

class FileHandler {
public:
    string fileName;
    vector<string> lines;

    FileHandler(const string& file) : fileName(file) {}

    bool openFile() {
        ifstream fileStream(fileName);
        if (!fileStream.is_open()) {
            cerr << "Error: Could not open file " << fileName << endl;
            return false;
        }

        string line;
        while (getline(fileStream, line)) {
            lines.push_back(line);
        }
        fileStream.close();
        return true;
    }

    void displayContent() const {
        for (const auto& line : lines) {
            cout << line << endl;
        }
    }
};

class Search {
public:
    string searchTerm;
    vector<pair<int, int>> matches;

    Search(const string& term) : searchTerm(term) {}

    void performSearch(const vector<string>& lines, bool isRegex) {
        regex regexPattern;
        if (isRegex) {
            regexPattern = regex(searchTerm);
        }

        for (int i = 0; i < lines.size(); ++i) {
            istringstream iss(lines[i]);
            string word;
            int wordIndex = 0;
            while (iss >> word) {
                if (isRegex && regex_match(word, regexPattern)) {
                    matches.emplace_back(i + 1, wordIndex + 1);
                }
                else if (!isRegex && word == searchTerm) {
                    matches.emplace_back(i + 1, wordIndex + 1);
                }
                ++wordIndex;
            }
        }
    }

    void displayResults() const {
        for (const auto& match : matches) {
            cout << "Line " << match.first << ", Word " << match.second << endl;
        }
    }
};

class Result {
public:
    int totalWords = 0;
    int hitCount = 0;

    void calculateStatistics(const vector<string>& lines, const vector<pair<int, int>>& matches) {
        for (const auto& line : lines) {
            istringstream iss(line);
            string word;
            while (iss >> word) {
                ++totalWords;
            }
        }
        hitCount = matches.size();
        double percentage = (double(hitCount) / totalWords) * 100;
        cout << "Search hits: " << hitCount << " (" << percentage << "% of total words)" << endl;
    }

    void saveResults(const string& fileName, const string& searchTerm, const string& resultFileName = "results.csv") const {
        ofstream resultFile(resultFileName, ios::app);
        if (!resultFile.is_open()) {
            cerr << "Error: Could not open result file " << resultFileName << endl;
            return;
        }

        double percentage = (double(hitCount) / totalWords) * 100;
        resultFile << fileName << "," << searchTerm << "," << percentage << "%" << endl;
        resultFile.close();
    }
};

int findArg(int argc, char* argv[], const string& pattern) {
    for (int n = 1; n < argc; n++) {
        string s1(argv[n]);
        if (s1 == pattern) {
            return n;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Error: Missing required parameters." << endl;
        return EXIT_FAILURE;
    }

    string fileName(argv[1]);
    string searchTerm;
    bool isRegex = false;

    // Handle various argument orderings
    if (argc == 4) {
        if (string(argv[3]) == "-regex") {
            searchTerm = argv[2];
            isRegex = true;
        }
        else if (string(argv[2]) == "-regex") {
            searchTerm = argv[3];
            isRegex = true;
        }
        else {
            cerr << "Error: Invalid arguments." << endl;
            return EXIT_FAILURE;
        }
    }
    else {
        searchTerm = argv[2];
    }

    FileHandler fileHandler(fileName);
    if (!fileHandler.openFile()) {
        return EXIT_FAILURE;
    }

    fileHandler.displayContent();

    Search search(searchTerm);
    search.performSearch(fileHandler.lines, isRegex);
    search.displayResults();

    Result result;
    result.calculateStatistics(fileHandler.lines, search.matches);
    result.saveResults(fileName, searchTerm);

    return EXIT_SUCCESS;
}
