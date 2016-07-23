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

	numberOfAgents = 1;
	agents[0] = &agent1;
	agents[1] = &agent2;
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
	for(int i = 0; i < numberOfAgents; i++) {
		agents[i]->tick();
		while(1) {
			Object object = agents[i]->getObject();
			if(object.getType() == -1) {break;}
			dataBase->pushToDataBase(object);
		}
	}
}


Agent* Market::getAgentById(int Id) {
	if(Id >= 0 && Id < numberOfAgents) {
		return agents[Id];
	} else {
		return NULL;
	}
}

void Market::handleDataBaseReturn(DataBaseReturn *returnedObjects) {
	while(returnedObjects->dealtObjects.getNumberOfObjects() != 0) {
		Object obj = returnedObjects->dealtObjects.pricePop();
		Agent *agent = getAgentById(obj.getAgentId());
		if(agent != NULL) {
			agent->handleObjectAfterDeal(obj);
		}
	}
	while(returnedObjects->timeoutObjects.getNumberOfObjects() != 0) {
		Object obj = returnedObjects->timeoutObjects.pricePop();
		Agent *agent = getAgentById(obj.getAgentId());
		if(agent != NULL) {
			agent->handleTimeoutObject(obj);
		}
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
	for(int i = 0; i < numberOfAgents; i++) {
		agents[i]->printAgentInfo();
	}
	dataBase->gatherStatistics();
	dataBase->refreshPicture();
}
/**********************************************************************
						End of interface
**********************************************************************/