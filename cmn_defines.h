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

#define MODELINGTIME			10000

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