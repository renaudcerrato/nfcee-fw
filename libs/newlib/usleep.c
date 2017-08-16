#include <hal.h>
#include <unistd.h>

int usleep(useconds_t usec) {
    osalThreadSleepMicroseconds(usec);
    return 0;
}
