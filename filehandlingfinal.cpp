#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    int a;    
    string data;
    string filename = "exam.txt";

    // Writing to a file (overwrite mode) with user input
    cout << "Enter text to write to the file (overwrite mode): ";
    getline(cin, data);

    ofstream writeFile(filename);
    if (!writeFile) {
        cerr << "Error creating file!" << endl;
        return 1;
    }
    writeFile << data << endl;
    writeFile.close();
    cout << "Data written successfully.\n";

    // Ask user for operation
    cout << "Enter 1 to append, 2 to read: ";
    cin >> a;
    cin.ignore(); // Clear newline from input buffer

    if (a == 1) {
        cout << "Enter text to append to the file: ";
        getline(cin, data);
        ofstream appendFile(filename, ios::app);
        if (!appendFile) {
            cerr << "Error opening file for appending!" << endl;
            return 1;
        }
        appendFile << data << endl;
        appendFile.close();
        cout << "Data appended successfully.\n";
    } else if (a == 2) {
        ifstream readFile(filename);
        if (!readFile) {
            cerr << "Error opening file for reading!" << endl;
            return 1;
        }
        cout << "\nFile content:\n";
        while (getline(readFile, data)) {
            cout << data << endl;
        }
        readFile.close();
    } else {
        cout << "Invalid option.\n";
    }

    return 0;
}

