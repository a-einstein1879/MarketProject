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

	void run();
	void tick();
	bool timeToAddSeller();
	bool timeToAddBuyer();
	bool timeToPrintDb();

	int addSeller();
	int addBuyer();
	void printDb();

	void resetSellingTimer();
	void resetBuyingTimer();
};

#endif