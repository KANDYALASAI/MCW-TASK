#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define VENDOR_ID 0x2717  // Replace with your device's Vendor ID
#define PRODUCT_ID 0xff40 // Replace with your device's Product ID

static struct usb_device_id simple_usb_table[] = {
    { USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
    {}
};

MODULE_DEVICE_TABLE(usb, simple_usb_table);

static int simple_usb_probe(struct usb_interface *interface, const struct usb_device_id *id) {
   struct usb_device *usb_dev = interface_to_usbdev(interface);

    printk(KERN_INFO "Simple USB Driver: Device is connected\n");
    printk(KERN_INFO "Device Class: %02x\n", usb_dev->descriptor.bDeviceClass);
    printk(KERN_INFO "Device Vendor ID: %04x\n", usb_dev->descriptor.idVendor);
    printk(KERN_INFO "Device Product ID: %04x\n", usb_dev->descriptor.idProduct);

    // Perform additional setup here
    return 0; // Return 0 to indicate successful probe
}


static void simple_usb_disconnect(struct usb_interface *interface) {
    printk(KERN_INFO "Simple USB Driver: Device is disconnected\n");
}

static struct usb_driver simple_usb_driver = {
    .name = "simple_usb_driver",
    .id_table = simple_usb_table,
    .probe = simple_usb_probe,
    .disconnect = simple_usb_disconnect,
};

static int __init simple_usb_init(void) {
    return usb_register(&simple_usb_driver);
}

static void __exit simple_usb_exit(void) {
    usb_deregister(&simple_usb_driver);
}

module_init(simple_usb_init);
module_exit(simple_usb_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kandyala sai kumar");
MODULE_DESCRIPTION("A Simple USB Device Driver");

