#ifndef DATABASE_H
#define DATABASE_H

#include "object.h"
#include "linklist.h"
#include "cmn_defines.h"
#include "interface.h"

class DataBase {
private:
	int timer;
	DataBase();
	static DataBase *p_DataBase;
	Cmn_Defines *cmn_defines;
	OpenGLInterface *ui;
	
	double lowestSellingPrice;
	LinkList objectsForSale;

	double highestBuyingPrice;
	LinkList objectsBought;

	LinkList dealsForSale, dealsBought;
	LinkList meanForSalePrice, meanBoughtPrice;
	LinkList meanForSaleNumberOfObjects, meanBoughtNumberOfObjects;
	
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
};

#endif