//
// Created by Denis Shevchenko on 23.04.2022.
//

#ifndef MIXERCORE_CHTTP_H
#define MIXERCORE_CHTTP_H

#include "Ccmd.h"


class CHttp {
    public:   CHttp();
    static void init(int port, Ccmd *pCmd );

};


#endif //MIXERCORE_CHTTP_H
