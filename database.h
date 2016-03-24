#ifndef DATABASE_H
#define DATABASE_H

#include "object.h"
#include "linklist.h"
#include "cmn_defines.h"

class DataBase {
private:
	int timer;
	DataBase();
	static DataBase *p_DataBase;
	Cmn_Defines *cmn_defines;
	
	double lowestSellingPrice;
	LinkList objectsForSale;

	double highestBuyingPrice;
	LinkList objectsBought;

	LinkList dealsForSale, dealsBought;
	
	void checkTimers();
	void refreshPrices();
public:
	static DataBase* getDataBase();
	void tick();
	int pushToDataBase(Object newObject);
	void addDeal(Object newObject);

	bool dealPossible();
	Object popLowestSeller();
	Object popHighestBuyer();

	void viewDataBase();
	void refreshPicture();
};

#endif