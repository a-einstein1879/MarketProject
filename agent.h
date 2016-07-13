#ifndef AGENT_H
#define AGENT_H

#include "object.h"
#include "configurator.h"

struct AgentInfo {
	int *buyerTimersMode;
	int *sellerTimersMode;
	int *buyerPricesMode;
	int *sellerPricesMode;
};

class Agent {
protected:
	Configurator *configurator;
	int numberOfObjectTypes;

	AgentInfo agentInfo;

	/* Timers */
	int timer;
	int *timeLeftBeforeNewSellingObject;
	int *timeLeftBeforeNewObjectBought;
	void switchTimers();
	/* End of timers */

	/* Memory */
	void allocateMemory();
	void freeMemory();
	void allocateAgentInfoMemory();
	void freeAgentInfoMemory();
	void allocateTimersMemory();
	void freeTimersMemory();
	/* End of memory */

	Object getSeller();
	Object getBuyer();

	virtual double formSellingPrice(int type) = 0;
	virtual double formBuyingPrice(int type) = 0;
	virtual void resetSellingTimer(int type) = 0;
	virtual void resetBuyingTimer(int type) = 0;
	
	/* Statistics */
	double getNormallyDistributedValue(double mean, double standartDeviation);
	double getExponentiallyDistributedValue(double lambda);
	/* End of statistics*/
public:
	Agent();
	~Agent();
	void tick();
	Object getObject();
};

class OrdinaryAgent : public Agent {
private:
	double formSellingPrice(int type);
	double formBuyingPrice(int type);
	void resetSellingTimer(int type);
	void resetBuyingTimer(int type);
public:
	OrdinaryAgent();
};

#endif