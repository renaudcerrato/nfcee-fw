//
// Created by Jocelyn MASSEROT on 5/4/17.
//

#ifndef NFCEE_TRACE_H
#define NFCEE_TRACE_H

#include "chprintf.h"

#define trace(x...)       chprintf((BaseSequentialStream *)&TRACE_SD, x)

#endif //NFCEE_TRACE_H
