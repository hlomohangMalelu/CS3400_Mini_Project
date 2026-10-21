#include "Tree.h"

Tree::Tree() {
    root = nullptr;
    heapSize = 0;
    nextOrder = 0;
}

Tree::~Tree() {
    clearTree(root);
}

void Tree::insertNode(Node*& current, string id) {
    if (current == nullptr) {
        current = new Node(id);
    }
    else if (id < current->studentID) {
        insertNode(current->left, id);
    }
    else if (id > current->studentID) {
        insertNode(current->right, id);
    }
}

void Tree::insertStudent(string id) {
    insertNode(root, id);
}

bool Tree::searchNode(Node* current, string id) {
    if (current == nullptr)
        return false;

    if (current->studentID == id)
        return true;

    if (id < current->studentID)
        return searchNode(current->left, id);

    return searchNode(current->right, id);
}

bool Tree::searchStudent(string id) {
    return searchNode(root, id);
}

void Tree::displayInOrder(Node* current) {
    if (current != nullptr) {
        displayInOrder(current->left);
        cout << current->studentID << endl;
        displayInOrder(current->right);
    }
}

void Tree::displayStudents() {
    displayInOrder(root);
}

void Tree::clearTree(Node* current) {
    if (current != nullptr) {
        clearTree(current->left);
        clearTree(current->right);
        delete current;
    }
}

bool Tree::comesBefore(RequestInfo a, RequestInfo b) {
    if (a.priority != b.priority)
        return a.priority < b.priority;

    return a.order < b.order;
}

void Tree::swapRequests(int a, int b) {
    RequestInfo temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

void Tree::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;

        if (!comesBefore(heap[index], heap[parent]))
            break;

        swapRequests(index, parent);
        index = parent;
    }
}

void Tree::heapifyDown(int index) {
    while (true) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heapSize &&
            comesBefore(heap[left], heap[smallest]))
            smallest = left;

        if (right < heapSize &&
            comesBefore(heap[right], heap[smallest]))
            smallest = right;

        if (smallest == index)
            break;

        swapRequests(index, smallest);
        index = smallest;
    }
}

void Tree::addRequest(string id, string resource, int priority) {
    if (heapSize == 100) {
        cout << "Priority request heap is full.\n";
        return;
    }

    if (priority < 1 || priority > 2) {
        cout << "Heap accepts priority 1 or 2 only.\n";
        return;
    }

    heap[heapSize].studentID = id;
    heap[heapSize].resource = resource;
    heap[heapSize].priority = priority;
    heap[heapSize].order = nextOrder++;

    heapifyUp(heapSize);
    heapSize++;
}

bool Tree::hasPriorityRequest() {
    return heapSize > 0;
}

Tree::RequestInfo Tree::nextRequest() {
    RequestInfo result;
    result.studentID = "";
    result.resource = "";
    result.priority = 0;
    result.order = 0;

    if (heapSize > 0)
        result = heap[0];

    return result;
}

void Tree::removeNextRequest() {
    if (heapSize == 0)
        return;

    heap[0] = heap[heapSize - 1];
    heapSize--;

    if (heapSize > 0)
        heapifyDown(0);
}