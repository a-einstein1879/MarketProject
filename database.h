#ifndef DATABASE_H
#define DATABASE_H

#include "object.h"
#include "linklist.h"

class DataBase {
private:
	DataBase();
	static DataBase *p_DataBase;
	
	double lowestSellingPrice;
	LinkList objectsForSale;

	double highestBuyingPrice;
	LinkList objectsBought;

	int findPositionForObject(Object object);
public:
	static DataBase* getDataBase();
	int pushToDataBase(Object newObject);

	bool dealPossible();
	Object popLowestSeller();
	Object popHighestBuyer();
	void refreshPrices();

	void viewDataBase();
};

#endif