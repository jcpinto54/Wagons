#pragma once

#include <ostream>
#include "Local/Local.h"
#include "GraphTemplate/Graph.h"

class Wagon {

    double avgSpeed;
public:
    Wagon(double avgSpeed);

    double getAvgSpeed() const;
};
