#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	lowestSellingPrice = -1;
	highestBuyingPrice = -1;
	cmn_defines = cmn_defines->getCmn_Defines();
};

DataBase* DataBase::getDataBase() {
	if(!p_DataBase) {
		p_DataBase = new DataBase();
	}
	return p_DataBase;
}

int DataBase::pushToDataBase(Object newObject) {
	if(newObject.getStatus() == FORSALE) {
		objectsForSale.push(newObject);
	} else {
		objectsBought.push(newObject);
	}
	refreshPrices();
	return 0;
}

void DataBase::addDeal(Object newObject) {
	deals.push(newObject);
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
		object = objectsForSale.pricePop(1);
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
		object = objectsBought.pricePop(objectsBought.getNumberOfObjects());
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
		object = objectsForSale.pricePop(1);
		lowestSellingPrice = object.getPrice();
		objectsForSale.push(object);
	} else {
		lowestSellingPrice = -1;
	}
	
	if(objectsBought.getNumberOfObjects() != 0) {
		object = objectsBought.pricePop(objectsBought.getNumberOfObjects());
		highestBuyingPrice = object.getPrice();
		objectsBought.push(object);
	} else {
		highestBuyingPrice = -1;
	}
}

#include <stdio.h>
void DataBase::viewDataBase() {
#ifndef SILENTMODE
	if(objectsForSale.getNumberOfObjects() != 0) {
		printf("The lowest selling price is %.2f\n", lowestSellingPrice);
		printf("Number of objects for sale is %d:\n", objectsForSale.getNumberOfObjects());
		objectsForSale.viewPrice();
	} else {
		printf("Noone is selling anything\n");
	}
	
	if(objectsBought.getNumberOfObjects() != 0) {
		printf("The highest buying price is %.2f\n", highestBuyingPrice);
		printf("Number of objects bought is %d:\n", objectsBought.getNumberOfObjects());
		objectsBought.viewPrice();
	} else {
		printf("Noone is buying anything\n");
	}

	printf("End of database\n\n");
#endif
}

#include "interface.h"
void DataBase::refreshPicture() {
	if(!(cmn_defines->getGraphicalMode())) {return;}
#ifdef VISUALMODE
	if(objectsForSale.getNumberOfObjects() == 0 || objectsBought.getNumberOfObjects() == 0) {return;}
	OpenGLInterface *ui;
	ui = ui->getOpenGLInterface();

	Object object = objectsForSale.pricePop(objectsForSale.getNumberOfObjects());
	double maxArgument = object.getPrice();
	objectsForSale.push(object);
	object = objectsBought.pricePop(1);
	double minArgument = object.getPrice();
	objectsBought.push(object);
	refreshPrices();

	/* TODO: this is workaround. Has to be removed after solution to -1.#J problem is found */
	maxArgument = 25;
	/* End of workaround */
	Histogram histogram(3, cmn_defines->getNumberOfPockets(), minArgument, maxArgument);

	histogram.setTmpIndex(0);
	objectsForSale.feelHistogram(histogram);
	histogram.setTmpIndex(1);
	deals.feelHistogram(histogram);
	histogram.setTmpIndex(2);
	objectsBought.feelHistogram(histogram);
	
	ui->printHistogram(histogram);
	Sleep(cmn_defines->getPictureDelayTime());
#endif
}