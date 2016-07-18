#ifndef DATABASE_H
#define DATABASE_H

#include "object.h"
#include "linklist.h"
#include "configurator.h"
#include "outputGraphics.h"
#include "statistics.h"

class DataBase {
private:
	int timer;
	DataBase();
	~DataBase();
	static DataBase *p_DataBase;
	Configurator *configurator;
	OpenGLInterface *ui;
	Statistics *statistics;

	int numberOfObjectTypes;
	
	double *lowestSellingPrice;
	LinkList *objectsForSale;

	double *highestBuyingPrice;
	LinkList *objectsBought;

	DataBaseReturn objectsForReturn;

	LinkList dealsForSale, dealsBought;
	
	Object popLowestSeller(int typeId);
	Object popHighestBuyer(int typeId);

	void checkTimers();
	void refreshPrices();

	FILE *outputFiles[8];
	/*
	0 - file where we store all deals prices
	1 - file where we store timers of deals where sellers appeared on market first
	2 - file where we store timers of deals where buyers appeared on market first

	3 - At the end of the modeling all buyers prices are stored in the file
	4 - At the end of the modeling all buyers timers are stored in the file
	5 - At the end of the modeling all seller prices are stored in the file
	6 - At the end of the modeling all seller timers are stored in the file

	7 - file where we store "prices	and timers" of all deals. The price is always seller price.
	Timer is timer when the seller appeared first and 0 when buyer appeared first.
	Also all the seller`s that remained on market prices and timers go to the same file at the end of the modeling
	*/
	void openFiles();
	void closeFiles();
public:
	static DataBase* getDataBase();
	void start(int Timer);
	int pushToDataBase(Object newObject);

	DataBaseReturn* getObjectsForHandling() {return &objectsForReturn;}
	DataBaseReturn* tick();
	DataBaseReturn* closeDatabase();

	/* Deals */
	bool dealPossible(int typeId);
	void runPossibleDeal(int typeId);
	void addDeal(Object newObject);

	void viewDataBaseInfo();
	void refreshPicture();
	void gatherStatistics();
};

#endif