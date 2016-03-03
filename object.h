#ifndef OBJECT_H
#define OBJECT_H

/*
Operators >= and <= are defined to find cheaper or more expensive objects not comparing objects statuses.
*/

class Object {
private:
	double price;
	double creationTime;
	bool status;
public:
	Object();
	Object(double Price, double CreationTime, bool Status);
	void setObject(double Price, double CreationTime, bool Status);
	double getPrice();
	double getCreationTime();
	bool   getStatus();
	Object& operator=(Object &object);
	bool operator<=(Object &object);
	bool operator>=(Object &object);
};

#endif