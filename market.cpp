#include "market.h"
#include "object.h"

Market* Market::p_Market = 0;

Market::Market() {
	timer = 1;
	dataBase = dataBase->getDataBase();
	dataBase->start(timer);
	configurator = configurator->getConfigurator();
	configurator->printConfiguration();
	numberOfObjectTypes = configurator->getNumberOfObjectTypes();
}

Market::~Market() {
}

Market* Market::getMarket() {
	if(!p_Market) {
		p_Market = new Market();
	}
	return p_Market;
}

int Market::tick() {
	tickAgents();
	for(int i = 0; i < numberOfObjectTypes; i++) {
		while (dealPossible(i))	{dataBase->runPossibleDeal(i);}
	}
	if(timeToPrintTimer())	{printTimer();}
	if(timeToRefreshPicture())	{refreshPicture();}
	if(timeToFinish())		{dataBase->closeDatabase(); return 0;}
	//if(timer % 3000 == 0)	{configurator->setSellersLambda(configurator->getSellersLambda(0) * 1.05, 0);}
	dataBase->tick();
	timer++;
	return 1;
}

void Market::tickAgents() {
	agent.tick();
	while(1) {
		Object object = agent.getObject();
		if(object.getType() == -1) {break;}
		dataBase->pushToDataBase(object);
	}
}

bool Market::timeToPrintTimer() {
	return (timer % configurator->getTimerPrintingFrequency() == 0) ? true : false;
}

bool Market::timeToRefreshPicture() {
	return (timer % configurator->getPictureRefreshFrequency() == 0) ? true : false;
}

bool Market::timeToFinish() {
	return (timer < configurator->getModelingTime()) ? false : true;
}

bool Market::dealPossible(int typeId) {
	return dataBase->dealPossible(typeId);
}

/**********************************************************************
							Interface
**********************************************************************/
#include <stdio.h>
void Market::printTimer() {
	printf("Timer = %d\n", timer);
}

void Market::refreshPicture() {
	dataBase->viewDataBaseInfo();
	dataBase->gatherStatistics();
	dataBase->refreshPicture();
}
/**********************************************************************
						End of interface
**********************************************************************/