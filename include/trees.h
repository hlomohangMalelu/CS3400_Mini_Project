#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <string>
using namespace std;

class Tree {
public:
    struct RequestInfo {
        string studentID;
        string resource;
        int priority;
        int order;
    };

private:
    struct Node {
        string studentID;
        Node* left;
        Node* right;

        Node(string id) {
            studentID = id;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    void insertNode(Node*& current, string id);
    bool searchNode(Node* current, string id);
    void displayInOrder(Node* current);
    void clearTree(Node* current);

    RequestInfo heap[100];
    int heapSize;
    int nextOrder;

    bool comesBefore(RequestInfo a, RequestInfo b);
    void swapRequests(int a, int b);
    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    Tree();
    ~Tree();

    void insertStudent(string id);
    bool searchStudent(string id);
    void displayStudents();

    void addRequest(string id, string resource, int priority);
    bool hasPriorityRequest();
    RequestInfo nextRequest();
    void removeNextRequest();
};

#endif