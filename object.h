#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "chart.h"
#include "configurator.h"

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
	int type;
	FILE *buyersFinalPricesFile, *buyersFinalTimersFile, *sellersFinalPricesFile, *sellersFinalTimersFile;
	Configurator *configurator;
public:
	Object();
	Object(double Price, double CreationTime, bool Status, int Type);
	void setObject(double Price, double Age, bool Status);
	void setFiles(FILE *BuyersFinalPricesFile, FILE *BuyersFinalTimersFile, FILE *SellersFinalPricesFile, FILE *SellersFinalTimersFile);
	void tick();
	void printObject();
	void printObjectToFinalFiles();

	/* Returns 1 if sale is not possible and 0 if it succeeded */
	bool adaptPrice();

	/* Get */
	double	getPrice()						{return price;}
	double	getAge()						{return age;}
	bool	getStatus()						{return status;}
	int		getNumberOfPriceReductions()	{return numberOfPriceReductions;}
	int		getTimer()						{return timer;}
	int		getType()						{return type;}
	FILE* getBuyersFinalPricesFile()		{return buyersFinalPricesFile;}
	FILE* getBuyersFinalTimersFile()		{return buyersFinalTimersFile;}
	FILE* getSellersFinalPricesFile()		{return sellersFinalPricesFile;}
	FILE* getSellersFinalTimersFile()		{return sellersFinalTimersFile;}
	/* End of get */

	Object& operator=(Object &object);
	bool operator<=(Object &object);
	bool operator>=(Object &object);
	bool operator<(Object &object);
	bool operator>(Object &object);

	void pushPriceToHistogram(Histogram &histogram);
	void pushPriceToChart(LineChart &lineChart);
};

#endif