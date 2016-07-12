#include "object.h"

Object::Object() {
	generalProperties.price		= 0;
	generalProperties.status	= 0;
	generalProperties.type		= 0;

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

void Object::setFiles(FILE *BuyersFinalPricesFile, FILE *BuyersFinalTimersFile, FILE *SellersFinalPricesFile, FILE *SellersFinalTimersFile) {
	buyersFinalPricesFile = BuyersFinalPricesFile;
	buyersFinalTimersFile = BuyersFinalTimersFile;
	sellersFinalPricesFile = SellersFinalPricesFile;
	sellersFinalTimersFile = SellersFinalTimersFile;
}

void Object::tick() {
	timers.timer++;
	timers.age++;
	timers.timeAfterPriceReduction++;
}

void Object::printObject() {
	printf("price = %.2f, age = %.2f, number of price reductions = %d status = \"%s\"\n", getPrice(), getAge(), agentStrategy.numberOfPriceAdaptations, getStatus()?"For sale":"Bought");
}

/* TODO: there is a problem with timers files. We will have creation times there instead of time spent on market. Don`t want to add timer as an argument, because objects don`t need to know anything about timers */
void Object::printObjectToFinalFiles() {
	if(generalProperties.status == BOUGHT) {
		fprintf(buyersFinalPricesFile, "%.2f\n", generalProperties.price);
		fprintf(buyersFinalTimersFile, "%.2f\n", timers.age);
	} else {
		fprintf(sellersFinalPricesFile, "%.2f\n", generalProperties.price);
		fprintf(sellersFinalTimersFile, "%.2f\n", timers.age);
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

ObjectGeneralProperties& ObjectGeneralProperties::operator=(const ObjectGeneralProperties& generalProperties) {
	price	= generalProperties.price;
	status	= generalProperties.status;
	type	= generalProperties.type;
	return *this;
}

AgentStrategy& AgentStrategy::operator=(const AgentStrategy& agentStrategy) {
	numberOfPossiblePriceAdaptations	= agentStrategy.numberOfPossiblePriceAdaptations;
	numberOfPriceAdaptations			= agentStrategy.numberOfPriceAdaptations;
	sellerPriceReduceShare				= agentStrategy.sellerPriceReduceShare;
	buyerPriceReduceShare				= agentStrategy.buyerPriceReduceShare;
	return *this;
}

Timers& Timers::operator=(const Timers& timers) {
	age						= timers.age;
	timeAfterPriceReduction	= timers.timeAfterPriceReduction;
	timer					= timers.timer;
	return *this;
}

Object& Object::operator=(Object &object) {
	generalProperties	= object.getGeneralProperties();
	agentStrategy		= object.getAgentStrategy();
	timers				= object.getTimers();

	buyersFinalPricesFile = object.getBuyersFinalPricesFile();
	buyersFinalTimersFile = object.getBuyersFinalTimersFile();
	sellersFinalPricesFile = object.getSellersFinalPricesFile();
	sellersFinalTimersFile = object.getSellersFinalTimersFile();
	return *this;
}

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

void Object::pushPriceToHistogram(Histogram &histogram) {
	histogram.addValueToTmpIndex(generalProperties.price);
}

void Object::pushPriceToChart(LineChart &lineChart) {
	lineChart.addNextValue(generalProperties.price);
}