/*
 * uORBManager.cpp
 *
 *  Created on: 2021年4月21日
 *      Author: admin
 */
#include "uORBManager.hpp"

uORB::Manager *uORB::Manager::_Instance = nullptr;

uORB::Manager::Manager()
{

}


uORB::Manager::~Manager()
{
	delete _device_master;
}


bool uORB::Manager::initialize()
{
  	if(_Instance ==nullptr)
  	{
  		_Instance = new uORB::Manager();
  	}

  	return _Instance != nullptr;

}

bool uORB::Manager::terminate()
{
	if (_Instance != nullptr) {
		delete _Instance;
		_Instance = nullptr;
		return true;
	}

	return false;
}


list<uORB::Manager::uORB_Node> * uORB::Manager::get_device_master()
{
  if(_device_master==nullptr)
  {
	  _device_master = new list<uORB_Node> ;

		if (_device_master == nullptr)
		{
			TR_ERR("Failed to allocate DeviceMaster");
			errno = ENOMEM;
		}
  }
  return _device_master;
}

orb_advert_t uORB::Manager::orb_advertise(const ORB_ID o_id_t,unsigned int queue_size)
{
	//添加消息队列创建函数，返还句柄， 取消data ，增加ID号
	orb_advert_t  TopicHandle = xQueueCreate(TOPICMES_NUM,queue_size);
	uORB::Manager::uORB_Node _data ;

	_data.o_id = o_id_t;
	_data.o_node_addr = TopicHandle ;

    uORB::Manager::get_instance()->_device_master->push_back(_data);

    return  TopicHandle;
}

int uORB::Manager::orb_unadvertise(orb_advert_t handle)
{
   list<uORB::Manager::uORB_Node>::iterator iter;
   list<uORB::Manager::uORB_Node> *List = uORB::Manager::get_instance()->_device_master;
   for(iter = List->begin(); iter != List->end() ;)
   {
	 if(handle == iter->o_node_addr)
	 {
		 List->erase(iter);
		 return 0;
	 }

   }

   return 1;
}
int  uORB::Manager::orb_publish(orb_advert_t handle, const void *data)
{

	uint8_t ret=xQueueSend(handle,data,0);

	return ret;

}


orb_advert_t uORB::Manager::orb_subscribe(const ORB_ID orb_id)
{
    list<uORB::Manager::uORB_Node>::iterator iter;
	list<uORB::Manager::uORB_Node> *List = uORB::Manager::get_instance()->_device_master;

	for(iter = List->begin(); iter != List->end() ;iter++)
	 {
		 if(orb_id == iter->o_id)
		 {
		   return iter->o_node_addr ;

		 }

	  }

	return nullptr;
}
