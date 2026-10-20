/**
 * @file CDev.hpp
 *
 * Definitions for the generic base classes in the device framework.
 */

#ifndef _DEVICE_CDEV_HPP
#define _DEVICE_CDEV_HPP

#include "Device.hpp"
#include "fs.h"
#include "log.h"
/**
 * Namespace encapsulating all device framework classes, functions and data.
 */
namespace device
{

using file_t = struct file;

/**
 * Abstract class for any character device
 */
class  CDev : public Device
{
public:
	/**
	 * Constructor
	 *
	 * @param name		Driver name
	 * @param devname	Device node name
	 */
	CDev(const char *name); // TODO: dagar remove name and Device inheritance

	virtual ~CDev() = default;

	virtual int	init();

	/**
	 * Perform an ioctl operation on the device.
	 *
	 * @param filep		Pointer to the NuttX file structure.
	 * @param cmd		The ioctl command value.
	 * @param arg		The ioctl argument value.
	 * @return		OK on success, or -errno otherwise.
	 */
	virtual int	ioctl(file_t *filep, int cmd, unsigned long arg) { return -88; }

};

} // namespace device

// class instance for primary driver of each class
enum CLASS_DEVICE {
	CLASS_DEVICE_PRIMARY = 0,
	CLASS_DEVICE_SECONDARY = 1,
	CLASS_DEVICE_TERTIARY = 2
};


#endif /* _DEVICE_CDEV_HPP */
