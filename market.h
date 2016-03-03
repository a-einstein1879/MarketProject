#ifndef MARKET_H
#define MARKET_H

#include "database.h"

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
	bool timeToPrintDb();
	bool dealPossible();

	int addSeller();
	int addBuyer();
	void printDb();
	void runDeal();

	double formSellingPrice();
	double formBuyingPrice();
	void resetSellingTimer();
	void resetBuyingTimer();
};

#endif