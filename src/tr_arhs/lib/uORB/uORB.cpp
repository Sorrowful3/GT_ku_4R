/**
 * @file uORB.cpp
 * A lightweight object broker.
 */

#include "uORB.h"
#include "uORBManager.hpp"
//#include "EKF2.hpp"
static list<uORB::Manager::uORB_Node> * g_dev = nullptr;


int uorb_start(void)
{
	if (g_dev != nullptr) {
		TR_WARN("already loaded");
		/* user wanted to start uorb, its already running, no error */
		return 0;
	}

	if (!uORB::Manager::initialize()) {
		TR_ERR("uorb manager alloc failed");
		return -ENOMEM;
	}

	/* create the driver */
	g_dev = uORB::Manager::get_instance()->get_device_master();

	if (g_dev == nullptr) {
		return -errno;
	}

	return 0;
}


orb_advert_t orb_advertise(const ORB_ID o_id_t,unsigned int queue_size)
{
  return uORB::Manager::get_instance()->orb_advertise( o_id_t,queue_size);
}

int orb_unadvertise(orb_advert_t handle)
{
	return uORB::Manager::get_instance()->orb_unadvertise(handle);
}

int  orb_publish( orb_advert_t handle, const void *data)
{
	return uORB::Manager::get_instance()->orb_publish(handle, data);
}

orb_advert_t  orb_subscribe(const ORB_ID orb_id)
{
	return uORB::Manager::get_instance()->orb_subscribe(orb_id);
}



