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
	double originalPrice;
	double price;

	bool status;
	/*
	0 - bought
	1 - for sale
	*/

	int type;
	/*
	number of types is defined in configuration
	*/

	int description;
	/*
	0 - objects on market
	1 - objects dealt
	2 - objects timeout
	3 - deals
	4 - statistics
	*/

	ObjectGeneralProperties& operator=(const ObjectGeneralProperties& generalProperties);
};

struct Timers {
	double age;
	int timeBeforePriceReduction;
	int timer;

	Timers& operator=(const Timers& timers);
	Timers operator++(int);
};

struct AgentIdProperties {
	int agentId;
	int objectId;

	AgentIdProperties& operator=(const AgentIdProperties& agentIdProperties);
};

struct AgentStrategy {
	bool priceAdaptationPossible;
	int possibleTimeOnMarket;
	int numberOfPossiblePriceAdaptations;
	int numberOfPriceAdaptations;
	double priceAdaptationShare;
	int priceAdaptationTimer;

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
	FILE *outputFiles[4];
	/* Files description can be found in database */

	Configurator *configurator;
public:
	Object();
	Object(double Price, double CreationTime, bool Status, int Type);

	void setObject(double Price, double Age, bool Status);
	void setFiles(FILE *buyersFinalPricesFile, FILE *buyersFinalTimersFile, FILE *sellersFinalPricesFile, FILE *sellersFinalTimersFile);
	void setAgentId(int id);
	void setDescription(int descr);
	void setAgentStrategy(AgentStrategy strategy);

	void tick();
	void printObject();
	void printObjectToFinalFiles();

	/* Returns 1 if it is not time for adaptation */
	bool adaptPrice();
	/* Returns 1 if object has to be removed and 0 if not */
	bool timeToLeaveMarket();

	/* Get */
	double	getOriginalPrice()				{return generalProperties.originalPrice;}
	double	getPrice()						{return generalProperties.price;}
	bool	getStatus()						{return generalProperties.status;}
	int		getType()						{return generalProperties.type;}
	int		getDescription()				{return generalProperties.description;}
	double	getAge()						{return timers.age;}
	int		getTimeBeforePriceReduction()	{return agentStrategy.priceAdaptationPossible?timers.timeBeforePriceReduction:1;}
	int		getTimer()						{return timers.timer;}
	int		getAgentId()					{return agentId.agentId;}
	ObjectGeneralProperties getGeneralProperties()	{return generalProperties;}
	AgentStrategy			getAgentStrategy()		{return agentStrategy;}
	AgentIdProperties		getAgentIdProperties()	{return agentId;}
	Timers					getTimers()				{return timers;}

	FILE* getOutputFile(int fileId)	{return outputFiles[fileId];}
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