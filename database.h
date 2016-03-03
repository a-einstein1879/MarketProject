#ifndef DATABASE_H
#define DATABASE_H

#include "object.h"
#include "linklist.h"

#define FORSALE 1
#define BOUGHT  0

class DataBase {
private:
	DataBase();
	static DataBase *p_DataBase;
	
	LinkList objectsForSale;
	int numberOfObjectsForSale;

	LinkList objectsBought;
	int numberOfObjectsBought;

	int findPositionForObject(Object object);
public:
	static DataBase* getDataBase();
	int pushToDataBase(Object newObject);
	void viewDataBase();
};

#endif