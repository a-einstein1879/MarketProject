#ifndef MARKET_H
#define MARKET_H

#include "database.h"
struct Deal {
	double price;
	double time;
};

class Market {
private:
	Market();
	static Market *p_Market;
	
	DataBase *dataBase;
	int timer;
	int timeLeftBeforeNewSellingObject;
	int timeLeftBeforeNewObjectBought;
public:
	static Market* getMarket();

	void tick();
	void switchTimers();

	bool timeToAddSeller();
	bool timeToAddBuyer();
	bool dealPossible();
	bool timeToPrintDb();

	int addSeller();
	int addBuyer();
	void runDeal();
	void printDeal(Deal deal);
	void printDb();

	double formSellingPrice();
	double formBuyingPrice();
	void resetSellingTimer();
	void resetBuyingTimer();
};

#endif