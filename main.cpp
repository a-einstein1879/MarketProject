#include "market.h"
#include <stdlib.h>

#include "object.h"
#include "linklist.h"

int main() {
	Market *market;
	market = market->getMarket();

	for(int i = 0; i < 8; i++) {
		market->tick();
	}
/*	Object obj1(1, 2, 1);
	Object obj2(2, 2, 1);
	LinkList list1;
	list1.push(obj1);
	list1.push(obj2);
	list1.view();

	list1.pop(1);
	list1.view();

	list1.push(obj2, 1);
	list1.view();*/

	system("pause");
	return 0;
};