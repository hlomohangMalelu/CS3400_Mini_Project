#include "../include/hashTable.h"

HashTable::HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++)
        table[i] = nullptr;
}

int HashTable::hashFunction(string id) {
    unsigned int hash = 0;

    for (int i = 0; i < id.length(); i++)
        hash = (hash * 31 + id[i]) % TABLE_SIZE;

    return hash;
}

HashTable::Student* HashTable::findStudent(string id) {
    int index = hashFunction(id);
    Student* current = table[index];

    while (current != nullptr) {
        if (current->id == id)
            return current;

        current = current->next;
    }

    return nullptr;
}

HashTable::~HashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Student* current = table[i];

        while (current != nullptr) {
            Student* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

void HashTable::tapCard(string id, string session, string faculty, int priority) {
    Student* student = findStudent(id);

    if (student != nullptr) {
        if (student->status == CHECKED_OUT) {
            student->session = session;
            student->faculty = faculty;
            student->priority = priority;
            student->currentZone = "";
            student->status = INSIDE;

            cout << "New session started for " << id << ".\n";
            return;
        }

        if (student->session != session) {
            cout << "Access denied: another session is active.\n";
            return;
        }

        if (student->status == TEMPORARILY_OUT) {
            student->status = INSIDE;
            cout << "Welcome back. Session restored.\n";
            return;
        }

        cout << "Student is already inside.\n";
        return;
    }

    int index = hashFunction(id);

    Student* newStudent =
        new Student(id, session, faculty, priority);

    newStudent->next = table[index];
    table[index] = newStudent;

    cout << "Card registered. Entry allowed.\n";
}

void HashTable::showSession(string id) {
    Student* student = findStudent(id);

    if (student == nullptr) {
        cout << "Student record not found.\n";
        return;
    }

    cout << "Student ID: " << student->id << endl;
    cout << "Session: " << student->session << endl;
    cout << "Faculty: " << student->faculty << endl;
    cout << "Priority: " << student->priority << endl;

    if (student->status == INSIDE)
        cout << "Status: INSIDE\n";
    else if (student->status == TEMPORARILY_OUT)
        cout << "Status: TEMPORARILY OUT\n";
    else
        cout << "Status: CHECKED OUT\n";

    cout << "Current zone: ";

    if (student->currentZone == "")
        cout << "None\n";
    else
        cout << student->currentZone << endl;
}

void HashTable::temporaryExit(string id) {
    Student* student = findStudent(id);

    if (student == nullptr || student->status != INSIDE) {
        cout << "Student must be inside to exit temporarily.\n";
        return;
    }

    if (student->currentZone != "") {
        cout << "Leave the current zone before exiting.\n";
        return;
    }

    student->status = TEMPORARILY_OUT;
    cout << "Temporary exit recorded. Session preserved.\n";
}

void HashTable::returnToLibrary(string id) {
    Student* student = findStudent(id);

    if (student == nullptr ||
        student->status != TEMPORARILY_OUT) {
        cout << "No temporary exit to return from.\n";
        return;
    }

    student->status = INSIDE;
    cout << "Return recorded. Previous session restored.\n";
}

void HashTable::checkout(string id) {
    Student* student = findStudent(id);

    if (student == nullptr || student->status != INSIDE) {
        cout << "Student must be inside to check out.\n";
        return;
    }

    if (student->currentZone != "") {
        cout << "Leave the current zone before checkout.\n";
        return;
    }

    student->status = CHECKED_OUT;
    cout << "Checkout complete. Record retained.\n";
}

bool HashTable::isRegistered(string id) {
    Student* student = findStudent(id);

    return student != nullptr && student->status != CHECKED_OUT;
}

bool HashTable::isInside(string id) {
    Student* student = findStudent(id);

    return student != nullptr &&
           student->status == INSIDE;
}

bool HashTable::hasCurrentZone(string id) {
    Student* student = findStudent(id);

    return student != nullptr &&
           student->currentZone != "";
}

string HashTable::getFaculty(string id) {
    Student* student = findStudent(id);

    if (student == nullptr)
        return "";

    return student->faculty;
}

string HashTable::getCurrentZone(string id) {
    Student* student = findStudent(id);

    if (student == nullptr)
        return "";

    return student->currentZone;
}

void HashTable::setCurrentZone(string id, string zone) {
    Student* student = findStudent(id);

    if (student != nullptr)
        student->currentZone = zone;
}

void HashTable::clearCurrentZone(string id) {
    Student* student = findStudent(id);

    if (student != nullptr)
        student->currentZone = "";
}