#include "object.h"

Object::Object() {
	price		 = 0;
	creationTime = -1;
	status		 = 0;
}

Object::Object(double Price, double CreationTime, bool Status) {
	price		 = Price;
	creationTime = CreationTime;
	status		 = Status;
}

void Object::setObject(double Price, double CreationTime, bool Status) {
	price		 = Price;
	creationTime = CreationTime;
	status		 = Status;
}

void Object::setFiles(FILE *BuyersFinalPricesFile, FILE *BuyersFinalTimersFile, FILE *SellersFinalPricesFile, FILE *SellersFinalTimersFile) {
	buyersFinalPricesFile = BuyersFinalPricesFile;
	buyersFinalTimersFile = BuyersFinalTimersFile;
	sellersFinalPricesFile = SellersFinalPricesFile;
	sellersFinalTimersFile = SellersFinalTimersFile;
}

void Object::printObject() {
	printf("price = %.2f, creation time = %.2f, status = \"%s\"\n", getPrice(), getCreationTime(), getStatus()?"For sale":"Bought");
}

/* TODO: there is a problem with timers files. We will have creation times there instead of time spent on market. Don`t want to add timer as an argument, because objects don`t need to know anything about timers */
void Object::printObjectToFinalFiles() {
	if(status == BOUGHT) {
		fprintf(buyersFinalPricesFile, "%.2f\n", price);
		fprintf(buyersFinalTimersFile, "%.2f\n", creationTime);
	} else {
		fprintf(sellersFinalPricesFile, "%.2f\n", price);
		fprintf(sellersFinalTimersFile, "%.2f\n", creationTime);
	}
}

double Object::getPrice() {
	return price;
}
double Object::getCreationTime() {
	return creationTime;
}

bool Object::getStatus() {
	return status;
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
	creationTime = object.getCreationTime();
	status		 = object.getStatus();
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