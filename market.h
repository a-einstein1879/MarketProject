#ifndef MARKET_H
#define MARKET_H

class Market {
private:
	Market();
	static Market *p_Market;

public:
	static Market* getMarket();

	void run();
};

#endif