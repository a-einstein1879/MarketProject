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
	static DataBase *p_DataBase;
	Configurator *configurator;
	OpenGLInterface *ui;
	Statistics *statistics;
	
	double lowestSellingPrice;
	LinkList objectsForSale;

	double highestBuyingPrice;
	LinkList objectsBought;

	LinkList dealsForSale, dealsBought;
	
	void checkTimers();
	void refreshPrices();
public:
	static DataBase* getDataBase();
	void start(int Timer);
	void tick();
	int pushToDataBase(Object newObject);
	void addDeal(Object newObject);

	bool dealPossible();
	Object popLowestSeller();
	Object popHighestBuyer();

	void viewDataBase();
	void refreshPicture();
	void gatherStatistics();
};

#endif