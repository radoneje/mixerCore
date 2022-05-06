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
        void loadPresImage(std::string filepath);
        void clearPresImage();
        unsigned char* PresImagePixels;
        int PresImageWidth;
        int PresImageHeight;
        int PresImageNumChannels;
};


#endif //MIXERCORE_CCMD_H
