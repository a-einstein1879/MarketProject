#include "configurator.h"

Configurator* Configurator::p_Configurator = 0;

Configurator::Configurator() {
	readConfiguration();
}

Configurator* Configurator::getConfigurator() {
	if(!p_Configurator) {
		p_Configurator = new Configurator();
	}
	return p_Configurator;
}

void Configurator::readConfiguration() {
	std::string line;
	std::ifstream configurationFile(CONFIGURATIONFILE);

	while(!configurationFile.eof()) {
		getline(configurationFile, line, '\n');
		handleConfigurationFileLine(line);
	}
	configurationFile.close();
}

/* BAD PART. NEEDS REFACTORING */
void Configurator::handleConfigurationFileLine(std::string line) {
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

double Configurator::getNumberFromString(std::string line) {
	bool found = false;
	bool dot = false;
	int numberOfDigitsAfterDot = 0;
    double number = 0;

    for (std::size_t i = 0; i < line.length(); i++) {
        const char ch = line[i];
        if (ch >= '0' && ch <= '9' || ch == '.') {
			if(ch == '.') {dot = true; numberOfDigitsAfterDot++; continue;}
            const int digit = ch - '0';
			if(dot == false)	{number = number * 10 + digit;}
			else				{number = number + double(digit) / pow(10.0, numberOfDigitsAfterDot); numberOfDigitsAfterDot++;}
            found = true;
        }
    }
	return number;

}
/* TODO: improve this part of code. Need some kind of hash */

void Configurator::defineVariable(int index, double value) {
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
	case 19:
		pictureRefreshFrequency = int(value);
		break;
	case 20:
		pictureDelayTime = int(value);
		break;
	case 21:
		numberOfPockets = int(value);
		break;
	case 22:
		graphicalMode = int(value);
		break;
	case 23:
		constantBoardersMode = int(value);
		break;
	case 24:
		minimumHistogramArgument = value;
		break;
	case 25:
		maximumHistogramArgument = value;
		break;
	case 26:
		sideGap = value;
		break;
	case 27:
		sellerPriceReduceAge = int(value);
		break;
	case 28:
		sellerPriceReduceShare = value;
		break;
	case 29:
		buyerPriceIncreaseAge = int(value);
		break;
	case 30:
		buyerPriceIncreaseShare = value;
		break;
	case 31:
		numberOfObjectTypes = int(value);
		break;
	}
}

void Configurator::printConfiguration() {
	std::cout << "Modeling time\t\t\t" << modelingTime << std::endl;
	std::cout << "Timer printing frequency\t" << timerPrintingFrequency << std::endl;
	std::cout << "Accuracy\t\t\t" << accuracy << std::endl;
	std::cout << "Number of object types\t\t\t" << numberOfObjectTypes << std::endl;

	std::cout << std::endl;

	std::cout << "Seller prices mode\t\t" << sellerPricesMode << std::endl;
	std::cout << "Maximum seller price\t\t" << maximumSellersPrice << std::endl;
	std::cout << "Minimum seller price\t\t" << minimumSellersPrice << std::endl;
	std::cout << "Seller mean price\t\t" << sellersMean << std::endl;
	std::cout << "Seller standart deviation\t" << sellersStandartDeviation << std::endl;
	std::cout << "Seller timer mode\t\t" << sellerTimersMode << std::endl;
	std::cout << "Seller frequency\t\t" << sellersFrequency << std::endl;
	std::cout << "Seller lambda\t\t\t" << sellersLambda << std::endl;

	std::cout << std::endl;

	std::cout << "Buyer prices mode\t\t" << buyerPricesMode << std::endl;
	std::cout << "Maximum buyer price\t\t" << maximumBuyersPrice << std::endl;
	std::cout << "Minimum buyer price\t\t" << minimumBuyersPrice << std::endl;
	std::cout << "Buyer mean price\t\t" << buyersMean << std::endl;
	std::cout << "Buyer standart deviation\t" << buyersStandartDeviation << std::endl;
	std::cout << "Buyer timer mode\t\t" << buyerTimersMode << std::endl;
	std::cout << "Buyer frequency\t\t\t" << buyersFrequency << std::endl;
	std::cout << "Buyer lambda\t\t\t" << buyersLambda << std::endl;

	std::cout << std::endl;
	
	std::cout << "Graphical mode\t\t\t" << ((graphicalMode == 1)?"On":"Off") << std::endl;
	std::cout << "Picture refresh frequency\t" << pictureRefreshFrequency << std::endl;
	std::cout << "Delay time\t\t\t" << pictureDelayTime << std::endl;
	std::cout << "Number of pockets\t\t" << numberOfPockets << std::endl;
	std::cout << "Histogram sidegap\t\t" << sideGap << std::endl;
	std::cout << "Constant boarders mode\t\t" << constantBoardersMode << std::endl;
	std::cout << "Minimum histogram argument\t" << minimumHistogramArgument << std::endl;
	std::cout << "Maximum histogram argument\t" << maximumHistogramArgument << std::endl;

	std::cout << std::endl;

	std::cout << "Seller price reduce age\t\t" << sellerPriceReduceAge << std::endl;
	std::cout << "Seller price reduce share\t" << sellerPriceReduceShare << std::endl;
	std::cout << "Buyer price increase age\t" << buyerPriceIncreaseAge << std::endl;
	std::cout << "Buyer price increase share\t" << buyerPriceIncreaseShare << std::endl;
	
	std::cout << std::endl;
}

/* END OF BAD PART */