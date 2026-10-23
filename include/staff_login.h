
#ifndef STAFF_LOGIN_H
#define STAFF_LOGIN_H

#include <string>
using namespace std;

class StaffLogin {
private:
    struct Staff {
        string staffID;
        string name;
        string password;
    };

    Staff staffMembers[100];
    int staffCount;

    void saveStaff();

public:
    StaffLogin();

    void loadStaff();
    bool registerStaff(string staffID, string name, string password);
    bool authenticate(string staffID, string password);
};

#endif