#ifndef CMN_DEFINES_H
#define CMN_DEFINES_H

#include <fstream>
#include <iostream>
#include <string>

#define CONFIGURATIONFILE "configuration/configuration.txt"
#define TYPEFOLDER "configuration/types/"
#define CONFIGURATIONEXPRESSIONS "Seller prices mode", "Maximum seller price", "Minimum seller price", "Seller mean price", "Seller standart deviation",\
"Seller timer mode", "Seller frequency", "Seller lambda", "Buyer prices mode", "Maximum buyer price", "Minimum buyer price", "Buyer mean price", "Buyer standart deviation",\
"Buyer timer mode", "Buyer frequency", "Buyer lambda", "Modeling time", "Timer printing frequency", "Accuracy", "Picture refresh frequency", "Delay time", "Number of pockets", \
"Graphical mode", "Constant boarders mode", "Minimum histogram argument", "Maximum histogram argument", "Histogram sidegap", \
"Seller price reduce age", "Seller price reduce share", "Buyer price increase age", "Buyer price increase share", "Number of object types"

class Configurator {
private:
	int modelingTime;
	int timerPrintingFrequency;
	int accuracy;

	/* Must be defined before memory allocation */
	int numberOfObjectTypes;
	void allocateMemory();
	void freeMemory();
	void readObjectSpecificConfiguration();

	/* 0 - uniform, 1 - normal distribution */
	int sellerPricesMode;
	double maximumSellersPrice;
	double minimumSellersPrice;

	double *sellersMean;
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

	/* Picture */
	int graphicalMode;
	int pictureRefreshFrequency;
	int pictureDelayTime;

	int numberOfPockets;
	double sideGap;
	int constantBoardersMode;
	double minimumHistogramArgument;
	double maximumHistogramArgument;

	/* Agent strategies */
	int sellerPriceReduceAge;
	double sellerPriceReduceShare;
	int buyerPriceIncreaseAge;
	double buyerPriceIncreaseShare;

	Configurator();
	~Configurator();
	static Configurator *p_Configurator;
	
	void readGeneralConfiguration();
	void handleConfigurationFileLine(std::string line, int type = -1);
	double getNumberFromString(std::string line);
	void defineVariable(int index, double value, int type = -1);
public:
	static Configurator* getConfigurator();
	void printConfiguration();
	
	/* Set */
	void setSellersLambda(double lambda)	{sellersLambda = lambda;}
	void setBuyersLambda(double lambda)		{buyersLambda = lambda;}
	/* End of set */

	/* Get */
	int getModelingTime()					{return modelingTime;}
	int getTimerPrintingFrequency()			{return timerPrintingFrequency;}
	int getAccuracy()						{return accuracy;}
	int getNumberOfObjectTypes()			{return numberOfObjectTypes;}

	int getSellerPricesMode()				{return sellerPricesMode;}
	double getMaximumSellersPrice()			{return maximumSellersPrice;}
	double getMinimumSellersPrice()			{return minimumSellersPrice;}

	double getSellersMean(int type)			{if(numberOfObjectTypes <= 0) {return -1;} else {return sellersMean[type];}}
	double getSellersStandartDeviation()	{return sellersStandartDeviation;}

	int getBuyerPricesMode()				{return buyerPricesMode;}
	double getMaximumBuyersPrice()			{return maximumBuyersPrice;}
	double getMinimumBuyersPrice()			{return minimumBuyersPrice;}

	double getBuyersMean()					{return buyersMean;}
	double getBuyersStandartDeviation()		{return buyersStandartDeviation;}

	int getSellerTimersMode()				{return sellerTimersMode;}
	int getSellersFrequency()				{return sellersFrequency;}
	double getSellersLambda()				{return sellersLambda;}

	int getBuyerTimersMode()				{return buyerTimersMode;}
	int getBuyersFrequency()				{return buyersFrequency;}
	double getBuyersLambda()				{return buyersLambda;}
	
	int getGraphicalMode()					{return graphicalMode;}
	int getPictureRefreshFrequency()		{return pictureRefreshFrequency;}
	int getPictureDelayTime()				{return pictureDelayTime;}

	int getNumberOfPockets()				{return numberOfPockets;}
	double getSideGap()						{return sideGap;}
	int getConstantBoardersMode()			{return constantBoardersMode;}
	double getMinimumHistogramArgument()	{return minimumHistogramArgument;}
	double getMaximumHistogramArgument()	{return maximumHistogramArgument;}

	int getSellerPriceReduceAge()			{return sellerPriceReduceAge;}
	double getSellerPriceReduceShare()		{return sellerPriceReduceShare;}
	int getBuyerPriceIncreaseAge()			{return buyerPriceIncreaseAge;}
	double getBuyerPriceIncreaseShare()		{return buyerPriceIncreaseShare;}
	/* End of get */
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

/***********************************************************
						END OF THE OUTPUT
***********************************************************/