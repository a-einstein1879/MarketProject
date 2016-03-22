#ifndef LINKLIST_H
#define LINKLIST_H

#include "object.h"

struct node 
{
    Object object;
    node* next;
    node* prev;
}; 

/*
This is doubly linked list. The first elemtn previous and the last element next is pointing at NULL.
New elements are added with use of findPositionForObject by default.
If position exceeds the number of elements in list, element will be added in the end.
Pop is done from the beginning of the list by default
*/

class LinkList { 
private: 
    node* first;
	int numberOfObjects;
public: 
    LinkList();
 
    void push(Object object, int pos = -1);
    Object pop(int pos = 1);
    void clean();
    void view();
	
	/* We can find position for the object with use of this function. It uses <= and >= from Object.h to define where to place object */
	int findPositionForObject(Object object);
	int getNumberOfObjects();
}; 

#endif