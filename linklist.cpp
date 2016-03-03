#include "linklist.h"
#include <stdlib.h>
#include <stdio.h>

LinkList::LinkList() {
	first = NULL;
	numberOfObjects = 0;
}

void LinkList::push(Object object, int pos) {
	node* newNode = new node;
	newNode->object = object;

	if(first != NULL) {
		node* temp = first;
		bool end = false;
		for (int i = 1; i < pos; i++) {
			if(temp->next != NULL) { /* In case we exceeded the number of elements, the element will be added in the end */
				temp = temp->next;
			} else {
				end = true;
			}
		}
		if(!end) {
			if(temp->prev != NULL) {
				temp->prev->next = newNode;
			} else {
				first = newNode;
			}
			newNode->prev = temp->prev;
			newNode->next = temp;
			temp   ->prev = newNode;
		} else {
			newNode->prev = temp;
			newNode->next = NULL;
			temp   ->next = newNode;
		}
	}
	else {
		newNode->next = NULL;
		newNode->prev = NULL;
		first = newNode;
	}
	numberOfObjects++;
}
 
Object LinkList::pop(int pos) {
	if(first == NULL) {
		Object null;
		return null;
	}

	Object returnValue;
	if(first->next != NULL) { /* if the element is not the only element */
		node* temp = first;
		for (int i = 1; i < pos; i++) {
			if(temp->next != NULL) {
				temp = temp->next;
			}
		}
		returnValue = temp->object;
		if(temp == first) {
			first = temp->next;
			temp->next->prev = NULL;
		} else {
			temp->prev->next = temp->next;
			if(temp->next != NULL) {
				temp->next->prev = temp->prev;
			}
		}
		delete temp;
	}
	else {
		returnValue = first->object; 
		delete first;
		first = NULL;
	}
	numberOfObjects--;
	return returnValue;
} 
 
void LinkList::clean() {
	if(first == NULL) {printf("List is already empty\n"); return;}
	for(node* newnode = first; newnode->next != NULL; newnode = first) {
		first = newnode->next;
		delete newnode;
	}
	delete first; 
	first = NULL; 
}
 
void LinkList::view() {
	if(first == NULL) {
		printf("List is empty\n");
		return;
	}
	node* newnode = first;
	do {
		newnode->object.printObject();
		newnode = newnode->next;
	} while(newnode != NULL);
	printf("End of the list\n");
}

int LinkList::findPositionForObject(Object object) {
	if(first == NULL) {return -1;}
	
	if(first->object >= object) {return 1;}

	int pos = 1;
	bool brk = 0;

	node* newnode = first;
	while(newnode->next != NULL) {
		if((newnode->object <= object) && (object <= newnode->next->object)) {brk = 1; pos++; break;}
		newnode = newnode->next;
		pos++;
	} 

	if(brk == 0) {
		return ++pos;
	}

	return pos;
}

int LinkList::getNumberOfObjects() {
	return numberOfObjects;
}