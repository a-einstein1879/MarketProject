#include "chart.h"

Chart::Chart() {
	numberOfCharts = -1;
	numberOfBins = -1;
	minArgument = -1;
	maxArgument = -1;
	maxValue = -1;
	tmpIndex = -1;
	chartType = -1;
}

Chart::Chart(int noc, int nob, double mA, double MA, int ChartType) {
	setParameters(noc, nob, mA, MA, ChartType);
}

void Chart::setParameters(int noc, int nob, double mA, double MA, int ChartType) {
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

Chart::~Chart() {
	if(numberOfCharts >= 0 && numberOfBins >= 0) {
		for(int i = 0; i < numberOfCharts; i++) {
			delete [] values[i];
		}
		delete [] values;
	}

	delete [] colors;
}

void Chart::addValue(int chartIndex, double value) {
	if(chartIndex < 0 || chartIndex >= numberOfCharts || value < minArgument || value > maxArgument) {return;}
	for(int i = 0; i < numberOfBins; i++) {
		if(value >= minArgument + i * binWidth && value < minArgument + (i + 1) * binWidth) {
			values[chartIndex][i]++;
			if(values[chartIndex][i] > maxValue) {maxValue = values[chartIndex][i];}
		}
	}
}

void Chart::setTmpIndex(int index) {
	tmpIndex = index;
}

void Chart::addValueToTmpIndex(double value) {
	addValue(tmpIndex, value);
}

void Chart::setColor(Color color, int chartIndex) {
	colors[chartIndex] = color;
}

int Chart::getChartType() {
	return chartType;
}

int Chart::getNumberOfCharts() {
	return numberOfCharts;
}

int Chart::getNumberOfBins() {
	return numberOfBins;
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

double Chart::getValue(int chartIndex, int bin) {
	if(!indexesLegal(chartIndex, bin)) {return -1;}
	return values[chartIndex][bin];
}

Color Chart::getColor(int chartIndex) {
	return colors[chartIndex];
}

bool Chart::indexesLegal(int chartIndex, int bin) {
	if(numberOfCharts >= 0 && numberOfBins >= 0 &&
		chartIndex < numberOfCharts && chartIndex >= 0 &&
		bin < numberOfBins && bin >= 0) {return true;}
	return false;
}

#include <stdio.h>
void Chart::printChart() {
	if(numberOfCharts < 0 || numberOfBins < 0) {return;}
	for(int i = 0; i < numberOfCharts; i++) {
		printf("Chart number %d:\n", i);
		for(int j = 0; j < numberOfBins; j++) {
			printf("%.2f ", values[i][j]);
		}
		printf("\n");
	}
}