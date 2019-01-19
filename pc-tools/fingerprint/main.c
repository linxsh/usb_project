#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "libusb.h"

#define VENDOR_ID  0x0bc2
#define PRODUCT_ID 0x2312
struct libusb_device_handle *devh = NULL;

static int LIBUSB_CALL usb_arrived_callback(struct libusb_context *ctx,
		struct libusb_device *dev, libusb_hotplug_event event, void *userdata)
{
	struct libusb_device_descriptor desc;
	int rc;

	libusb_get_device_descriptor(dev, &desc);
	printf("Add usb device: \n");
	printf("\tCLASS(0x%x) SUBCLASS(0x%x) PROTOCOL(0x%x)\n",
			desc.bDeviceClass, desc.bDeviceSubClass, desc.bDeviceProtocol);
	printf("\tVENDOR(0x%x) PRODUCT(0x%x)\n", desc.idVendor, desc.idProduct);

	if ((desc.idVendor != VENDOR_ID) || (desc.idProduct != PRODUCT_ID)) {
		printf("vendor: %x, product: %x not found\n", VENDOR_ID, PRODUCT_ID);
		return -1;
	}

	rc = libusb_open(dev, &devh);
	if (LIBUSB_SUCCESS != rc) {
		printf("Could not open USB device: %s\n", libusb_error_name(rc));
		return -1;
	}

	return 0;
}

static int LIBUSB_CALL usb_detach_callback(struct libusb_context *ctx,
		struct libusb_device *dev, libusb_hotplug_event event, void *userdata)
{
	struct libusb_device_descriptor desc;

	libusb_get_device_descriptor(dev, &desc);
	printf("Remove usb device: CLASS(0x%x) SUBCLASS(0x%x) iSerialNumber(0x%x)\n",
			desc.bDeviceClass, desc.bDeviceSubClass, desc.iSerialNumber);

	return 0;
}

void usb_dev_print(libusb_device *dev)
{
	struct libusb_device_descriptor desc;
	struct libusb_config_descriptor *config;
	int rc = libusb_get_device_descriptor(dev, &desc);
	unsigned char buf[512];

	printf("---------------------------------------\n");
	printf("Vendor ID:    0x%x\n", desc.idVendor);
	printf("Product ID:   0x%x\n", desc.idProduct);
	printf("Device Class: 0x%x\n", (int)desc.bDeviceClass);

	if (rc < 0) {
		printf("failed to get device descriptor: %s\n", libusb_error_name(rc));
		return;
	}

	memset(buf, 0, sizeof(buf));
	rc = libusb_get_string_descriptor_ascii(devh, desc.iManufacturer, buf, sizeof(buf));
	if (rc < 0) {
		printf("Get Manufacturer failed: %s\n", libusb_error_name(rc));
		return;
	} else {
		printf("Manufacturer: %s\n", buf);
	}

	memset(buf, 0, sizeof(buf));
	rc = libusb_get_string_descriptor_ascii(devh, desc.iProduct, buf, sizeof(buf));
	if (rc < 0) {
		printf("Get Product failed: %s\n", libusb_error_name(rc));
		return;
	} else {
		printf("Product:      %s\n", buf);
	}

	memset(buf, 0, sizeof(buf));
	rc = libusb_get_string_descriptor_ascii(devh, desc.iSerialNumber, buf, sizeof(buf));
	if (rc < 0) {
		printf("Get SerialNumber failed: %s\n", libusb_error_name(rc));
		return;
	} else {
		printf("SerialNumber: %s\n", buf);
	}

	printf("Number of possible configurations: %d\n", (int)desc.bNumConfigurations);
	libusb_get_config_descriptor(dev, 0, &config);
	printf("\tInterfaces: %d\n",(int)config->bNumInterfaces);

	for (int i = 0; i < (int)config->bNumInterfaces; i++) {
		const struct libusb_interface *inter = &config->interface[i];
		printf("\t\tNumber of alternate settings: %d\n",inter->num_altsetting);

		for (int j=0; j<inter->num_altsetting; j++) {
			const struct libusb_interface_descriptor *interdesc = &inter->altsetting[j];
			printf("\t\t\tInterface Number:    %d\n",(int)interdesc->bInterfaceNumber);
			printf("\t\t\tNumber of endpoints: %d\n",(int)interdesc->bNumEndpoints);

			for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
				const struct libusb_endpoint_descriptor *epdesc = &interdesc->endpoint[k];
				printf("\t\t\t\tDesc Type:  %d\n",(int)epdesc->bDescriptorType);
				printf("\t\t\t\tEP Address: 0x%x\n",(int)epdesc->bEndpointAddress);
			}
		}
	}

	libusb_free_config_descriptor(config);
}

int main(void)
{
	struct libusb_device *dev;
	libusb_hotplug_callback_handle hp[2];
	int rc = 0;

	rc = libusb_init(NULL);
	if (rc < 0) {
		printf("failed to initialise libusb: %s\n", libusb_error_name(rc));
		return -1;
	}

	devh = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
	if (!devh) {
		if (!libusb_has_capability(LIBUSB_CAP_HAS_HOTPLUG)) {
			printf("Hotplug capabilites are not supported on this platform\n");
			libusb_exit(NULL);
			return -1;
		}

		rc = libusb_hotplug_register_callback(NULL,
				LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED, LIBUSB_HOTPLUG_NO_FLAGS,
				LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY,
				usb_arrived_callback, NULL, &hp[0]);
		if (LIBUSB_SUCCESS != rc) {
			printf("Error registering callback 0: %s\n", libusb_error_name(rc));
			libusb_exit(NULL);
			return -1;
		}

		rc = libusb_hotplug_register_callback (NULL,
				LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, LIBUSB_HOTPLUG_NO_FLAGS,
				LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY,
				usb_detach_callback, NULL, &hp[1]);
		if (LIBUSB_SUCCESS != rc) {
			printf("Error registering callback 1: %s\n", libusb_error_name(rc));
			libusb_exit(NULL);
			return -1;
		}
		while (!devh) {
			libusb_handle_events_completed(NULL, NULL);
			usleep(10*1000);
		}
	}

	if(libusb_kernel_driver_active(devh, 1) == 1) {
		if(libusb_detach_kernel_driver(devh, 1) == 0) {
			//
		}
	}

	dev = libusb_get_device(devh);
	usb_dev_print(dev);

	libusb_hotplug_deregister_callback(NULL, hp[0]);
	libusb_hotplug_deregister_callback(NULL, hp[1]);
	libusb_exit(NULL);

	return 0;
}
