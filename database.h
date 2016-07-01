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
	
	double lowestSellingPrice;
	LinkList objectsForSale;

	double highestBuyingPrice;
	LinkList objectsBought;

	LinkList dealsForSale, dealsBought;
	
	Object popLowestSeller();
	Object popHighestBuyer();

	void checkTimers();
	void refreshPrices();

	FILE *dealFile, *sellersFile, *buyersFile;
	void openFiles();
	void closeFiles();
public:
	static DataBase* getDataBase();
	void start(int Timer);
	void tick();
	void closeDatabase();
	int pushToDataBase(Object newObject);
	void addDeal(Object newObject);

	bool dealPossible();
	void runPossibleDeal();

	void viewDataBaseInfo();
	void refreshPicture();
	void gatherStatistics();
};

#endif