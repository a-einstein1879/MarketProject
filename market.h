#ifndef MARKET_H
#define MARKET_H

#include "database.h"
#include "configurator.h"
#include "agent.h"
#include <stdio.h>

class Market {
private:
	Market();
	~Market();
	static Market *p_Market;
	
	DataBase *dataBase;
	Configurator *configurator;

	int numberOfObjectTypes;
	OrdinaryAgent agent;
	Agent* getAgentById(int Id);
	void tickAgents();
	void handleDataBaseReturn(DataBaseReturn *returnedObjects);

	void runDeals();

	/* Timers */
	int timer;
	/* End of timers */

	/* Timer checkers */
	bool timeToPrintTimer();
	bool timeToRefreshPicture();
	bool timeToFinish();
	/* End of timer checkers */
	
	void printTimer();
	void refreshPicture();
public:
	static Market* getMarket();

	int tick();
	void finish();
};

#endif