#ifndef STATISTICS_H
#define STATISTICS_H

#include "linklist.h"
#include "chart.h"
#include "configurator.h"
#include "outputGraphics.h"

class Statistics {
private:
	static Statistics *p_Statistics;
	Configurator *configurator;
	OpenGLInterface *ui;

	Statistics();
	void memeryAllocation();

	~Statistics();
	void memoryClear();

	int numberOfObjectTypes;
	int numberOfStatistics;
	LinkList **statistics;
public:
	static Statistics* getStatistics();
	void addStatisticsElement(double newValue, int type, int statisticsId);
	double getStatisticsElement(int type, int statisticsId);
	double getMeanValue(int type, int statisticsId);
	void drawStatistics();
};

/*
0 - prices of objects that were sold in case seller appeared on market first
1 - prices of objects that were sold in case buyer appeared on market first
2 - number of objects on market for sale
3 - number of objects on market bought
4 - bid price
5 - ask price
6 - spread
*/

#define FORSALEPRICEID				0
#define BOUGHTPRICEID				1
#define FORSALENUMBEROFOBJECTSID	2
#define BOUGHTNUMBEROFBJECTSID		3
#define BIDPRICEID					4
#define ASKPRICEID					5
#define SPREADID					6

#endif
