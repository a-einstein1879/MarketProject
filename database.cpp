#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	lowestSellingPrice = -1;
	highestBuyingPrice = -1;
	cmn_defines = cmn_defines->getCmn_Defines();
	ui = ui->getOpenGLInterface();
};

DataBase* DataBase::getDataBase() {
	if(!p_DataBase) {
		p_DataBase = new DataBase();
	}
	return p_DataBase;
}

void DataBase::start(int Timer) {
	timer = Timer;
}

void DataBase::tick() {
	checkTimers();
	objectsForSale.tick();
	objectsBought.tick();
	timer++;
}

/* TODO: maybe checkTimers should be refactored so that sales will be produced in objects themselves, but it is hard to do it without disabling sales for buyers and for deals
If new class for storing deals and other data will be created, saling can be enabled in objects */
void DataBase::checkTimers() {
	Object object;

	bool ret = true;
	if(objectsForSale.getNumberOfObjects() != 0) {
		object = objectsForSale.timerPop(1);
		if(object.getAge() > cmn_defines->getSellerPriceReduceAge()) {
			if(object.adaptPrice()) {ret = false;}
		}
		if(ret) {objectsForSale.push(object);}
	}
	
	ret = true;
	if(objectsBought.getNumberOfObjects() != 0) {
		object = objectsBought.timerPop(1);
		if(object.getAge() > cmn_defines->getBuyerPriceIncreaseAge()) {
			if(object.adaptPrice()) {ret = false;}
		}
		if(ret) {objectsBought.push(object);}
	}

	refreshPrices();
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
	if(newObject.getStatus() == FORSALE) {
		dealsForSale.push(newObject);
	} else {
		dealsBought.push(newObject);
	}
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
	printf("Number of objects:\nBuying: %d; Deals(Bought / For sale) = %d/%d; For sale: %d\n", objectsBought.getNumberOfObjects(), dealsBought.getNumberOfObjects(), dealsForSale.getNumberOfObjects(), objectsForSale.getNumberOfObjects());
	printf("Mean prices:\nBuying = %.2f; Deals(Bought / For sale) = %.2f/%.2f; Selling = %.2f\n",
		objectsBought.getMeanPrice(), dealsBought.getMeanPrice(), dealsForSale.getMeanPrice(), objectsForSale.getMeanPrice());
	printf("Mean timers:\nBuying = %.2f; Deals(Bought / For sale) = %.2f/%.2f; Selling = %.2f\n",
		objectsBought.getMeanTimer(), dealsBought.getMeanTimer(), dealsForSale.getMeanTimer(), objectsForSale.getMeanTimer());

	double sellersMeanWaitingTime, buyersMeanWaitingTime;
	int numberOfSellers = dealsForSale.getNumberOfObjects() + objectsForSale.getNumberOfObjects();
	int numberOfBuyers = dealsBought.getNumberOfObjects() + objectsBought.getNumberOfObjects();
	sellersMeanWaitingTime = dealsForSale.getMeanTimer() * double(dealsForSale.getNumberOfObjects()) / double(numberOfSellers) + objectsForSale.getMeanTimer() * double(objectsForSale.getNumberOfObjects()) / double(numberOfSellers);
	buyersMeanWaitingTime = dealsBought.getMeanTimer() * double(dealsBought.getNumberOfObjects()) / double(numberOfBuyers) + objectsBought.getMeanTimer() * double(objectsBought.getNumberOfObjects()) / double(numberOfBuyers);
	if(highestBuyingPrice != -1 && lowestSellingPrice != -1) {
		printf("Mean waiting time(Bought / For sale) = %.2f/%.2f\n", buyersMeanWaitingTime, sellersMeanWaitingTime);
	}

	if(highestBuyingPrice != -1 && lowestSellingPrice != -1) {
		printf("Spread = %.2f, Mean spread = %.2f\n\n", lowestSellingPrice - highestBuyingPrice, meanSpread.getMeanPrice());
	}
	/* End of info part */
	
	/* TODO: there is some trouble with pushing to position. Needs clearifing */
	Object newObject(objectsForSale.getMeanPrice(), -1, 0);
	meanForSalePrice.push(newObject, meanForSalePrice.getNumberOfObjects() + 1);
	newObject.setObject(objectsBought.getMeanPrice(), -1, 0);
	meanBoughtPrice.push(newObject, meanBoughtPrice.getNumberOfObjects() + 1);
	
	newObject.setObject(highestBuyingPrice, -1, 0);
	bidPrice.push(newObject, bidPrice.getNumberOfObjects() + 1);
	newObject.setObject(lowestSellingPrice, -1, 0);
	askPrice.push(newObject, askPrice.getNumberOfObjects() + 1);
	
	newObject.setObject(objectsBought.getNumberOfObjects(), -1, 0);
	meanBoughtNumberOfObjects.push(newObject, meanBoughtNumberOfObjects.getNumberOfObjects() + 1);
	newObject.setObject(objectsForSale.getNumberOfObjects(), -1, 0);
	meanForSaleNumberOfObjects.push(newObject, meanForSaleNumberOfObjects.getNumberOfObjects() + 1);
	
	newObject.setObject(lowestSellingPrice - highestBuyingPrice, -1, 0);
	meanSpread.push(newObject, meanForSaleNumberOfObjects.getNumberOfObjects() + 1);
#ifndef SILENTMODE
	if(objectsForSale.getNumberOfObjects() != 0) {
		printf("The lowest selling price is %.2f\n", lowestSellingPrice);
		printf("Number of objects for sale is %d:\n", objectsForSale.getNumberOfObjects());
		objectsForSale.viewTimers();
	} else {
		printf("Noone is selling anything\n");
	}
	
	if(objectsBought.getNumberOfObjects() != 0) {
		printf("The highest buying price is %.2f\n", highestBuyingPrice);
		printf("Number of objects bought is %d:\n", objectsBought.getNumberOfObjects());
		objectsBought.viewTimers();
	} else {
		printf("Noone is buying anything\n");
	}

	printf("End of database\n\n");
#endif
}

void DataBase::refreshPicture() {
	if(!(cmn_defines->getGraphicalMode())) {return;}

	/* Price histogram */
	if(objectsForSale.getNumberOfObjects() == 0 || objectsBought.getNumberOfObjects() == 0) {return;}
	
	double maxArgument;
	double minArgument;
	if(cmn_defines->getConstantBoardersMode() == 0) {
		minArgument = cmn_defines->getMinimumHistogramArgument();
		maxArgument = cmn_defines->getMaximumHistogramArgument();
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
	histogram.setTmpChartIndex(0);
	objectsForSale.feelHistogram(histogram);
	/*histogram.setTmpChartIndex(1);
	dealsForSale.feelHistogram(histogram);
	dealsBought.feelHistogram(histogram);*/
	histogram.setTmpChartIndex(2);
	objectsBought.feelHistogram(histogram);

	/* Mean prices charts */
	int numberOfArguments = cmn_defines->getModelingTime() / cmn_defines->getTimerPrintingFrequency();
	LineChart lineChart1(5, 0, numberOfArguments);
	lineChart1.setTmpChartIndex(0);
	meanForSalePrice.feelLineChart(lineChart1);
	lineChart1.setTmpChartIndex(1);
	askPrice.feelLineChart(lineChart1);
	lineChart1.setTmpChartIndex(2);
	bidPrice.feelLineChart(lineChart1);
	lineChart1.setTmpChartIndex(3);
	meanBoughtPrice.feelLineChart(lineChart1);
	/*lineChart1.setTmpChartIndex(4);
	meanSpread.feelLineChart(lineChart1);*/

	LineChart lineChart2(2, 0, numberOfArguments);
	lineChart2.setTmpChartIndex(0);
	meanForSaleNumberOfObjects.feelLineChart(lineChart2);
	lineChart2.setTmpChartIndex(1);
	meanBoughtNumberOfObjects.feelLineChart(lineChart2);

	ui->printCharts(histogram, lineChart1, lineChart2);
	Sleep(cmn_defines->getPictureDelayTime());
}