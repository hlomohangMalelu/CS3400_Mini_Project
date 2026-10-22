
#include "../include/library_system.h"
#include <limits>

// Constructor
LibrarySystem::LibrarySystem() {
    laptops = 10;
    books = 20;
    rooms = 5;

    totalLaptops = 10;
    totalBooks = 20;
    totalRooms = 5;

    nextRequestID = 1;
}

// Start the system
void LibrarySystem::run() {
    int choice;

    do {
        cout << "\n===== LIBRARYHUB =====\n";
        cout << "1. Library User Menu\n";
        cout << "2. Library Staff Menu\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                userMenu();
                break;

            case 2:
                staffMenu();
                break;

            case 0:
                cout << "Exiting LibraryHub.\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

// User menu
void LibrarySystem::userMenu() {
    int choice;

    do {
        cout << "\n===== LIBRARY USER MENU =====\n";
        cout << "1. Tap student/lecturer card\n";
        cout << "2. Submit resource request\n";
        cout << "3. View library zones\n";
        cout << "4. Check connection between zones\n";
        cout << "5. Enter a library zone\n";
        cout << "6. Leave current zone\n";
        cout << "7. Show my session\n";
        cout << "8. Temporary exit\n";
        cout << "9. Return to library\n";
        cout << "10. Final checkout\n";
        cout << "0. Back\n";
        cout << "Choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                tapStudentCard();
                break;

            case 2:
                submitResourceRequest();
                break;

            case 3:
                graph.showZones();
                break;

            case 4: {
                string first, second;

                cout << "First zone: ";
                cin >> first;

                cout << "Second zone: ";
                cin >> second;

                if (graph.connected(first, second))
                    cout << "The zones are connected.\n";
                else
                    cout << "No path exists between the zones.\n";

                break;
            }

            case 5:
                navigateLibrary();
                break;

            case 6: {
                string id;

                cout << "Enter your user ID: ";
                cin >> id;

                if (!cards.isInside(id)) {
                    cout << "You must be inside the library.\n";
                    break;
                }

                if (!cards.hasCurrentZone(id)) {
                    cout << "You are not currently in a zone.\n";
                    break;
                }

                string zone = cards.getCurrentZone(id);

                if (graph.leaveZone(zone)) {
                    cards.clearCurrentZone(id);
                    cout << "You have left " << zone << ".\n";
                }

                break;
            }

            case 7: {
                string id;

                cout << "Enter your user ID: ";
                cin >> id;

                cards.showSession(id);
                break;
            }

            case 8: {
                string id;

                cout << "Enter your user ID: ";
                cin >> id;

                cards.temporaryExit(id);
                break;
            }

            case 9: {
                string id;

                cout << "Enter your user ID: ";
                cin >> id;

                cards.returnToLibrary(id);
                break;
            }

            case 10: {
                string id;

                cout << "Enter your user ID: ";
                cin >> id;

                cards.checkout(id);
                break;
            }

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

// Staff menu
void LibrarySystem::staffMenu() {
    int choice;

    do {
        cout << "\n===== LIBRARY STAFF MENU =====\n";
        cout << "1. Process next request\n";
        cout << "2. Display pending request counts\n";
        cout << "3. Add library zone\n";
        cout << "4. Connect library zones\n";
        cout << "5. Display zones and inventory\n";
        cout << "6. Return resource to inventory\n";
        cout << "7. Search student ID\n";
        cout << "8. Display student IDs\n";
        cout << "0. Back\n";
        cout << "Choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
            case 1:
                processNextRequest();
                break;

            case 2:
                requestManager.displayRequestCounts();
                break;

            case 3: {
                string name, faculty;

                cout << "Zone name (no spaces): ";
                cin >> name;

                cout << "Allowed faculty (or ALL): ";
                cin >> faculty;

                graph.addZone(name, faculty);
                break;
            }

            case 4: {
                string first, second;

                cout << "First zone: ";
                cin >> first;

                cout << "Second zone: ";
                cin >> second;

                graph.connectZones(first, second);
                break;
            }

            case 5:
                displayInventory();
                graph.showZones();
                break;

            case 6:
                returnResource();
                break;

            case 7: {
                string id;

                cout << "Student ID: ";
                cin >> id;

                if (tree.searchStudent(id))
                    cout << "Student found.\n";
                else
                    cout << "Student not found.\n";

                break;
            }

            case 8:
                tree.displayStudents();
                break;

            case 0:
                break;

            default:
                cout << "Invalid choice.\n";
        }

    } while (choice != 0);
}

// Tap/register a user card
void LibrarySystem::tapStudentCard() {
    string id, session, faculty;
    int priority;

    cout << "Enter student/lecturer ID: ";
    cin >> id;

    cout << "Enter session ID: ";
    cin >> session;

    cout << "Enter faculty: ";
    cin >> faculty;

    cout << "Priority (1=urgent, 2=medium, 3=normal): ";
    cin >> priority;

    if (cin.fail() || priority < 1 || priority > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid priority.\n";
        return;
    }

    cards.tapCard(id, session, faculty, priority);

    // The BST stores student IDs, not lecturer IDs.
    if (!id.empty() && id[0] == 's' && cards.isRegistered(id)) {
        tree.insertStudent(id);
    }
}

// Submit a resource request
void LibrarySystem::submitResourceRequest() {
    string userID, resourceType, date;
    int priority;

    cout << "Enter your user ID: ";
    cin >> userID;

    if (!cards.isInside(userID)) {
        cout << "You must tap your card and be inside the library first.\n";
        return;
    }

    cout << "Resource (Laptop, Book, Room): ";
    cin >> resourceType;

    int resourceID = getResourceID(resourceType);

    if (resourceID == 0) {
        cout << "Unknown resource type.\n";
        return;
    }

    cout << "Request date (YYYY-MM-DD): ";
    cin >> date;

    cout << "Priority (1=urgent, 2=medium, 3=normal): ";
    cin >> priority;

    if (cin.fail() || priority < 1 || priority > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid priority.\n";
        return;
    }

    Request request(
        nextRequestID,
        userID,
        resourceID,
        resourceType,
        date,
        priority,
        PENDING
    );

    nextRequestID++;

    if (priority == 3) {
        requestManager.submitNormalRequest(request);
    } else {
        requestManager.submitPriorityRequest(request);
    }

    cout << "Your request ID is "
         << request.getRequestID() << ".\n";
}

// Process the next available request
void LibrarySystem::processNextRequest() {
    Request request;

    // Priority requests are processed first.
    if (requestManager.hasPriorityRequests()) {
        request = requestManager.getNextPriorityRequest();
    } else if (requestManager.hasNormalRequests()) {
        request = requestManager.getNextNormalRequest();
    } else {
        cout << "There are no pending requests.\n";
        return;
    }

    int* available = getAvailableInventory(
        request.getResourceType()
    );

    if (available == nullptr) {
        cout << "Unknown resource type. Request rejected.\n";
        request.setStatus(REJECTED);
        request.display();
        return;
    }

    if (*available <= 0) {
        cout << "Resource unavailable. Request remains pending.\n";

        // Put the request back into its original structure.
        if (request.getPriority() == 3) {
            requestManager.submitNormalRequest(request);
        } else {
            requestManager.submitPriorityRequest(request);
        }

        return;
    }

    (*available)--;

    request.setStatus(APPROVED);

    cout << "Request approved.\n";
    request.display();

    cout << "Remaining " << request.getResourceType()
         << " resources: " << *available << "\n";
}

// Display inventory
void LibrarySystem::displayInventory() {
    cout << "\n===== RESOURCE INVENTORY =====\n";
    cout << "Laptops: " << laptops
         << "/" << totalLaptops << "\n";

    cout << "Books: " << books
         << "/" << totalBooks << "\n";

    cout << "Rooms: " << rooms
         << "/" << totalRooms << "\n";
}

// Return a resource to inventory
void LibrarySystem::returnResource() {
    string resourceType;

    cout << "Resource to return (Laptop, Book, Room): ";
    cin >> resourceType;

    int* available = getAvailableInventory(resourceType);
    int total = getTotalInventory(resourceType);

    if (available == nullptr) {
        cout << "Unknown resource type.\n";
        return;
    }

    if (*available >= total) {
        cout << "All resources of this type are already available.\n";
        return;
    }

    (*available)++;
    cout << "Resource returned successfully.\n";
}

// Get resource ID
int LibrarySystem::getResourceID(string resourceType) {
    if (resourceType == "Laptop") return 1;
    if (resourceType == "Book") return 2;
    if (resourceType == "Room") return 3;

    return 0;
}

// Get a pointer to the available inventory count
int* LibrarySystem::getAvailableInventory(string resourceType) {
    if (resourceType == "Laptop") return &laptops;
    if (resourceType == "Book") return &books;
    if (resourceType == "Room") return &rooms;

    return nullptr;
}

// Get the total inventory count
int LibrarySystem::getTotalInventory(string resourceType) {
    if (resourceType == "Laptop") return totalLaptops;
    if (resourceType == "Book") return totalBooks;
    if (resourceType == "Room") return totalRooms;

    return 0;
}

// Enter a library zone
void LibrarySystem::navigateLibrary() {
    string userID, zone;

    cout << "Enter your user ID: ";
    cin >> userID;

    if (!cards.isInside(userID)) {
        cout << "You must be inside the library.\n";
        return;
    }

    if (cards.hasCurrentZone(userID)) {
        cout << "Leave your current zone before entering another.\n";
        return;
    }

    cout << "Enter zone name: ";
    cin >> zone;

    string faculty = cards.getFaculty(userID);

    if (graph.enterZone(zone, faculty)) {
        cards.setCurrentZone(userID, zone);
        cout << "You entered " << zone << ".\n";
    }
}
