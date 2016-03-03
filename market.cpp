#include "market.h"
#include "object.h"
#include "database.h"

Market* Market::p_Market = 0;

Market::Market() {}

Market* Market::getMarket() {
	if(!p_Market) {
		p_Market = new Market();
	}
	return p_Market;
}

#include <stdlib.h>
void Market::run() {
	DataBase *dataBase;
	dataBase = dataBase->getDataBase();

	Object object;
	for(int i = 0; i < 15; i++) {
		object.setObject(rand()%15, i, rand()%2);
		dataBase->pushToDataBase(object);
	}

	dataBase->viewDataBase();
}