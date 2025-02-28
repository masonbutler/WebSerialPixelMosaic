#ifndef USBREQUESTS_H_
#define USBREQUESTS_H_
#include "Standard/UsbStandard.h"
#include "UsbDevice.h"
#include <stdbool.h>

void UsbHandleRequest(sUsbDevice *usbDevice, UsbRequest *request);



#endif