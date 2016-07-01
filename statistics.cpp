#include "statistics.h"

Statistics* Statistics::p_Statistics = 0;

Statistics* Statistics::getStatistics() {
	if(!p_Statistics) {
		p_Statistics = new Statistics();
	}
	return p_Statistics;
}

Statistics::Statistics() {
	numberOfObjectTypes = 2;
	numberOfStatistics = 7;
	memeryAllocation();
	configurator = configurator->getConfigurator();
	ui = ui->getOpenGLInterface();
}

Statistics::~Statistics() {
	memoryClear();
}

void Statistics::addStatisticsElement(double newValue, int type, int statisticsId) {
	if(type >= numberOfObjectTypes || type < 0 || statisticsId >= numberOfStatistics || statisticsId < 0) {return;}
	Object newObject(newValue, -1, 0);
	statistics[type][statisticsId].push(newObject, statistics[type][statisticsId].getNumberOfObjects() + 1);
}

void Statistics::drawStatistics() {
	int numberOfArguments = configurator->getModelingTime() / configurator->getTimerPrintingFrequency();

	LineChart lineChart1(5, 0, numberOfArguments);
	lineChart1.setTmpChartIndex(0);
	statistics[0][FORSALEPRICEID].feelLineChart(lineChart1);
	lineChart1.setTmpChartIndex(1);
	statistics[0][ASKPRICEID].feelLineChart(lineChart1);
	lineChart1.setTmpChartIndex(2);
	statistics[0][BIDPRICEID].feelLineChart(lineChart1);
	lineChart1.setTmpChartIndex(3);
	statistics[0][BOUGHTPRICEID].feelLineChart(lineChart1);
	/*lineChart1.setTmpChartIndex(4);
	meanSpread.feelLineChart(lineChart1);*/

	LineChart lineChart2(2, 0, numberOfArguments);
	lineChart2.setTmpChartIndex(0);
	statistics[0][FORSALENUMBEROFOBJECTSID].feelLineChart(lineChart2);
	lineChart2.setTmpChartIndex(1);
	statistics[0][BOUGHTNUMBEROFBJECTSID].feelLineChart(lineChart2);

	ui->drawStaticsCharts(lineChart1, lineChart2);
	ui->tick();
	Sleep(configurator->getPictureDelayTime());
}

double Statistics::getStatisticsElement(int type, int statisticsId) {
	return statistics[type][statisticsId].pricePop().getPrice();
}

double Statistics::getMeanValue(int type, int statisticsId) {
	return statistics[type][statisticsId].getMeanPrice();
}

void Statistics::memeryAllocation() {
	statistics = new LinkList* [numberOfObjectTypes];

	for(int i = 0; i < numberOfObjectTypes; i++) {
		statistics[i] = new LinkList[numberOfStatistics];
	}
}

void Statistics::memoryClear() {
	for(int i = 0; i < numberOfObjectTypes; i++) {
		delete [] statistics[i];
	}
	delete [] statistics;
}