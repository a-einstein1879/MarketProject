#ifndef CHART_H
#define CHART_H

#include "interfaceClasses.h"

class Chart {
protected:
	int numberOfCharts;
	double minArgument;
	double maxArgument;
	double maxValue;

	double **values;
	Color *colors;
public:
	Chart();
	void setColor(Color color, int chartIndex);
	
	int getNumberOfCharts();
	double getMinArgument();
	double getMaxArgument();
	double getMaxValue();
	Color getColor(int chartIndex);
	virtual void printChart() {}
};

class Histogram : public Chart {
private:
	int numberOfBins;
	double binWidth;
	void setParameters(int noc, int nob, double mA, double MA);
	
	bool indexesLegal(int chartIndex, int bin);
	int tmpIndex;
public:
	Histogram();
	Histogram(int noc, int nob, double mA, double MA);
	~Histogram();
	
	int getNumberOfBins();
	double getValue(int chartIndex, int bin);
	
	void addValue(int chartIndex, double value);
	void setTmpIndex(int index);
	void addValueToTmpIndex(double value);
	void printChart();
};

class LineChart : public Chart {
private:
	int numberOfArguments;
	double unitInterval;
	int maxActiveValue;
	
	bool valuesLegal(int chartIndex = -1, int argumentIndex = -1);
	void setParameters(int noc, double mA, double MA);
public:
	LineChart();
	LineChart(int noc, double mA, double MA);
	~LineChart();
};

#endif