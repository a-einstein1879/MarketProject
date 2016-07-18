#include "object.h"

Object::Object() {
	generalProperties.price		= 0;
	generalProperties.status	= 0;
	generalProperties.type		= -1;

	agentStrategy.numberOfPriceAdaptations = 0;
	
	timers.timer					= 1;
	timers.age						= -1;
	timers.timeAfterPriceReduction	= -1;

	configurator = configurator->getConfigurator();
}

Object::Object(double Price, double CreationTime, bool Status, int Type) {
	configurator = configurator->getConfigurator();
	timers.timer = int(CreationTime);
	generalProperties.type = Type;
	setObject(Price, 0, Status);
}

void Object::setObject(double Price, double Age, bool Status) {
	generalProperties.price					= Price;
	generalProperties.status				= Status;
	agentStrategy.numberOfPriceAdaptations	= 0;
	timers.age								= Age;
	timers.timeAfterPriceReduction			= 0;
}

void Object::setFiles(FILE *buyersFinalPricesFile, FILE *buyersFinalTimersFile, FILE *sellersFinalPricesFile, FILE *sellersFinalTimersFile) {
	outputFiles[0] = buyersFinalPricesFile;
	outputFiles[1] = buyersFinalTimersFile;
	outputFiles[2] = sellersFinalPricesFile;
	outputFiles[3] = sellersFinalTimersFile;
}

void Object::setAgentId(int id) {
	agentId.agentId = id;
}

void Object::tick() {
	timers++;
}

void Object::printObject() {
	printf("price = %.2f, age = %.2f, number of price reductions = %d status = \"%s\"\n", getPrice(), getAge(), agentStrategy.numberOfPriceAdaptations, getStatus()?"For sale":"Bought");
}

/* TODO: there is a problem with timers files. We will have creation times there instead of time spent on market. Don`t want to add timer as an argument, because objects don`t need to know anything about timers */
void Object::printObjectToFinalFiles() {
	if(generalProperties.status == BOUGHT) {
		fprintf(outputFiles[0], "%.2f\n", generalProperties.price);
		fprintf(outputFiles[1], "%.2f\n", timers.age);
	} else {
		fprintf(outputFiles[2], "%.2f\n", generalProperties.price);
		fprintf(outputFiles[3], "%.2f\n", timers.age);
	}
}

bool Object::adaptPrice() {
	if(agentStrategy.numberOfPriceAdaptations++ >= configurator->getNumberOfPriceAdaptations()) {return 1;}
	if(generalProperties.status == FORSALE) {
		generalProperties.price = (1 - configurator->getSellerPriceReduceShare())  * generalProperties.price;
	} else {
		generalProperties.price = (1 + configurator->getBuyerPriceIncreaseShare()) * generalProperties.price;
	}
	timers.timeAfterPriceReduction = 0;
	return 0;
}

/**********************************************************************
							= and ++ operations
**********************************************************************/
ObjectGeneralProperties& ObjectGeneralProperties::operator=(const ObjectGeneralProperties& generalProperties) {
	price	= generalProperties.price;
	status	= generalProperties.status;
	type	= generalProperties.type;
	return *this;
}

Timers& Timers::operator=(const Timers& timers) {
	age						= timers.age;
	timeAfterPriceReduction	= timers.timeAfterPriceReduction;
	timer					= timers.timer;
	return *this;
}

Timers Timers::operator++(int) {
	timer++;
	age++;
	timeAfterPriceReduction++;
	return *this;
}

AgentIdProperties& AgentIdProperties::operator=(const AgentIdProperties& agentIdProperties) {
	agentId		= agentIdProperties.agentId;
	objectId	= agentIdProperties.objectId;
	return *this;
}

AgentStrategy& AgentStrategy::operator=(const AgentStrategy& agentStrategy) {
	numberOfPossiblePriceAdaptations	= agentStrategy.numberOfPossiblePriceAdaptations;
	numberOfPriceAdaptations			= agentStrategy.numberOfPriceAdaptations;
	sellerPriceReduceShare				= agentStrategy.sellerPriceReduceShare;
	buyerPriceReduceShare				= agentStrategy.buyerPriceReduceShare;
	return *this;
}

Object& Object::operator=(Object &object) {
	generalProperties	= object.getGeneralProperties();
	agentStrategy		= object.getAgentStrategy();
	agentId				= object.getAgentIdProperties();
	timers				= object.getTimers();

	for(int i = 0; i < 4; i++) {
		outputFiles[i] = object.getOutputFile(i);
	}
	return *this;
}
/**********************************************************************
						End of = and ++ operations
**********************************************************************/

/**********************************************************************
						>, < and <=, >= operations
**********************************************************************/
bool Object::operator<=(Object &object) {
	if(generalProperties.type != object.getType()) {
		printf("Type mismatch\n");
	//	system("pause");
	}
	if(generalProperties.price <= object.getPrice()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator>=(Object &object) {
	if(generalProperties.type != object.getType()) {
		printf("Type mismatch\n");
	//	system("pause");
	}
	if(generalProperties.price >= object.getPrice()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator<(Object &object) {
	if(generalProperties.type != object.getType()) {
		printf("Type mismatch\n");
	//	system("pause");
	}
	if(timers.timeAfterPriceReduction <= object.getTimeAfterPriceReduction()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator>(Object &object) {
	if(generalProperties.type != object.getType()) {
		printf("Type mismatch\n");
	//	system("pause");
	}
	if(timers.timeAfterPriceReduction >= object.getTimeAfterPriceReduction()) {
		return true;
	} else {
		return false;
	}
}

/**********************************************************************
					End of >, < and <=, >= operations
**********************************************************************/

void Object::pushPriceToHistogram(Histogram &histogram) {
	histogram.addValueToTmpIndex(generalProperties.price);
}

void Object::pushPriceToChart(LineChart &lineChart) {
	lineChart.addNextValue(generalProperties.price);
}