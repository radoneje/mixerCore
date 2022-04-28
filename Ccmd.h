//
// Created by Denis Shevchenko on 28.04.2022.
//

#ifndef MIXERCORE_CCMD_H
#define MIXERCORE_CCMD_H
#include <vector>
#include <mutex>


class Ccmd {

    public:
        Ccmd();
        std::vector<int> activeTextureId;
        std::mutex locker;
};


#endif //MIXERCORE_CCMD_H
