#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "histogram.h"

/*
Operators >= and <= are defined to find cheaper or more expensive objects not comparing objects statuses.
Operators > and < are defined to find younger or older objects not comparing objects statuses.
*/

#define FORSALE 1
#define BOUGHT  0

class Object {
private:
	double price;
	double creationTime;
	bool status;
	FILE *buyersFinalPricesFile, *buyersFinalTimersFile, *sellersFinalPricesFile, *sellersFinalTimersFile;
public:
	Object();
	Object(double Price, double CreationTime, bool Status);
	void setObject(double Price, double CreationTime, bool Status);
	void setFiles(FILE *BuyersFinalPricesFile, FILE *BuyersFinalTimersFile, FILE *SellersFinalPricesFile, FILE *SellersFinalTimersFile);
	void printObject();
	void printObjectToFinalFiles();

	double getPrice();
	double getCreationTime();
	bool   getStatus();
	FILE* getBuyersFinalPricesFile();
	FILE* getBuyersFinalTimersFile();
	FILE* getSellersFinalPricesFile();
	FILE* getSellersFinalTimersFile();

	Object& operator=(Object &object);
	bool operator<=(Object &object);
	bool operator>=(Object &object);
	bool operator<(Object &object);
	bool operator>(Object &object);

	void pushToHistogram(Histogram &histogram);
};

#endif