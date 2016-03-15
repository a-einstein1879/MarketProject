#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 1;
	resetSellingTimer();
	resetBuyingTimer();
	dataBase = dataBase->getDataBase();

	openFiles();
}

Market::~Market() {
	closeFiles();
}

Market* Market::getMarket() {
	if(!p_Market) {
		p_Market = new Market();
	}
	return p_Market;
}

int Market::tick() {
	switchTimers();
	if(timeToAddSeller())	{addSeller();}
	if(timeToAddBuyer())	{addBuyer();}
	while (dealPossible())	{runDeal();}
	if(timeToPrintTimer())	{printTimer();}
	if(timeToFinish())		{finish(); return 0;}
	return 1;
}

void Market::switchTimers() {
	timer++;
	timeLeftBeforeNewSellingObject--;
	timeLeftBeforeNewObjectBought--;
}

bool Market::timeToAddSeller() {
	return (timeLeftBeforeNewSellingObject < 1) ? true : false;
}
bool Market::timeToAddBuyer() {
	return (timeLeftBeforeNewObjectBought < 1) ? true : false;
}

bool Market::timeToPrintTimer() {
	return (timer % TIMERPRINTINGFREQUENCY == 0) ? true : false;
}

bool Market::timeToFinish() {
	return (timer < MODELINGTIME) ? false : true;
}

bool Market::dealPossible() {
	return dataBase->dealPossible();
}

int Market::addSeller() {
	Object object(formSellingPrice(), timer, FORSALE);
	object.setFiles(buyersFinalPricesFile, buyersFinalTimersFile, sellersFinalPricesFile, sellersFinalTimersFile);
	dataBase->pushToDataBase(object);
	resetSellingTimer();
	return 0;
}

int Market::addBuyer() {
	Object object(formBuyingPrice(), timer, BOUGHT);
	object.setFiles(buyersFinalPricesFile, buyersFinalTimersFile, sellersFinalPricesFile, sellersFinalTimersFile);
	dataBase->pushToDataBase(object);
	resetBuyingTimer();
	return 0;
}

void Market::runDeal() {
	Object seller, buyer;
	buyer = dataBase->popHighestBuyer();
	if(buyer.getCreationTime() == -1) {return;}
	seller = dataBase->popLowestSeller();
	if(seller.getCreationTime() == -1) {dataBase->pushToDataBase(buyer); return;}
	Deal deal;
	deal.price = ( buyer.getPrice() + seller.getPrice() ) / 2;
	deal.time = buyer.getCreationTime() - seller.getCreationTime();
	printDeal(deal);
}

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
#include <cmath>

double Market::formSellingPrice() {
	//return MINIMUMSELLINGPRICE + rand()%(MAXIMUMSELLINGPRICE - MINIMUMSELLINGPRICE + 1);
	return getNormallyDistributedValue(15, 3);
}

double Market::formBuyingPrice() {
	//return MINIMUMBUYINGPRICE + rand()%(MAXIMUMBUYINGPRICE - MINIMUMBUYINGPRICE + 1);
	return getNormallyDistributedValue(10, 3);
}

void Market::resetSellingTimer() {
	timeLeftBeforeNewSellingObject = getExponentiallyDistributedValue(SELLERSLAMBDA);
}

void Market::resetBuyingTimer() {
	timeLeftBeforeNewObjectBought = getExponentiallyDistributedValue(BUYERSLAMBDA);
}

double Market::getNormallyDistributedValue(double mean, double standartDeviation) {
	double u, v, s;
	do {
		u = 2 * double(rand()%ACCURACY) / ACCURACY - 1;
		v = 2 * double(rand()%ACCURACY) / ACCURACY - 1;
		s = u * u + v * v;
	} while(s > 1);
	s = sqrt(-2 * log(s) / s);
	return u * s * standartDeviation + mean;
}

double Market::getExponentiallyDistributedValue(double lambda) {
	double val = -1 * log(1 - double(rand()%ACCURACY) / ACCURACY) / lambda;
	return val;
}

/**********************************************************************
						End of statistical part
**********************************************************************/

#include <stdio.h>
void Market::printTimer() {
	printf("Timer = %d\n", timer);
#ifndef SILENTMODE
	dataBase->viewDataBase();
#endif
}

void Market::printDeal(Deal deal) {
	fprintf(dealFile, "%.2f\n", deal.price);
	if(deal.time >= 0) {
		fprintf(sellersFile, "%.2f\n", deal.time);
	} else {
		fprintf(buyersFile, "%.2f\n", - deal.time);
	}
}

void Market::openFiles() {
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

void Market::closeFiles() {
	fclose(dealFile);
	fclose(sellersFile);
	fclose(buyersFile);
	fclose(buyersFinalPricesFile);
	fclose(buyersFinalTimersFile);
	fclose(sellersFinalPricesFile);
	fclose(sellersFinalTimersFile);
}

void Market::finish() {
	Object object;
	while(1) {
		object = dataBase->popLowestSeller();
		if(object.getCreationTime() == -1)	{break;}
		else								{object.printObjectToFinalFiles(); continue;}
	}

	while(1) {
		object = dataBase->popHighestBuyer();
		if(object.getCreationTime() == -1)	{break;}
		else								{object.printObjectToFinalFiles(); continue;}
	}
}