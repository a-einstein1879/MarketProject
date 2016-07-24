#include "configurator.h"

Configurator* Configurator::p_Configurator = 0;

Configurator::Configurator() {
	numberOfObjectTypes = -1;
	readGeneralConfiguration();
}

Configurator::~Configurator() {
	if(numberOfObjectTypes != -1) {freeMemory();}
}

void Configurator::allocateMemory() {
	if(numberOfObjectTypes <= 0) {return;}
	/* Seller prices */
	sellerPricesMode		= new int[numberOfObjectTypes];
	maximumSellersPrice		= new double[numberOfObjectTypes];
	minimumSellersPrice		= new double[numberOfObjectTypes];

	sellersMean				= new double[numberOfObjectTypes];
	sellersStandartDeviation= new double[numberOfObjectTypes];

	/* Buyer prices */
	buyerPricesMode			= new int[numberOfObjectTypes];
	maximumBuyersPrice		= new double[numberOfObjectTypes];
	minimumBuyersPrice		= new double[numberOfObjectTypes];

	buyersMean				= new double[numberOfObjectTypes];
	buyersStandartDeviation = new double[numberOfObjectTypes];

	/* Seller timers */
	sellerTimersMode	= new int[numberOfObjectTypes];
	sellersFrequency	= new int[numberOfObjectTypes];
	sellersLambda		= new double[numberOfObjectTypes];

	/* Buyer timers */
	buyerTimersMode		= new int[numberOfObjectTypes];
	buyersFrequency		= new int[numberOfObjectTypes];
	buyersLambda		= new double[numberOfObjectTypes];
}

void Configurator::freeMemory() {
	if(numberOfObjectTypes <= 0) {return;}
	/* Seller prices */
	delete [] sellerPricesMode;
	delete [] maximumSellersPrice;
	delete [] minimumSellersPrice;

	delete [] sellersMean;
	delete [] sellersStandartDeviation;

	/* Buyer prices */
	delete [] buyerPricesMode;
	delete [] maximumBuyersPrice;
	delete [] minimumBuyersPrice;

	delete [] buyersMean;
	delete [] buyersStandartDeviation;

	/* Seller timers */
	delete [] sellerTimersMode;
	delete [] sellersFrequency;
	delete [] sellersLambda;

	/* Buyer timers */
	delete [] buyerTimersMode;
	delete [] buyersFrequency;
	delete [] buyersLambda;
}

Configurator* Configurator::getConfigurator() {
	if(!p_Configurator) {
		p_Configurator = new Configurator();
	}
	return p_Configurator;
}

void Configurator::readGeneralConfiguration() {
	std::string line;
	std::ifstream configurationFile(CONFIGURATIONFILE);

	while(!configurationFile.eof()) {
		getline(configurationFile, line, '\n');
		handleConfigurationFileLine(line);
	}

	configurationFile.close();
}

void Configurator::readObjectSpecificConfiguration() {
	std::string line;
	if(numberOfObjectTypes <= 0) {return;}
	for(int i = 0; i < numberOfObjectTypes; i++) {
		std::ifstream configurationFile(TYPEFOLDER + std::to_string(i + 1) + ".txt");
	
		while(!configurationFile.eof()) {
			getline(configurationFile, line, '\n');
			handleConfigurationFileLine(line, i);
		}

		configurationFile.close();
	}
}

/* BAD PART. NEEDS REFACTORING */
void Configurator::handleConfigurationFileLine(std::string line, int type) {
	std::string configurationExpressions[] = {CONFIGURATIONEXPRESSIONS};
	int numberOfExpressions = sizeof(configurationExpressions) / sizeof(std::string);
	double value = -1;
	for(int i = 0; i < numberOfExpressions; i++) {
		std::string toFind(configurationExpressions[i]);
		std::size_t found = line.find(toFind);
		if (found != std::string::npos) {
			value = getNumberFromString(line);
			defineVariable(i, value, type);
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

void Configurator::defineVariable(int index, double value, int type) {
	switch(index) {
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
	case 31: {
		numberOfObjectTypes = int(value);
		allocateMemory();
		readObjectSpecificConfiguration();
		break;
			 }
	case 32:
		numberOfPriceAdaptations = int(value);
		break;
	case 33:
		sellerTimeOnMarket = int(value);
		break;
	case 34:
		buyerTimeOnMarket = int(value);
		break;
	case 35:
		soloSellingAgentPrice = value;
		break;
	default:
		break;
	}

	
	if(type < 0) {return;}
	switch(index) {
	case 0:
		sellerPricesMode[type] = int(value);
		break;
	case 1:
		maximumSellersPrice[type] = value;
		break;
	case 2:
		minimumSellersPrice[type] = value;
		break;
	case 3:
		sellersMean[type] = value;
		break;
	case 4:
		sellersStandartDeviation[type] = value;
		break;
	case 5:
		sellerTimersMode[type] = int(value);
		break;
	case 6:
		sellersFrequency[type] = int(value);
		break;
	case 7:
		sellersLambda[type] = value;
		break;
	case 8:
		buyerPricesMode[type] = int(value);
		break;
	case 9:
		maximumBuyersPrice[type] = value;
		break;
	case 10:
		minimumBuyersPrice[type] = value;
		break;
	case 11:
		buyersMean[type] = value;
		break;
	case 12:
		buyersStandartDeviation[type] = value;
		break;
	case 13:
		buyerTimersMode[type] = int(value);
		break;
	case 14:
		buyersFrequency[type] = int(value);
		break;
	case 15:
		buyersLambda[type] = value;
		break;
	default:
		break;
	}
}

void Configurator::printConfiguration() {
	std::cout << "Modeling time\t\t\t" << modelingTime << std::endl;
	std::cout << "Timer printing frequency\t" << timerPrintingFrequency << std::endl;
	std::cout << "Accuracy\t\t\t" << accuracy << std::endl;
	std::cout << "Number of object types\t\t" << numberOfObjectTypes << std::endl;

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
	
	for(int i = 0; i < numberOfObjectTypes; i++) {

		std::cout << "Type " << i << ":" << std::endl;

		std::cout << "Seller prices mode\t\t"		<< sellerPricesMode[i] << std::endl;
		std::cout << "Maximum seller price\t\t"		<< maximumSellersPrice[i] << std::endl;
		std::cout << "Minimum seller price\t\t"		<< minimumSellersPrice[i] << std::endl;
		std::cout << "Seller mean price\t\t"		<< sellersMean[i] << std::endl;
		std::cout << "Seller standart deviation\t"	<< sellersStandartDeviation[i] << std::endl;
		std::cout << "Seller timer mode\t\t"		<< sellerTimersMode[i] << std::endl;
		std::cout << "Seller frequency\t\t"			<< sellersFrequency[i] << std::endl;
		std::cout << "Seller lambda\t\t\t"			<< sellersLambda[i] << std::endl;

		std::cout << std::endl;

		std::cout << "Buyer prices mode\t\t"		<< buyerPricesMode[i] << std::endl;
		std::cout << "Maximum buyer price\t\t"		<< maximumBuyersPrice[i] << std::endl;
		std::cout << "Minimum buyer price\t\t"		<< minimumBuyersPrice[i] << std::endl;
		std::cout << "Buyer mean price\t\t"			<< buyersMean[i] << std::endl;
		std::cout << "Buyer standart deviation\t"	<< buyersStandartDeviation[i] << std::endl;
		std::cout << "Buyer timer mode\t\t"			<< buyerTimersMode[i] << std::endl;
		std::cout << "Buyer frequency\t\t\t"		<< buyersFrequency[i] << std::endl;
		std::cout << "Buyer lambda\t\t\t"			<< buyersLambda[i] << std::endl;

		std::cout << std::endl;
	}
	
	std::cout << "Seller price reduce age\t\t"	<< sellerPriceReduceAge << std::endl;
	std::cout << "Seller price reduce share\t"	<< sellerPriceReduceShare << std::endl;
	std::cout << "Seller time on market\t\t"	<< sellerTimeOnMarket << std::endl;
	std::cout << "Buyer price increase age\t"	<< buyerPriceIncreaseAge << std::endl;
	std::cout << "Buyer price increase share\t" << buyerPriceIncreaseShare << std::endl;
	std::cout << "Buyer time on market\t\t"		<< buyerTimeOnMarket << std::endl;
	
	std::cout << std::endl;

	system("pause");
}

/* END OF BAD PART */