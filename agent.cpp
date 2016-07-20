#include "agent.h"
#include <stdio.h>

int Agent::agentCounter = 0;

Agent::Agent() {
	configurator = configurator->getConfigurator();
	timer = 1;
	agentInfo.agentId = agentCounter++;

	/* Agent info */
	agentInfo.numberOfObjects = 0;
	agentStatistics.forsale.numberOfObjects = 0;
	agentStatistics.forsale.averageWaitingTime = 0;
	agentStatistics.forsale.averageStartPrice = 0;
	agentStatistics.forsale.numberOfObjectsGenerated = 0;
	agentStatistics.forsale.averagePriceAtMomentOfDeal = 0;
	agentStatistics.bought.numberOfObjects = 0;
	agentStatistics.bought.averageWaitingTime = 0;
	agentStatistics.bought.averageStartPrice = 0;
	agentStatistics.bought.numberOfObjectsGenerated = 0;
	agentStatistics.bought.averagePriceAtMomentOfDeal = 0;
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
		if(readyToGenerateBuyer(i)) {
			resetBuyingTimer(i);
			agentInfo.numberOfObjects++;
			agentStatistics.bought.numberOfObjectsGenerated++;
			return getBuyer();
		}
		if(readyToGenerateSeller(i))	{
			resetSellingTimer(i);
			agentInfo.numberOfObjects++;
			agentStatistics.forsale.numberOfObjectsGenerated++;
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

#define wt(arg)	 agentStatistics.arg.averageWaitingTime
#define no(arg)	 agentStatistics.arg.numberOfObjects
#define pr(arg)	 agentStatistics.arg.averageStartPrice
#define dpr(arg) agentStatistics.bought.averagePriceAtMomentOfDeal

void Agent::handleObjectAfterDeal(Object newObject) {
	if(newObject.getStatus() == FORSALE) {
		wt(forsale) = wt(forsale) * double(no(forsale)) / double (no(forsale) + 1) + double(newObject.getAge()) / double (no(forsale) + 1);
		wt(bought)	= wt(bought)  * double(no(bought))  / double (no(bought)  + 1) + double(0)                  / double (no(bought)  + 1);
		pr(forsale) = pr(forsale) * double(no(forsale)) / double (no(forsale) + 1) + double(newObject.getOriginalPrice()) / double (no(forsale) + 1);
		dpr(forsale) = dpr(forsale) * double(no(forsale)) / double (no(forsale) + 1) + double(newObject.getPrice()) / double (no(forsale) + 1);
		no(forsale)++;
	} else {
		wt(forsale) = wt(forsale) * double(no(forsale)) / double (no(forsale) + 1) + double(0)                  / double (no(forsale) + 1);
		wt(bought)  = wt(bought)  * double(no(bought))  / double (no(bought)  + 1) + double(newObject.getAge()) / double (no(bought)  + 1);
		pr(bought) = pr(bought) * double(no(bought)) / double (no(bought) + 1) + double(newObject.getOriginalPrice()) / double (no(bought) + 1);
		dpr(bought) = dpr(bought) * double(no(bought)) / double (no(bought) + 1) + double(newObject.getPrice()) / double (no(bought) + 1);
		no(bought)++;
	}
	agentInfo.numberOfObjects--;
}

void Agent::handleTimeoutObject(Object newObject) {
	if(newObject.getStatus() == FORSALE) {
		wt(forsale) = wt(forsale) * double(no(forsale)) / double (no(forsale) + 1) + double(newObject.getAge()) / double (no(forsale) + 1);
	} else {
		wt(bought) = wt(bought) * double(no(bought)) / double (no(bought) + 1) + double(newObject.getAge()) / double (no(bought) + 1);
	}
	agentInfo.numberOfObjects--;
}

void Agent::printAgentInfo() {
	printf("Agent info:\n");
	printAgentType();
	printf("Agent id = %d\n", agentInfo.agentId);
	printf("Number of objects owned = %d\n", agentInfo.numberOfObjects);
	printf("Number of objects sold  /generated = %d/%d (%.2f%%)\n", no(forsale), agentStatistics.forsale.numberOfObjectsGenerated, (agentStatistics.forsale.numberOfObjectsGenerated != 0)?100 * double(no(forsale)) / double(agentStatistics.forsale.numberOfObjectsGenerated):0);
	printf("Number of objects bought/generated = %d/%d (%.2f%%)\n", no(bought),  agentStatistics.bought.numberOfObjectsGenerated,  (agentStatistics.bought.numberOfObjectsGenerated  != 0)?100 * double(no(bought))  / double(agentStatistics.bought.numberOfObjectsGenerated):0);
	printf("Average waiting time sold = %.2f\n", wt(forsale));
	printf("Average waiting time bought = %.2f\n", wt(bought));
	printf("Average start/sold   price = %.2f/%.2f\n", pr(forsale), dpr(forsale));
	printf("Average start/bought price = %.2f/%.2f\n", pr(bought), dpr(bought));
	printf("\n");
}

#undef wt
#undef no
#undef pr

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

bool OrdinaryAgent::readyToGenerateSeller(int type) {
	if(timeLeftBeforeNewSellingObject[type] <= 0) {return true;}
	else {return false;}
}

bool OrdinaryAgent::readyToGenerateBuyer(int type) {
	if(timeLeftBeforeNewObjectBought[type] <= 0) {return true;}
	else {return false;}
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

bool SoloObjectSellingAgent::readyToGenerateSeller(int type) {
	if(agentInfo.numberOfObjects == 0) {return true;}
	else {return false;}
}

bool SoloObjectSellingAgent::readyToGenerateBuyer(int type) {
	return false;
}

double SoloObjectSellingAgent::formSellingPrice(int type) {
	return getNormallyDistributedValue(configurator->getSellersMean(type), 0);
}

double SoloObjectSellingAgent::formBuyingPrice(int type) {
	return -1;
}

void SoloObjectSellingAgent::resetSellingTimer(int type) {
	return;
}

void SoloObjectSellingAgent::resetBuyingTimer(int type) {
	return;
}

void SoloObjectSellingAgent::printAgentType() {
	printf("Solo object selling agent\n");
}

void SoloObjectSellingAgent::additionalTickActions() {
}

/**********************************************************************
					End of solo object selling agent
**********************************************************************/