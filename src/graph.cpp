#include "../include/graph.h"

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