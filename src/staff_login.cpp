#include "../include/staff_login.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

StaffLogin::StaffLogin() {
    staffCount = 0;
    loadStaff();
}

void StaffLogin::loadStaff() {
    ifstream file("data/staff.txt");

    if (!file.is_open()) {
        cout << "No staff file found yet.\n";
        return;
    }

    staffCount = 0;
    string line;

    while (getline(file, line)) {
        if (staffCount >= 100)
            break;

        string id, name, password;
        stringstream ss(line);

        if (getline(ss, id, ',') &&
            getline(ss, name, ',') &&
            getline(ss, password)) {

            staffMembers[staffCount].staffID = id;
            staffMembers[staffCount].name = name;
            staffMembers[staffCount].password = password;

            staffCount++;
        }
    }

    file.close();
}

void StaffLogin::saveStaff() {
    ofstream file("data/staff.txt");

    if (!file.is_open()) {
        cout << "Could not save staff records.\n";
        return;
    }

    for (int i = 0; i < staffCount; i++) {
        file << staffMembers[i].staffID << ","
             << staffMembers[i].name << ","
             << staffMembers[i].password << "\n";
    }

    file.close();
}

bool StaffLogin::registerStaff(string staffID, string name, string password) {
    if (staffID.length() < 3 || staffID.substr(0, 2) != "st") {
        cout << "Invalid staff ID. It must start with st.\n";
        return false;
    }

    if (name.empty() || password.empty() ||
        staffID.find(',') != string::npos ||
        name.find(',') != string::npos ||
        password.find(',') != string::npos) {
        cout << "Invalid details.\n";
        return false;
    }

    for (int i = 0; i < staffCount; i++) {
        if (staffMembers[i].staffID == staffID) {
            cout << "This staff ID is already registered.\n";
            return false;
        }
    }

    if (staffCount >= 100) {
        cout << "Staff storage is full.\n";
        return false;
    }

    staffMembers[staffCount].staffID = staffID;
    staffMembers[staffCount].name = name;
    staffMembers[staffCount].password = password;

    staffCount++;

    saveStaff();

    cout << "Staff registered successfully!\n";
    return true;
}

bool StaffLogin::authenticate(string staffID, string password) {
    for (int i = 0; i < staffCount; i++) {
        if (staffMembers[i].staffID == staffID &&
            staffMembers[i].password == password) {
            return true;
        }
    }

    return false;
}