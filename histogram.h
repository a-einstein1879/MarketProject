#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include "interfaceClasses.h"

class Histogram {
private:
	int numberOfCharts;
	int numberOfBins;
	double minArgument;
	double maxArgument;
	double maxValue;
	double binWidth;

	double **values;
	Color *colors;

	bool indexesLegal(int chartIndex, int bin);
	int tmpIndex;
	void setParameters(int noc, int nob, double mA, double MA);
public:
	Histogram();
	Histogram(int noc, int nob, double mA, double MA);
	~Histogram();
	void setColor(Color color, int chartIndex);

	void addValue(int chartIndex, double value);
	void setTmpIndex(int index);
	void addValueToTmpIndex(double value);
	
	int getNumberOfCharts();
	int getNumberOfBins();
	double getMinArgument();
	double getMaxArgument();
	double getMaxValue();
	double getValue(int chartIndex, int bin);
	Color getColor(int chartIndex);
	void printHistogram();
};

#endif