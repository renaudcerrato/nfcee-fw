//
// Created by Jocelyn MASSEROT on 5/4/17.
//

#ifndef NFCEE_TRACE_H
#define NFCEE_TRACE_H

#include "chprintf.h"

void trace_init(void);

#if defined(USB_CDC_TRACE)
#define trace(x...)			if(serusbcfg.usbp->state == USB_ACTIVE)	{ \
								chprintf((BaseSequentialStream *)&TRACE_SD, x); \
							}
#else
#define trace(x...)       chprintf((BaseSequentialStream *)&TRACE_SD, x)
#endif //USB_CDC_TRACE

#endif //NFCEE_TRACE_H
