//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H


class CRender {
private:
    static void Reshape(int width, int height);
    static void  Display();
    static const float fMinX = -5.0, fMinY = -5.0, fNearZ = 1.0,
            fMaxX = 5.0, fMaxY = 5.0, fFarZ = 10.0;
public:
    static void StartRender(int argc, char **argv);

    CRender();

};


#endif //MIXERCORE_CRENDER_H
