#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	timer = 1;
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

void DataBase::tick() {
	timer++;
	objectsForSale.tick();
	objectsBought.tick();
	checkTimers();
}

/* TODO: maybe checkTimers should be refactored so that sales will be produced in objects themselves, but it is hard to do it without disabling sales for buyers and for deals
If new class for storing deals and other data will be done, saling can be enabled in objects */
void DataBase::checkTimers() {
	Object object;

	bool ret = true;
	if(objectsForSale.getNumberOfObjects() != 0) {
		object = objectsForSale.timerPop(1);
		if(object.getAge() > 100) {
			if(object.adaptPrice()) {ret = false;}
		}
		if(ret) {objectsForSale.push(object);}
	}

	if(objectsBought.getNumberOfObjects() != 0) {
		object = objectsBought.timerPop(1);
		if(object.getAge() > 100) {
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
	printf("Mean prices:\nBuying = %.2f; Deals(For sale / Bought) = %.2f/%.2f; Selling = %.2f\n",
		objectsBought.getMeanPrice(), dealsForSale.getMeanPrice(), dealsBought.getMeanPrice(), objectsForSale.getMeanPrice());
	printf("Number of objects. Bought: %d; For sale: %d\n", objectsBought.getNumberOfObjects(), objectsForSale.getNumberOfObjects());
	if(highestBuyingPrice != -1 && lowestSellingPrice != -1) {
		printf("Spread = %.2f\n\n", lowestSellingPrice - highestBuyingPrice);
	}
	Object newObject(objectsForSale.getMeanPrice(), -1, 0);
	/* TODO: there is some trouble with pushing to position. Needs clearifing */
	meanForSalePrice.push(newObject, meanForSalePrice.getNumberOfObjects() + 1);
	newObject.setObject(objectsBought.getMeanPrice(), -1, 0);
	meanBoughtPrice.push(newObject, meanBoughtPrice.getNumberOfObjects() + 1);
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
	system("pause");
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
	LineChart lineChart(2, 0, numberOfArguments);
	lineChart.setTmpChartIndex(0);
	meanForSalePrice.feelLineChart(lineChart);
	lineChart.setTmpChartIndex(1);
	meanBoughtPrice.feelLineChart(lineChart);

	ui->printCharts(histogram, lineChart);
	Sleep(cmn_defines->getPictureDelayTime());
}