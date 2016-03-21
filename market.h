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
	Cmn_Defines *cmn_defines;
	int timer;
	int timeLeftBeforeNewSellingObject;
	int timeLeftBeforeNewObjectBought;

	void switchTimers();

	bool timeToAddSeller();
	bool timeToAddBuyer();
	bool dealPossible();
	bool timeToPrintTimer();
	bool timeToFinish();
	
	/* Statistics */
	double formSellingPrice();
	double formBuyingPrice();
	void resetSellingTimer();
	void resetBuyingTimer();
	
	double getNormallyDistributedValue(double mean, double standartDeviation);
	double getExponentiallyDistributedValue(double lambda);
	/* End of statistics*/

	int addSeller();
	int addBuyer();
	void runDeal();

	FILE *dealFile, *sellersFile, *buyersFile, *buyersFinalPricesFile, *buyersFinalTimersFile, *sellersFinalPricesFile, *sellersFinalTimersFile;
	void openFiles();
	void closeFiles();
	void printTimer();
	void printDeal(Deal deal);
public:
	static Market* getMarket();

	int tick();
	void finish();
};

#endif