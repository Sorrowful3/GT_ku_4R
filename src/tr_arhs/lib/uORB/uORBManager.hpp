/*
 * uORBMAnager.cpp
 *
 *  Created on: 2021Äê4ÔÂ21ÈÕ
 *      Author: admin
 */

#ifndef SRC_LIB_UORB_UORBMANAGER_CPP_
#define SRC_LIB_UORB_UORBMANAGER_CPP_

#include <stdint.h>
#include <string>
#include <set>
#include <list>
#include "uORB.h"
#include "log.h"

#include "FreeRTOS.h"
#include "queue.h"
//#define ORBSet std::set<std::string>
using namespace std;

#define TOPICMES_NUM   1

namespace uORB
{
class Manager;
}

/**
 * This is implemented as a singleton.  This class manages creating the
 * uORB nodes for each uORB topics and also implements the behavor of the
 * uORB Api's.
 */
class uORB::Manager

{
public:
	// public interfaces for this class.
#pragma pack(push, 1)
	typedef struct
	{
	  ORB_ID    o_id;
	  void     *o_node_addr;
	}uORB_Node;
#pragma pack(pop)

	/**
	 * Initialize the singleton. Call this before everything else.
	 * @return true on success
	 */
	static bool initialize();

	/**
	 * Terminate the singleton. Call this after everything else.
	 * @return true on success
	 */
	static bool terminate();

	/**
	 * Method to get the singleton instance for the uORB::Manager.
	 * Make sure initialize() is called first.
	 * @return uORB::Manager*
	 */
	static uORB::Manager *get_instance() { return _Instance; }

	/**
	 * Get the DeviceMaster. If it does not exist,
	 * it will be created and initialized.
	 * Note: the first call to this is not thread-safe.
	 * @return nullptr if initialization failed (and errno will be set)
	 */

	list<uORB_Node> * get_device_master();

	// ==== uORB interface methods ====
	/**
	 * Advertise as the publisher of a topic.
	 *
	 * This performs the initial advertisement of a topic; it creates the topic
	 * node in /obj if required and publishes the initial data.
	 *
	 * Any number of advertisers may publish to a topic; publications are atomic
	 * but co-ordination between publishers is not provided by the ORB.
	 *
	 * Internally this will call orb_advertise_multi with an instance of 0.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @param data    A pointer to the initial data to be published.
	 *      For topics updated by interrupt handlers, the advertisement
	 *      must be performed from non-interrupt context.
	 * @param queue_size  Maximum number of buffered elements. If this is 1, no queuing is
	 *      used.
	 * @return    nullptr on error, otherwise returns an object pointer
	 *      that can be used to publish to the topic.
	 *      If the topic in question is not known (due to an
	 *      ORB_DEFINE with no corresponding ORB_DECLARE)
	 *      this function will return nullptr and set errno to ENOENT.
	 */
	orb_advert_t orb_advertise(const ORB_ID o_id_t,unsigned int queue_size = 1);


	/**
	 * Unadvertise a topic.
	 *
	 * @param handle  handle returned by orb_advertise or orb_advertise_multi.
	 * @return 0 on success
	 */
	int orb_unadvertise(orb_advert_t handle);

	/**
	 * Publish new data to a topic.
	 *
	 * The data is atomically published to the topic and any waiting subscribers
	 * will be notified.  Subscribers that are not waiting can check the topic
	 * for updates using orb_check.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @handle    The handle returned from orb_advertise.
	 * @param data    A pointer to the data to be published.
	 * @return    OK on success, PX4_ERROR otherwise with errno set accordingly.
	 */
	int  orb_publish( orb_advert_t handle, const void *data);

	/**
	 * Subscribe to a topic.
	 *
	 * The returned value is a file descriptor that can be passed to poll()
	 * in order to wait for updates to a topic, as well as topic_read,
	 * orb_check.
	 *
	 * If there were any publications of the topic prior to the subscription,
	 * an orb_check right after orb_subscribe will return true.
	 *
	 * Subscription will succeed even if the topic has not been advertised;
	 * in this case the topic will have a timestamp of zero, it will never
	 * signal a poll() event, checking will always return false and it cannot
	 * be copied. When the topic is subsequently advertised, poll, check,
	 * stat and copy calls will react to the initial publication that is
	 * performed as part of the advertisement.
	 *
	 * Subscription will fail if the topic is not known to the system, i.e.
	 * there is nothing in the system that has declared the topic and thus it
	 * can never be published.
	 *
	 * Internally this will call orb_subscribe_multi with instance 0.
	 *
	 * @param meta    The uORB metadata (usually from the ORB_ID() macro)
	 *      for the topic.
	 * @return    PX4_ERROR on error, otherwise returns a handle
	 *      that can be used to read and update the topic.
	 */
	orb_advert_t  orb_subscribe(const ORB_ID orb_id);

private: // data members
	static Manager *_Instance;

	list<uORB_Node> * _device_master{nullptr};


private: //class methods
	Manager();
	virtual ~Manager();
};

#endif /* SRC_LIB_UORB_UORBMANAGER_CPP_ */
