#include "agent.h"

Agent::Agent() {
	configurator = configurator->getConfigurator();
	timer = 1;

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
}

Object Agent::getObject() {
	Object object;
	for(int i = 0; i < numberOfObjectTypes; i++) {
		if(timeLeftBeforeNewObjectBought[i] == 0)	{
			resetBuyingTimer(i);
			return getBuyer();
		}
		if(timeLeftBeforeNewSellingObject[i] == 0)	{
			resetSellingTimer(i);
			return getSeller();
		}
	}
	return object;
}

Object Agent::getSeller() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formSellingPrice(rnd), timer, FORSALE, rnd);
	return object;
}

Object Agent::getBuyer() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formBuyingPrice(rnd), timer, BOUGHT, rnd);
	return object;
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
	agentInfo.buyerTimersMode = new int[numberOfObjectTypes];
	agentInfo.sellerTimersMode = new int[numberOfObjectTypes];
	agentInfo.buyerPricesMode = new int[numberOfObjectTypes];
	agentInfo.sellerPricesMode = new int[numberOfObjectTypes];
}

void Agent::freeAgentInfoMemory() {
	delete [] agentInfo.buyerTimersMode;
	delete [] agentInfo.sellerTimersMode;
	delete [] agentInfo.buyerPricesMode;
	delete [] agentInfo.sellerPricesMode;
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
		agentInfo.buyerTimersMode[i] = configurator->getBuyerTimersMode(i);
		agentInfo.sellerTimersMode[i] = configurator->getSellerTimersMode(i);
		agentInfo.buyerPricesMode[i] = configurator->getBuyerPricesMode(i);
		agentInfo.sellerPricesMode[i] = configurator->getSellerPricesMode(i);

		resetSellingTimer(i);
		resetBuyingTimer(i);
	}
}

double OrdinaryAgent::formSellingPrice(int type) {
	switch(agentInfo.sellerPricesMode[type]) {
		case 1:
			return getNormallyDistributedValue(configurator->getSellersMean(type), configurator->getSellersStandartDeviation(type));
		case 0:
			return configurator->getMinimumSellersPrice(type) + rand()%int(configurator->getMaximumSellersPrice(type) - configurator->getMinimumSellersPrice(type) + 1);
	}
	return -1;
}

double OrdinaryAgent::formBuyingPrice(int type) {
	switch(agentInfo.buyerPricesMode[type]) {
		case 1:
			return getNormallyDistributedValue(configurator->getBuyersMean(type), configurator->getBuyersStandartDeviation(type));
		case 0:
			return configurator->getMinimumBuyersPrice(type) + rand()%int(configurator->getMaximumBuyersPrice(type) - configurator->getMinimumBuyersPrice(type) + 1);
	}
	return -1;
}

void OrdinaryAgent::resetSellingTimer(int type) {
	switch(agentInfo.sellerTimersMode[type]) {
		case 1:
			timeLeftBeforeNewSellingObject[type] = int(getExponentiallyDistributedValue(configurator->getSellersLambda(type)));
			return;
		case 0:
			timeLeftBeforeNewSellingObject[type] = configurator->getSellersFrequency(type);
			return;
	}
}

void OrdinaryAgent::resetBuyingTimer(int type) {
	switch(agentInfo.buyerTimersMode[type]) {
		case 1:
			timeLeftBeforeNewObjectBought[type] = int(getExponentiallyDistributedValue(configurator->getBuyersLambda(type)));
			return;
		case 0:
			timeLeftBeforeNewObjectBought[type] = configurator->getBuyersFrequency(type);
			return;
	}
}

/**********************************************************************
						End of ordinary agent
**********************************************************************/