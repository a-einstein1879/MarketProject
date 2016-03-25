#include "chart.h"

/************************************
				CHART
************************************/

Chart::Chart() {
	numberOfCharts = -1;
	minArgument = -1;
	maxArgument = -1;
	maxValue = -1;
}

void Chart::setColor(Color color, int chartIndex) {
	colors[chartIndex] = color;
}

int Chart::getNumberOfCharts() {
	return numberOfCharts;
}

double Chart::getMinArgument() {
	return minArgument;
}

double Chart::getMaxArgument() {
	return maxArgument;
}

double Chart::getMaxValue() {
	return maxValue;
}

Color Chart::getColor(int chartIndex) {
	return colors[chartIndex];
}


/************************************
			HISTOGRAM
************************************/
Histogram::Histogram() {
	numberOfBins = -1;
	tmpIndex = -1;
}

Histogram::Histogram(int noc, int nob, double mA, double MA) {
	setParameters(noc, nob, mA, MA);
}

Histogram::~Histogram() {
	if(numberOfCharts >= 0 && numberOfBins >= 0) {
		for(int i = 0; i < numberOfCharts; i++) {
			delete [] values[i];
		}
		delete [] values;
	}

	delete [] colors;
}

void Histogram::setParameters(int noc, int nob, double mA, double MA) {
	if(noc < 0 || nob < 0) {return;}
	numberOfCharts = noc;
	numberOfBins = nob;
	minArgument = mA;
	maxArgument = MA;
	binWidth = (MA - mA) / double(numberOfBins);
	maxValue = -1;
	tmpIndex = -1;
	
	values = new double*[numberOfCharts];
	for(int i = 0; i < numberOfCharts; i++) {
		values[i] = new double[numberOfBins];
	}
	for(int i = 0; i < numberOfCharts; i++) {
		for(int j = 0; j < numberOfBins; j++)
			values[i][j] = 0;
	}

	colors = new Color[numberOfCharts];
	if(numberOfCharts == 3) {
		colors[0].set(1, 0, 0);
		colors[1].set(0, 1, 0);
		colors[2].set(0, 0, 1);
	}
}

bool Histogram::indexesLegal(int chartIndex, int bin) {
	if(numberOfCharts >= 0 && numberOfBins >= 0 &&
		chartIndex < numberOfCharts && chartIndex >= 0 &&
		bin < numberOfBins && bin >= 0) {return true;}
	return false;
}

double Histogram::getValue(int chartIndex, int bin) {
	if(!indexesLegal(chartIndex, bin)) {return -1;}
	return values[chartIndex][bin];
}

int Histogram::getNumberOfBins() {
	return numberOfBins;
}

void Histogram::addValue(int chartIndex, double value) {
	if(chartIndex < 0 || chartIndex >= numberOfCharts || value < minArgument || value > maxArgument) {return;}
	for(int i = 0; i < numberOfBins; i++) {
		if(value >= minArgument + i * binWidth && value < minArgument + (i + 1) * binWidth) {
			values[chartIndex][i]++;
			if(values[chartIndex][i] > maxValue) {maxValue = values[chartIndex][i];}
		}
	}
}

void Histogram::setTmpIndex(int index) {
	tmpIndex = index;
}

void Histogram::addValueToTmpIndex(double value) {
	addValue(tmpIndex, value);
}

#include <stdio.h>
void Histogram::printChart() {
	if(numberOfCharts < 0 || numberOfBins < 0) {return;}
	for(int i = 0; i < numberOfCharts; i++) {
		printf("Chart number %d:\n", i);
		for(int j = 0; j < numberOfBins; j++) {
			printf("%.2f ", values[i][j]);
		}
		printf("\n");
	}
}


/************************************
			LINECHART
************************************/
LineChart::LineChart() {
	numberOfArguments = -1;
	maxActiveValue = 0;
}

LineChart::LineChart(int noc, double mA, double MA) {
	setParameters(noc, mA, MA);
	if(valuesLegal()) {
		values = new double*[numberOfCharts];
		for(int i = 0; i < numberOfCharts; i++) {
			values[i] = new double[numberOfArguments];
		}
		for(int i = 0; i < numberOfCharts; i++) {
			for(int j = 0; j < numberOfArguments; j++)
				values[i][j] = 0;
		}

		colors = new Color[numberOfCharts];
	}
}

LineChart::~LineChart() {
	if(valuesLegal()) {
		for(int i = 0; i < numberOfCharts; i++) {
			delete [] values[i];
		}
		delete [] values;
	}

	delete [] colors;
}

bool LineChart::valuesLegal(int chartIndex, int argumentIndex) {
	if(chartIndex == -1 && argumentIndex == -1 && numberOfCharts >= 0 && numberOfArguments >= 0) {return true;}
	if(chartIndex >= 0 && chartIndex < numberOfCharts &&
		argumentIndex >= 0 && argumentIndex < numberOfArguments &&
		numberOfCharts >= 0 && numberOfArguments >= 0) {return true;}
	return false;
}

void LineChart::setParameters(int noc, double mA, double MA) {
	if(noc < 0) {return;}
	numberOfCharts = noc;
	numberOfArguments = int(MA - mA);
	minArgument = mA;
	maxArgument = MA;
	maxActiveValue = 0;
	unitInterval = 1;
	maxValue = -1;
}