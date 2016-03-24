#include "object.h"

Object::Object() {
	timer = 1;
	price = 0;
	age = -1;
	status = 0;
	numberOfPriceReductions = 0;
}

Object::Object(double Price, double CreationTime, bool Status) {
	timer = int(CreationTime);
	setObject(Price, 0, Status);
}

void Object::setObject(double Price, double Age, bool Status) {
	price					= Price;
	age						= Age;
	status					= Status;
	numberOfPriceReductions = 0;
}

void Object::setFiles(FILE *BuyersFinalPricesFile, FILE *BuyersFinalTimersFile, FILE *SellersFinalPricesFile, FILE *SellersFinalTimersFile) {
	buyersFinalPricesFile = BuyersFinalPricesFile;
	buyersFinalTimersFile = BuyersFinalTimersFile;
	sellersFinalPricesFile = SellersFinalPricesFile;
	sellersFinalTimersFile = SellersFinalTimersFile;
}

void Object::tick() {
	timer++;
	age++;
}

void Object::printObject() {
	printf("price = %.2f, age = %.2f, number of price reductions = %d status = \"%s\"\n", getPrice(), getAge(), getNumberOfPriceReductions(), getStatus()?"For sale":"Bought");
}

/* TODO: there is a problem with timers files. We will have creation times there instead of time spent on market. Don`t want to add timer as an argument, because objects don`t need to know anything about timers */
void Object::printObjectToFinalFiles() {
	if(status == BOUGHT) {
		fprintf(buyersFinalPricesFile, "%.2f\n", price);
		fprintf(buyersFinalTimersFile, "%.2f\n", age);
	} else {
		fprintf(sellersFinalPricesFile, "%.2f\n", price);
		fprintf(sellersFinalTimersFile, "%.2f\n", age);
	}
}

bool Object::adaptPrice() {
	if(numberOfPriceReductions++ > 0) {return 1;}
	if(status == FORSALE) {
		price = 0.8 * price;
	} else {
		price = 1.2 * price;
	}
	age = 0;
	return 0;
}

double Object::getPrice() {
	return price;
}
double Object::getAge() {
	return age;
}

bool Object::getStatus() {
	return status;
}

int	Object::getNumberOfPriceReductions() {
	return numberOfPriceReductions;
}

int Object::getTimer() {
	return timer;
}

FILE* Object::getBuyersFinalPricesFile() {
	return buyersFinalPricesFile;
}

FILE* Object::getBuyersFinalTimersFile() {
	return buyersFinalTimersFile;
}

FILE* Object::getSellersFinalPricesFile() {
	return sellersFinalPricesFile;
}

FILE* Object::getSellersFinalTimersFile() {
	return sellersFinalTimersFile;
}

Object& Object::operator=(Object &object) {
	price		 = object.getPrice();
	age			 = object.getAge();
	status		 = object.getStatus();
	numberOfPriceReductions = object.getNumberOfPriceReductions();
	timer		 = object.getTimer();

	buyersFinalPricesFile = object.getBuyersFinalPricesFile();
	buyersFinalTimersFile = object.getBuyersFinalTimersFile();
	sellersFinalPricesFile = object.getSellersFinalPricesFile();
	sellersFinalTimersFile = object.getSellersFinalTimersFile();
	return *this;
}

bool Object::operator<=(Object &object) {
	if(price <= object.getPrice()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator>=(Object &object) {
	if(price >= object.getPrice()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator<(Object &object) {
	if(age <= object.getAge()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator>(Object &object) {
	if(age >= object.getAge()) {
		return true;
	} else {
		return false;
	}
}

void Object::pushToHistogram(Chart &histogram) {
	histogram.addValueToTmpIndex(price);
}