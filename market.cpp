#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 1;
	dataBase = dataBase->getDataBase();
	dataBase->start(timer);
	configurator = configurator->getConfigurator();
	configurator->printConfiguration();
	resetSellingTimer();
	resetBuyingTimer();
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
	while (dealPossible())	{dataBase->runPossibleDeal();}
	if(timeToPrintTimer())	{printTimer();}
	if(timeToRefreshPicture())	{refreshPicture();}
	if(timeToFinish())		{dataBase->closeDatabase(); return 0;}
	//if(timer % 3000 == 0)	{configurator->setSellersLambda(configurator->getSellersLambda() * 1.05);}
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

bool Market::dealPossible() {
	return dataBase->dealPossible();
}

int Market::addSeller() {
	Object object(formSellingPrice(), timer, FORSALE);
	//object.setFiles(buyersFinalPricesFile, buyersFinalTimersFile, sellersFinalPricesFile, sellersFinalTimersFile);
	dataBase->pushToDataBase(object);
	resetSellingTimer();
	return 0;
}

int Market::addBuyer() {
	Object object(formBuyingPrice(), timer, BOUGHT);
	//object.setFiles(buyersFinalPricesFile, buyersFinalTimersFile, sellersFinalPricesFile, sellersFinalTimersFile);
	dataBase->pushToDataBase(object);
	resetBuyingTimer();
	return 0;
}

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
#include <cmath>

double Market::formSellingPrice() {
	switch(configurator->getSellerPricesMode()) {
		case 1:
			return getNormallyDistributedValue(configurator->getSellersMean(), configurator->getSellersStandartDeviation());
		case 0:
			return configurator->getMinimumSellersPrice() + rand()%int(configurator->getMaximumSellersPrice() - configurator->getMinimumSellersPrice() + 1);
	}
	return -1;
}

double Market::formBuyingPrice() {
	switch(configurator->getBuyerPricesMode()) {
		case 1:
			return getNormallyDistributedValue(configurator->getBuyersMean(), configurator->getBuyersStandartDeviation());
		case 0:
			return configurator->getMinimumBuyersPrice() + rand()%int(configurator->getMaximumBuyersPrice() - configurator->getMinimumBuyersPrice() + 1);
	}
	return -1;
}

void Market::resetSellingTimer() {
	switch(configurator->getBuyerPricesMode()) {
		case 1:
			timeLeftBeforeNewSellingObject = int(getExponentiallyDistributedValue(configurator->getSellersLambda()));
			return;
		case 0:
			timeLeftBeforeNewSellingObject = configurator->getSellersFrequency();
			return;
	}
}

void Market::resetBuyingTimer() {
	switch(configurator->getBuyerPricesMode()) {
		case 1:
			timeLeftBeforeNewObjectBought = int(getExponentiallyDistributedValue(configurator->getBuyersLambda()));
			return;
		case 0:
			timeLeftBeforeNewObjectBought = configurator->getBuyersFrequency();
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

#include <stdio.h>
void Market::printTimer() {
	printf("Timer = %d\n", timer);
}

void Market::refreshPicture() {
	dataBase->viewDataBaseInfo();
	dataBase->gatherStatistics();
	dataBase->refreshPicture();
}