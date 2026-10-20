/*
 * Publication.hpp
 *
 *  Created on: 2021Äê4ÔÂ21ÈÕ
 *      Author: admin
 */

#ifndef SRC_LIB_UORB_PUBLICATION_HPP_
#define SRC_LIB_UORB_PUBLICATION_HPP_
#include "uORB.h"
#include <stdint.h>
#include <string.h>
#include <set>
#include <list>
#include "uORBManager.hpp"

namespace uORB
{
template <typename T>
class Publication
{
public:

	bool advertised() const { return _handle != nullptr; }

	bool advertise()
	{
		if (!advertised()) {
			_handle = orb_advertise(_orb_id,sizeof(T));
		}

		return advertised();
	}

	bool unadvertise()
	{
		_handle = nullptr;

		return true;
	}
	/**
	 * Publish the struct
	 * @param data The uORB message struct we are updating.
	 */
	bool publish(const T &data)
	{
		if (!advertised()) {
			advertise();
		}

		return (orb_publish(_handle, &data) == 0);
	}


	Publication(ORB_ID id) : _orb_id(id) {}

	~Publication()
	{

	}
protected:
	orb_advert_t _handle{nullptr};
	const ORB_ID _orb_id;

};

}

#endif /* SRC_LIB_UORB_PUBLICATION_HPP_ */
