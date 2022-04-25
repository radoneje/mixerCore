//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H


class CRender {
private:
    static void Reshape(int width, int height);
    static void  Display();
public:
    static void StartRender(int argc, char **argv);

    CRender();

};


#endif //MIXERCORE_CRENDER_H
