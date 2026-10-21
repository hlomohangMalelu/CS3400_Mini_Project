#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <iostream>
#include <string>
using namespace std;

enum Status {
    INSIDE,
    TEMPORARILY_OUT,
    CHECKED_OUT
};

class HashTable {
private:
    struct Student {
        string id;
        string session;
        string faculty;
        string currentZone;
        int priority;
        Status status;
        Student* next;

        Student(string i, string s, string f, int p) {
            id = i;
            session = s;
            faculty = f;
            currentZone = "";
            priority = p;
            status = INSIDE;
            next = nullptr;
        }
    };

    static const int TABLE_SIZE = 101;
    Student* table[TABLE_SIZE];

    int hashFunction(string id);
    Student* findStudent(string id);

public:
    HashTable();
    ~HashTable();

    void tapCard(string id, string session,
                 string faculty, int priority);
    void showSession(string id);
    void temporaryExit(string id);
    void returnToLibrary(string id);
    void checkout(string id);

    bool isRegistered(string id);
    bool isInside(string id);
    bool hasCurrentZone(string id);

    string getFaculty(string id);
    string getCurrentZone(string id);

    void setCurrentZone(string id, string zone);
    void clearCurrentZone(string id);
};

#endif