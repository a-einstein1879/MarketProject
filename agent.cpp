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
	object.setAgentStrategy(getAgentStrategy(FORSALE));
	return object;
}

Object Agent::getBuyer() {
	int rnd = rand()%numberOfObjectTypes;
	Object object(formBuyingPrice(rnd), timer, BOUGHT, rnd);
	object.setAgentId(agentInfo.agentId);
	object.setAgentStrategy(getAgentStrategy(BOUGHT));
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


#define sellers		agentConfiguration.sellers
#define buyers		agentConfiguration.buyers

void Agent::allocateAgentInfoMemory() {
	buyers.timerMode = new int[numberOfObjectTypes];
	buyers.timerFrequency = new int[numberOfObjectTypes];
	buyers.timerLambda = new double[numberOfObjectTypes];
	
	buyers.priceMode = new int[numberOfObjectTypes];
	buyers.minimumPrice = new double[numberOfObjectTypes];
	buyers.maximumPrice = new double[numberOfObjectTypes];
	buyers.mean = new double[numberOfObjectTypes];
	buyers.standartDeviation = new double[numberOfObjectTypes];
	
	sellers.timerMode = new int[numberOfObjectTypes];
	sellers.timerFrequency = new int[numberOfObjectTypes];
	sellers.timerLambda = new double[numberOfObjectTypes];

	sellers.priceMode = new int[numberOfObjectTypes];
	sellers.minimumPrice = new double[numberOfObjectTypes];
	sellers.maximumPrice = new double[numberOfObjectTypes];
	sellers.mean = new double[numberOfObjectTypes];
	sellers.standartDeviation = new double[numberOfObjectTypes];
}

void Agent::freeAgentInfoMemory() {
	delete [] buyers.timerMode;
	delete [] buyers.timerFrequency;
	delete [] buyers.timerLambda;
	
	delete [] buyers.priceMode;
	delete [] buyers.minimumPrice;
	delete [] buyers.maximumPrice;
	delete [] buyers.mean;
	delete [] buyers.standartDeviation;
	
	delete [] sellers.timerMode;
	delete [] sellers.timerFrequency;
	delete [] sellers.timerLambda;

	delete [] sellers.priceMode;
	delete [] sellers.minimumPrice;
	delete [] sellers.maximumPrice;
	delete [] sellers.mean;
	delete [] sellers.standartDeviation;
}

#undef sellers
#undef buyers

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
	setAgentConfiguration();
	for(int i = 0; i < numberOfObjectTypes; i++) {
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

#define sellers		agentConfiguration.sellers
#define buyers		agentConfiguration.buyers

double OrdinaryAgent::formSellingPrice(int type) {
	switch(sellers.priceMode[type]) {
		case 1:
			return getNormallyDistributedValue(sellers.mean[type], sellers.standartDeviation[type]);
		case 0:
			return sellers.minimumPrice[type] + rand()%int(sellers.maximumPrice[type] - sellers.minimumPrice[type] + 1);
	}
	return -1;
}

double OrdinaryAgent::formBuyingPrice(int type) {
	switch(buyers.priceMode[type]) {
		case 1:
			return getNormallyDistributedValue(buyers.mean[type], buyers.standartDeviation[type]);
		case 0:
			return buyers.minimumPrice[type] + rand()%int(buyers.maximumPrice[type] - buyers.minimumPrice[type] + 1);
	}
	return -1;
}

void OrdinaryAgent::resetSellingTimer(int type) {
	switch(sellers.timerMode[type]) {
		case 1:
			timeLeftBeforeNewSellingObject[type] = int(getExponentiallyDistributedValue(sellers.timerLambda[type]));
			return;
		case 0:
			timeLeftBeforeNewSellingObject[type] = sellers.timerFrequency[type];
			return;
	}
}

void OrdinaryAgent::resetBuyingTimer(int type) {
	switch(buyers.timerMode[type]) {
		case 1:
			timeLeftBeforeNewObjectBought[type] = int(getExponentiallyDistributedValue(buyers.timerLambda[type]));
			return;
		case 0:
			timeLeftBeforeNewObjectBought[type] = buyers.timerFrequency[type];
			return;
	}
}

#define conf		agentConfiguration
#define c			configurator->

void OrdinaryAgent::setAgentConfiguration() {
	for(int i = 0; i < numberOfObjectTypes; i++) {
		buyers.timerMode[i]			= c getBuyerTimersMode(i);
		buyers.timerFrequency[i]	= c getBuyersFrequency(i);
		buyers.timerLambda[i]		= c getBuyersLambda(i);

		buyers.priceMode[i]			= c getBuyerPricesMode(i);
		buyers.minimumPrice[i]		= c getMinimumBuyersPrice(i);
		buyers.maximumPrice[i]		= c getMaximumBuyersPrice(i);
		buyers.mean[i]				= c getBuyersMean(i);
		buyers.standartDeviation[i] = c getBuyersStandartDeviation(i);

		sellers.timerMode[i]		= c getSellerTimersMode(i);
		sellers.timerFrequency[i]	= c getSellersFrequency(i);
		sellers.timerLambda[i]		= c getSellersLambda(i);

		sellers.priceMode[i]		= c getSellerPricesMode(i);
		sellers.minimumPrice[i]		= c getMinimumSellersPrice(i);
		sellers.maximumPrice[i]		= c getMaximumSellersPrice(i);
		sellers.mean[i]				= c getSellersMean(i);
		sellers.standartDeviation[i] = c getSellersStandartDeviation(i);
	}

	conf.numberOfPossiblePriceAdaptations	= c getNumberOfPriceAdaptations();
	conf.amountOfPriceReduction				= c getSellerPriceReduceShare();
	conf.timeOfPriceReduction				= c getSellerPriceReduceAge();
	conf.sellerTimeOnMarket					= c getSellerTimeOnMarket();
	conf.amountOfPriceMagnification			= c getBuyerPriceIncreaseShare();
	conf.timeOfPriceMagnification			= c getBuyerPriceIncreaseAge();
	conf.buyerTimeOnMarket					= c getBuyerTimeOnMarket();
}

void OrdinaryAgent::printAgentType() {
	printf("Ordinary agent\n");
}

AgentStrategy OrdinaryAgent::getAgentStrategy(int status) {
	AgentStrategy strategy;
	strategy.priceAdaptationPossible = true;
	strategy.numberOfPossiblePriceAdaptations = conf.numberOfPossiblePriceAdaptations;
	if(status == FORSALE) {
		strategy.priceAdaptationShare = conf.amountOfPriceReduction;
		strategy.priceAdaptationTimer = conf.timeOfPriceReduction;
		strategy.possibleTimeOnMarket = conf.sellerTimeOnMarket;
	}
	if(status == BOUGHT) {
		strategy.priceAdaptationShare = conf.amountOfPriceMagnification;
		strategy.priceAdaptationTimer = conf.timeOfPriceMagnification;
		strategy.possibleTimeOnMarket = conf.buyerTimeOnMarket;
	}
	return strategy;
}

#undef sellers
#undef buyers
#undef conf
#undef c

/**********************************************************************
						End of ordinary agent
**********************************************************************/


/**********************************************************************
					Solo object selling agent
**********************************************************************/

SoloObjectSellingAgent::SoloObjectSellingAgent() {
	setAgentConfiguration();
	for(int i = 0; i < numberOfObjectTypes; i++) {
		resetSellingTimer(i);
		resetBuyingTimer(i);
	}
	generationFrequency = configurator->getModelingTime() / 20;
	lastObjectCreationTime = -INT_MAX;
}

bool SoloObjectSellingAgent::readyToGenerateSeller(int type) {
	if(agentInfo.numberOfObjects == 0 && lastObjectCreationTime - generationFrequency >= 0) {lastObjectCreationTime = 0; return true;}
	else {lastObjectCreationTime++; return false;}
}

bool SoloObjectSellingAgent::readyToGenerateBuyer(int type) {
	return false;
}

double SoloObjectSellingAgent::formSellingPrice(int type) {
	return getNormallyDistributedValue(configurator->getSoloSellingAgentPrice(), 0);
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

void SoloObjectSellingAgent::setAgentConfiguration() {}

void SoloObjectSellingAgent::printAgentType() {
	printf("Solo object selling agent\n");
}

AgentStrategy SoloObjectSellingAgent::getAgentStrategy(int status) {
	AgentStrategy strategy;
	strategy.priceAdaptationPossible = false;
	strategy.possibleTimeOnMarket = generationFrequency;
	return strategy;
}

/**********************************************************************
					End of solo object selling agent
**********************************************************************/