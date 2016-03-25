#ifndef LINKLIST_H
#define LINKLIST_H

#include "object.h"

struct Node 
{
    Object object;
    Node* nextPrice;
    Node* prevPrice;

	Node* nextTimer;
	Node* prevTimer;
}; 

/*
This is doubly linked list. The first elemtn previous and the last element next is pointing at NULL.
New elements are added with use of findPositionForObject by default.
If position exceeds the number of elements in list, element will be added in the end.
Pop is done from the beginning of the list by default
*/

class LinkList { 
private: 
    Node* firstPrice;
	Node* firstTimer;
	int numberOfObjects;
	double meanPrice;
	double meanTimer;
	int timer;
public: 
    LinkList();
	void tick();
 
    void push(Object object, int pricePosition = -1, int timerPosition = -1);
    Object pricePop(int pos = 1);
    Object timerPop(int pos = 1);
    void clean();
    void viewPrice();
    void viewTimers();
	
	/* We can find position for the object with use of this function. It uses <= and >= from Object.h to define where to place object */
	int findPricePositionForObject(Object object);
	int findTimerPositionForObject(Object object);
	int getNumberOfObjects();
	double getMeanPrice();
	double getMeanTimer();

	void feelHistogram(Histogram &histogram);
}; 

#endif