#ifndef MODEL_HPP
#define MODEL_HPP

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    bool getButtonValue();
    bool getCH1ToggleValue();

    void toggleLed(char selection);

    void toggleDelay(int selection);

protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
