//
// Created by lmy on 2023/10/14.
//

#include "FkTime.h"

FK_IMPL_CLASS_TYPE(FkTime, FkObject)

FkTime FkTime::zero() {
    return makeUS(0);
}

FkTime FkTime::makeUS(int64_t us) {
    return {us, {1, 1}};
}

FkTime::FkTime(int64_t value, FkRational timeBase) : FkObject(), value(value), timeBase(timeBase) {

}

FkTime::FkTime(const FkTime &o) : FkObject(), value(o.value), timeBase(o.timeBase) {

}

FkTime::~FkTime() {

}

int64_t FkTime::get(kUnit unit) {
    switch (unit) {
        case kUnit::NS:
            return value * timeBase.num / timeBase.den * 1000;
        case kUnit::US:
            return value * timeBase.num / timeBase.den;
        case kUnit::MS:
            return value * timeBase.num / timeBase.den / 1000;
        case kUnit::SEC:
            return value * timeBase.num / timeBase.den / 1000000;
        case kUnit::ORIGIN:
        default:
            return value;
    }

}