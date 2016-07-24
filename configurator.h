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
"Seller price reduce age", "Seller price reduce share", "Buyer price increase age", "Buyer price increase share", "Number of object types", \
"Number of possible price adaptations", "Seller time on market", "Buyer time on market", "Solo selling agent price"

class Configurator {
private:
	int modelingTime;
	int timerPrintingFrequency;
	int accuracy;

	/* Picture */
	int graphicalMode;
	int pictureRefreshFrequency;
	int pictureDelayTime;

	int numberOfPockets;
	double sideGap;
	int constantBoardersMode;
	double minimumHistogramArgument;
	double maximumHistogramArgument;

	/* Must be defined before memory allocation */
	int numberOfObjectTypes;
	void allocateMemory();
	void freeMemory();
	void readObjectSpecificConfiguration();

	/* 0 - uniform, 1 - normal distribution */
	int *sellerPricesMode;
	double *maximumSellersPrice;
	double *minimumSellersPrice;

	double *sellersMean;
	double *sellersStandartDeviation;

	int *buyerPricesMode;
	double *maximumBuyersPrice;
	double *minimumBuyersPrice;

	double *buyersMean;
	double *buyersStandartDeviation;

	/* 0 - uniform, 1 - exponencial */
	int *sellerTimersMode;
	int *sellersFrequency;
	double *sellersLambda;

	int *buyerTimersMode;
	int *buyersFrequency;
	double *buyersLambda;

	/* Agent strategies */
	int numberOfPriceAdaptations;
	int sellerPriceReduceAge;
	double sellerPriceReduceShare;
	int sellerTimeOnMarket;
	int buyerPriceIncreaseAge;
	double buyerPriceIncreaseShare;
	int buyerTimeOnMarket;

	double soloSellingAgentPrice;

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
	void setSellersLambda(double lambda, int type)	{if(numberOfObjectTypes <= 0) {return;} else {sellersLambda[type] = lambda;}}
	void setBuyersLambda(double lambda, int type)	{if(numberOfObjectTypes <= 0) {return;} else {buyersLambda[type] = lambda;}}
	/* End of set */

	/* Get */
	int getModelingTime()					{return modelingTime;}
	int getTimerPrintingFrequency()			{return timerPrintingFrequency;}
	int getAccuracy()						{return accuracy;}
	int getNumberOfObjectTypes()			{return numberOfObjectTypes;}

	int getSellerPricesMode(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return sellerPricesMode[type];}}
	double getMaximumSellersPrice(int type)			{if(numberOfObjectTypes <= 0) {return -1;} else {return maximumSellersPrice[type];}}
	double getMinimumSellersPrice(int type)			{if(numberOfObjectTypes <= 0) {return -1;} else {return minimumSellersPrice[type];}}

	double getSellersMean(int type)					{if(numberOfObjectTypes <= 0) {return -1;} else {return sellersMean[type];}}
	double getSellersStandartDeviation(int type)	{if(numberOfObjectTypes <= 0) {return -1;} else {return sellersStandartDeviation[type];}}

	int getBuyerPricesMode(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return buyerPricesMode[type];}}
	double getMaximumBuyersPrice(int type)			{if(numberOfObjectTypes <= 0) {return -1;} else {return maximumBuyersPrice[type];}}
	double getMinimumBuyersPrice(int type)			{if(numberOfObjectTypes <= 0) {return -1;} else {return minimumBuyersPrice[type];}}

	double getBuyersMean(int type)					{if(numberOfObjectTypes <= 0) {return -1;} else {return buyersMean[type];}}
	double getBuyersStandartDeviation(int type)		{if(numberOfObjectTypes <= 0) {return -1;} else {return buyersStandartDeviation[type];}}

	int getSellerTimersMode(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return sellerTimersMode[type];}}
	int getSellersFrequency(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return sellersFrequency[type];}}
	double getSellersLambda(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return sellersLambda[type];}}

	int getBuyerTimersMode(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return buyerTimersMode[type];}}
	int getBuyersFrequency(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return buyersFrequency[type];}}
	double getBuyersLambda(int type)				{if(numberOfObjectTypes <= 0) {return -1;} else {return buyersLambda[type];}}
	
	int getNumberOfPriceAdaptations()		{return numberOfPriceAdaptations;}
	int getSellerPriceReduceAge()			{return sellerPriceReduceAge;}
	double getSellerPriceReduceShare()		{return sellerPriceReduceShare;}
	int getSellerTimeOnMarket()				{return sellerTimeOnMarket;}
	int getBuyerPriceIncreaseAge()			{return buyerPriceIncreaseAge;}
	double getBuyerPriceIncreaseShare()		{return buyerPriceIncreaseShare;}
	int getBuyerTimeOnMarket()				{return buyerTimeOnMarket;}
	
	double getSoloSellingAgentPrice()		{return soloSellingAgentPrice;}
	
	int getGraphicalMode()					{return graphicalMode;}
	int getPictureRefreshFrequency()		{return pictureRefreshFrequency;}
	int getPictureDelayTime()				{return pictureDelayTime;}

	int getNumberOfPockets()				{return numberOfPockets;}
	double getSideGap()						{return sideGap;}
	int getConstantBoardersMode()			{return constantBoardersMode;}
	double getMinimumHistogramArgument()	{return minimumHistogramArgument;}
	double getMaximumHistogramArgument()	{return maximumHistogramArgument;}
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
#define TIMEEXPOSITIONFILE		"output/timeExposition.txt"
#define SILENTMODE

/***********************************************************
						END OF THE OUTPUT
***********************************************************/