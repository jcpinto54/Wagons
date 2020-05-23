#pragma once

#include <ostream>
#include "Local/Local.h"
#include "GraphTemplate/Graph.h"
#include "Date/Time.h"

class Wagon {

    double avgSpeed;
public:
    Wagon(double avgSpeed);

    double getAvgSpeed() const;

    // Acerca do valor de retorno:
    // unsigned - número de dias que demora percorrer a distância
    // Time - tempo que demora percorrer a distância
    // Para obter tempo total é necessário fazer algo do género: Time + unsigned
    pair<Time, unsigned> distToTime(double dist);
};
