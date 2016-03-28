#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 1;
	dataBase = dataBase->getDataBase();
	cmn_defines = cmn_defines->getCmn_Defines();
	cmn_defines->printConfiguration();
	resetSellingTimer();
	resetBuyingTimer();

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
	if(timeToRefreshPicture())	{refreshPicture();}
	if(timeToFinish())		{finish(); return 0;}
	return 1;
}

void Market::switchTimers() {
	dataBase->tick();
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
	return (timer % cmn_defines->getTimerPrintingFrequency() == 0) ? true : false;
}

bool Market::timeToRefreshPicture() {
	return (timer % cmn_defines->getPictureRefreshFrequency() == 0) ? true : false;
}

bool Market::timeToFinish() {
	return (timer < cmn_defines->getModelingTime()) ? false : true;
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
	if(buyer.getAge() == -1) {return;}
	seller = dataBase->popLowestSeller();
	if(seller.getAge() == -1) {dataBase->pushToDataBase(buyer); return;}
	Deal deal;
	deal.price = ( buyer.getPrice() + seller.getPrice() ) / 2;
	deal.time = buyer.getAge() - seller.getAge();
	printDeal(deal);

	Object newDeal;
	newDeal.setObject(deal.price, (deal.time > 0)?deal.time:-deal.time, (deal.time > 0)?FORSALE:BOUGHT);
	dataBase->addDeal(newDeal);
}

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
#include <cmath>

double Market::formSellingPrice() {
	switch(cmn_defines->getSellerPricesMode()) {
		case 1:
			return getNormallyDistributedValue(cmn_defines->getSellersMean(), cmn_defines->getSellersStandartDeviation());
		case 0:
			return cmn_defines->getMinimumSellersPrice() + rand()%int(cmn_defines->getMaximumSellersPrice() - cmn_defines->getMinimumSellersPrice() + 1);
	}
	return -1;
}

double Market::formBuyingPrice() {
	switch(cmn_defines->getBuyerPricesMode()) {
		case 1:
			return getNormallyDistributedValue(cmn_defines->getBuyersMean(), cmn_defines->getBuyersStandartDeviation());
		case 0:
			return cmn_defines->getMinimumBuyersPrice() + rand()%int(cmn_defines->getMaximumBuyersPrice() - cmn_defines->getMinimumBuyersPrice() + 1);
	}
	return -1;
}

void Market::resetSellingTimer() {
	switch(cmn_defines->getBuyerPricesMode()) {
		case 1:
			timeLeftBeforeNewSellingObject = int(getExponentiallyDistributedValue(cmn_defines->getSellersLambda()));
			return;
		case 0:
			timeLeftBeforeNewSellingObject = cmn_defines->getSellersFrequency();
			return;
	}
}

void Market::resetBuyingTimer() {
	switch(cmn_defines->getBuyerPricesMode()) {
		case 1:
			timeLeftBeforeNewObjectBought = int(getExponentiallyDistributedValue(cmn_defines->getBuyersLambda()));
			return;
		case 0:
			timeLeftBeforeNewObjectBought = cmn_defines->getBuyersFrequency();
			return;
	}
}

double Market::getNormallyDistributedValue(double mean, double standartDeviation) {
	double u, v, s;
	int accuracy = cmn_defines->getAccuracy();
	do {
		u = 2 * double(rand()%accuracy) / accuracy - 1;
		v = 2 * double(rand()%accuracy) / accuracy - 1;
		s = u * u + v * v;
	} while(s > 1 || s == 0);
	s = sqrt(-2 * log(s) / s);
	return u * s * standartDeviation + mean;
}

double Market::getExponentiallyDistributedValue(double lambda) {
	int accuracy = cmn_defines->getAccuracy();
	double val = -1 * log(1 - double(rand()%accuracy) / accuracy) / lambda;
	return val;
}

/**********************************************************************
						End of statistical part
**********************************************************************/

#include <stdio.h>
void Market::printTimer() {
	printf("Timer = %d\n", timer);
	dataBase->viewDataBase();
}

void Market::refreshPicture() {
	dataBase->refreshPicture();
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
		if(object.getAge() == -1)	{break;}
		else								{object.printObjectToFinalFiles(); continue;}
	}

	while(1) {
		object = dataBase->popHighestBuyer();
		if(object.getAge() == -1)	{break;}
		else								{object.printObjectToFinalFiles(); continue;}
	}
}