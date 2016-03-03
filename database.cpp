#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	numberOfObjectsForSale = 0;
	numberOfObjectsBought  = 0;
};

DataBase* DataBase::getDataBase() {
	if(!p_DataBase) {
		p_DataBase = new DataBase();
	}
	return p_DataBase;
}

int DataBase::findPositionForObject(Object object) {return 0;}

int DataBase::pushToDataBase(Object newObject) {
	if(newObject.getStatus() == FORSALE) {
		numberOfObjectsForSale++;
		objectsForSale.push(newObject);
	} else {
		numberOfObjectsBought++;
		objectsBought.push(newObject);
	}
	return 0;
}

#include <stdio.h>
void DataBase::viewDataBase() {
	printf("Number of objects for sale is %d:\n", numberOfObjectsForSale);
	objectsForSale.view();
	printf("Number of objects bought is %d:\n", numberOfObjectsBought);
	objectsBought.view();
}