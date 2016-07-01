#ifndef STATISTICS_H
#define STATISTICS_H

#include "linklist.h"

class Statistics {
private:
	static Statistics *p_Statistics;

	Statistics();
	void memeryAllocation();

	~Statistics();
	void memoryClear();

	int numberOfObjectTypes;
	LinkList *dealsForSale, *dealsBought;
	LinkList *meanForSalePrice, *meanBoughtPrice;
	LinkList *meanForSaleNumberOfObjects, *meanBoughtNumberOfObjects;
	LinkList *bidPrice, *askPrice;
	LinkList *meanSpread;
public:
	static Statistics* getStatistics();
};

#define DEALSFORSALEID					0
#define DEALSBOUGHTID					1
#define MEANFORSALEPRICEID				2
#define MEANBOUGHTPRICEID				3
#define MEANFORSALENUMBEROFOBJECTSID	4
#define MEANBOUGHTNUMBEROFBJECTSID		5
#define BIDPRICEID						6
#define ASKPRICEID						7
#define MEANSPREADID					8

#endif
