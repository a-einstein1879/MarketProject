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
	if(timeToPrintDb())		{printDb();}
	if(timeToFinish())		{return 0;}
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

bool Market::timeToPrintDb() {
	return (timer % TIMERPRINTINGFREQUENCY == 0) ? true : false;
}

bool Market::timeToFinish() {
	return (timer < MODELINGTIME) ? false : true;
}

bool Market::dealPossible() {
	return dataBase->dealPossible();
}

int Market::addSeller() {
	Object object;
	object.setObject(formSellingPrice(), timer, FORSALE);
	dataBase->pushToDataBase(object);
	resetSellingTimer();
	return 0;
}

int Market::addBuyer() {
	Object object;
	object.setObject(formBuyingPrice(), timer, BOUGHT);
	dataBase->pushToDataBase(object);
	resetBuyingTimer();
	return 0;
}

void Market::runDeal() {
	Object seller, buyer;
	buyer = dataBase->popHighestBuyer();
	seller = dataBase->popLowestSeller();
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
	return MINIMUMSELLINGPRICE + rand()%(MAXIMUMSELLINGPRICE - MINIMUMSELLINGPRICE);
}

double Market::formBuyingPrice() {
	return MINIMUMBUYINGPRICE + rand()%(MAXIMUMBUYINGPRICE - MINIMUMBUYINGPRICE);
}

void Market::resetSellingTimer() {
	timeLeftBeforeNewSellingObject = getExponentiallyDistributedValue(0.2);
}

void Market::resetBuyingTimer() {
	timeLeftBeforeNewObjectBought = getExponentiallyDistributedValue(0.6);
}

double Market::getNormallyDistributedValue(double mean, double dispersion) {
	double u, v, s;
	do {
		u = 2 * double(rand()%ACCURACY) / ACCURACY - 1;
		v = 2 * double(rand()%ACCURACY) / ACCURACY - 1;
		s = u * u + v * v;
	} while(s > 1);
	s = sqrt(-2 * log(s) / s);
	return u * s * dispersion + mean;
}

double Market::getExponentiallyDistributedValue(double lambda) {
	double val = -1 * log(1 - double(rand()%ACCURACY) / ACCURACY) / lambda;
	return val;
}

/**********************************************************************
						End of statistical part
**********************************************************************/

#include <stdio.h>
void Market::printDb() {
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
}

void Market::closeFiles() {
	fclose(dealFile);
	fclose(sellersFile);
	fclose(buyersFile);
}

void Market::finish() {
}