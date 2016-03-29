#include "linklist.h"
#include <stdlib.h>
#include <stdio.h>

LinkList::LinkList() {
	firstPrice = NULL;
	firstTimer = NULL;
	numberOfObjects = 0;
	meanPrice = 0;
	timer = 1;
}

void LinkList::tick() {
	timer++;
	if(firstTimer == NULL) {
		return;
	}
	Node* newnode = firstTimer;
	do {
		newnode->object.tick();
		newnode = newnode->nextTimer;
	} while(newnode != NULL);
}

void LinkList::push(Object object, int pricePosition, int timerPosition) {
	if(pricePosition == -1) {pricePosition = findPricePositionForObject(object);}
	if(timerPosition == -1) {timerPosition = findTimerPositionForObject(object);}

	Node* newNode = new Node;
	newNode->object = object;

	if(firstPrice != NULL) {
		Node* temp = firstPrice;
		bool end = false;
		for (int i = 1; i < pricePosition; i++) {
			if(temp->nextPrice != NULL) { /* In case we exceeded the number of elements, the element will be added in the end */
				temp = temp->nextPrice;
			} else {
				end = true;
			}
		}
		if(!end) {
			if(temp->prevPrice != NULL) {
				temp->prevPrice->nextPrice = newNode;
			} else {
				firstPrice = newNode;
			}
			newNode->prevPrice = temp->prevPrice;
			newNode->nextPrice = temp;
			temp   ->prevPrice = newNode;
		} else {
			newNode->prevPrice = temp;
			newNode->nextPrice = NULL;
			temp   ->nextPrice = newNode;
		}
	}
	else {
		newNode->nextPrice = NULL;
		newNode->prevPrice = NULL;
		firstPrice = newNode;
	}
	
	if(firstTimer != NULL) {
		Node* temp = firstTimer;
		bool end = false;
		for (int i = 1; i < timerPosition; i++) {
			if(temp->nextTimer != NULL) { /* In case we exceeded the number of elements, the element will be added in the end */
				temp = temp->nextTimer;
			} else {
				end = true;
			}
		}
		if(!end) {
			if(temp->prevTimer != NULL) {
				temp->prevTimer->nextTimer = newNode;
			} else {
				firstTimer = newNode;
			}
			newNode->prevTimer = temp->prevTimer;
			newNode->nextTimer = temp;
			temp   ->prevTimer = newNode;
		} else {
			newNode->prevTimer = temp;
			newNode->nextTimer = NULL;
			temp   ->nextTimer = newNode;
		}
	}
	else {
		newNode->nextTimer = NULL;
		newNode->prevTimer = NULL;
		firstTimer = newNode;
	}
	
	if(numberOfObjects != 0) {
		meanPrice = meanPrice * double(numberOfObjects) / (double(numberOfObjects) + 1) + object.getPrice() / (double(numberOfObjects) + 1);
	} else {
		meanPrice = object.getPrice();
	}
	numberOfObjects++;
}
 
Object LinkList::pricePop(int pos) {
	if(numberOfObjects == 0) {
		Object null;
		return null;
	}

	Object returnValue;
	if(firstPrice->nextPrice != NULL) { /* if the element is not the only element */
		Node* temp = firstPrice;
		for (int i = 1; i < pos; i++) {
			if(temp->nextPrice != NULL) {
				temp = temp->nextPrice;
			}
		}
		returnValue = temp->object;
		if(temp == firstPrice) {
			firstPrice = temp->nextPrice;
			temp->nextPrice->prevPrice = NULL;
		} else {
			temp->prevPrice->nextPrice = temp->nextPrice;
			if(temp->nextPrice != NULL) {
				temp->nextPrice->prevPrice = temp->prevPrice;
			}
		}
		if(temp == firstTimer) {
			firstTimer = temp->nextTimer;
			temp->nextTimer->prevTimer = NULL;
		} else {
			temp->prevTimer->nextTimer = temp->nextTimer;
			if(temp->nextTimer != NULL) {
				temp->nextTimer->prevTimer = temp->prevTimer;
			}
		}
		delete temp;
	}
	else {
		returnValue = firstPrice->object; 
		delete firstPrice;
		firstPrice = NULL;
		firstTimer = NULL;
	}

	if(numberOfObjects != 1) {
		meanPrice = meanPrice * double(numberOfObjects) / (double(numberOfObjects) - 1) - returnValue.getPrice() / (double(numberOfObjects) - 1);
	}
	else {
		meanPrice = 0;
	}
	numberOfObjects--;
	return returnValue;
} 

Object LinkList::timerPop(int pos) {
	if(numberOfObjects == 0) {
		Object null;
		return null;
	}

	Object returnValue;
	if(firstTimer->nextTimer != NULL) { /* if the element is not the only element */
		Node* temp = firstTimer;
		for (int i = 1; i < pos; i++) {
			if(temp->nextTimer != NULL) {
				temp = temp->nextTimer;
			}
		}
		returnValue = temp->object;
		if(temp == firstPrice) {
			firstPrice = temp->nextPrice;
			temp->nextPrice->prevPrice = NULL;
		} else {
			temp->prevPrice->nextPrice = temp->nextPrice;
			if(temp->nextPrice != NULL) {
				temp->nextPrice->prevPrice = temp->prevPrice;
			}
		}
		if(temp == firstTimer) {
			firstTimer = temp->nextTimer;
			temp->nextTimer->prevTimer = NULL;
		} else {
			temp->prevTimer->nextTimer = temp->nextTimer;
			if(temp->nextTimer != NULL) {
				temp->nextTimer->prevTimer = temp->prevTimer;
			}
		}
		delete temp;
	}
	else {
		returnValue = firstTimer->object; 
		delete firstTimer;
		firstPrice = NULL;
		firstTimer = NULL;
	}

	if(numberOfObjects != 1) {
		meanPrice = meanPrice * double(numberOfObjects) / (double(numberOfObjects) - 1) - returnValue.getPrice() / (double(numberOfObjects) - 1);
	}
	else {
		meanPrice = 0;
	}
	numberOfObjects--;
	return returnValue;
}
 
void LinkList::clean() {
	if(firstPrice == NULL) {printf("List is already empty\n"); return;}
	for(Node* newnode = firstPrice; newnode->nextPrice != NULL; newnode = firstPrice) {
		firstPrice = newnode->nextPrice;
		delete newnode;
	}
	delete firstPrice;
	firstPrice = NULL;
	firstTimer = NULL;
}
 
void LinkList::viewPrice() {
	if(firstPrice == NULL) {
		printf("Price list is empty\n");
		return;
	}
	Node* newnode = firstPrice;
	printf("Number of objects is %d\n", numberOfObjects);
	do {
		newnode->object.printObject();
		newnode = newnode->nextPrice;
	} while(newnode != NULL);
	printf("End of the price list\n");
}

void LinkList::viewTimers() {
	if(firstTimer == NULL) {
		printf("Timer list is empty\n");
		return;
	}
	Node* newnode = firstTimer;
	printf("Number of objects is %d\n", numberOfObjects);
	do {
		newnode->object.printObject();
		newnode = newnode->nextTimer;
	} while(newnode != NULL);
	printf("End of the timer list\n");
}

/* To understand next two functions it`s important to understand the difference between > and >= operators for objects.
> works with creation times, while >= works with prices */

int LinkList::findPricePositionForObject(Object object) {
	if(firstPrice == NULL) {return -1;}
	
	if(firstPrice->object >= object) {return 1;}

	int pos = 1;
	bool brk = 0;

	Node* newnode = firstPrice;
	while(newnode->nextPrice != NULL) {
		if((newnode->object <= object) && (object <= newnode->nextPrice->object)) {brk = 1; pos++; break;}
		newnode = newnode->nextPrice;
		pos++;
	} 

	if(brk == 0) {
		return ++pos;
	}

	return pos;
}

int LinkList::findTimerPositionForObject(Object object) {
	if(firstTimer == NULL) {return -1;}
	
	if(firstTimer->object < object) {return 1;}

	int pos = 1;
	bool brk = 0;

	Node* newnode = firstTimer;
	while(newnode->nextTimer != NULL) {
		if((newnode->object > object) && (object > newnode->nextTimer->object)) {brk = 1; pos++; break;}
		newnode = newnode->nextTimer;
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

double LinkList::getMeanPrice() {
	return meanPrice;
}

double LinkList::getMeanTimer() {
	if(firstPrice == NULL) {return 0;}

	Node* newnode = firstPrice;
	double meanTimer = 0;
	do {
		meanTimer += newnode->object.getAge();
		newnode = newnode->nextPrice;
	} while(newnode != NULL);
	return meanTimer / double(numberOfObjects);
}

void LinkList::feelHistogram(Histogram &histogram) {
	if(firstPrice == NULL) {return;}
	Node* newnode = firstPrice;
	do {
		newnode->object.pushPriceToHistogram(histogram);
		newnode = newnode->nextPrice;
	} while(newnode != NULL);
}

void LinkList::feelLineChart(LineChart &lineChart) {
	if(firstPrice == NULL) {return;}
	Node* newnode = firstPrice;
	do {
		newnode->object.pushPriceToChart(lineChart);
		newnode = newnode->nextPrice;
	} while(newnode != NULL);
}