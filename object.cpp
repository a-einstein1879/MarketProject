#include "object.h"

Object::Object() {
	price		 = 0;
	creationTime = -1;
	status		 = 0;
}

Object::Object(double Price, double CreationTime, bool Status) {
	price		 = Price;
	creationTime = CreationTime;
	status		 = Status;
}

void Object::setObject(double Price, double CreationTime, bool Status) {
	price		 = Price;
	creationTime = CreationTime;
	status		 = Status;
}

double Object::getPrice() {
	return price;
}
double Object::getCreationTime() {
	return creationTime;
}

bool Object::getStatus() {
	return status;
}

Object& Object::operator=(Object &object) {
	price		 = object.getPrice();
	creationTime = object.getCreationTime();
	status		 = object.getStatus();
	return *this;
}

bool Object::operator<=(Object &object) {
	if(price <= object.getPrice()) {
		return true;
	} else {
		return false;
	}
}

bool Object::operator>=(Object &object) {
	if(price >= object.getPrice()) {
		return true;
	} else {
		return false;
	}
}