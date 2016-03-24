#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	timer = 1;
	lowestSellingPrice = -1;
	highestBuyingPrice = -1;
	cmn_defines = cmn_defines->getCmn_Defines();
	meanSpread = 0;
};

DataBase* DataBase::getDataBase() {
	if(!p_DataBase) {
		p_DataBase = new DataBase();
	}
	return p_DataBase;
}

void DataBase::tick() {
	timer++;
	objectsForSale.tick();
	objectsBought.tick();
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
	/* Info part */
	printf("Buying mean price = %.2f Deals mean price = %.2f Selling mean price = %.2f\n", objectsBought.getMeanPrice(), deals.getMeanPrice(), objectsForSale.getMeanPrice());
	if(highestBuyingPrice != -1 && lowestSellingPrice != -1) {
		printf("Spread is = %.2f\n", lowestSellingPrice - highestBuyingPrice);
	}
	/* End of info part */

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
	if(objectsForSale.getNumberOfObjects() == 0 || objectsBought.getNumberOfObjects() == 0) {return;}
	OpenGLInterface *ui;
	ui = ui->getOpenGLInterface();
	
	double maxArgument;
	double minArgument;
	if(cmn_defines->getConstantBoardersMode() == 0) {
		minArgument = 0;
		maxArgument = 25;
	} else {
		Object object = objectsForSale.pricePop(objectsForSale.getNumberOfObjects());
		maxArgument = object.getPrice();
		objectsForSale.push(object);
		object = objectsBought.pricePop(1);
		minArgument = object.getPrice();
		objectsBought.push(object);
		refreshPrices();
	}

	Histogram histogram(3, cmn_defines->getNumberOfPockets(), minArgument, maxArgument);

	histogram.setTmpIndex(0);
	objectsForSale.feelHistogram(histogram);
	histogram.setTmpIndex(1);
	deals.feelHistogram(histogram);
	histogram.setTmpIndex(2);
	objectsBought.feelHistogram(histogram);
	 
	ui->printHistogram(histogram);
	Sleep(cmn_defines->getPictureDelayTime());
}