#ifndef CMOTOR_H
#define CMOTOR_H

#include <Arduino.h>

class CMotor {

    private:
        int freq;
        int pwmChannel;
        int resolution;
        int max_res;

        int motorPin_1; 
        int motorPin_2; 
        int enablePin;

    public:
        CMotor(int frequency, int channel, int resolution, int pin1, int pin2, int enable_pin);
        void init();
        void forward();
        void forward(int mills);
        void backward();
        void stop();
};

#endif