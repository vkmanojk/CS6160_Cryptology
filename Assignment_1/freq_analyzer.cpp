#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>

using namespace std;

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cerr << "Missing input file\n";
        cerr << "Usage: " << argv[0] << " <filename.txt>\n";
        return 1;
    }

    ifstream inputFile(argv[1]);

    if (!inputFile) {
        cerr << "Error: Could not open file.\n";
        return 1;
    }

    int count[26] = {0};
    int totalLetters = 0;

    bool insideTag = false;
    char ch;

    while (inputFile.get(ch)) {

        if (ch == '<') {
            insideTag = true;
            continue;
        }

        else if (ch == '>') {
            insideTag = false;
            continue;
        }

        else if (insideTag)
            continue;

        else if (isalpha(ch)) {
            ch = tolower(ch);
            count[ch - 'a']++;
            totalLetters++;
        }
    }

    inputFile.close();

    cout << fixed << setprecision(2);
    cout << "\nTotal Characters: " << totalLetters << "\n\n";
    cout << "Letter\tCount\tFrequency (%)\n";
    cout << "-------------------------------\n";

    for (int i = 0; i < 26; i++) {
        double frequency = 0.0;
        if (totalLetters > 0)
            frequency = (double)count[i] / totalLetters * 100.0;
        cout << char('A' + i) << "\t" << count[i] << "\t" << frequency << "%\n";
    }

    return 0;
}