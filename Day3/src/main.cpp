#include <cctype>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::istringstream;
using std::string;
using std::vector;

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

bool IsValidNeighbor(char c) {
    // const string invalidNeighbors = "1234567890.";
    // for (auto& invalid : invalidNeighbors) {
    //     if (c == invalid) {
    //         return false;        
    //     }
    // }
    //
    // std::cout << "Valid neighbor " << c << " found!" << endl << endl;
    // return true;

    const string validNeighbors = "#$%&*+-/=@";
    for (auto& neighbor : validNeighbors) {
        if (neighbor == c) {
            std::cout << "Valid neighbor " << neighbor << " found!" << endl << endl;
            return true;
        }
    }

    return false;
}

bool AnyValidNeighborsNearby(vector<string>* file, int digitX, int wordLength, int digitY, int maxBounds) {
    for (int y = digitY - 1; y <= digitY + 1; y++) {
        if (y < 0 || y > maxBounds) {
            continue;
        }

        for (int x = digitX - 1; x <= digitX + wordLength; x++) {
            if (x < 0 || x > maxBounds) {
                continue;
            }

            if (IsValidNeighbor((*file)[y][x])) {
                return true;
            }
        }
    }

    return false;
}

int main (int argc, char *argv[]) {
    auto file = ReadFile(argv[1]);
    auto rows = (*file)[0].size();
    auto columns = file->size();


    // rows ------
    // columns |
    //         |
    //         |

    int sumOfAll = 0;
    for (int y = 0; y < file->size(); y++) {
        std::cout << "Line number: " << y + 1 << std::endl;

        int numLength = 0;
        bool isWordValid = false;
        int num = 0;
        for (int x = 0; x < (*file)[y].size(); x++) {
            if (numLength - 1 > 0) {
                numLength--;
                continue;
            }

            char c = (*file)[y][x];

            if (isdigit(c)) {
                numLength = GetNumLen((*file)[y], x);
                num = std::stoi((*file)[y].substr(x, numLength));

                std::cout << "Digit found: " << num << endl;

                if (AnyValidNeighborsNearby(file, x, numLength, y, rows - 1)) {
                    sumOfAll += num;
                }
            }
        }
        std::cout << std::endl;
    }

    std::cout << "The sum of everything is: " << sumOfAll << std::endl;

    return 0;
}
