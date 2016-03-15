#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	lowestSellingPrice = -1;
	highestBuyingPrice = -1;
};

DataBase* DataBase::getDataBase() {
	if(!p_DataBase) {
		p_DataBase = new DataBase();
	}
	return p_DataBase;
}

int DataBase::findPositionForObject(Object object) {
	if(object.getStatus() == FORSALE) {
		return objectsForSale.findPositionForObject(object);
	} else {
		return objectsBought.findPositionForObject(object);
	}
	return -1;
}

int DataBase::pushToDataBase(Object newObject) {
	if(newObject.getStatus() == FORSALE) {
		objectsForSale.push(newObject, findPositionForObject(newObject));
	} else {
		objectsBought.push(newObject, findPositionForObject(newObject));
	}
	refreshPrices();
	return 0;
}

bool DataBase::dealPossible() {
	if(objectsForSale.getNumberOfObjects() == 0 || objectsBought.getNumberOfObjects() == 0) {return false;}
	if(highestBuyingPrice >= lowestSellingPrice) {
		return true;
	} else {
		return false;
	}
}

Object DataBase::popLowestSeller() {
	if(objectsForSale.getNumberOfObjects() > 0) {
		Object object;
		object = objectsForSale.pop(1);
		refreshPrices();
		return object;
	} else {
		Object object;
		return object;
	}
}

Object DataBase::popHighestBuyer() {
	if(objectsBought.getNumberOfObjects() > 0) {
		Object object;
		object = objectsBought.pop(objectsBought.getNumberOfObjects());
		refreshPrices();
		return object;
	} else {
		Object object;
		return object;
	}
}

#include <stdlib.h>
void DataBase::refreshPrices() {
	Object object;

	if(objectsForSale.getNumberOfObjects() != 0) {
		object = objectsForSale.pop(1);
		lowestSellingPrice = object.getPrice();
		objectsForSale.push(object, 1);
	} else {
		lowestSellingPrice = -1;
	}
	
	if(objectsBought.getNumberOfObjects() != 0) {
		object = objectsBought.pop(objectsBought.getNumberOfObjects());
		highestBuyingPrice = object.getPrice();
		objectsBought.push(object, findPositionForObject(object));
	} else {
		highestBuyingPrice = -1;
	}
}

#include <stdio.h>
void DataBase::viewDataBase() {
	if(objectsForSale.getNumberOfObjects() != 0) {
		printf("The lowest selling price is %.2f\n", lowestSellingPrice);
		printf("Number of objects for sale is %d:\n", objectsForSale.getNumberOfObjects());
		objectsForSale.view();
	} else {
		printf("Noone is selling anything\n");
	}
	
	if(objectsBought.getNumberOfObjects() != 0) {
		printf("The highest buying price is %.2f\n", highestBuyingPrice);
		printf("Number of objects bought is %d:\n", objectsBought.getNumberOfObjects());
		objectsBought.view();
	} else {
		printf("Noone is buying anything\n");
	}

	printf("End of database\n\n");
}