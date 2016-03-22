#include "cmn_defines.h"

Cmn_Defines* Cmn_Defines::p_Cmn_Defines = 0;

Cmn_Defines::Cmn_Defines() {
	readConfiguration();
}

Cmn_Defines::~Cmn_Defines() {
}

void Cmn_Defines::readConfiguration() {
	std::string line;
	std::ifstream configurationFile(CONFIGURATIONFILE);

	while(!configurationFile.eof()) {
		getline(configurationFile, line, '\n');
		handleConfigurationFileLine(line);
	}
	configurationFile.close();
}

/* BAD PART. NEEDS REFACTORING */
void Cmn_Defines::handleConfigurationFileLine(std::string line) {
	std::string configurationExpressions[] = {CONFIGURATIONEXPRESSIONS};
	int numberOfExpressions = sizeof(configurationExpressions) / sizeof(std::string);
	double value = -1;
	for(int i = 0; i < numberOfExpressions; i++) {
		std::string toFind(configurationExpressions[i]);
		std::size_t found = line.find(toFind);
		if (found != std::string::npos) {
			value = getNumberFromString(line);
			defineVariable(i, value);
		}
	}
}

double Cmn_Defines::getNumberFromString(std::string line) {
	bool found = false;
	bool dot = false;
    double number = 0;

    for (std::size_t i = 0; i < line.length(); i++) {
        const char ch = line[i];
        if (ch >= '0' && ch <= '9' || ch == '.') {
			if(ch == '.') {dot = true; continue;}
            const int digit = ch - '0';
			if(dot == false)	{number = number * 10 + digit;}
			else				{number = number + double(digit) / 10;}
            found = true;
        }
    }
	return number;

}
/* TODO: improve this part of code. Need some kind of hash */

void Cmn_Defines::defineVariable(int index, double value) {
	switch(index) {
	case 0:
		sellerPricesMode = int(value);
		break;
	case 1:
		maximumSellersPrice = value;
		break;
	case 2:
		minimumSellersPrice = value;
		break;
	case 3:
		sellersMean = value;
		break;
	case 4:
		sellersStandartDeviation = value;
		break;
	case 5:
		sellerTimersMode = int(value);
		break;
	case 6:
		sellersFrequency = int(value);
		break;
	case 7:
		sellersLambda = value;
		break;
	case 8:
		buyerPricesMode = int(value);
		break;
	case 9:
		maximumBuyersPrice = value;
		break;
	case 10:
		minimumBuyersPrice = value;
		break;
	case 11:
		buyersMean = value;
		break;
	case 12:
		buyersStandartDeviation = value;
		break;
	case 13:
		buyerTimersMode = int(value);
		break;
	case 14:
		buyersFrequency = int(value);
		break;
	case 15:
		buyersLambda = value;
		break;
	case 16:
		modelingTime = int(value);
		break;
	case 17:
		timerPrintingFrequency = int(value);
		break;
	case 18:
		accuracy = int(value);
		break;
	}
}

void Cmn_Defines::printConfiguration() {
	std::cout << "Modeling time\t" << modelingTime << std::endl;
	std::cout << "Timer printing frequency\t" << timerPrintingFrequency << std::endl;
	std::cout << "Accuracy\t" << accuracy << std::endl;

	std::cout << "Seller prices mode\t" << sellerPricesMode << std::endl;
	std::cout << "Maximum seller price\t" << maximumSellersPrice << std::endl;
	std::cout << "Minimum seller price\t" << minimumSellersPrice << std::endl;
	std::cout << "Seller mean price\t" << sellersMean << std::endl;
	std::cout << "Seller standart deviation\t" << sellersStandartDeviation << std::endl;
	std::cout << "Seller timer mode\t" << sellerTimersMode << std::endl;
	std::cout << "Seller frequency\t" << sellersFrequency << std::endl;
	std::cout << "Seller lambda\t" << sellersLambda << std::endl;

	std::cout << std::endl;

	std::cout << "Buyer prices mode\t" << buyerPricesMode << std::endl;
	std::cout << "Maximum buyer price\t" << maximumBuyersPrice << std::endl;
	std::cout << "Minimum buyer price\t" << minimumBuyersPrice << std::endl;
	std::cout << "Buyer mean price\t" << buyersMean << std::endl;
	std::cout << "Buyer standart deviation\t" << buyersStandartDeviation << std::endl;
	std::cout << "Buyer timer mode\t" << buyerTimersMode << std::endl;
	std::cout << "Buyer frequency\t" << buyersFrequency << std::endl;
	std::cout << "Buyer lambda\t" << buyersLambda << std::endl;
}

/* END OF BAD PART */

Cmn_Defines* Cmn_Defines::getCmn_Defines() {
	if(!p_Cmn_Defines) {
		p_Cmn_Defines = new Cmn_Defines();
	}
	return p_Cmn_Defines;
}

int Cmn_Defines::getModelingTime() {
	return modelingTime;
}

int Cmn_Defines::getTimerPrintingFrequency() {
	return timerPrintingFrequency;
}

int Cmn_Defines::getAccuracy() {
	return accuracy;
}

int Cmn_Defines::getSellerPricesMode() {
	return sellerPricesMode;
}

double Cmn_Defines::getMaximumSellersPrice() {
	return maximumSellersPrice;
}

double Cmn_Defines::getMinimumSellersPrice() {
	return minimumSellersPrice;
}

double Cmn_Defines::getSellersMean() {
	return sellersMean;
}

double Cmn_Defines::getSellersStandartDeviation() {
	return sellersStandartDeviation;
}

int Cmn_Defines::getBuyerPricesMode() {
	return buyerPricesMode;
}

double Cmn_Defines::getMaximumBuyersPrice() {
	return maximumBuyersPrice;
}

double Cmn_Defines::getMinimumBuyersPrice() {
	return minimumBuyersPrice;
}

double Cmn_Defines::getBuyersMean() {
	return buyersMean;
}

double Cmn_Defines::getBuyersStandartDeviation() {
	return buyersStandartDeviation;
}

int Cmn_Defines::getSellerTimersMode() {
	return sellerTimersMode;
}

int Cmn_Defines::getSellersFrequency() {
	return sellersFrequency;
}

double Cmn_Defines::getSellersLambda() {
	return sellersLambda;
}

int Cmn_Defines::getBuyerTimersMode() {
	return buyerTimersMode;
}

int Cmn_Defines::getBuyersFrequency() {
	return buyersFrequency;
}

double Cmn_Defines::getBuyersLambda() {
	return buyersLambda;
}
