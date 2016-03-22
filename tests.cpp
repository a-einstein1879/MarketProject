/*
Test for object constructor
	Object object(3, 5, FORSALE);
	printf("price = %.2e, creation time = %.2e, status = %d\n", object.getPrice(), object.getCreationTime(), object.getStatus());
*/

/*
Test for <= and >= operators in object
	Object object1(3, 5, FORSALE);
	Object object2(4, 3, BOUGHT);
	if(object1 <= object2) {printf("1 < 2\n");}
	else {printf("2 < 1\n");}
*/

/*
Push/pop test for linklist
	Object object1(3, 5, FORSALE);
	Object object2(2, 3, BOUGHT);
	Object object3(1, 7, FORSALE);
	LinkList list1;
	list1.push(object1);
	list1.viewPrice();
	list1.viewTimers();
	list1.push(object2);
	list1.viewPrice();
	list1.viewTimers();
	list1.push(object3, 2);
	list1.viewPrice();
	list1.viewTimers();

	
	object1 = list1.pricePop();
	object1.printObject();
	object1 = list1.pricePop();
	object1.printObject();
	object1 = list1.pricePop();
	object1.printObject();

	list1.viewPrice();
	list1.viewTimers();
*/

/*
Test for pushing in the end of the list
	Object object1(3, 5, FORSALE);
	Object object2(2, 3, BOUGHT);
	Object object3(1, 7, FORSALE);
	LinkList list1;
	list1.push(object1);
	list1.push(object2, 2);
	list1.push(object3, 3);
	list1.view();
*/

/*
Test for linklist empty list
	LinkList list1;
	list1.clean();
*/

/*
Test for clean
	Object object1(3, 5, FORSALE);
	Object object2(2, 3, BOUGHT);
	Object object3(1, 7, FORSALE);
	LinkList list1;
	list1.push(object1);
	list1.push(object2);
	list1.push(object3, 2);
	list1.view();

	list1.clean();
	list1.view();
*/