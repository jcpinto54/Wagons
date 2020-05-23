//
// Created by joao on 16/05/20.
//

#include "Wagon.h"
#include "Utilities/utils.h"
#include "Utilities/InvalidInput.h"

using namespace Util;

Wagon::Wagon(double avgSpeed) : avgSpeed(avgSpeed) {}


double Wagon::getAvgSpeed() const {
    return avgSpeed;
}
