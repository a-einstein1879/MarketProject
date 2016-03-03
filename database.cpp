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
		if((lowestSellingPrice == -1) || newObject.getPrice() < lowestSellingPrice) {lowestSellingPrice = newObject.getPrice();}
	} else {
		objectsBought.push(newObject, findPositionForObject(newObject));
		if((highestBuyingPrice == -1) || newObject.getPrice() > highestBuyingPrice) {highestBuyingPrice = newObject.getPrice();}
	}
	return 0;
}

#include <stdio.h>
void DataBase::viewDataBase() {
	printf("The lowest selling price is %.2f\n", lowestSellingPrice);
	printf("Number of objects for sale is %d:\n", objectsForSale.getNumberOfObjects());
	objectsForSale.view();

	printf("The highest buying price is %.2f\n", highestBuyingPrice);
	printf("Number of objects bought is %d:\n", objectsBought.getNumberOfObjects());
	objectsBought.view();
}

bool DataBase::dealPossible() {
	if(highestBuyingPrice >= lowestSellingPrice) {
		return true;
	} else {
		return false;
	}
}