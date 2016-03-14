#ifndef MARKET_H
#define MARKET_H

#include "database.h"
#include "cmn_defines.h"
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
	bool timeToFinish();
	
	/* Statistics */
	double formSellingPrice();
	double formBuyingPrice();
	void resetSellingTimer();
	void resetBuyingTimer();
	
	double getNormallyDistributedValue(double mean, double dispersion);
	double getExponentiallyDistributedValue(double lambda);
	/* End of statistics*/

	int addSeller();
	int addBuyer();
	void runDeal();

	FILE *dealFile, *sellersFile, *buyersFile;
	void openFiles();
	void closeFiles();
	void printDb();
	void printDeal(Deal deal);
public:
	static Market* getMarket();

	int tick();
	void finish();
};

#endif