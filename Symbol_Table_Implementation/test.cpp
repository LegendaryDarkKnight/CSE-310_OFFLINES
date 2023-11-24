#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
const int MAX_TOKEN_LENGTH = 70; // Maximum length of a token

int main() {
    ifstream file("input.txt"); // Replace "example.txt" with your file name
    if (!file.is_open()) {
        cerr << "Unable to open the file!" << endl;
        return 1;
    }

    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string token;

        while (iss >> token) {
            // Limit the token length and process it
            if (token.length() > MAX_TOKEN_LENGTH) {
                token = token.substr(0, MAX_TOKEN_LENGTH);
            }

            // Print each token while parsing the line
            cout << "Token: " << token << endl;
        }
    }

    file.close();
    return 0;
}
