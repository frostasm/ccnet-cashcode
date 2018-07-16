#ifndef TIMINGSPECIFICATIONS_H
#define TIMINGSPECIFICATIONS_H

#include <boost/chrono.hpp>

namespace TimingSpecifications {
const auto TInterByteMax = boost::chrono::milliseconds(5);
const auto TResponseMax = boost::chrono::milliseconds(10);
const auto TBusResetMin = boost::chrono::milliseconds(100);
const auto TNonResponseMax = boost::chrono::seconds(5);
const auto TPoll = boost::chrono::milliseconds(100);

// Currently t-free is defined as 10 mS of Bus silence, but for reliable
// operation of future multi-device buses the recommended value of t-free is 20mS.
const auto TFree = boost::chrono::milliseconds(20);
}

#endif // TIMINGSPECIFICATIONS_H
