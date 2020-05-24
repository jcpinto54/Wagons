//
// Created by joao on 23/05/20.
//

#ifndef MEATWAGONS_PRISIONER_H
#define MEATWAGONS_PRISIONER_H


#include <Local/Local.h>
#include <Local/POI.h>

enum TransportType {
    PRISON_TRANSFER, ATTEND_COURT, POLICE_PRISON, COMMUNITY
};

class Prisioner {
    unsigned id;
    static unsigned nextId;
    POI *start;
    POI *end;
    TransportType type;
    bool ready = false;
public:
    Prisioner(POI *start, POI *end, TransportType type);

    bool isReady() const;

    POI *getStart() const;

    POI *getEnd() const;

    string getType() const;

    unsigned int getId() const;

    void setReady(bool ready);
};


#endif //MEATWAGONS_PRISIONER_H
