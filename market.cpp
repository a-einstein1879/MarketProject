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

void Market::tick() {
	switchTimers();
	if(timeToAddSeller())	{addSeller();}
	if(timeToAddBuyer())	{addBuyer();}
	while (dealPossible())	{runDeal();}
	if(timeToPrintDb())		{printDb();}
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
	return (timer % 1000 == 0) ? true : false;
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
	return 10 + rand()%15;
}

double Market::formBuyingPrice() {
	return 5 + rand()%10;
}

void Market::resetSellingTimer() {
	timeLeftBeforeNewSellingObject = 3;
}

void Market::resetBuyingTimer() {
	timeLeftBeforeNewObjectBought = 1;
}

#define ACCURACY 100

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
	fprintf(dealFile, "Deal price\n");
	
	sellersFile = fopen(SELLERSFILE, "w");
	if(sellersFile == NULL) {
		printf("File '%s' can`t be open ", SELLERSFILE);
	}
	fprintf(sellersFile, "Deal time\n");
	
	buyersFile = fopen(BUYERSFILE, "w");
	if(buyersFile == NULL) {
		printf("File '%s' can`t be open ", BUYERSFILE);
	}
	fprintf(buyersFile, "Deal time\n");
}

void Market::closeFiles() {
	fclose(dealFile);
	fclose(sellersFile);
	fclose(buyersFile);
}