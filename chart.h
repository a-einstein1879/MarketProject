#ifndef CHART_H
#define CHART_H

#include "interfaceClasses.h"

class Chart {
protected:
	int numberOfCharts;
	double minArgument;
	double maxArgument;
	double maxValue;
	double minValue;
	int numberOfArguments;
	int tmpChartIndex;

	double **values;
	Color *colors;
public:
	Chart();
	void setColor(Color color, int chartIndex);
	
	void setTmpChartIndex(int index);

	int getNumberOfCharts();
	double getMinArgument();
	double getMaxArgument();
	double getMaxValue();
	double getMinValue();
	int getNumberOfArguments();
	Color getColor(int chartIndex);
	double getValue(int chartIndex, int argument);

	void printChart();
	bool indexesLegal(int chartIndex = -1, int argument = -1);
};

class Histogram : public Chart {
private:
	double binWidth;
	void setParameters(int noc, int noa, double mA, double MA);
	
	bool indexesLegal(int chartIndex, int bin);
public:
	Histogram();
	Histogram(int noc, int noa, double mA, double MA);
	~Histogram();
	
	void addValue(int chartIndex, double value);
	void addValueToTmpIndex(double value);
};

class LineChart : public Chart {
private:
	double unitInterval;
	int *maxActiveArgument;
	
	void setParameters(int noc, double mA, double MA);
public:
	LineChart();
	LineChart(int noc, double mA, double MA);
	~LineChart();

	void addNextValue(double value, int chartIndex = -1);
	int getMaxActiveArgument(int chartIndex);
};

#endif