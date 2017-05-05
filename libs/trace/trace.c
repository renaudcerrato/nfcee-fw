//
// Created by Jocelyn MASSEROT on 5/5/17.
//
#include "hal.h"

#if defined(USB_CDC_TRACE)
#include "usbcfg.h"
#endif


void trace_init(void)
{
#if defined(USB_CDC_TRACE)
	/*
 	 * Initializes a serial-over-USB CDC driver.
 	 */
	sduObjectInit(&TRACE_SD);
	sduStart(&TRACE_SD, &serusbcfg);

	/*
 	 * Activates the USB driver and then the USB bus pull-up on D+.
 	 * Note, a delay is inserted in order to not have to disconnect the cable
 	 * after a reset.
 	 */
	usbDisconnectBus(serusbcfg.usbp);
	chThdSleepMilliseconds(1500);
	usbStart(serusbcfg.usbp, &usbcfg);
	usbConnectBus(serusbcfg.usbp);

#else
	sdStart(&TRACE_SD, NULL);
#endif
}

