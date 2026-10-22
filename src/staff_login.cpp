
#include "../include/staff_login.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool StaffLogin::registerStaff(string staffID, string name, string password) {
    // Check that the staff ID starts with "st"
    if (staffID.length() < 3 || staffID.substr(0, 2) != "st") {
        cout << "Invalid staff ID. It must start with st.\n";
        return false;
    }

    if (name.empty() || password.empty() ||
        staffID.find(',') != string::npos ||
        name.find(',') != string::npos ||
        password.find(',') != string::npos) {
        cout << "Invalid details. Fields cannot be empty or contain commas.\n";
        return false;
    }

    // Open the file to check whether the ID already exists
    ifstream inFile("data/staff.txt");

    string line;

    while (getline(inFile, line)) {
        string storedID;
        string storedName;
        string storedPassword;

        stringstream ss(line);

        if (getline(ss, storedID, ',') &&
            getline(ss, storedName, ',') &&
            getline(ss, storedPassword)) {

            if (storedID == staffID) {
                cout << "This staff ID is already registered.\n";
                inFile.close();
                return false;
            }
        }
    }

    inFile.close();

    // Append the new staff member to the file
    ofstream outFile("data/staff.txt", ios::app);

    if (!outFile.is_open()) {
        cout << "Could not open data/staff.txt.\n";
        cout << "Make sure the data folder exists.\n";
        return false;
    }

    outFile << staffID << "," << name << "," << password << "\n";
    outFile.close();

    cout << "Staff registered successfully!\n";
    return true;
}

bool StaffLogin::authenticate(string staffID, string password) {
    if (staffID.length() < 3 || staffID.substr(0, 2) != "st") {
        return false;
    }

    ifstream file("data/staff.txt");

    if (!file.is_open()) {
        return false;
    }

    string line;

    while (getline(file, line)) {
        string storedID;
        string storedName;
        string storedPassword;

        stringstream ss(line);

        if (getline(ss, storedID, ',') &&
            getline(ss, storedName, ',') &&
            getline(ss, storedPassword)) {

            if (storedID == staffID && storedPassword == password) {
                file.close();
                return true;
            }
        }
    }

    file.close();
    return false;
}
