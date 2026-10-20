/*
 * Subscription.hpp
 *
 *  Created on: 2021年4月21日
 *      Author: admin
 */

#ifndef SRC_LIB_UORB_SUBSCRIPTION_HPP_
#define SRC_LIB_UORB_SUBSCRIPTION_HPP_
#include "uORB.h"
#include <stdint.h>
#include <string.h>
#include <set>
#include <list>
#include "uORBManager.hpp"
#include "topics/sensor_accel.h"
#include "topics/sensor_gyro.h"

#include "FreeRTOS.h"
#include "queue.h"

using namespace std;
namespace uORB
{
 class SubscriptionBase;
 template <typename T>
 class Subscription;
}

class uORB::SubscriptionBase
{
public:
	/**
	 * Constructor
	 *
	 * @param id The uORB ORB_ID enum for the topic.
	 */
	SubscriptionBase(ORB_ID id);
    ~SubscriptionBase();

	bool subscribe();
	void unsubscribe();

	bool valid() const { return _node!= nullptr; }
	bool advertised();

	/**
	 * Check if there is a new update.
	 */
	bool updated();


	/**
	 * Copy the struct
	 * @param dst The uORB message struct we are updating.
	 */
	 bool copy(void *dst);

protected:
	 void *_node{nullptr};

	ORB_ID _orb_id{ORB_ID::INVALID};

};

template <typename T>
class uORB::Subscription : public SubscriptionBase
{
public:
	Subscription(ORB_ID id):
		SubscriptionBase(id)
	{

	}

	~Subscription()
	{

	}

	bool updated()
	{
	  //句柄为空会怎样？ 需测试
	  uint8_t ret = xQueueReceive(_node,&_source_data,portMAX_DELAY);

	  return  advertised() && ret;

	}

	bool copy(void *dst)
	{
		 uint8_t ret = advertised();
		 if(ret)
		   memcpy(dst,&_source_data,sizeof(_source_data)) ;
		 return ret;
	}
private:
	T _source_data;

};


#endif /* SRC_LIB_UORB_SUBSCRIPTION_HPP_ */
