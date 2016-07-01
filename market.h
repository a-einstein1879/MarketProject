#ifndef MARKET_H
#define MARKET_H

#include "database.h"
#include "configurator.h"
#include <stdio.h>

class Market {
private:
	Market();
	~Market();
	static Market *p_Market;
	
	DataBase *dataBase;
	Configurator *configurator;

	/* Timers */
	int timer;
	int timeLeftBeforeNewSellingObject;
	int timeLeftBeforeNewObjectBought;
	void switchTimers();
	/* End of timers */

	/* Timer checkers */
	bool timeToAddSeller();
	bool timeToAddBuyer();
	bool dealPossible();
	bool timeToPrintTimer();
	bool timeToRefreshPicture();
	bool timeToFinish();
	/* End of timer checkers */
	
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

	void printTimer();
	void refreshPicture();
public:
	static Market* getMarket();

	int tick();
	void finish();
};

#endif