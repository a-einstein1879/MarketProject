#ifndef HISTOGRAM_H
#define HISTOGRAM_H

class Histogram {
private:
	int numberOfCharts;
	int numberOfBins;
	double minValue;
	double maxValue;
	double **values;
	bool indexesLegal(int chartIndex, int bin);
public:
	Histogram();
	Histogram(int noc, int nob, double mV, double MV);
	~Histogram();
	void setParameters(int noc, int nob, double mV, double MV);
	void setValue(int chartIndex, int bin, double value);

	double getValue(int chartIndex, int bin);
	void printHistogram();
};

#endif