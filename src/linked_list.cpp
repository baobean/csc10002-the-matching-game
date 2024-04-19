#include "linked_list.h"

LinkedList::LinkedList() {
    head = nullptr;
    tail = nullptr;
}

Cell *createNode(Cell *_node) {
    Cell *new_node = new Cell(_node);
    return new_node;
}

bool LinkedList::isEmpty() {
    if (head == nullptr && tail == nullptr) return true;
    return false;
}

void LinkedList::addTail(Cell *_node) {
    Cell *new_node = createNode(_node);

    if (isEmpty() == true) {
        head = tail = new_node;
        return;
    }

    tail->next = new_node;
    tail = new_node;
}

Cell *LinkedList::getNode(int pos) {
    Cell *curr_node = head;
    for(int curr_pos = 0; curr_pos < pos; ++curr_pos) {
        curr_node = curr_node->next;
    }

    return curr_node;
}

void LinkedList::shiftCell(Cell *target_node) {
    Cell *curr_node = head;

    if (head == target_node) {
        target_node->cell_value = 0;
        return;
    }
    while (curr_node->next != target_node) {
        curr_node = curr_node->next;
    }

    curr_node->next = target_node->next;
    target_node->next = head;
    head = target_node;
    target_node->cell_value = 0;
}

void LinkedList::deleteNode(Cell *target_node) {
    if (target_node->next == nullptr) {
        delete target_node;
        return;
    }

    deleteNode(target_node->next);
    target_node->next = nullptr;
    delete target_node;
}

void LinkedList::removeAll() {
    Cell *target_cell = head;
    deleteNode(head);
}

void LinkedList::printList() {
    Cell *curr_node = head;
    while (curr_node != nullptr) {
        if (curr_node->cell_value != 0) {
            std::cout << curr_node->cell_value << ' ';
        }
        else std::cout << 0 << ' ';
        curr_node = curr_node->next;
    }
}
