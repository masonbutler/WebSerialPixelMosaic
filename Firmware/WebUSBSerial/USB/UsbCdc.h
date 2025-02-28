#ifndef USBCDC_H_
#define USBCDC_H_
#include <stdbool.h>
#include "Standard/UsbCdcStandard.h"
#include "UsbDevice.h"

void UsbCdcInit();
void UsbCdcReset();
//void UsbCdcSendSerialStateNotification(UsbDevice *device);
void UsbCdcSetLineCoding(uint8_t *data, uint32_t size);

UsbCdcLineCoding *GetLineCoding();


#endif