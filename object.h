#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "chart.h"

/*
Operators >= and <= are defined to find cheaper or more expensive objects not comparing objects statuses.
Operators > and < are defined to find younger or older objects not comparing objects statuses.
*/

#define FORSALE 1
#define BOUGHT  0

class Object {
private:
	double price;
	double age;
	bool status;
	int numberOfPriceReductions;
	int timer;
	FILE *buyersFinalPricesFile, *buyersFinalTimersFile, *sellersFinalPricesFile, *sellersFinalTimersFile;
public:
	Object();
	Object(double Price, double CreationTime, bool Status);
	void setObject(double Price, double Age, bool Status);
	void setFiles(FILE *BuyersFinalPricesFile, FILE *BuyersFinalTimersFile, FILE *SellersFinalPricesFile, FILE *SellersFinalTimersFile);
	void tick();
	void printObject();
	void printObjectToFinalFiles();

	/* Returns 1 if sale is not possible and 0 if it succeeded */
	bool adaptPrice();
	double	getPrice();
	double	getAge();
	bool	getStatus();
	int		getNumberOfPriceReductions();
	int		getTimer();
	FILE* getBuyersFinalPricesFile();
	FILE* getBuyersFinalTimersFile();
	FILE* getSellersFinalPricesFile();
	FILE* getSellersFinalTimersFile();

	Object& operator=(Object &object);
	bool operator<=(Object &object);
	bool operator>=(Object &object);
	bool operator<(Object &object);
	bool operator>(Object &object);

	void pushPriceToHistogram(Histogram &histogram);
	void pushPriceToChart(LineChart &lineChart);
};

#endif