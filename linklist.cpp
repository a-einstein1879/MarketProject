#include "linklist.h"
#include <stdlib.h>
#include <stdio.h>

LinkList::LinkList() {
	first = NULL;
}

void LinkList::push(Object object, int pos) {
	/* TODO: Push at the end of the list will be a problem */
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
		printf("price = %.2e, creation time = %.2e, status = %d\n", newnode->object.getPrice(), newnode->object.getCreationTime(), newnode->object.getStatus());
		newnode = newnode->next;
	} while(newnode != NULL);
	printf("End of the list\n");
}