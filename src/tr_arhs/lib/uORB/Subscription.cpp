/*
 * Subscription.cpp
 *
 *  Created on: 2021Äê4ÔÂ22ÈÕ
 *      Author: admin
 */
#include "Subscription.hpp"


using namespace uORB ;


SubscriptionBase::SubscriptionBase(ORB_ID id) :_orb_id(id)
{
	subscribe();
}

SubscriptionBase::~SubscriptionBase()
{
	unsubscribe();
}


bool SubscriptionBase::subscribe()
{
	// check if already subscribed
	if (_node != nullptr) {
		return true;
	}

	if (_orb_id != ORB_ID::INVALID) {

		_node = orb_subscribe(_orb_id);
		if(_node != nullptr)
			return true;
		}

	return false;
}


void SubscriptionBase::unsubscribe()
{
	if (_node != nullptr) {

		_node = nullptr;
	}

}


bool SubscriptionBase::advertised()
{
	if (valid()) {
		return true;
}
	// try to initialize
	if (subscribe()) {
		// check again if valid
		if (valid()) {
			return true;
		}
	}

	return false;
}

