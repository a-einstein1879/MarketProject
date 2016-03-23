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
	double **values;
	Color *colors;
	bool indexesLegal(int chartIndex, int bin);
public:
	Histogram();
	Histogram(int noc, int nob, double mA, double MA);
	~Histogram();
	void setParameters(int noc, int nob, double mA, double MA);
	void setValue(int chartIndex, int bin, double value);
	void setColor(Color color, int chartIndex);
	
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