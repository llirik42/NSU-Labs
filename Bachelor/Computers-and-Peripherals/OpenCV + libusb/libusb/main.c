#include <libusb.h>
#include <stdio.h>

char* get_type_of_device(unsigned char device_class){
    switch(device_class){
        case 0x1:
            return "Audio";
        case 0x2:
            return "Communications and CDC Control";
        case 0x3:
            return "HID (Human Interface Device)";
        case 0x5:
            return "Physical";
        case 0x6:
            return "Image";
        case 0x7:
            return "Printer";
        case 0x8:
            return "Mass storage";
        case 0x9:
            return "Hub";
        case 0xA:
            return "CDC-Data";
        case 0xB:
            return "Smart Card";
        case 0xD:
            return "Content security";
        case 0xE:
            return "Video";
        case 0xF:
            return "Personal healthcare";
        case 0x10:
            return "Audio/Video device";
        case 0x11:
            return "Billboard device";
        case 0x12:
            return "USB Type-c bridge";
        case 0x3C:
            return "I3C devie";
        case 0xDC:
            return "Diagnostic device";
        case 0xE0:
            return "Wireless controller";
        case 0xEF:
            return "Miscellaneous";
        case 0xFE:
            return "Application specific";
        case 0xFF:
            return "Vendor specific";
        default:
            return "Unknown";
    }
}

void printdev(libusb_device *device){
    struct libusb_device_descriptor device_descriptor;
    struct libusb_config_descriptor* config_desciptor; 

    // 0 - success, LIBUSB_ERROR - failure
    int code1 = libusb_get_device_descriptor(device, &device_descriptor);
    if (code1){
        printf("Error, couldn't get descriptor of a device\n\n");
        return;
    }

    // 0 - Success, LIBUSB_ERROR_NOT_FOUND or LIBUSB_ERROR - failure    
    int code2 = libusb_get_config_descriptor(device, 0, &config_desciptor); // 0 - the index of the configuration to retrieve
    if (code2){
        printf("Error, couldn't get config descriptor\n\n");
        return;
    }

    struct libusb_device_handle* device_handle;
    
    libusb_open(device, &device_handle);

    printf("%-25s%s\n", "Type", get_type_of_device(device_descriptor.bDeviceClass));
    printf("%-25s%02Xh\n", "Class", device_descriptor.bDeviceClass);
    printf("%-25s%02Xh\n", "Subclass", device_descriptor.bDeviceSubClass);
    printf("%-25s%02Xh\n", "Protocol", device_descriptor.bDeviceProtocol);
    printf("%-25s%u\n", "Vendor ID", device_descriptor.idVendor);
    printf("%-25s%u\n", "Product ID", device_descriptor.idProduct);

    char i1 = device_descriptor.iManufacturer;
    char i2 = device_descriptor.idProduct;
    char i3 = device_descriptor.iSerialNumber;

    unsigned char buffer1[1000] = {'\0'};
    int c1 = libusb_get_string_descriptor_ascii(device_handle, i1, buffer1, 1000);
    printf("%-25s", "Manufacturer");
    if (c1 > 0){
        buffer1[c1] = '\0';
        printf("%s", buffer1);
    }
    printf("\n");

    unsigned char buffer2[1000] = {'\0'};
    int c2 = 0;
    c2 = libusb_get_string_descriptor_ascii(device_handle, i2, buffer2, 1000);
    printf("%-25s", "Product");
    if (c2 > 0){
        buffer2[c2] = '\0';
        printf("%s", buffer2);
    }
    printf("\n");

    unsigned char buffer3[1000] = {'\0'};
    int c3 = 0;
    c3 = libusb_get_string_descriptor_ascii(device_handle, i3, buffer3, 1000);
    printf("%-25s", "Serial number");
    if (c3 > 0){
        buffer3[c3] = '\0';
        printf("%s", buffer3);
    }
    printf("\n");

    if (device_descriptor.bDeviceClass == 0){
        for(unsigned int i = 0; i < (unsigned int)config_desciptor->bNumInterfaces; ++i){
            const struct libusb_interface* interface = &config_desciptor->interface[i];
            for(unsigned int j = 0; j < interface->num_altsetting; ++j){
                const struct libusb_interface_descriptor* interface_descriptor = &interface->altsetting[j];

                printf("%-25s%s\n", "Interface class", get_type_of_device(interface_descriptor->bInterfaceClass));
            }
        }
    }

    libusb_close(device_handle);

    printf("\n\n");

    libusb_free_config_descriptor(config_desciptor);
}

int main(){
    // https://www.usb.org/defined-class-codes

    struct libusb_context* ctx = NULL; // Context of session

    // Init libusb and start session
    // 0 - success, LIBUSB_ERROR - failure
    int init_code = libusb_init(&ctx);
    
    if (init_code){
        printf("Error, initialization didn't succeed\n");
        return 1;
    }

    struct libusb_device** devices; // all found devices
                
    libusb_set_option(ctx, LIBUSB_OPTION_LOG_LEVEL, LIBUSB_LOG_LEVEL_WARNING); // Sets level of log messages

    unsigned int devices_counter = libusb_get_device_list(ctx, &devices);
    
    if(devices_counter <= 0){
        printf("Error, couldn't get devices list\n");
        return 1;
    }

    printf("Found devices: %u\n\n", devices_counter);
    
    for(unsigned int i = 0; i < devices_counter; ++i){
        printdev(devices[i]);
    }
    
    libusb_free_device_list(devices, 1); // 1 - whether to unref the devices in the list
    
    libusb_exit(ctx);

    return 0;
}
