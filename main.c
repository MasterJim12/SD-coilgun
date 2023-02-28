#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"
#include "inc/hw_ints.h"
#include "driverlib/can.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"
#include "driverlib/adc.h"
#include "driverlib/rom.h"

void delayMicroSeconds(uint32_t ui32Ms){
    SysCtlDelay(ui32Ms * (SysCtlClockGet() / 3 / 1000000));
}
void triggerInterrupt(void){//On trigger pull
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_6);//Clear the interrupt from pin 6
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 1); //Turn on Coil 1

    //Disable this interrupt till projectile is out of barrel
    GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_6);
}
void sensor1Interrupt(void){
    //What to do for this interrupt
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_1);//Clear the interrupt from pin 1
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0); //Turn off Coil 1
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 1); //Turn on Coil 2
}
void sensor2Interrupt(void){
    //What to do for this interrupt
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_2);//Clear the interrupt from pin 2
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0); //Turn off Coil 2
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 1); //Turn on Coil 3
}
void sensor3Interrupt(void){
    //What to do for this interrupt
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_3);//Clear the interrupt from pin 3
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0); //Turn off Coil 3
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 1); //Turn on Coil 4
}
void sensor4Interrupt(void){
    //What to do for this interrupt
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_4);//Clear the interrupt from pin 4
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0); //Turn off Coil 4
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 1); //Turn on Coil 5
}
void sensor5Interrupt(void){
    //What to do for this interrupt
    GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_5);//Clear the interrupt from pin 5
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0); //Turn off Coil 5
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 1); //Turn on Coil 6
    delayMicroSeconds(200);//Delay .2ms
    GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0);//Turn off Coil 6

    //Remember to re enable trigger interrupt


}
int main(void) {
    //Set clock speed to 80Mhz
    SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    //GPIO Port A Setup Inputs
    //Enable Clock for peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    //Wait for port to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA))
    {
    }
    //Set Pins to Input
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6); // 5 Input Sensors
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);// Adds pull up resistor to pins as specified by sensor

    //Register Interrupts at the pin level
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_1|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6); //Allow all our inputs to trigger an interrupt
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_LOW_LEVEL|GPIO_DISCRETE_INT);// Sets all of our interrupts to trigger when pin is pulled Low. Since we have a pull up resistor

    GPIOIntRegisterPin(GPIO_PORTA_BASE, GPIO_PIN_6, triggerInterrupt);// Configures Port A pin 6 interrupt, gives function that will handle the interrupt
    GPIOIntRegisterPin(GPIO_PORTA_BASE, GPIO_PIN_5, sensor5Interrupt);//
    GPIOIntRegisterPin(GPIO_PORTA_BASE, GPIO_PIN_4, sensor4Interrupt);//
    GPIOIntRegisterPin(GPIO_PORTA_BASE, GPIO_PIN_3, sensor3Interrupt);//
    GPIOIntRegisterPin(GPIO_PORTA_BASE, GPIO_PIN_2, sensor2Interrupt);//
    GPIOIntRegisterPin(GPIO_PORTA_BASE, GPIO_PIN_1, sensor1Interrupt);//

    //GPIO Port B Setup Outputs
    //Enable Clock for peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //Wait for port to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }
    //Set Pins to Output
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6); // 6 Output Gate Driver
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);// We want to make sure there is no voltage at the gate when we dont want it there.

    while(true){
      //Stay running
    }

    return 0;
}
