#ifndef MARKET_H
#define MARKET_H

#include "database.h"
#include <stdio.h>

struct Deal {
	double price;
	double time;
};

class Market {
private:
	Market();
	~Market();
	static Market *p_Market;
	
	DataBase *dataBase;
	int timer;
	int timeLeftBeforeNewSellingObject;
	int timeLeftBeforeNewObjectBought;

	void switchTimers();

	bool timeToAddSeller();
	bool timeToAddBuyer();
	bool dealPossible();
	bool timeToPrintDb();

	int addSeller();
	int addBuyer();
	void runDeal();

	FILE *file;
	void printDeal(Deal deal);
public:
	static Market* getMarket();

	void tick();


	double formSellingPrice();
	double formBuyingPrice();
	void resetSellingTimer();
	void resetBuyingTimer();

	
	void printDb();
};

#endif