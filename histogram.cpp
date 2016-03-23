#include "histogram.h"

Histogram::Histogram() {
	numberOfCharts = -1;
	numberOfBins = -1;
	minValue = -1;
	maxValue = -1;
}

Histogram::Histogram(int noc, int nob, double mV, double MV) {
	if(noc < 0 || nob < 0) {return;}
	numberOfCharts = noc;
	numberOfBins = nob;
	minValue = mV;
	maxValue = MV;
	
	values = new double*[numberOfCharts];
	for(int i = 0; i < numberOfCharts; i++) {
		values[i] = new double[numberOfBins];
	}
	for(int i = 0; i < numberOfCharts; i++) {
		for(int j = 0; j < numberOfBins; j++)
			values[i][j] = 0;
	}
}

Histogram::~Histogram() {
	if(numberOfCharts >= 0 && numberOfBins >= 0) {
		for(int i = 0; i < numberOfCharts; i++) {
			delete [] values[i];
		}
		delete [] values;
	}
}

void Histogram::setParameters(int noc, int nob, double mV, double MV) {
	if(numberOfCharts != -1 && numberOfBins != -1 && minValue != -1 && maxValue != -1) {return;}
	if(noc < 0 || nob < 0) {return;}
	numberOfCharts = noc;
	numberOfBins = nob;
	minValue = mV;
	maxValue = MV;
	
	values = new double*[numberOfCharts];
	for(int i = 0; i < numberOfCharts; i++) {
		values[i] = new double[numberOfBins];
	}
	for(int i = 0; i < numberOfCharts; i++) {
		for(int j = 0; j < numberOfBins; j++)
			values[i][j] = 0;
	}
}

void Histogram::setValue(int chartIndex, int bin, double value) {
	if(indexesLegal(chartIndex, bin)) {
		values[chartIndex][bin] = value;
	}
}

double Histogram::getValue(int chartIndex, int bin) {
	if(!indexesLegal(chartIndex, bin)) {return -1;}
	return values[chartIndex][bin];
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