#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::istringstream;
using std::set;
using std::string;
using std::vector;

struct NumLocation {
    int start;
    int end;
    int column;

    friend inline bool operator<(const NumLocation& lhs, const NumLocation& rhs) {
        return lhs.start < rhs.start || lhs.column < rhs.column;
    }
};

vector<string>* ReadFile(char* filePath) {
    vector<string>* lines = new vector<string>();

    std::ifstream file(filePath);

    string currentLine;
    while (std::getline(file, currentLine)) {
        std::istringstream iss(currentLine);
        lines->push_back(iss.str());
    }

    return lines;
}

int GetNumLen(string line, int start) {
    int pos = start;
    int len = 0;
    while (isdigit(line[pos + 1])) {
        len++;
        pos++;
    }

    return len + 1;
}

set<NumLocation>* AllValidNeighborsNearby(vector<string>* file, int gearX, int gearY, int maxBounds) {
    auto* allNeighborsPos = new set<NumLocation>();

    for (int y = gearY - 1; y <= gearY + 1; y++) {
        if (y < 0 || y > maxBounds) {
            continue;
        }

        for (int x = gearX - 1; x <= gearX + 1; x++) {
            if (x < 0 || x > maxBounds) {
                continue;
            }

            auto line = (*file)[y];
            auto isDigit = isdigit(line[x]);
            if (isDigit) {
                std::cout << line[x] << std::endl;

                int start = line.find_last_not_of("1234567890", x) + 1;
                if (start == string::npos + 1) {
                    start = 0;
                }

                int end = line.find_first_not_of("1234567890", x);
                if (end == string::npos) {
                    end = line.size();
                }

                allNeighborsPos->insert(NumLocation { start, end, y });
                for (auto& asd : *allNeighborsPos) {
                    std::cout << "Start: " << asd.start << ", End: " << asd.end << ", Y: " << asd.column << std::endl;
                }
            }
        }
    }

    return allNeighborsPos;
}

int main (int argc, char *argv[]) {
    auto file = ReadFile(argv[1]);
    auto rows = (*file)[0].size();
    auto columns = file->size();
    const string invalidNeighbors = "#$%&+-/=@."; 

    for (string& i : *file) {
        for (auto& j : invalidNeighbors) {
            std::replace(i.begin(), i.end(), j, ' ');
        }
        std::cout << i << std::endl;
    }

    // rows ------
    // columns |
    //         |
    //         |

    int sumOfAll = 0;
    for (int y = 0; y < file->size(); y++) {
        std::cout << "Line number: " << y + 1 << std::endl;

        for (int x = 0; x < (*file)[y].size(); x++) {
            char c = (*file)[y][x];

            if (c == '*') {
                auto nearbys = AllValidNeighborsNearby(file, x, y, rows);
                if (nearbys->size() < 2) {
                    continue;
                }

                int productOfNeighbors = 1;
                for (auto& pos : *nearbys) {
                    string substr = (*file)[pos.column].substr(pos.start, pos.end - pos.start);
                    std::cout << substr << std::endl;
                    int num = std::stoi(substr);
                    productOfNeighbors *= num;
                }

                sumOfAll += productOfNeighbors;
                std::cout << "Product of each gear: " << productOfNeighbors << std::endl;
            }
        }
        std::cout << std::endl;
    }

    std::cout << "The sum of everything is: " << sumOfAll << std::endl;

    return 0;
}
