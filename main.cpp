#include <stdlib.h>

#include "object.h"
#include "database.h"

int main() {
	DataBase *dataBase;
	dataBase = dataBase->getDataBase();

	Object object;
	for(int i = 0; i < 15; i++) {
		object.setObject(rand()%15, i, rand()%2);
		dataBase->pushToDataBase(object);
	}

	dataBase->viewDataBase();

	system("pause");
	return 0;
};