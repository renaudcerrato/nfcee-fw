#include "trf797x.h"

void trf797x_initiator_init(Trf797xInitiatorDriver *driver) {
    trf797x_init((Trf797xDriver *) driver);
    //TODO
}

void trf797x_initiator_start(Trf797xInitiatorDriver *driver, const Trf797xInitiatorConfig *config) {
    trf797x_start((Trf797xDriver *) driver, (const Trf797xConfig *) config);
    //TODO
}

int trf797x_initiator_transceive(Trf797xInitiatorDriver *driver, struct trf797x_transfer *tr) {
    //TODO
    return 0;
}

void trf797x_initiator_stop(Trf797xInitiatorDriver *driver) {
    //TODO
    trf797x_stop((Trf797xDriver *) driver);
}