#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 0;
	timeLeftBeforeNewSellingObject = 3;
	timeLeftBeforeNewObjectBought = 5;
}

Market* Market::getMarket() {
	if(!p_Market) {
		p_Market = new Market();
	}
	return p_Market;
}

#include <stdlib.h>
void Market::run() {
	dataBase = dataBase->getDataBase();

/*	Object object;
	for(int i = 0; i < 15; i++) {
		object.setObject(rand()%15, i, rand()%2);
		dataBase->pushToDataBase(object);
	}
	dataBase->viewDataBase();*/
}

#include <stdio.h>
void Market::tick() {
	timer++;
	timeLeftBeforeNewSellingObject--;
	timeLeftBeforeNewObjectBought--;

	if(timeToAddSeller())	{addSeller();}
	if(timeToAddBuyer())	{addBuyer();}
	if(timeToPrintDb())		{printDb();}
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

int Market::addSeller() {
	Object object;
	object.setObject(rand()%15, timer, FORSALE);
	dataBase->pushToDataBase(object);
	resetSellingTimer();
	return 0;
}

int Market::addBuyer() {
	Object object;
	object.setObject(rand()%15, timer, BOUGHT);
	dataBase->pushToDataBase(object);
	resetBuyingTimer();
	return 0;
}

void Market::printDb() {
	printf("Timer = %d\n", timer);
	dataBase->viewDataBase();
}

void Market::resetSellingTimer() {
	timeLeftBeforeNewSellingObject = 3;
}

void Market::resetBuyingTimer() {
	timeLeftBeforeNewObjectBought = 5;
}