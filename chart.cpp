#include "chart.h"

/************************************
				CHART
************************************/

Chart::Chart() {
	numberOfCharts = -1;
	minArgument = -1;
	maxArgument = -1;
	maxValue = -1;
	minValue = -10000;
	numberOfArguments = -1;
	tmpChartIndex = -1;
}

void Chart::setColor(Color color, int chartIndex) {
	colors[chartIndex] = color;
}

void Chart::setTmpChartIndex(int chartIndex) {
	tmpChartIndex = chartIndex;
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

double Chart::getMinValue() {
	return minValue;
}

Color Chart::getColor(int chartIndex) {
	return colors[chartIndex];
}

double Chart::getValue(int chartIndex, int argument) {
	if(!indexesLegal(chartIndex, argument)) {return -1;}
	return values[chartIndex][argument];
}

int Chart::getNumberOfArguments() {
	return numberOfArguments;
}

bool Chart::indexesLegal(int chartIndex, int argument) {
	if(chartIndex == -1 && argument == -1 && numberOfCharts >= 0 && numberOfArguments >= 0) {return true;}
	if(numberOfCharts >= 0 && numberOfArguments >= 0 &&
		chartIndex < numberOfCharts && chartIndex >= 0 &&
		argument < numberOfArguments && argument >= 0) {return true;}
	return false;
}

#include <stdio.h>
void Chart::printChart() {
	if(!indexesLegal()) {return;}
	for(int i = 0; i < numberOfCharts; i++) {
		printf("Chart number %d:\n", i);
		for(int j = 0; j < numberOfArguments; j++) {
			printf("%.2f ", values[i][j]);
		}
		printf("\n");
	}
}

/************************************
			HISTOGRAM
************************************/
Histogram::Histogram() {
}

Histogram::Histogram(int noc, int noa, double mA, double MA) {
	setParameters(noc, noa, mA, MA);
}

Histogram::~Histogram() {
	if(numberOfCharts >= 0 && numberOfArguments >= 0) {
		for(int i = 0; i < numberOfCharts; i++) {
			delete [] values[i];
		}
		delete [] values;
	}

	delete [] colors;
}

void Histogram::setParameters(int noc, int noa, double mA, double MA) {
	if(noc < 0 || noa < 0) {return;}
	numberOfCharts = noc;
	numberOfArguments = noa;
	minArgument = mA;
	maxArgument = MA;
	binWidth = (MA - mA) / double(numberOfArguments);
	maxValue = -1;
	
	values = new double*[numberOfCharts];
	for(int i = 0; i < numberOfCharts; i++) {
		values[i] = new double[numberOfArguments];
	}
	for(int i = 0; i < numberOfCharts; i++) {
		for(int j = 0; j < numberOfArguments; j++)
			values[i][j] = 0;
	}

	colors = new Color[numberOfCharts];
	if(numberOfCharts == 3) {
		colors[0].set(1, 0, 0);
		colors[1].set(0, 0, 1);
		colors[2].set(0, 1, 0);
	}
}

void Histogram::addValue(int chartIndex, double value) {
	if(chartIndex < 0 || chartIndex >= numberOfCharts || value < minArgument || value > maxArgument) {return;}
	for(int i = 0; i < numberOfArguments; i++) {
		if(value >= minArgument + i * binWidth && value < minArgument + (i + 1) * binWidth) {
			values[chartIndex][i]++;
			if(values[chartIndex][i] > maxValue) {maxValue = values[chartIndex][i];}
		}
	}
}

void Histogram::addValueToTmpIndex(double value) {
	addValue(tmpChartIndex, value);
}

/************************************
			LINECHART
************************************/
LineChart::LineChart() {
}

LineChart::LineChart(int noc, double mA, double MA) {
	setParameters(noc, mA, MA);
	if(indexesLegal()) {
		values = new double*[numberOfCharts];
		for(int i = 0; i < numberOfCharts; i++) {
			values[i] = new double[numberOfArguments];
		}
		for(int i = 0; i < numberOfCharts; i++) {
			for(int j = 0; j < numberOfArguments; j++)
				values[i][j] = 0;
		}

		colors = new Color[numberOfCharts];
		if(numberOfCharts == 2) {
			colors[0].set(1, 0, 0);
			colors[1].set(0, 1, 0);
		}

		maxActiveArgument = new int[numberOfCharts];
		for(int i = 0; i < numberOfCharts; i++) {
			maxActiveArgument[i] = 0;
		}
	}
}

LineChart::~LineChart() {
	if(indexesLegal()) {
		for(int i = 0; i < numberOfCharts; i++) {
			delete [] values[i];
		}
		delete [] values;
	}

	delete [] colors;
}

void LineChart::setParameters(int noc, double mA, double MA) {
	if(noc < 0) {return;}
	numberOfCharts = noc;
	numberOfArguments = int(MA - mA);
	minArgument = mA;
	maxArgument = MA;
	maxActiveArgument = 0;
	unitInterval = 1;
	maxValue = -1;
}

void LineChart::addNextValue(double value, int chartIndex) {
	if(chartIndex == -1) {
		if(tmpChartIndex != -1) {
			addNextValue(value, tmpChartIndex);
		}
		return;
	}
	values[chartIndex][maxActiveArgument[chartIndex]++] = value;
	if(value > maxValue) {maxValue = value;}
	if(minValue == -10000) {minValue = value;}
	else {
		if(value < minValue) {minValue = value;}
	}
}

int LineChart::getMaxActiveArgument(int chartIndex) {
	return maxActiveArgument[chartIndex];
}