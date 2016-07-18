#include "agent.h"
#include <stdio.h>

int Agent::agentCounter = 0;

Agent::Agent() {
	configurator = configurator->getConfigurator();
	timer = 1;
	agentInfo.agentId = agentCounter++;

	/* Agent info */
	agentInfo.numberOfObjects = 0;
	numberOfObjectsSold = 0;
	numberOfObjectsBought = 0;
	/* End of agent info */

	numberOfObjectTypes = configurator->getNumberOfObjectTypes();
	allocateMemory();
}

Agent::~Agent() {
	freeMemory();
}

void Agent::switchTimers() {
	timer++;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		timeLeftBeforeNewObjectBought[i]--;
		timeLeftBeforeNewSellingObject[i]--;
	}
}

void Agent::tick() {
	switchTimers();
	additionalTickActions();
}

Object Agent::getObject() {
	Object object;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		if(timeLeftBeforeNewObjectBought[i] == 0)	{
			resetBuyingTimer(i);
			agentInfo.numberOfObjects++;
			return getBuyer();
		}
		if(timeLeftBeforeNewSellingObject[i] == 0)	{
			resetSellingTimer(i);
			agentInfo.numberOfObjects++;
			return getSeller();
		}
	}
	return object;
}

Object Agent::getSeller() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formSellingPrice(rnd), timer, FORSALE, rnd);
	object.setAgentId(agentInfo.agentId);
	return object;
}

Object Agent::getBuyer() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formBuyingPrice(rnd), timer, BOUGHT, rnd);
	object.setAgentId(agentInfo.agentId);
	return object;
}

void Agent::handleObjectAfterDeal(Object newObject) {
	if(newObject.getStatus() == FORSALE) {
		numberOfObjectsSold++;
	} else {
		numberOfObjectsBought++;
	}
	agentInfo.numberOfObjects--;
}

void Agent::printAgentInfo() {
	printf("Agent info:\n");
	printAgentType();
	printf("Agent id = %d\n", agentInfo.agentId);
	printf("Number of objects owned = %d\n", agentInfo.numberOfObjects);
	printf("Number of objects sold = %d\n", numberOfObjectsSold);
	printf("Number of objects bought = %d\n", numberOfObjectsBought);
	printf("\n");
}

/**********************************************************************
							Memory allocation
**********************************************************************/
void Agent::allocateMemory() {
	allocateTimersMemory();
	allocateAgentInfoMemory();
}

void Agent::freeMemory() {
	freeTimersMemory();
	freeAgentInfoMemory();
}

void Agent::allocateTimersMemory() {
	timeLeftBeforeNewSellingObject = new int[numberOfObjectTypes];
	timeLeftBeforeNewObjectBought = new int[numberOfObjectTypes];
}

void Agent::freeTimersMemory() {
	delete [] timeLeftBeforeNewSellingObject;
	delete [] timeLeftBeforeNewObjectBought;
}

void Agent::allocateAgentInfoMemory() {
	agentMode.buyerTimersMode = new int[numberOfObjectTypes];
	agentMode.sellerTimersMode = new int[numberOfObjectTypes];
	agentMode.buyerPricesMode = new int[numberOfObjectTypes];
	agentMode.sellerPricesMode = new int[numberOfObjectTypes];
}

void Agent::freeAgentInfoMemory() {
	delete [] agentMode.buyerTimersMode;
	delete [] agentMode.sellerTimersMode;
	delete [] agentMode.buyerPricesMode;
	delete [] agentMode.sellerPricesMode;
}

/**********************************************************************
							End of memory allocation
**********************************************************************/

/**********************************************************************
							Statistical part
**********************************************************************/
#include <stdlib.h>
#include <cmath>

double Agent::getNormallyDistributedValue(double mean, double standartDeviation) {
	double u, v, s;
	int accuracy = configurator->getAccuracy();
	do {
		u = 2 * double(rand()%accuracy) / accuracy - 1;
		v = 2 * double(rand()%accuracy) / accuracy - 1;
		s = u * u + v * v;
	} while(s > 1 || s == 0);
	s = sqrt(-2 * log(s) / s);
	return u * s * standartDeviation + mean;
}

double Agent::getExponentiallyDistributedValue(double lambda) {
	int accuracy = configurator->getAccuracy();
	double val = -1 * log(1 - double(rand()%accuracy) / accuracy) / lambda;
	return val;
}

/**********************************************************************
						End of statistical part
**********************************************************************/

/**********************************************************************
							Ordinary agent
**********************************************************************/

OrdinaryAgent::OrdinaryAgent() {
	for(int i = 0; i < numberOfObjectTypes; i++) {
		agentMode.buyerTimersMode[i] = configurator->getBuyerTimersMode(i);
		agentMode.sellerTimersMode[i] = configurator->getSellerTimersMode(i);
		agentMode.buyerPricesMode[i] = configurator->getBuyerPricesMode(i);
		agentMode.sellerPricesMode[i] = configurator->getSellerPricesMode(i);

		resetSellingTimer(i);
		resetBuyingTimer(i);
	}
}

double OrdinaryAgent::formSellingPrice(int type) {
	switch(agentMode.sellerPricesMode[type]) {
		case 1:
			return getNormallyDistributedValue(configurator->getSellersMean(type), configurator->getSellersStandartDeviation(type));
		case 0:
			return configurator->getMinimumSellersPrice(type) + rand()%int(configurator->getMaximumSellersPrice(type) - configurator->getMinimumSellersPrice(type) + 1);
	}
	return -1;
}

double OrdinaryAgent::formBuyingPrice(int type) {
	switch(agentMode.buyerPricesMode[type]) {
		case 1:
			return getNormallyDistributedValue(configurator->getBuyersMean(type), configurator->getBuyersStandartDeviation(type));
		case 0:
			return configurator->getMinimumBuyersPrice(type) + rand()%int(configurator->getMaximumBuyersPrice(type) - configurator->getMinimumBuyersPrice(type) + 1);
	}
	return -1;
}

void OrdinaryAgent::resetSellingTimer(int type) {
	switch(agentMode.sellerTimersMode[type]) {
		case 1:
			timeLeftBeforeNewSellingObject[type] = int(getExponentiallyDistributedValue(configurator->getSellersLambda(type)));
			return;
		case 0:
			timeLeftBeforeNewSellingObject[type] = configurator->getSellersFrequency(type);
			return;
	}
}

void OrdinaryAgent::resetBuyingTimer(int type) {
	switch(agentMode.buyerTimersMode[type]) {
		case 1:
			timeLeftBeforeNewObjectBought[type] = int(getExponentiallyDistributedValue(configurator->getBuyersLambda(type)));
			return;
		case 0:
			timeLeftBeforeNewObjectBought[type] = configurator->getBuyersFrequency(type);
			return;
	}
}

void OrdinaryAgent::printAgentType() {
	printf("Ordinary agent\n");
}

void OrdinaryAgent::additionalTickActions() {}

/**********************************************************************
						End of ordinary agent
**********************************************************************/


/**********************************************************************
					Solo object selling agent
**********************************************************************/

SoloObjectSellingAgent::SoloObjectSellingAgent() {
	for(int i = 0; i < numberOfObjectTypes; i++) {
		resetSellingTimer(i);
		resetBuyingTimer(i);
	}
}

double SoloObjectSellingAgent::formSellingPrice(int type) {
	return getNormallyDistributedValue(configurator->getSellersMean(type), 0);
}

double SoloObjectSellingAgent::formBuyingPrice(int type) {
	return -1;
}

void SoloObjectSellingAgent::resetSellingTimer(int type) {
	switch(configurator->getSellerTimersMode(type)) {
		case 1:
			timeLeftBeforeNewSellingObject[type] = int(getExponentiallyDistributedValue(configurator->getSellersLambda(type))) * 100;
			return;
		case 0:
			timeLeftBeforeNewSellingObject[type] = configurator->getSellersFrequency(type) * 100;
			return;
	}
}

void SoloObjectSellingAgent::resetBuyingTimer(int type) {
	timeLeftBeforeNewObjectBought[type] = -1;
}

void SoloObjectSellingAgent::printAgentType() {
	printf("Solo object selling agent\n");
}

void SoloObjectSellingAgent::additionalTickActions() {
}

/**********************************************************************
					End of solo object selling agent
**********************************************************************/