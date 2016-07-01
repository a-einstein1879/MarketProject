#include "statistics.h"

Statistics* Statistics::p_Statistics = 0;

Statistics* Statistics::getStatistics() {
	if(!p_Statistics) {
		p_Statistics = new Statistics();
	}
	return p_Statistics;
}

Statistics::Statistics() {
	numberOfObjectTypes = 2;
	memeryAllocation();
}

Statistics::~Statistics() {
	memoryClear();
}

void Statistics::memeryAllocation() {
	dealsForSale				= new LinkList[numberOfObjectTypes];
	dealsBought					= new LinkList[numberOfObjectTypes];
	meanForSalePrice			= new LinkList[numberOfObjectTypes];
	meanBoughtPrice				= new LinkList[numberOfObjectTypes];
	meanForSaleNumberOfObjects	= new LinkList[numberOfObjectTypes];
	meanBoughtNumberOfObjects	= new LinkList[numberOfObjectTypes];
	bidPrice					= new LinkList[numberOfObjectTypes];
	askPrice					= new LinkList[numberOfObjectTypes];
	meanSpread					= new LinkList[numberOfObjectTypes];
}

void Statistics::memoryClear() {
	delete [] dealsForSale;
	delete [] dealsBought;
	delete [] meanForSalePrice;
	delete [] meanBoughtPrice;
	delete [] meanForSaleNumberOfObjects;
	delete [] meanBoughtNumberOfObjects;
	delete [] bidPrice;
	delete [] askPrice;
	delete [] meanSpread;
}