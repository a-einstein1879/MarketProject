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

DataBaseReturn* DataBase::tick() {
	checkTimers();
	for(int i = 0; i < numberOfObjectTypes; i++) {
		objectsForSale[i].tick();
		objectsBought[i].tick();
	}
	timer++;
	return getObjectsForHandling();
}

DataBaseReturn* DataBase::closeDatabase() {
	Object object;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		while(1) {
			object = popLowestSeller(i);
			if(object.getAge() == -1)	{break;}
			else						{object.printObjectToFinalFiles(); fprintf(outputFiles[7], "%d\t%.2f\t%.2f\n", object.getAgentId(), object.getPrice(), object.getAge()); continue;}
		}

		while(1) {
			object = popHighestBuyer(i);
			if(object.getAge() == -1)	{break;}
			else						{object.printObjectToFinalFiles(); continue;}
		}
	}
	return getObjectsForHandling();
}

/* TODO: maybe checkTimers should be refactored so that sales will be produced in objects themselves, but it is hard to do it without disabling sales for buyers and for deals
If new class for storing deals and other data will be created, saling can be enabled in objects */
void DataBase::checkTimers() {
	Object object;
	bool end = false;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		if(objectsForSale[i].getNumberOfObjects() != 0) {
			while(1) {
				object = objectsForSale[i].timerPop(1);
				if(object.timeToLeaveMarket()) {objectsForReturn.timeoutObjects.push(object); continue;}
				else {
					if(object.adaptPrice()) {end = true;}
					pushToDataBase(object);
					if(end == true) {break;}
				}
			}
		}
	
		end = false;
		if(objectsBought[i].getNumberOfObjects() != 0) {
			while(1) {
				object = objectsBought[i].timerPop(1);
				if(object.timeToLeaveMarket()) {objectsForReturn.timeoutObjects.push(object); continue;}
				else {
					if(object.adaptPrice()) {end = true;}
					pushToDataBase(object);
					if(end == true) {break;}
				}
			}
		}

		refreshPrices();
	}
}

int DataBase::pushToDataBase(Object newObject) {
	newObject.setFiles(outputFiles[3], outputFiles[4], outputFiles[5], outputFiles[6]);
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

	objectsForReturn.dealtObjects.push(seller);
	objectsForReturn.dealtObjects.push(buyer);

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
	fprintf(outputFiles[0], "%.2f\n", price);
	if(time >= 0) {
		fprintf(outputFiles[1], "%.2f\n", time);
		fprintf(outputFiles[2],	 "%.2f\n", 0);
	} else {
		fprintf(outputFiles[1], "%.2f\n", 0);
		fprintf(outputFiles[2],	 "%.2f\n", - time);
	}
	fprintf(outputFiles[7], "%d\t%.2f\t%.2f\n", seller.getAgentId(), seller.getPrice(), seller.getAge());
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
	for(int i = 0; i < numberOfObjectTypes; i++) {
		if(objectsForSale[i].getNumberOfObjects() != 0) {
			printf("The lowest selling price is %.2f\n", lowestSellingPrice[i]);
			printf("Number of objects for sale is %d:\n", objectsForSale[i].getNumberOfObjects());
			objectsForSale[i].viewTimers();
		} else {
			printf("Noone is selling anything\n");
		}
	
		if(objectsBought[i].getNumberOfObjects() != 0) {
			printf("The highest buying price is %.2f\n", highestBuyingPrice[i]);
			printf("Number of objects bought is %d:\n", objectsBought[i].getNumberOfObjects());
			objectsBought[i].viewTimers();
		} else {
			printf("Noone is buying anything\n");
		}
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
			Object object;
			if(objectsForSale[i].getNumberOfObjects() != 0) {
				object = objectsForSale[i].pricePop(objectsForSale[i].getNumberOfObjects());
				maxArgument = (object.getPrice() > maxArgument) ? object.getPrice() : maxArgument;
				objectsForSale[i].push(object);
			}
			if(objectsBought[i].getNumberOfObjects() != 0) {
				object = objectsBought[i].pricePop(1);
				minArgument = (object.getPrice() < minArgument) ? object.getPrice() : minArgument;
				objectsBought[i].push(object);
			}
			refreshPrices();
		}
	}
	if(maxArgument == -100) {maxArgument = configurator->getMaximumHistogramArgument();}
	if(minArgument == 100)	{minArgument = configurator->getMinimumHistogramArgument();}

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
	outputFiles[0] = fopen(DEALFILE, "w");
	if(outputFiles[0] == NULL) {
		printf("File '%s' can`t be open ", DEALFILE);
	}
	
	outputFiles[1] = fopen(SELLERSFILE, "w");
	if(outputFiles[1] == NULL) {
		printf("File '%s' can`t be open ", SELLERSFILE);
	}
	
	outputFiles[2] = fopen(BUYERSFILE, "w");
	if(outputFiles[2] == NULL) {
		printf("File '%s' can`t be open ", BUYERSFILE);
	}

	outputFiles[3] = fopen(BUYERSFINALPRICESFILE, "w");
	if(outputFiles[3] == NULL) {
		printf("File '%s' can`t be open ", BUYERSFINALPRICESFILE);
	}

	outputFiles[4] = fopen(BUYERSFINALTIMERSFILE, "w");
	if(outputFiles[4] == NULL) {
		printf("File '%s' can`t be open ", BUYERSFINALTIMERSFILE);
	}

	outputFiles[5] = fopen(SELLERSFINALPRICESFILE, "w");
	if(outputFiles[5] == NULL) {
		printf("File '%s' can`t be open ", SELLERSFINALPRICESFILE);
	}

	outputFiles[6] = fopen(SELLERSFINALTIMERSFILE, "w");
	if(outputFiles[6] == NULL) {
		printf("File '%s' can`t be open ", SELLERSFINALTIMERSFILE);
	}
	
	outputFiles[7] = fopen(TIMEEXPOSITIONFILE, "w");
	if(outputFiles[7] == NULL) {
		printf("File '%s' can`t be open ", TIMEEXPOSITIONFILE);
	}
}

void DataBase::closeFiles() {
	for(int i = 0; i < 8; i++) {
		fclose(outputFiles[i]);
	}
}