#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include "chart.h"
#include "configurator.h"

/*
Operators >= and <= are defined to find cheaper or more expensive objects not comparing objects statuses.
Operators > and < are defined to find younger or older objects not comparing objects statuses.
*/

struct ObjectGeneralProperties {
	double price;
	bool status;
	int type;

	ObjectGeneralProperties& operator=(const ObjectGeneralProperties& generalProperties);
};

struct Timers {
	double age;
	int timeAfterPriceReduction;
	int timer;

	Timers& operator=(const Timers& timers);
};

struct AgentIdProperties {
	int agentId;
	int objectId;
};

struct AgentStrategy {
	int numberOfPossiblePriceAdaptations;
	int numberOfPriceAdaptations;
	double sellerPriceReduceShare;
	double buyerPriceReduceShare;

	AgentStrategy& operator=(const AgentStrategy& agentStrategy);
};

#define FORSALE 1
#define BOUGHT  0

class Object {
private:
	ObjectGeneralProperties generalProperties;
	AgentStrategy agentStrategy;
	AgentIdProperties agentId;
	Timers timers;
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
	double	getPrice()						{return generalProperties.price;}
	bool	getStatus()						{return generalProperties.status;}
	int		getType()						{return generalProperties.type;}
	double	getAge()						{return timers.age;}
	int		getTimeAfterPriceReduction()	{return timers.timeAfterPriceReduction;}
	int		getTimer()						{return timers.timer;}
	ObjectGeneralProperties getGeneralProperties()	{return generalProperties;}
	AgentStrategy			getAgentStrategy()		{return agentStrategy;}
	AgentIdProperties		getAgentIdProperties()	{return agentId;}
	Timers					getTimers()				{return timers;}

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