#include "../include/graph.h"
#include <fstream>
#include <sstream>

Graph::Graph() {
    zoneCount = 0;
}

Graph::~Graph() {
    for (int i = 0; i < zoneCount; i++) {
        Edge* current = zones[i].edges;

        while (current != nullptr) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }
    }
}

int Graph::findZone(string name) {
    for (int i = 0; i < zoneCount; i++) {
        if (zones[i].name == name)
            return i;
    }

    return -1;
}

void Graph::addZone(string name, string allowedFaculty) {
    if (zoneCount == 100) {
        cout << "Maximum number of zones reached.\n";
        return;
    }

    if (findZone(name) != -1) {
        cout << "Zone already exists.\n";
        return;
    }

    zones[zoneCount].name = name;
    zones[zoneCount].allowedFaculty = allowedFaculty;
    zones[zoneCount].occupancy = 0;
    zones[zoneCount].edges = nullptr;

    zoneCount++;

    cout << "Zone added successfully.\n";
}

void Graph::connectZones(string first, string second) {
    int a = findZone(first);
    int b = findZone(second);

    if (a == -1 || b == -1) {
        cout << "Both zones must exist first.\n";
        return;
    }

    if (a == b) {
        cout << "A zone cannot connect to itself.\n";
        return;
    }

    Edge* current = zones[a].edges;

    while (current != nullptr) {
        if (current->destination == b) {
            cout << "Zones are already connected.\n";
            return;
        }

        current = current->next;
    }

    Edge* edgeA = new Edge(b);
    edgeA->next = zones[a].edges;
    zones[a].edges = edgeA;

    Edge* edgeB = new Edge(a);
    edgeB->next = zones[b].edges;
    zones[b].edges = edgeB;

    cout << "Zones connected.\n";
}

bool Graph::searchPath(int current, int target, bool visited[]) {
    if (current == target)
        return true;

    visited[current] = true;

    Edge* edge = zones[current].edges;

    while (edge != nullptr) {
        int nextZone = edge->destination;

        if (!visited[nextZone] &&
            searchPath(nextZone, target, visited))
            return true;

        edge = edge->next;
    }

    return false;
}

bool Graph::connected(string first, string second) {
    int a = findZone(first);
    int b = findZone(second);

    if (a == -1 || b == -1)
        return false;

    bool visited[100] = {};

    return searchPath(a, b, visited);
}

bool Graph::enterZone(string zone, string faculty) {
    int index = findZone(zone);

    if (index == -1) {
        cout << "Zone not found.\n";
        return false;
    }

    if (zones[index].allowedFaculty != "ALL" &&
        zones[index].allowedFaculty != faculty) {
        cout << "Access denied: faculty restriction.\n";
        return false;
    }

    zones[index].occupancy++;

    cout << "Entry allowed. Occupancy: "
         << zones[index].occupancy << endl;

    return true;
}

bool Graph::leaveZone(string zone) {
    int index = findZone(zone);

    if (index == -1) {
        cout << "Zone not found.\n";
        return false;
    }

    if (zones[index].occupancy == 0) {
        cout << "Zone is already empty.\n";
        return false;
    }

    zones[index].occupancy--;

    cout << "Exit recorded. Occupancy: "
         << zones[index].occupancy << endl;

    return true;
}

void Graph::showZones() {
    for (int i = 0; i < zoneCount; i++) {
        cout << "\nZone: " << zones[i].name
             << " | Faculty: " << zones[i].allowedFaculty
             << " | Occupancy: " << zones[i].occupancy
             << endl;

        cout << "Connected zones: ";

        Edge* current = zones[i].edges;

        while (current != nullptr) {
            cout << zones[current->destination].name << " ";
            current = current->next;
        }

        cout << endl;
    }
}

void Graph::saveToFile(string filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        cout << "ERROR: Cannot open " << filename << endl;
        cout << "Check the current working directory and file permissions.\n";
        return;
    }

    cout << "File opened successfully!\n";

    for (int i = 0; i < zoneCount; i++) {
        file << "ZONE,"
             << zones[i].name << ","
             << zones[i].allowedFaculty << ","
             << zones[i].occupancy << "\n";
    }

    for (int i = 0; i < zoneCount; i++) {
        Edge* current = zones[i].edges;

        while (current != nullptr) {
            int destination = current->destination;

            if (i < destination) {
                file << "EDGE,"
                     << zones[i].name << ","
                     << zones[destination].name << "\n";
            }

            current = current->next;
        }
    }

    file.close();

    cout << "Zones saved successfully.\n";
}

void Graph::loadFromFile(string filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Zones file not found. Starting with empty zones.\n";
        return;
    }

    // Remove old connections
    for (int i = 0; i < zoneCount; i++) {
        Edge* current = zones[i].edges;

        while (current != nullptr) {
            Edge* temp = current;
            current = current->next;
            delete temp;
        }

        zones[i].edges = nullptr;
    }

    // Clear the existing zones
    zoneCount = 0;

    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string recordType;
        getline(ss, recordType, ',');

        if (recordType == "ZONE") {
            string name;
            string faculty;
            string occupancyText;

            getline(ss, name, ',');
            getline(ss, faculty, ',');
            getline(ss, occupancyText, ',');

            if (zoneCount < 100 && findZone(name) == -1) {
                zones[zoneCount].name = name;
                zones[zoneCount].allowedFaculty = faculty;
                zones[zoneCount].occupancy = stoi(occupancyText);
                zones[zoneCount].edges = nullptr;

                zoneCount++;
            }
        }
        else if (recordType == "EDGE") {
            string first;
            string second;

            getline(ss, first, ',');
            getline(ss, second, ',');

            int a = findZone(first);
            int b = findZone(second);

            if (a != -1 && b != -1 && a != b) {
                // Check whether the connection already exists
                bool exists = false;
                Edge* current = zones[a].edges;

                while (current != nullptr) {
                    if (current->destination == b) {
                        exists = true;
                        break;
                    }

                    current = current->next;
                }

                if (!exists) {
                    Edge* edgeA = new Edge(b);
                    edgeA->next = zones[a].edges;
                    zones[a].edges = edgeA;

                    Edge* edgeB = new Edge(a);
                    edgeB->next = zones[b].edges;
                    zones[b].edges = edgeB;
                }
            }
        }
    }

    file.close();
}