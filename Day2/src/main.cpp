#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

using std::endl;
using std::istringstream;
using std::string;
using std::vector;

struct ColorInfo {
public:
    string color;
    int amount;

    ColorInfo(string col, int a) {
        color = col;
        amount = a;
    }

};

vector<string>* ReadFile(char* filePath)  {
    vector<string>* lines = new vector<string>();

    std::ifstream file(filePath);

    string currentLine;
    while (std::getline(file, currentLine)) {
        std::istringstream iss(currentLine);
        lines->push_back(iss.str());
    }

    return lines;
}

int GetGameID(string line) {
    string gameNum = line.substr(5, line.find(':') - 5);
    return std::stoi(gameNum);
}

string GetSubset(string line) {
    int foundColonPos = line.find(':') + 2;
    string subset = line.substr(foundColonPos, line.length() - foundColonPos);
    return subset;
}

vector<string>* GetSets(string subset) {
    vector<string>* sets = new vector<string>();

    int last = 0;
    int current = 0;

    while (true) {
        current = subset.find(';', last);

        if (current == string::npos) {
            sets->push_back(subset.substr(last, subset.size() - last));
            break;
        }

        sets->push_back(subset.substr(last, current - last));

        last = current + 2;
    }

    return sets;
}

vector<ColorInfo>* GetValues(string set) {
    auto* values = new vector<ColorInfo>();

    int start = 0;
    int end = 0;

    while (true) {
        end = set.find(',', start);

        int spaceStart = set.find(' ', start);
        int amount = std::stoi(set.substr(start, spaceStart));

        if (end == string::npos) {
            auto color = set.substr(spaceStart + 1, set.size() - start - 2);
            values->push_back(ColorInfo(color, amount));
            break;
        }

        values->push_back(ColorInfo(set.substr(spaceStart + 1, end - spaceStart - 1), amount));

        start = end + 2;
    }

    return values;
}

int main (int argc, char *argv[]) {
    vector<string> &lines = *ReadFile(argv[1]);

    std::map<string, int> maxSums = {
        {"red", 12},
        {"green", 13},
        {"blue", 14},
    };

    int sumOfAll = 0;
    for (auto& line : lines) {
        // Game 100
        int gameId = GetGameID(line);

        // 8 red, 3 green; 4 green, 1 blue, 15 red; 10 red, 8 green, 1 blue
        string subset = GetSubset(line);


        std::map<string, int> largestTaken = {
            {"red", 0},
            {"green", 0},
            {"blue", 0},
        };

        // Handle cases
        // 8 red, 3 green
        // 4 green, 1 blue, 15 red
        // 10 red, 8 green, 1 blue
        for (auto& val : *GetSets(subset)) {
            // Game's sets values
            // 8 red                 3 green
            // 4 green               1 blue                  15 red
            // 10 red                8 green                 1 blue
            for (auto& v : *GetValues(val)) {
                int takenValue = largestTaken[v.color];

                if (takenValue <= v.amount) {
                    largestTaken[v.color] = v.amount;
                }
            }
        }

        bool isGamePossible = true;
        std::cout << "Game " << gameId << ":" << endl;
        for (const auto& [col, largest] : largestTaken) {
            const auto& max = maxSums[col];
            std::cout << col << ": " << largest << ", max: " << max << endl;

            if (largest > max) {
                isGamePossible = false;
                break;
            }
        }

        if (isGamePossible) {
            std::cout << "Game " << gameId << " possible!" << endl;
            sumOfAll += gameId;
        } else {
            std::cout << "Game " << gameId << " impossible!" << endl;
        }

        std::cout << endl;
    }

    std::cout << std::endl << "Sum of all possible games: " << sumOfAll << endl;

    return 0;
}
