#ifndef IEVENTSLISTENER_H
#define IEVENTSLISTENER_H

#include "commands.h"

#include <string>

class IBillValidatorEventsListener {
public:
    virtual ~IBillValidatorEventsListener() {}

    virtual void billValidatorReceptionStarted() = 0;
    virtual void billValidatorReceptionStopped() = 0;
    virtual void billValidatorErrorOccured(const std::string& error) = 0;
    virtual void billValidatorBillAccepted(int denomination) = 0;
    virtual void billValidatorBillRejected(int denomination) = 0;
};

#endif // IEVENTSLISTENER_H
