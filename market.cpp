#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 1;
	resetSellingTimer();
	resetBuyingTimer();
	dataBase = dataBase->getDataBase();
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
//	while (dealPossible())	{runDeal();}
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
	return (timer % 2 == 0) ? true : false;
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

void Market::runDeal() {}

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
double Market::formSellingPrice() {
	return rand()%15;
}

double Market::formBuyingPrice() {
	return rand()%15;
}

void Market::resetSellingTimer() {
	timeLeftBeforeNewSellingObject = 3;
}

void Market::resetBuyingTimer() {
	timeLeftBeforeNewObjectBought = 5;
}
/**********************************************************************
						End of statistical part
**********************************************************************/

#include <stdio.h>
void Market::printDb() {
	printf("Timer = %d\n", timer);
	dataBase->viewDataBase();
}