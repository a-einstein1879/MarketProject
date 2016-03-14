#ifndef MARKET_H
#define MARKET_H

#include "database.h"
#include <stdio.h>

#define DEALFILE		"output/dealsOutput.txt"
#define SELLERSFILE		"output/sellersOutput.txt"
#define BUYERSFILE		"output/buyersOutput.txt"
#define SILENTMODE

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
	
	/* Statistics */
	double formSellingPrice();
	double formBuyingPrice();
	void resetSellingTimer();
	void resetBuyingTimer();

	double getNormallyDistributedValue(double mean, double dispersion);
	/* end of statistics*/

	int addSeller();
	int addBuyer();
	void runDeal();

	FILE *dealFile, *sellersFile, *buyersFile;
	void openFiles();
	void closeFiles();
	void printDeal(Deal deal);
public:
	static Market* getMarket();

	void tick();
	
	void printDb();
};

#endif