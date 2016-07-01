#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	lowestSellingPrice = -1;
	highestBuyingPrice = -1;
	configurator = configurator->getConfigurator();
	ui = ui->getOpenGLInterface();
	statistics = statistics->getStatistics();
	openFiles();
};
DataBase::~DataBase() {
	closeFiles();
}

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

void DataBase::closeDatabase() {
	Object object;
	while(1) {
		object = popLowestSeller();
		if(object.getAge() == -1)	{break;}
		else						{object.printObjectToFinalFiles(); continue;}
	}

	while(1) {
		object = popHighestBuyer();
		if(object.getAge() == -1)	{break;}
		else						{object.printObjectToFinalFiles(); continue;}
	}
}

/* TODO: maybe checkTimers should be refactored so that sales will be produced in objects themselves, but it is hard to do it without disabling sales for buyers and for deals
If new class for storing deals and other data will be created, saling can be enabled in objects */
void DataBase::checkTimers() {
	Object object;

	bool ret = true;
	if(objectsForSale.getNumberOfObjects() != 0) {
		object = objectsForSale.timerPop(1);
		if(object.getAge() > configurator->getSellerPriceReduceAge()) {
			if(object.adaptPrice()) {ret = false;}
		}
		if(ret) {objectsForSale.push(object);}
	}
	
	ret = true;
	if(objectsBought.getNumberOfObjects() != 0) {
		object = objectsBought.timerPop(1);
		if(object.getAge() > configurator->getBuyerPriceIncreaseAge()) {
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

void DataBase::runPossibleDeal() {
	Object seller, buyer;
	buyer = popHighestBuyer();
	if(buyer.getAge() == -1) {return;}
	seller = popLowestSeller();
	if(seller.getAge() == -1) {pushToDataBase(buyer);return;}

	double price, time;
	price = ( buyer.getPrice() + seller.getPrice() ) / 2;
	time = buyer.getAge() - seller.getAge();

	/* Adding new deal to deal database */
	Object newDeal;
	if(time > 0) {
		newDeal.setObject(price, time, FORSALE);
		addDeal(newDeal);
		newDeal.setObject(price, 0, BOUGHT);
		addDeal(newDeal);
	} else {
		newDeal.setObject(price, 0, FORSALE);
		addDeal(newDeal);
		newDeal.setObject(price, - time, BOUGHT);
		addDeal(newDeal);
	}

	/* Printing deal data to output files */
	fprintf(dealFile, "%.2f\n", price);
	if(time >= 0) {
		fprintf(sellersFile, "%.2f\n", time);
		fprintf(buyersFile,	 "%.2f\n", 0);
	} else {
		fprintf(sellersFile, "%.2f\n", 0);
		fprintf(buyersFile,	 "%.2f\n", - time);
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
void DataBase::viewDataBaseInfo() {
	/* Info part */
	printf("Number of objects:\nBuying: %d; Deals(Bought / For sale) = %d/%d; For sale: %d\n", objectsBought.getNumberOfObjects(), dealsBought.getNumberOfObjects(), dealsForSale.getNumberOfObjects(), objectsForSale.getNumberOfObjects());
	printf("Mean prices:\nBuying = %.2f; Deals(Bought / For sale) = %.2f/%.2f; Selling = %.2f\n",
		objectsBought.getMeanPrice(), dealsBought.getMeanPrice(), dealsForSale.getMeanPrice(), objectsForSale.getMeanPrice());
	printf("Mean timers:\nBuying = %.2f; Deals(Bought / For sale) = %.2f/%.2f; Selling = %.2f\n",
		objectsBought.getMeanTimer(), dealsBought.getMeanTimer(), dealsForSale.getMeanTimer(), objectsForSale.getMeanTimer());

	/* Count mean waiting time */
	double sellersMeanWaitingTime, buyersMeanWaitingTime;
	int numberOfSellers = dealsForSale.getNumberOfObjects() + objectsForSale.getNumberOfObjects();
	int numberOfBuyers = dealsBought.getNumberOfObjects() + objectsBought.getNumberOfObjects();
	sellersMeanWaitingTime = dealsForSale.getMeanTimer() * double(dealsForSale.getNumberOfObjects()) / double(numberOfSellers) + objectsForSale.getMeanTimer() * double(objectsForSale.getNumberOfObjects()) / double(numberOfSellers);
	buyersMeanWaitingTime = dealsBought.getMeanTimer() * double(dealsBought.getNumberOfObjects()) / double(numberOfBuyers) + objectsBought.getMeanTimer() * double(objectsBought.getNumberOfObjects()) / double(numberOfBuyers);
	if(highestBuyingPrice != -1 && lowestSellingPrice != -1) {
		printf("Mean waiting time(Bought / For sale) = %.2f/%.2f\n", buyersMeanWaitingTime, sellersMeanWaitingTime);
	}

	if(highestBuyingPrice != -1 && lowestSellingPrice != -1) {
		printf("Spread = %.2f, Mean spread = %.2f\n\n", lowestSellingPrice - highestBuyingPrice, statistics->getMeanValue(0, SPREADID));
	}
	/* End of info part */
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

void DataBase::gatherStatistics() {
	/* TODO: there is some trouble with pushing to position. Needs clearifing */
	statistics->addStatisticsElement(objectsForSale.getMeanPrice(),				0, FORSALEPRICEID);
	statistics->addStatisticsElement(objectsBought.getMeanPrice(),				0, BOUGHTPRICEID);
	statistics->addStatisticsElement(objectsForSale.getNumberOfObjects(),		0, FORSALENUMBEROFOBJECTSID);
	statistics->addStatisticsElement(objectsBought.getNumberOfObjects(),		0, BOUGHTNUMBEROFBJECTSID);
	statistics->addStatisticsElement(highestBuyingPrice,						0, BIDPRICEID);
	statistics->addStatisticsElement(lowestSellingPrice,						0, ASKPRICEID);
	statistics->addStatisticsElement(lowestSellingPrice - highestBuyingPrice,	0, SPREADID);
}

void DataBase::refreshPicture() {
	if(!(configurator->getGraphicalMode())) {return;}

	/* Price histogram */
	if(objectsForSale.getNumberOfObjects() == 0 || objectsBought.getNumberOfObjects() == 0) {return;}
	
	double maxArgument;
	double minArgument;
	if(configurator->getConstantBoardersMode() == 0) {
		minArgument = configurator->getMinimumHistogramArgument();
		maxArgument = configurator->getMaximumHistogramArgument();
	} else {
		Object object = objectsForSale.pricePop(objectsForSale.getNumberOfObjects());
		maxArgument = object.getPrice();
		objectsForSale.push(object);
		object = objectsBought.pricePop(1);
		minArgument = object.getPrice();
		objectsBought.push(object);
		refreshPrices();
	}

	Histogram histogram(3, configurator->getNumberOfPockets(), minArgument, maxArgument);
	histogram.setTmpChartIndex(0);
	objectsForSale.feelHistogram(histogram);
	/*histogram.setTmpChartIndex(1);
	dealsForSale.feelHistogram(histogram);
	dealsBought.feelHistogram(histogram);*/
	histogram.setTmpChartIndex(2);
	objectsBought.feelHistogram(histogram);
	ui->drawMarketHistogram(histogram);

	statistics->drawStatistics();
}

#include <stdio.h>
void DataBase::openFiles() {
	dealFile = fopen(DEALFILE, "w");
	if(dealFile == NULL) {
		printf("File '%s' can`t be open ", DEALFILE);
	}
	
	sellersFile = fopen(SELLERSFILE, "w");
	if(sellersFile == NULL) {
		printf("File '%s' can`t be open ", SELLERSFILE);
	}
	
	buyersFile = fopen(BUYERSFILE, "w");
	if(buyersFile == NULL) {
		printf("File '%s' can`t be open ", BUYERSFILE);
	}
}

void DataBase::closeFiles() {
	fclose(dealFile);
	fclose(sellersFile);
	fclose(buyersFile);
}