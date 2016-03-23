#ifndef DATABASE_H
#define DATABASE_H

#include "object.h"
#include "linklist.h"
#include "cmn_defines.h"

class DataBase {
private:
	DataBase();
	static DataBase *p_DataBase;
	
	double lowestSellingPrice;
	LinkList objectsForSale;

	double highestBuyingPrice;
	LinkList objectsBought;

	LinkList deals;
public:
	static DataBase* getDataBase();
	int pushToDataBase(Object newObject);
	void addDeal(Object newObject);

	bool dealPossible();
	Object popLowestSeller();
	Object popHighestBuyer();
	void refreshPrices();

	void viewDataBase();
	void refreshPicture();
};

#endif