#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "interfaceClasses.h"

class Chart {
private:
	int numberOfCharts;
	int numberOfBins;
	double minArgument;
	double maxArgument;
	double maxValue;
	double binWidth;
	
	/* 0 - histogram */
	int chartType;

	double **values;
	Color *colors;

	bool indexesLegal(int chartIndex, int bin);
	int tmpIndex;
	void setParameters(int noc, int nob, double mA, double MA, int ChartType);
public:
	Chart();
	Chart(int noc, int nob, double mA, double MA, int ChartType);
	~Chart();
	void setColor(Color color, int chartIndex);

	void addValue(int chartIndex, double value);
	void setTmpIndex(int index);
	void addValueToTmpIndex(double value);
	
	int getChartType();
	int getNumberOfCharts();
	int getNumberOfBins();
	double getMinArgument();
	double getMaxArgument();
	double getMaxValue();
	double getValue(int chartIndex, int bin);
	Color getColor(int chartIndex);
	void printChart();
};

#endif