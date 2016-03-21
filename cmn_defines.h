#ifndef CMN_DEFINES_H
#define CMN_DEFINES_H

#include <fstream>
#include <iostream>
#include <string>

#define CONFIGURATIONFILE "configuration.txt"
#define CONFIGURATIONEXPRESSIONS "Seller prices mode", "Maximum seller price", "Minimum seller price", "Seller mean price", "Seller standart deviation",\
"Seller timer mode", "Seller frequency", "Seller lambda", "Buyer prices mode", "Maximum buyer price", "Minimum buyer price", "Buyer mean price", "Buyer standart deviation",\
"Buyer timer mode", "Buyer frequency", "Buyer lambda", "Modeling time"

class Cmn_Defines {
private:
	int modelingTime;

	/* 0 - uniform, 1 - normal distribution */
	int sellerPricesMode;
	double maximumSellersPrice;
	double minimumSellersPrice;

	double sellersMean;
	double sellersStandartDeviation;

	int buyerPricesMode;
	double maximumBuyersPrice;
	double minimumBuyersPrice;

	double buyersMean;
	double buyersStandartDeviation;

	/* 0 - uniform, 1 - exponencial */
	int sellerTimersMode;
	int sellersFrequency;
	double sellersLambda;

	int buyerTimersMode;
	int buyersFrequency;
	double buyersLambda;

	Cmn_Defines();
	~Cmn_Defines();
	static Cmn_Defines *p_Cmn_Defines;
	
	void readConfiguration();
	void handleConfigurationFileLine(std::string line);
	double getNumberFromString(std::string line);
	void defineVariable(int index, double value);
public:
	static Cmn_Defines* getCmn_Defines();
	void printConfiguration();

	int getModelingTime();
	int getSellerPricesMode();
	double getMaximumSellersPrice();
	double getMinimumSellersPrice();

	double getSellersMean();
	double getSellersStandartDeviation();

	int getBuyerPricesMode();
	double getMaximumBuyersPrice();
	double getMinimumBuyersPrice();

	double getBuyersMean();
	double getBuyersStandartDeviation();

	int getSellerTimersMode();
	int getSellersFrequency();
	double getSellersLambda();

	int getBuyerTimersMode();
	int getBuyersFrequency();
	double getBuyersLambda();
};

#endif

/***********************************************************
							OUTPUT
***********************************************************/

#define DEALFILE		"output/dealsOutput.txt"
#define SELLERSFILE		"output/sellersOutput.txt"
#define BUYERSFILE		"output/buyersOutput.txt"

#define BUYERSFINALPRICESFILE	"output/buyersFinalPrices.txt"
#define BUYERSFINALTIMERSFILE	"output/buyersFinalTimers.txt"

#define SELLERSFINALPRICESFILE	"output/sellersFinalPrices.txt"
#define SELLERSFINALTIMERSFILE	"output/sellersFinalTimers.txt"
#define SILENTMODE

#define TIMERPRINTINGFREQUENCY 1000

/***********************************************************
						END OF THE OUTPUT
***********************************************************/

/**********************************************************************
							Market forming
**********************************************************************/

#define MODELINGTIME			10

#define MINIMUMSELLINGPRICE		10
#define MAXIMUMSELLINGPRICE		25

#define MINIMUMBUYINGPRICE		5
#define MAXIMUMBUYINGPRICE		15

#define SELLERSLAMBDA			0.2
#define BUYERSLAMBDA			0.6

/* Accuracy of random in forming of distributions */
#define ACCURACY 100

/**********************************************************************
						End of market forming
**********************************************************************/