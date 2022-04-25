//
// Created by Denis Shevchenko on 20.04.2022.
//

#ifndef MIXERCORE_CRENDER_H
#define MIXERCORE_CRENDER_H


class CRender {
private:
    void Reshape(int width, int height);
    void  Display();
public:
    static void StartRender();

    CRender();

};


#endif //MIXERCORE_CRENDER_H
