

#ifndef STAFF_LOGIN_H
#define STAFF_LOGIN_H

#include <string>
using namespace std;

class StaffLogin {
public:
    bool registerStaff(string staffID, string name, string password);
    bool authenticate(string staffID, string password);
};

#endif
