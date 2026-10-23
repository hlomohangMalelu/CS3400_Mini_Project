
#include "../include/library_system.h"
#include <limits>
#include <fstream>
#include <sstream>

// Constructor
LibrarySystem::LibrarySystem() {
    laptops = 10;
    books = 20;
    rooms = 5;

    totalLaptops = 10;
    totalBooks = 20;
    totalRooms = 5;

    nextRequestID = 1;
    resourceCount = 0;
    requestCount = 0;
    nextRequestID = 1;

    loadResources();
    loadRequests();
    loadZones();
}
// Start the system

void LibrarySystem::run() {
    int choice;

    do {
        cout << "\n===== LIBRARYHUB =====\n";
        cout << "1. Student/Lecturer Menu\n";
        cout << "2. Register Staff\n";
        cout << "3. Staff Login\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input.\n";
            choice = -1;
            continue;
        }

        switch (choice) {
            case 1:
                userMenu();
                break;

            case 2: {
                string staffID, name, password;

                cout << "Enter Staff ID (starts with st): ";
                cin >> staffID;

                cout << "Enter staff name: ";
                cin >> name;

                cout << "Enter password: ";
                cin >> password;

                staffLogin.registerStaff(staffID, name, password);
                break;
            }

            case 3: {
                string staffID, password;

                cout << "Enter Staff ID: ";
                cin >> staffID;

                cout << "Enter password: ";
                cin >> password;

                if (staffLogin.authenticate(staffID, password)) {
                    cout << "Login successful. Welcome, "
                         << staffID << "!\n";
                    staffMenu();
                } else {
                    cout << "Access denied. Invalid Staff ID or password.\n";
                }
                break;
            }

            case 0:
                saveZones();
                cout << "Exiting LibraryHub...\n";
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
                    saveZones();
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
        cout<<  "9. save resources\n";
        cout<<  "10. Add new resource\n";      
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
                saveZones();
                break;
            }

            case 4: {
                string first, second;

                cout << "First zone: ";
                cin >> first;

                cout << "Second zone: ";
                cin >> second;

                graph.connectZones(first, second);
                saveZones();
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
             case 9:
                saveResources();
                 break;
            case 10:
                addResource();
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

    if (requestCount >= 1000) {
        cout << "Request history is full. Cannot submit request.\n";
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

    // Store a copy in the request history.
    requestHistory[requestCount] = request;
    requestCount++;

    nextRequestID++;

    if (priority == 3) {
        requestManager.submitNormalRequest(request);
    } else {
        requestManager.submitPriorityRequest(request);
    }

    cout << "DEBUG: requestCount = " << requestCount << endl;
    saveRequests();

    cout << "Your request ID is "<< request.getRequestID() << ".\n";
}
// Process the next available request
void LibrarySystem::processNextRequest() {
    Request request;

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
    }
    else if (*available <= 0) {
        cout << "Resource unavailable. Request remains pending.\n";

        if (request.getPriority() == 3) {
            requestManager.submitNormalRequest(request);
        } else {
            requestManager.submitPriorityRequest(request);
        }

        return;
    }
    else {
        (*available)--;
        request.setStatus(APPROVED);

        cout << "Request approved.\n";
        cout << "Remaining " << request.getResourceType()
             << " resources: " << *available << "\n";
    }

    // Update the request in the history array.
    for (int i = 0; i < requestCount; i++) {
        if (requestHistory[i].getRequestID()
            == request.getRequestID()) {

            requestHistory[i].setStatus(request.getStatus());
            break;
        }
    }

    saveRequests();
    request.display();
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
          displayResources();
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
        saveZones();
        cout << "You entered " << zone << ".\n";
    }
}

//loadresources
void LibrarySystem::loadResources() {
    ifstream file("data/resources.txt");

    if (!file.is_open()) {
        cout << "Could not open data/resources.txt\n";
        return;
    }

    string line;

    while (getline(file, line)) {
        if (resourceCount >= 100) {
            cout << "Maximum resource storage reached.\n";
            break;
        }

        string idText, name, type, location;
        string capacityText, availableText;

        stringstream ss(line);

        if (getline(ss, idText, ',') &&
            getline(ss, name, ',') &&
            getline(ss, type, ',') &&
            getline(ss, location, ',') &&
            getline(ss, capacityText, ',') &&
            getline(ss, availableText)) {

            try {
                int id = stoi(idText);
                int capacity = stoi(capacityText);
                bool available = (stoi(availableText) == 1);

                resources[resourceCount] = Resource(
                    id, name, type, location, capacity, available
                );

                resourceCount++;
            }
            catch (...) {
                cout << "Skipping invalid resource record.\n";
            }
        }
    }

    file.close();

    cout << resourceCount << " resource record(s) loaded.\n";
}
//save resources
void LibrarySystem::saveResources() {
    ofstream file("data/resources.txt");

    if (!file.is_open()) {
        cout << "Could not save resources to file.\n";
        return;
    }

    for (int i = 0; i < resourceCount; i++) {
        file << resources[i].getResourceID() << ","
             << resources[i].getName() << ","
             << resources[i].getType() << ","
             << resources[i].getLocation() << ","
             << resources[i].getCapacity() << ","
             << (resources[i].isAvailable() ? 1 : 0)
             << endl;
    }

    file.close();

    cout << "Resources saved successfully.\n";
}

void LibrarySystem::displayResources() {
    cout << "\n===== SAVED RESOURCES =====\n";

    if (resourceCount == 0) {
        cout << "No resources found.\n";
        return;
    }

    for (int i = 0; i < resourceCount; i++) {
        resources[i].display();
        cout << "----------------------\n";
    }
}

void LibrarySystem::addResource() {
    if (resourceCount >= 100) {
        cout << "Resource storage is full.\n";
        return;
    }

    int id;
    string name, type, location;
    int capacity;
    int available;

    cout << "\n===== ADD RESOURCE =====\n";

    cout << "Enter resource ID: ";
    cin >> id;

    // Check for duplicate ID
    for (int i = 0; i < resourceCount; i++) {
        if (resources[i].getResourceID() == id) {
            cout << "A resource with this ID already exists.\n";
            return;
        }
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter resource name: ";
    getline(cin, name);

    cout << "Enter resource type (Laptop/Book/Room): ";
    getline(cin, type);

    cout << "Enter resource location: ";
    getline(cin, location);

    cout << "Enter capacity: ";
    cin >> capacity;

    if (capacity <= 0) {
        cout << "Capacity must be greater than zero.\n";
        return;
    }

    cout << "Is the resource available? (1 = Yes, 0 = No): ";
    cin >> available;

    if (available != 0 && available != 1) {
        cout << "Enter 1 for Yes or 0 for No.\n";
        return;
    }

    // Create and store the resource
    resources[resourceCount] =
        Resource(id, name, type, location, capacity, available == 1);

    resourceCount++;

    // Save all resource records to the file
    saveResources();

    cout << "Resource added successfully!\n";
}

//save requests
void LibrarySystem::saveRequests() {
    
    ofstream file("data/requests.txt");

    if (!file.is_open()) {
        cout << "Could not save requests.\n";
        return;
    }

    for (int i = 0; i < requestCount; i++) {
        file << requestHistory[i].getRequestID() << ","
             << requestHistory[i].getUserID() << ","
             << requestHistory[i].getResourceID() << ","
             << requestHistory[i].getResourceType() << ","
             << requestHistory[i].getRequestDate() << ","
             << requestHistory[i].getPriority() << ","
             << requestHistory[i].statusToString()
             << "\n";
    }

    file.close();
    cout << "Requests saved successfully.\n";
}

//load requests
void LibrarySystem::loadRequests() {
    ifstream file("data/requests.txt");

    if (!file.is_open()) {
        cout << "Could not open data/requests.txt\n";
        return;
    }

    string line;
    int largestID = 0;

    while (getline(file, line)) {
        if (requestCount >= 1000) {
            cout << "Maximum request history reached.\n";
            break;
        }

        string idText, userID, resourceIDText;
        string resourceType, date, priorityText, statusText;

        stringstream ss(line);

        if (getline(ss, idText, ',') &&
            getline(ss, userID, ',') &&
            getline(ss, resourceIDText, ',') &&
            getline(ss, resourceType, ',') &&
            getline(ss, date, ',') &&
            getline(ss, priorityText, ',') &&
            getline(ss, statusText)) {

            try {
                int id = stoi(idText);
                int resourceID = stoi(resourceIDText);
                int priority = stoi(priorityText);

                RequestStatus status;

                if (statusText == "PENDING")
                    status = PENDING;
                else if (statusText == "APPROVED")
                    status = APPROVED;
                else if (statusText == "REJECTED")
                    status = REJECTED;
                else if (statusText == "CANCELLED")
                    status = CANCELLED;
                else
                    continue;

                Request request(
                    id,
                    userID,
                    resourceID,
                    resourceType,
                    date,
                    priority,
                    status
                );

                requestHistory[requestCount] = request;
                requestCount++;

                if (id > largestID)
                    largestID = id;

                // Restore pending requests to their queue or heap.
                if (status == PENDING) {
                    if (priority == 3) {
                        requestManager.submitNormalRequest(request);
                    } else {
                        requestManager.submitPriorityRequest(request);
                    }
                }
            }
            catch (...) {
                cout << "Skipping invalid request record.\n";
            }
        }
    }

    file.close();

    nextRequestID = largestID + 1;

    cout << requestCount << " saved request(s) loaded.\n";
}


void LibrarySystem::loadZones() {
    graph.loadFromFile("data/zones.txt");
}

void LibrarySystem::saveZones() {
    cout<< "Degug:saving zone...\n";
    graph.saveToFile("data/zones.txt");
}