// -----------------------------------------------------------------------------
// USB STATUS FOR USB CLASS
// -----------------------------------------------------------------------------
// CONTRIBUTORS : MAXIME FONTAINE
// VERSION : 1.0
// DATE : 27/01/2017
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

#ifndef _CAPEXUSBSTATUS_H
#define _CAPEXUSBSTATUS_H

#if defined(__WIN32__)
	// Success (no error)
	#define LIBUSB_SUCCESS					0
	// Input/output error
	#define LIBUSB_ERROR_IO					-1
		// Invalid parameter
	#define LIBUSB_ERROR_INVALID_PARAM		-2
	// Access denied (insufficient permissions)
	#define LIBUSB_ERROR_ACCESS				-3
	// No such device (it may have been disconnected)
	#define LIBUSB_ERROR_NO_DEVICE			-4
	// Entity not found
	#define LIBUSB_ERROR_NOT_FOUND			-5
	// Resource busy
	#define LIBUSB_ERROR_BUSY				-6
	// Operation timed out
	#define LIBUSB_ERROR_TIMEOUT			-7
	// Overflow
	#define LIBUSB_ERROR_OVERFLOW			-8
	// Pipe error
	#define LIBUSB_ERROR_PIPE				-9
	// System call interrupted (perhaps due to signal)
	#define LIBUSB_ERROR_INTERRUPTED		-10
	// Insufficient memory
	#define LIBUSB_ERROR_NO_MEM				-11
	// Operation not supported or unimplemented on this platform
	#define LIBUSB_ERROR_NOT_SUPPORTED		-12
#endif
	// Too much devices found
	#define CAPEXUSB_TOO_MUCH_DEVICES		-20
	// Problem for opening the device
	#define CAPEXUSB_CANNOT_OPEN_DEVICE		-21
	// Device not connected
	#define CAPEXUSB_DEVICE_NOT_CONNECTED	-22
	// Problem during a write operation on EndPoint 0
	#define CAPEXUSB_EP0_WRITE_ERROR		-51
	// Problem during a read operation on EndPoint 0
	#define CAPEXUSB_EP0_READ_ERROR			-52

	// Other error
	#define LIBUSB_ERROR_OTHER				-99

#endif