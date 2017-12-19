//#include "usb.h"

using std::cout;
using std::endl;

namespace capex
{

	CAPEX_CALL usb::usb()
	{
		// Initialize a library session
		this->session = NULL;
		this->status = libusb_init(&(this->session));

		this->devices.clear();
		this->connected = false;
	}
	// -------------------------------------------------------------------


	CAPEX_CALL usb::usb(int VID, int PID)
	{
		// Initialize a library session
		this->session = NULL;
		this->status = libusb_init(&(this->session));

		// Get the list of devices with corresponding VID PID
		this->connected = false;
		this->devices = this->FoundDevices(VID, PID);

		// If only one device has been found, the constructor connects automatically to this device
		if(this->devices.size() < 1)
			this->status = LIBUSB_ERROR_NO_DEVICE;
		else if(this->devices.size() > 1)
			this->status = CAPEXUSB_TOO_MUCH_DEVICES;
		else if(this->devices.size() == 1)
			this->status = this->Open(this->devices[0]);

	}
	// -------------------------------------------------------------------


	CAPEX_CALL usb::~usb()
	{
		// Close the device if connected
		this->Close();

		// Free the list of USB devices
		libusb_free_device_list(this->devs, 1);

		// Close the session
		libusb_exit(this->session);
	}
	// -------------------------------------------------------------------


	std::vector<DeviceUSB> CAPEX_CALL usb::FoundDevices(int VID, int PID)
	{
		std::vector<DeviceUSB> list;
		list.clear();

		// Get the list of devices
		ssize_t CountDevices = libusb_get_device_list(this->session, &this->devs);

		// Get descriptor of searched devices
		for(ssize_t i = 0; i < CountDevices; i++)
		{
			libusb_device_descriptor desc;
			this->status = libusb_get_device_descriptor(this->devs[i], &desc);
			if(((desc.idVendor == VID) || (VID == -1)) && ((desc.idProduct == PID) || (PID == -1)))
			{
				DeviceUSB d;
				d.VID = desc.idVendor;
				d.PID = desc.idProduct;
				d.USBVersion = desc.bcdUSB;
				d.DeviceClass = desc.bDeviceClass;
				d.lib.device = devs[i];
				this->status = libusb_open(d.lib.device, &(d.lib.handle));
				if(this->status >= 0)
				{
					unsigned char Buffer[256];
					this->status = libusb_get_string_descriptor_ascii(d.lib.handle, desc.iManufacturer, &Buffer[0], 256);
					if(this->status >= 0)
						d.Manufacturer = std::string((char*)(&Buffer[0]), this->status);
					this->status = libusb_get_string_descriptor_ascii(d.lib.handle, desc.iProduct, &Buffer[0], 256);
					if(this->status >= 0)
						d.Product = std::string((char*)(&Buffer[0]), this->status);
					this->status = libusb_get_string_descriptor_ascii(d.lib.handle, desc.iSerialNumber, &Buffer[0], 256);
					if(this->status >= 0)
						d.SerialNumber = std::string((char*)(&Buffer[0]), this->status);
					libusb_close(d.lib.handle);
				}
				list.push_back(d);
			}
		}

		return list;
	}
	// -------------------------------------------------------------------


	int CAPEX_CALL usb::Open(DeviceUSB device, int usbinterface)
	{
		this->status = libusb_open(device.lib.device, &(device.lib.handle));
		if(this->status >= 0)
		{
			this->device = device;
			this->status = libusb_claim_interface(device.lib.handle, usbinterface);
			this->connected = true;

			return LIBUSB_SUCCESS;
		}

		return ((int)(this->status));
	}
	// -------------------------------------------------------------------


	void CAPEX_CALL usb::Close()
	{
		if(this->connected)
		{
			libusb_close(this->device.lib.handle);
			this->connected = false;
		}
	}
	// -------------------------------------------------------------------


	bool CAPEX_CALL usb::IsConnected()
	{
		return this->connected;
	}
	// -------------------------------------------------------------------


	int CAPEX_CALL usb::GetVID()
	{
		return this->device.VID;
	}
	// -------------------------------------------------------------------


	int CAPEX_CALL usb::GetPID()
	{
		return this->device.PID;
	}
	// -------------------------------------------------------------------


	std::string CAPEX_CALL usb::GetUsbVersion()
	{
		std::string s1, s2;
		char buffer[32];
		s1 = std::string(std::itoa(this->device.USBVersion >> 8, buffer, 10));
		s2 = std::string(std::itoa(this->device.USBVersion & 0xFF, buffer, 10));
		//s1 = std::to_string(this->device.USBVersion >> 8);
		//s2 = std::to_string(this->device.USBVersion & 0xFF);
		return (s1 + "." + s2);
	}
	// -------------------------------------------------------------------


	std::string CAPEX_CALL usb::GetManufacturer()
	{
		return this->device.Manufacturer;
	}
	// -------------------------------------------------------------------


	std::string CAPEX_CALL usb::GetProduct()
	{
		return this->device.Product;
	}
	// -------------------------------------------------------------------


	std::string CAPEX_CALL usb::GetSerialNumber()
	{
		return this->device.SerialNumber;
	}
	// -------------------------------------------------------------------
	
	
	std::string CAPEX_CALL usb::GetErrorMessage(int Status)
	{
		if(Status == 1)
			Status = this->status;
		
		std::string msg = "";

		if(Status >= -12)
			msg = libusb_strerror((libusb_error)(Status));
		else
		{
			switch(Status)
			{
				case LIBUSB_SUCCESS:
					msg = "Success";
					break;
				case LIBUSB_ERROR_IO:
					msg = "Input/output error";
					break;
				case LIBUSB_ERROR_INVALID_PARAM:
					msg = "Invalid parameter";
					break;
				case LIBUSB_ERROR_ACCESS:
					msg = "Access denied (insufficient permissions)";
					break;
				case LIBUSB_ERROR_NO_DEVICE:
					msg = "No such device (it may have been disconnected)";
					break;
				case LIBUSB_ERROR_NOT_FOUND:
					msg = "Entity not found";
					break;
				case LIBUSB_ERROR_BUSY:
					msg = "Resource busy";
					break;
				case LIBUSB_ERROR_TIMEOUT:
					msg = "Operation timed out";
					break;
				case LIBUSB_ERROR_OVERFLOW:
					msg = "Overflow";
					break;
				case LIBUSB_ERROR_PIPE:
					msg = "Pipe error";
					break;
				case LIBUSB_ERROR_INTERRUPTED:
					msg = "System call interrupted (perhaps due to signal)";
					break;
				case LIBUSB_ERROR_NO_MEM:
					msg = "Insufficient memory";
					break;
				case LIBUSB_ERROR_NOT_SUPPORTED:
					msg = "Operation not supported or unimplemented on this platform";
					break;
				case CAPEXUSB_TOO_MUCH_DEVICES:
					msg = "Too much USB devices found";
					break;
				case CAPEXUSB_DEVICE_NOT_CONNECTED:
					msg = "No USB device is connected";
					break;
				case CAPEXUSB_EP0_WRITE_ERROR:
					msg = "Error for writing in EP0";
					break;
				case CAPEXUSB_EP0_READ_ERROR:
					msg = "Error for reading in EP0";
					break;
				case LIBUSB_ERROR_OTHER:
					msg = "Error not known";
					break;
				default:
					char buffer[32];
					msg = "Error " + std::string(std::itoa(Status, buffer, 10)) + " not known";
					break;
			}
		}
		
		return msg;
	}
	// -------------------------------------------------------------------
	
	
	int CAPEX_CALL usb::EP0Write(unsigned char Request, unsigned short Value, unsigned short Index, unsigned char *Data, unsigned long Length)
	{
		if(!this->connected)
			return CAPEXUSB_DEVICE_NOT_CONNECTED;

		unsigned int timeout = 1000;
		static const int CONTROL_REQUEST_TYPE_OUT = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE;
		this->status = libusb_control_transfer(this->device.lib.handle,
											CONTROL_REQUEST_TYPE_OUT,
											Request, Value, Index,
											Data, Length, timeout);
		return this->status;
	}
	// -------------------------------------------------------------------
	
	
	int CAPEX_CALL usb::EP0Read(unsigned char Request, unsigned short Value, unsigned short Index, unsigned char *Data, unsigned long Length)
	{
		if(!this->connected)
			return CAPEXUSB_DEVICE_NOT_CONNECTED;

		unsigned int timeout = 1000;
		static const int CONTROL_REQUEST_TYPE_IN = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR | LIBUSB_RECIPIENT_INTERFACE;
		this->status = libusb_control_transfer(this->device.lib.handle,
											CONTROL_REQUEST_TYPE_IN,
											Request, Value, Index,
											Data, Length, timeout);
		return this->status;
	}
	// -------------------------------------------------------------------
}



























