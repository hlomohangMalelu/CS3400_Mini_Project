#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <string>
using namespace std;

class Graph {
private:
    struct Edge {
        int destination;
        Edge* next;

        Edge(int d) {
            destination = d;
            next = nullptr;
        }
    };

    struct Zone {
        string name;
        string allowedFaculty;
        int occupancy;
        Edge* edges;
    };

    Zone zones[100];
    int zoneCount;

    int findZone(string name);
    bool searchPath(int current, int target, bool visited[]);

public:
    Graph();
    ~Graph();

    void addZone(string name, string allowedFaculty);
    void connectZones(string first, string second);
    bool connected(string first, string second);

    bool enterZone(string zone, string faculty);
    bool leaveZone(string zone);
    void showZones();

    void saveToFile(string filename);
    void loadFromFile(string filename); 
};

#endif