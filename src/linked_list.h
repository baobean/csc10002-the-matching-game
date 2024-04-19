#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>

#include "cell.h"

struct LinkedList {
    Cell *head;
    Cell *tail;
    int list_size;

    LinkedList();               //create linkedlist

    bool isEmpty();             //check if linkedlist is empty

    void addTail(Cell *);       //add node to tail

    Cell *getNode(int);         //get a node in linkedlist

    void shiftCell(Cell *);     //shift a cell to right

    void deleteNode(Cell *);    //delete a node
    void removeAll();           //remove all nodes

    void printList();           //print linkedlist (for debug)
};

#endif // LINKED_LIST_H
