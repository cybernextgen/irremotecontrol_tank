# IR remote control tank

This project is simple implementation of IR remote control caterpillar chassis.

![Overview](/img/1.jpg)

You can use any popular IR-reciever and IR-controller. Just bind key codes from your controller to chassis actions in `handleIR` function.

My setup is: arduino uno, Keyes_L298P motor shield and LFN-TY IR-reciever.

As `irremote` library uses hardware PWM, remains only one PWM for motor shield communication, thus there is no possibility to control motors speed independant. You need to cut pin PWMA of motor shield and connect that input with PWMB as shown below.

![Schematic](/img/2.png)

