#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::endl;
using std::istringstream;
using std::stoi;
using std::string;
using std::vector;

int main(int argc, char *argv[]) {
    std::map<string, int>map = {
        {"0", 0},
        {"1", 1},
        {"2", 2},
        {"3", 3},
        {"4", 4},
        {"5", 5},
        {"6", 6},
        {"7", 7},
        {"8", 8},
        {"9", 9},
        {"zero", 0},
        {"one", 1},
        {"two", 2},
        {"three", 3},
        {"four", 4},
        {"five", 5},
        {"six", 6},
        {"seven", 7},
        {"eight", 8},
        {"nine", 9},
    };

    vector<std::string> words;

    std::ifstream file(argv[1]);

    string line;
    while (std::getline(file, line)) {
        istringstream iss(line);
        words.push_back(iss.str());
    }

    vector<string> numbers;

    for (const string &word : words) {
        int first = 0;

        int pos = 999999999;
        for (auto& val : map) {
            int success = 999999999;
            success = word.find(val.first);

            if (success != std::string::npos && success < pos) {
                first = val.second;
                pos = success;
            }
        }

        numbers.push_back(std::to_string(first));
    }

    int i = 0;
    for (const string &word : words) {
        int last = 0;

        int pos = 0;
        for (auto& val : map) {
            int success = 0;
            success = word.rfind(val.first);

            if (success != std::string::npos && success >= pos) {
                last = val.second;
                pos = success;
            }
        }

        numbers[i] = numbers[i].append(std::to_string(last));
        i++;
    }

    int linessss = 0;
    int sum = 0;
    for (auto &asd : numbers) {
        sum += stoi(asd);
        linessss++;
    }
    std::cout << sum << endl;
    std::cout << linessss << endl;

    return 0;
}
