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
	runDeals();
	if(timeToPrintTimer())	{printTimer();}
	if(timeToRefreshPicture())	{refreshPicture();}
	if(timeToFinish())		{handleDataBaseReturn(dataBase->closeDatabase()); return 0;}
	//if(timer % 3000 == 0)	{configurator->setSellersLambda(configurator->getSellersLambda(0) * 1.05, 0);}
	handleDataBaseReturn(dataBase->tick());
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


Agent* Market::getAgentById(int Id) {
	return &agent;
}

void Market::handleDataBaseReturn(DataBaseReturn *returnedObjects) {
	while(returnedObjects->linkList.getNumberOfObjects() != 0) {
		Object obj = returnedObjects->linkList.pricePop();
		getAgentById(obj.getAgentId())->handleObjectAfterDeal(obj);
	}
}

void Market::runDeals() {
	for(int i = 0; i < numberOfObjectTypes; i++) {
		while (dataBase->dealPossible(i))	{dataBase->runPossibleDeal(i);}
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

/**********************************************************************
							Interface
**********************************************************************/
#include <stdio.h>
void Market::printTimer() {
	printf("Timer = %d\n", timer);
}

void Market::refreshPicture() {
	dataBase->viewDataBaseInfo();
	agent.printAgentInfo();
	dataBase->gatherStatistics();
	dataBase->refreshPicture();
}
/**********************************************************************
						End of interface
**********************************************************************/