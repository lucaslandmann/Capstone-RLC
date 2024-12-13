#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void newButtonValue(bool button){}

    virtual void toggleLed(char selection){}

    virtual void newCH1ReverbToggleValue(bool button){}

    virtual void toggleDelay(int selection) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
