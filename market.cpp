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

#define module(a, b) (a > b ? a - b : b - a)
void Market::runDeal() {
	Object seller, buyer;
	buyer = dataBase->popHighestBuyer();
	seller = dataBase->popLowestSeller();
	Deal deal;
	deal.price = ( buyer.getPrice() + seller.getPrice() ) / 2;
	deal.time = module(buyer.getCreationTime(), seller.getCreationTime());
	printDeal(deal);
}

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
double Market::formSellingPrice() {
	return 10 + rand()%15;
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

void Market::printDeal(Deal deal) {
	printf("Deal price = %.2f, deal time = %.2f\n", deal.price, deal.time);
}