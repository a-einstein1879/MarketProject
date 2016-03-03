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
	return 0;
}

#include <stdio.h>
void DataBase::viewDataBase() {
	printf("Number of objects for sale is %d:\n", objectsForSale.getNumberOfObjects());
	objectsForSale.view();
	printf("Number of objects bought is %d:\n", objectsBought.getNumberOfObjects());
	objectsBought.view();
}