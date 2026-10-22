#include <iostream>
#include <string>

#include "./include/hashTable.h"
#include "./include/trees.h"
#include "./include/graph.h"
#include "./include/queue.h"

using namespace std;

struct WaitingRequest {
    string studentID;
    string resource;
    int priority;
    int order;
};

int* availableFor(string resource,
                  int& laptops,
                  int& books,
                  int& rooms) {
    if (resource == "Laptop")
        return &laptops;

    if (resource == "Book")
        return &books;

    if (resource == "Room")
        return &rooms;

    return nullptr;
}

int main() {
    HashTable cards;
    Tree tree;
    Graph graph;

    Queue<WaitingRequest> waitingQueue;

    int laptops = 10;
    int books = 20;
    int rooms = 5;

    int totalLaptops = 10;
    int totalBooks = 20;
    int totalRooms = 5;

    int nextFIFOOrder = 0;
    int choice;

    do {
        cout << "\n LIBRARYHUB \n";
        cout << "1. Tap/register student card\n";
        cout << "2. Show student session\n";
        cout << "3. Temporary exit\n";
        cout << "4. Return to library\n";
        cout << "5. Final checkout\n";
        cout << "6. Search student in BST\n";
        cout << "7. Display student IDs in sorted order\n";
        cout << "8. Request a resource\n";
        cout << "9. Show next request\n";
        cout << "10. Serve next available request\n";
        cout << "11. Add library zone\n";
        cout << "12. Connect zones\n";
        cout << "13. Check path between zones\n";
        cout << "14. Enter a zone\n";
        cout << "15. Leave current zone\n";
        cout << "16. Display zones and occupancy\n";
        cout << "17. Return a resource to inventory\n";
        cout << "0. Exit\n";
        cout << "Choice: ";

        cin >> choice;

        if (choice == 1) {
            string id, session, faculty;
            int priority;

            cout << "Student ID: ";
            cin >> id;

            cout << "Session: ";
            cin >> session;

            cout << "Faculty: ";
            cin >> faculty;

            cout << "Priority (1=urgent, 2=medium, 3=normal): ";
            cin >> priority;

            if (priority < 1 || priority > 3) {
                cout << "Invalid priority.\n";
            }
            else {
                cards.tapCard(id, session, faculty, priority);

                if (cards.isRegistered(id))
                    tree.insertStudent(id);
            }
        }

        else if (choice == 2) {
            string id;

            cout << "Student ID: ";
            cin >> id;

            cards.showSession(id);
        }

        else if (choice == 3) {
            string id;

            cout << "Student ID: ";
            cin >> id;

            cards.temporaryExit(id);
        }

        else if (choice == 4) {
            string id;

            cout << "Student ID: ";
            cin >> id;

            cards.returnToLibrary(id);
        }

        else if (choice == 5) {
            string id;

            cout << "Student ID: ";
            cin >> id;

            cards.checkout(id);
        }

        else if (choice == 6) {
            string id;

            cout << "Student ID to search: ";
            cin >> id;

            if (tree.searchStudent(id))
                cout << "Student ID found in BST.\n";
            else
                cout << "Student ID not found in BST.\n";
        }

        else if (choice == 7) {
            cout << "\nStudent IDs in sorted order:\n";
            tree.displayStudents();
        }

        else if (choice == 8) {
            string id, resource;
            int priority;

            cout << "Student ID: ";
            cin >> id;

            if (!cards.isInside(id)) {
                cout << "Student must be inside the library to request a resource.\n";
                continue;
            }

            cout << "Resource (Laptop, Book, Room): ";
            cin >> resource;

            int* stock = availableFor(
                resource, laptops, books , rooms
            );

            if (stock == nullptr) {
                cout << "Unknown resource type.\n";
                continue;
            }

            cout << "Priority (1=urgent, 2=medium, 3=normal): ";
            cin >> priority;

            if (priority < 1 || priority > 3) {
                cout << "Invalid priority.\n";
                continue;
            }

            if (priority == 3) {
                WaitingRequest request;

                request.studentID = id;
                request.resource = resource;
                request.priority = priority;
                request.order = nextFIFOOrder++;

                waitingQueue.enqueue(request);

                cout << "Request added to the FIFO waiting queue.\n";
            }
            else {
                tree.addRequest(id, resource, priority);
                cout << "Priority request submitted.\n";
            }
        }

        else if (choice == 9) {
            if (tree.hasPriorityRequest()) {
                Tree::RequestInfo request = tree.nextRequest();

                cout << "Next priority request:\n";
                cout << "Student: " << request.studentID << endl;
                cout << "Resource: " << request.resource << endl;
                cout << "Priority: " << request.priority << endl;
            }
            else if (!waitingQueue.isEmpty()) {
                WaitingRequest request = waitingQueue.peek();

                cout << "Next FIFO request:\n";
                cout << "Student: " << request.studentID << endl;
                cout << "Resource: " << request.resource << endl;
            }
            else {
                cout << "No waiting requests.\n";
            }
        }

        else if (choice == 10) {
            bool served = false;

            //highest-priority request first
            if (tree.hasPriorityRequest()) {
                Tree::RequestInfo request = tree.nextRequest();

                int* stock = availableFor(
                    request.resource, laptops, books, rooms
                );

                if (stock != nullptr && *stock > 0) {
                    (*stock)--;

                    cout << "Serving priority request for "
                         << request.studentID << ".\n";
                    cout << "Resource allocated: "
                         << request.resource << endl;

                    tree.removeNextRequest();
                    served = true;
                }
                else {
                    cout << "Priority request is waiting for "
                         << request.resource << ".\n";
                }
            }

            // If no priority request was served, FIFO is applied
            if (!served && !waitingQueue.isEmpty()) {
                WaitingRequest request = waitingQueue.peek();

                int* stock = availableFor(
                    request.resource, laptops, books, rooms
                );

                if (stock != nullptr && *stock > 0) {
                    (*stock)--;

                    cout << "Serving FIFO request for "
                         << request.studentID << ".\n";
                    cout << "Resource allocated: "
                         << request.resource << endl;

                    waitingQueue.dequeue();
                    served = true;
                }
                else {
                    cout << "FIFO request is waiting for "
                         << request.resource << ".\n";
                }
            }

            if (!served)
                cout << "No request could be served right now.\n";
        }

        else if (choice == 11) {
            string zone, faculty;

            cout << "Zone name (no spaces): ";
            cin >> zone;

            cout << "Allowed faculty, or ALL: ";
            cin >> faculty;

            graph.addZone(zone, faculty);
        }

        else if (choice == 12) {
            string first, second;

            cout << "First zone: ";
            cin >> first;

            cout << "Second zone: ";
            cin >> second;

            graph.connectZones(first, second);
        }

        else if (choice == 13) {
            string first, second;

            cout << "Starting zone: ";
            cin >> first;

            cout << "Destination zone: ";
            cin >> second;

            if (graph.connected(first, second))
                cout << "A path exists.\n";
            else
                cout << "No path exists, or a zone is missing.\n";
        }

        else if (choice == 14) {
            string id, zone;

            cout << "Student ID: ";
            cin >> id;

            if (!cards.isInside(id)) {
                cout << "Entry denied: student is not inside the library.\n";
                continue;
            }

            if (cards.hasCurrentZone(id)) {
                cout << "Student must leave their current zone first.\n";
                continue;
            }

            cout << "Zone to enter: ";
            cin >> zone;

            if (graph.enterZone(zone, cards.getFaculty(id)))
                cards.setCurrentZone(id, zone);
        }

        else if (choice == 15) {
            string id;

            cout << "Student ID: ";
            cin >> id;

            if (!cards.isInside(id)) {
                cout << "Student must be inside the library.\n";
                continue;
            }

            if (!cards.hasCurrentZone(id)) {
                cout << "Student is not recorded in a zone.\n";
                continue;
            }

            string zone = cards.getCurrentZone(id);

            if (graph.leaveZone(zone))
                cards.clearCurrentZone(id);
        }

        else if (choice == 16) {
            graph.showZones();

            cout << "\nInventory available:\n";
            cout << "Laptops: " << laptops << endl;
            cout << "Books: " << books << endl;
            cout << "Study rooms: " << rooms << endl;
        }

        else if (choice == 17) {
            string resource;

            cout << "Resource returned (Laptop, Book, Room): ";
            cin >> resource;

            int* stock = availableFor(
                resource, laptops, books, rooms
            );

            if (stock == nullptr) {
                cout << "Unknown resource type.\n";
            }
            else if (resource == "Laptop" &&
                     laptops == totalLaptops) {
                cout << "All laptops are already available.\n";
            }
            else if (resource == "Book" && books== totalBooks) {
                cout << "All books are already available.\n";
            }
            else if (resource == "Room" &&
                     rooms == totalRooms) {
                cout << "All study rooms are already available.\n";
            }
            else {
                (*stock)++;
                cout << "Resource returned. Available count: "
                     << *stock << endl;
            }
        }

        else if (choice == 0) {
            cout << "Exiting LibraryHub.\n";
        }

        else {
            cout << "Invalid choice.\n";
        }

    } while (choice != 0);

    return 0;
}