#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 1;
	dataBase = dataBase->getDataBase();
	dataBase->start(timer);
	configurator = configurator->getConfigurator();
	configurator->printConfiguration();
	numberOfObjectTypes = configurator->getNumberOfObjectTypes();
	for(int i = 0; i < numberOfObjectTypes; i++) {
		resetSellingTimer(i);
		resetBuyingTimer(i);
	}
}

Market::~Market() {
}

Market* Market::getMarket() {
	if(!p_Market) {
		p_Market = new Market();
	}
	return p_Market;
}

int Market::tick() {
	if(timeToAddSeller())	{addSeller();}
	if(timeToAddBuyer())	{addBuyer();}
	for(int i = 0; i < numberOfObjectTypes; i++) {
		while (dealPossible(i))	{dataBase->runPossibleDeal(i);}
	}
	if(timeToPrintTimer())	{printTimer();}
	if(timeToRefreshPicture())	{refreshPicture();}
	if(timeToFinish())		{dataBase->closeDatabase(); return 0;}
	//if(timer % 3000 == 0)	{configurator->setSellersLambda(configurator->getSellersLambda(0) * 1.05, 0);}
	dataBase->tick();
	switchTimers();
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
	return (timer % configurator->getTimerPrintingFrequency() == 0) ? true : false;
}

bool Market::timeToRefreshPicture() {
	return (timer % configurator->getPictureRefreshFrequency() == 0) ? true : false;
}

bool Market::timeToFinish() {
	return (timer < configurator->getModelingTime()) ? false : true;
}

bool Market::dealPossible(int typeId) {
	return dataBase->dealPossible(typeId);
}

int Market::addSeller() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formSellingPrice(rnd), timer, FORSALE, rnd);
	dataBase->pushToDataBase(object);
	resetSellingTimer(rnd);
	return 0;
}

int Market::addBuyer() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formBuyingPrice(rnd), timer, BOUGHT, rnd);
	dataBase->pushToDataBase(object);
	resetBuyingTimer(rnd);
	return 0;
}

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
#include <cmath>

double Market::formSellingPrice(int type) {
	switch(configurator->getSellerPricesMode(type)) {
		case 1:
			return getNormallyDistributedValue(configurator->getSellersMean(type), configurator->getSellersStandartDeviation(type));
		case 0:
			return configurator->getMinimumSellersPrice(type) + rand()%int(configurator->getMaximumSellersPrice(type) - configurator->getMinimumSellersPrice(type) + 1);
	}
	return -1;
}

double Market::formBuyingPrice(int type) {
	switch(configurator->getBuyerPricesMode(type)) {
		case 1:
			return getNormallyDistributedValue(configurator->getBuyersMean(type), configurator->getBuyersStandartDeviation(type));
		case 0:
			return configurator->getMinimumBuyersPrice(type) + rand()%int(configurator->getMaximumBuyersPrice(type) - configurator->getMinimumBuyersPrice(type) + 1);
	}
	return -1;
}

void Market::resetSellingTimer(int type) {
	switch(configurator->getSellerTimersMode(type)) {
		case 1:
			timeLeftBeforeNewSellingObject = int(getExponentiallyDistributedValue(configurator->getSellersLambda(type)));
			return;
		case 0:
			timeLeftBeforeNewSellingObject = configurator->getSellersFrequency(type);
			return;
	}
}

void Market::resetBuyingTimer(int type) {
	switch(configurator->getBuyerTimersMode(type)) {
		case 1:
			timeLeftBeforeNewObjectBought = int(getExponentiallyDistributedValue(configurator->getBuyersLambda(type)));
			return;
		case 0:
			timeLeftBeforeNewObjectBought = configurator->getBuyersFrequency(type);
			return;
	}
}

double Market::getNormallyDistributedValue(double mean, double standartDeviation) {
	double u, v, s;
	int accuracy = configurator->getAccuracy();
	do {
		u = 2 * double(rand()%accuracy) / accuracy - 1;
		v = 2 * double(rand()%accuracy) / accuracy - 1;
		s = u * u + v * v;
	} while(s > 1 || s == 0);
	s = sqrt(-2 * log(s) / s);
	return u * s * standartDeviation + mean;
}

double Market::getExponentiallyDistributedValue(double lambda) {
	int accuracy = configurator->getAccuracy();
	double val = -1 * log(1 - double(rand()%accuracy) / accuracy) / lambda;
	return val;
}

/**********************************************************************
						End of statistical part
**********************************************************************/

/**********************************************************************
							Interface
**********************************************************************/
#include <stdio.h>
void Market::printTimer() {
	printf("Timer = %d\n", timer);
}

void Market::refreshPicture() {
	dataBase->viewDataBaseInfo();
	dataBase->gatherStatistics();
	dataBase->refreshPicture();
}
/**********************************************************************
						End of interface
**********************************************************************/