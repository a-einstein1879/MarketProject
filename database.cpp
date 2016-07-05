#include "database.h"

DataBase* DataBase::p_DataBase = 0;

DataBase::DataBase() {
	configurator = configurator->getConfigurator();
	ui = ui->getOpenGLInterface();
	statistics = statistics->getStatistics();
	openFiles();
	numberOfObjectTypes = configurator->getNumberOfObjectTypes();

	/* Memory allocation */
	lowestSellingPrice = new double[numberOfObjectTypes];
	highestBuyingPrice = new double[numberOfObjectTypes];
	objectsForSale = new LinkList[numberOfObjectTypes];
	objectsBought = new LinkList[numberOfObjectTypes];

	for(int i = 0; i < numberOfObjectTypes; i++) {
		lowestSellingPrice[i] = -1;
		highestBuyingPrice[i] = -1;
	}
}

DataBase::~DataBase() {
	closeFiles();
	delete [] lowestSellingPrice;
	delete [] highestBuyingPrice;
	delete [] objectsForSale;
	delete [] objectsBought;
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
	for(int i = 0; i < numberOfObjectTypes; i++) {
		objectsForSale[i].tick();
		objectsBought[i].tick();
	}
	timer++;
}

void DataBase::closeDatabase() {
	Object object;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		while(1) {
			object = popLowestSeller(i);
			if(object.getAge() == -1)	{break;}
			else						{object.printObjectToFinalFiles(); continue;}
		}

		while(1) {
			object = popHighestBuyer(i);
			if(object.getAge() == -1)	{break;}
			else						{object.printObjectToFinalFiles(); continue;}
		}
	}
}

/* TODO: maybe checkTimers should be refactored so that sales will be produced in objects themselves, but it is hard to do it without disabling sales for buyers and for deals
If new class for storing deals and other data will be created, saling can be enabled in objects */
void DataBase::checkTimers() {
	Object object;
	
	for(int i = 0; i < numberOfObjectTypes; i++) {
		bool ret = true;
		if(objectsForSale[i].getNumberOfObjects() != 0) {
			object = objectsForSale[i].timerPop(1);
			if(object.getAge() > configurator->getSellerPriceReduceAge()) {
				if(object.adaptPrice()) {ret = false;}
			}
			if(ret) {objectsForSale[i].push(object);}
		}
	
		ret = true;
		if(objectsBought[i].getNumberOfObjects() != 0) {
			object = objectsBought[i].timerPop(1);
			if(object.getAge() > configurator->getBuyerPriceIncreaseAge()) {
				if(object.adaptPrice()) {ret = false;}
			}
			if(ret) {objectsBought[i].push(object);}
		}

		refreshPrices();
	}
}

int DataBase::pushToDataBase(Object newObject) {
	newObject.setFiles(buyersFinalPricesFile, buyersFinalTimersFile, sellersFinalPricesFile, sellersFinalTimersFile);
	int type = newObject.getType();
	if(newObject.getStatus() == FORSALE) {
		objectsForSale[type].push(newObject);
	} else {
		objectsBought[type].push(newObject);
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

bool DataBase::dealPossible(int typeId) {
	if(objectsForSale[typeId].getNumberOfObjects() == 0 || objectsBought[typeId].getNumberOfObjects() == 0) {return false;}
	if(highestBuyingPrice[typeId] >= lowestSellingPrice[typeId]) {
		return true;
	} else {
		return false;
	}
}

void DataBase::runPossibleDeal(int typeId) {
	Object seller, buyer;
	buyer = popHighestBuyer(typeId);
	if(buyer.getAge() == -1) {return;}
	seller = popLowestSeller(typeId);
	if(seller.getAge() == -1) {pushToDataBase(buyer); return;}

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

Object DataBase::popLowestSeller(int typeId) {
	if(objectsForSale[typeId].getNumberOfObjects() > 0) {
		Object object;
		object = objectsForSale[typeId].pricePop(1);
		refreshPrices();
		return object;
	} else {
		Object object;
		return object;
	}
}

Object DataBase::popHighestBuyer(int typeId) {
	if(objectsBought[typeId].getNumberOfObjects() > 0) {
		Object object;
		object = objectsBought[typeId].pricePop(objectsBought[typeId].getNumberOfObjects());
		refreshPrices();
		return object;
	} else {
		Object object;
		return object;
	}
}

void DataBase::refreshPrices() {
	Object object;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		if(objectsForSale[i].getNumberOfObjects() != 0) {
			object = objectsForSale[i].pricePop(1);
			lowestSellingPrice[i] = object.getPrice();
			objectsForSale[i].push(object);
		} else {
			lowestSellingPrice[i] = -1;
		}
	
		if(objectsBought[i].getNumberOfObjects() != 0) {
			object = objectsBought[i].pricePop(objectsBought[i].getNumberOfObjects());
			highestBuyingPrice[i] = object.getPrice();
			objectsBought[i].push(object);
		} else {
			highestBuyingPrice[i] = -1;
		}
	}
}

#include <stdio.h>
void DataBase::viewDataBaseInfo() {
	for(int i = 0; i < numberOfObjectTypes; i++) {
		printf("Type %d:\n", i);
		printf("Number of objects:\nBuying: %d; Deals(Bought / For sale) = %d/%d; For sale: %d\n", objectsBought[i].getNumberOfObjects(), dealsBought.getNumberOfObjects(), dealsForSale.getNumberOfObjects(), objectsForSale[i].getNumberOfObjects());
		printf("Mean prices:\nBuying = %.2f; Deals(Bought / For sale) = %.2f/%.2f; Selling = %.2f\n",
			objectsBought[i].getMeanPrice(), dealsBought.getMeanPrice(), dealsForSale.getMeanPrice(), objectsForSale[i].getMeanPrice());
		printf("Mean timers:\nBuying = %.2f; Deals(Bought / For sale) = %.2f/%.2f; Selling = %.2f\n",
			objectsBought[i].getMeanTimer(), dealsBought.getMeanTimer(), dealsForSale.getMeanTimer(), objectsForSale[i].getMeanTimer());

		/* Count mean waiting time */
		double sellersMeanWaitingTime, buyersMeanWaitingTime;
		int numberOfSellers = dealsForSale.getNumberOfObjects() + objectsForSale[i].getNumberOfObjects();
		int numberOfBuyers = dealsBought.getNumberOfObjects() + objectsBought[i].getNumberOfObjects();
		sellersMeanWaitingTime = dealsForSale.getMeanTimer() * double(dealsForSale.getNumberOfObjects()) / double(numberOfSellers) + objectsForSale[i].getMeanTimer() * double(objectsForSale[i].getNumberOfObjects()) / double(numberOfSellers);
		buyersMeanWaitingTime = dealsBought.getMeanTimer() * double(dealsBought.getNumberOfObjects()) / double(numberOfBuyers) + objectsBought[i].getMeanTimer() * double(objectsBought[i].getNumberOfObjects()) / double(numberOfBuyers);
		if(highestBuyingPrice[i] != -1 && lowestSellingPrice[i] != -1) {
			printf("Mean waiting time(Bought / For sale) = %.2f/%.2f\n", buyersMeanWaitingTime, sellersMeanWaitingTime);
		}

		if(highestBuyingPrice[i] != -1 && lowestSellingPrice[i] != -1) {
			printf("Spread = %.2f, Mean spread = %.2f\n", lowestSellingPrice[i] - highestBuyingPrice[i], statistics->getMeanValue(i, SPREADID));
		}
		printf("\n");
	}
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
	for(int i = 0; i < numberOfObjectTypes; i++) {
		/* TODO: there is some trouble with pushing to position. Needs clearifing */
		statistics->addStatisticsElement(objectsForSale[i].getMeanPrice(),				i, FORSALEPRICEID);
		statistics->addStatisticsElement(objectsBought[i].getMeanPrice(),				i, BOUGHTPRICEID);
		statistics->addStatisticsElement(objectsForSale[i].getNumberOfObjects(),		i, FORSALENUMBEROFOBJECTSID);
		statistics->addStatisticsElement(objectsBought[i].getNumberOfObjects(),			i, BOUGHTNUMBEROFBJECTSID);
		statistics->addStatisticsElement(highestBuyingPrice[i],							i, BIDPRICEID);
		statistics->addStatisticsElement(lowestSellingPrice[i],							i, ASKPRICEID);
		statistics->addStatisticsElement(lowestSellingPrice[i] - highestBuyingPrice[i],	i, SPREADID);
	}
}

void DataBase::refreshPicture() {
	if(!(configurator->getGraphicalMode())) {return;}
	
	double maxArgument = -100;
	double minArgument = 100;
	if(configurator->getConstantBoardersMode() == 1) {
		minArgument = configurator->getMinimumHistogramArgument();
		maxArgument = configurator->getMaximumHistogramArgument();
	} else {
		for(int i = 0; i < numberOfObjectTypes; i++) {
			Object object = objectsForSale[i].pricePop(objectsForSale[i].getNumberOfObjects());
			maxArgument = (object.getPrice() > maxArgument) ? object.getPrice() : maxArgument;
			objectsForSale[i].push(object);
			object = objectsBought[i].pricePop(1);
			minArgument = (object.getPrice() < minArgument) ? object.getPrice() : minArgument;
			objectsBought[i].push(object);
			refreshPrices();
		}
	}
	Histogram histogram(3, configurator->getNumberOfPockets(), minArgument, maxArgument);
	for(int i = 0; i < numberOfObjectTypes; i++) {
		/* Price histogram */
		if(objectsForSale[i].getNumberOfObjects() == 0 || objectsBought[i].getNumberOfObjects() == 0) {continue;}

		histogram.setTmpChartIndex(0);
		objectsForSale[i].feelHistogram(histogram);
		/*histogram.setTmpChartIndex(1);
		dealsForSale.feelHistogram(histogram);
		dealsBought.feelHistogram(histogram);*/
		histogram.setTmpChartIndex(2);
		objectsBought[i].feelHistogram(histogram);
	}
	
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

	buyersFinalPricesFile = fopen(BUYERSFINALPRICESFILE, "w");
	if(buyersFinalPricesFile == NULL) {
		printf("File '%s' can`t be open ", BUYERSFINALPRICESFILE);
	}

	buyersFinalTimersFile = fopen(BUYERSFINALTIMERSFILE, "w");
	if(buyersFinalTimersFile == NULL) {
		printf("File '%s' can`t be open ", BUYERSFINALTIMERSFILE);
	}

	sellersFinalPricesFile = fopen(SELLERSFINALPRICESFILE, "w");
	if(sellersFinalPricesFile == NULL) {
		printf("File '%s' can`t be open ", SELLERSFINALPRICESFILE);
	}

	sellersFinalTimersFile = fopen(SELLERSFINALTIMERSFILE, "w");
	if(sellersFinalTimersFile == NULL) {
		printf("File '%s' can`t be open ", SELLERSFINALTIMERSFILE);
	}
}

void DataBase::closeFiles() {
	fclose(dealFile);
	fclose(sellersFile);
	fclose(buyersFile);
	fclose(buyersFinalPricesFile);
	fclose(buyersFinalTimersFile);
	fclose(sellersFinalPricesFile);
	fclose(sellersFinalTimersFile);
}