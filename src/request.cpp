
#include "request.h"
#include <iostream>
using namespace std;

// Default constructor
Request::Request() {
    requestID = 0;
    userID = "";
    resourceID = 0;
    resourceType = "";
    requestDate = "";
    priority = 0;
    status = PENDING;
}

// Parameterized constructor
Request::Request(int rid, string uid, int resid, string type, string date, int p, RequestStatus s) {
    requestID = rid;
    userID = uid;
    resourceID = resid;
    resourceType = type;
    requestDate = date;
    priority = p;
    status = s;
}

// Getters
int Request::getRequestID() const {
    return requestID;
}

string Request::getUserID() const {
    return userID;
}

int Request::getResourceID() const {
    return resourceID;
}

string Request::getResourceType() const {
    return resourceType;
}

string Request::getRequestDate() const {
    return requestDate;
}

int Request::getPriority() const {
    return priority;
}

RequestStatus Request::getStatus() const {
    return status;
}

// Setters
void Request::setRequestID(int rid) {
    requestID = rid;
}

void Request::setUserID(string uid) {
    userID = uid;
}

void Request::setResourceID(int resid) {
    resourceID = resid;
}

void Request::setResourceType(string type) {
    resourceType = type;
}

void Request::setRequestDate(string date) {
    requestDate = date;
}

void Request::setPriority(int p) {
    priority = p;
}

void Request::setStatus(RequestStatus s) {
    status = s;
}

// Convert enum to readable text
string Request::statusToString() const {
    switch (status) {
        case PENDING:
            return "Pending";

        case APPROVED:
            return "Approved";

        case REJECTED:
            return "Rejected";

        case CANCELLED:
            return "Cancelled";

        default:
            return "Unknown";
    }
}

// Display request details
void Request::display() const {
    cout << "Request ID: " << requestID << endl;
    cout << "User ID: " << userID << endl;
    cout << "Resource ID: " << resourceID << endl;
    cout << "Resource Type: " << resourceType << endl;
    cout << "Request Date: " << requestDate << endl;
    cout << "Priority: " << priority << endl;
    cout << "Status: " << statusToString() << endl;
}

// Compare requests for MinHeap
bool Request::operator<(const Request& other) const {
    return priority < other.priority;
}
