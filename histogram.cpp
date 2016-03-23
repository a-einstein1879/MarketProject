#include "histogram.h"

Histogram::Histogram() {
	numberOfCharts = -1;
	numberOfBins = -1;
	minArgument = -1;
	maxArgument = -1;
	maxValue = -1;
}

Histogram::Histogram(int noc, int nob, double mA, double MA) {
	if(noc < 0 || nob < 0) {return;}
	numberOfCharts = noc;
	numberOfBins = nob;
	minArgument = mA;
	maxArgument = MA;
	
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
	if(numberOfCharts != -1 && numberOfBins != -1) {return;}
	if(noc < 0 || nob < 0) {return;}
	numberOfCharts = noc;
	numberOfBins = nob;
	minArgument = mA;
	maxArgument = MA;
	
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

void Histogram::setValue(int chartIndex, int bin, double value) {
	if(indexesLegal(chartIndex, bin)) {
		values[chartIndex][bin] = value;
		if(value > maxValue) {maxValue = value;}
	}
}

void Histogram::setColor(Color color, int chartIndex) {
	colors[chartIndex] = color;
}

int Histogram::getNumberOfCharts() {
	return numberOfCharts;
}

int Histogram::getNumberOfBins() {
	return numberOfBins;
}

double Histogram::getMinArgument() {
	return minArgument;
}

double Histogram::getMaxArgument() {
	return maxArgument;
}

double Histogram::getMaxValue() {
	return maxValue;
}

double Histogram::getValue(int chartIndex, int bin) {
	if(!indexesLegal(chartIndex, bin)) {return -1;}
	return values[chartIndex][bin];
}

Color Histogram::getColor(int chartIndex) {
	return colors[chartIndex];
}

bool Histogram::indexesLegal(int chartIndex, int bin) {
	if(numberOfCharts >= 0 && numberOfBins >= 0 &&
		chartIndex < numberOfCharts && chartIndex >= 0 &&
		bin < numberOfBins && bin >= 0) {return true;}
	return false;
}

#include <stdio.h>
void Histogram::printHistogram() {
	if(numberOfCharts < 0 || numberOfBins < 0) {return;}
	for(int i = 0; i < numberOfCharts; i++) {
		printf("Chart number %d:\n", i);
		for(int j = 0; j < numberOfBins; j++) {
			printf("%.2f ", values[i][j]);
		}
		printf("\n");
	}
}