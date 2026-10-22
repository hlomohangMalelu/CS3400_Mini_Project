
#ifndef LIBRARY_SYSTEM_H
#define LIBRARY_SYSTEM_H

#include <iostream>
#include <string>

#include "hashTable.h"
#include "trees.h"
#include "graph.h"
#include "request_manager.h"

using namespace std;

class LibrarySystem {
private:
    // Existing data structures
    HashTable cards;
    Tree tree;
    Graph graph;
    RequestManager requestManager;

    // Available resources
    int laptops;
    int books;
    int rooms;

    // Maximum inventory
    int totalLaptops;
    int totalBooks;
    int totalRooms;

    // Generate unique request IDs
    int nextRequestID;

    // Main menus
    void userMenu();
    void staffMenu();

    // User operations
    void tapStudentCard();
    void submitResourceRequest();
    void checkRequestStatus();
    void navigateLibrary();

    // Staff operations
    void processNextRequest();
    void displayPendingRequests();
    void manageLibraryZones();
    void displayInventory();
    void returnResource();

    // Helper functions
    int getResourceID(string resourceType);
    int* getAvailableInventory(string resourceType);
    int getTotalInventory(string resourceType);

public:
    // Constructor
    LibrarySystem();

    // Start the application
    void run();
};

#endif
