#include <stdbool.h>
#include <stdint.h>
#include <math.h>

#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_can.h"

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
    GPIOPinTypeGPIOInput(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6); // 5 Input Sensors
    GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);// Adds pull up resistor to pins as specified by sensor

    //Register Interrupts at the pin level


    IntEnable(INT_GPIOA);
    GPIOIntTypeSet(GPIO_PORTA_BASE, GPIO_PIN_7|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6, GPIO_LOW_LEVEL);// Sets all of our interrupts to trigger when pin is pulled Low. Since we have a pull up resistor
    GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6); //Allow all our inputs to trigger an interrupt

    IntMasterEnable();

    //GPIO Port B Setup Outputs
    //Enable Clock for peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    //Wait for port to be ready
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB))
    {
    }
    //Set Pins to Output
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_6); // 6 Output Gate Driver
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_0|GPIO_PIN_5|GPIO_PIN_6, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPD);// We want to make sure there is no voltage at the gate when we dont want it there.

    while(true){
      //Stay running
    }

}
void triggerInterrupt(void){//
    if (GPIOIntStatus(GPIO_PORTA_BASE, false) & GPIO_PIN_6){//trigger
        GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_6);//Disable this interrupt till projectile is out of barrel
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 2); //Turn on Coil 1
        GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_7|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6);//Clear the interrupt from pins
    }
    else if (GPIOIntStatus(GPIO_PORTA_BASE, false) & GPIO_PIN_7){//Sensor 1
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_1, 0); //Turn off Coil 1
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 4); //Turn on Coil 2
        GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_7);//Disable this interrupt till projectile is out of barrel
        GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_7);//Clear the interrupt from pin 7
    }
    else if (GPIOIntStatus(GPIO_PORTA_BASE, false) & GPIO_PIN_2){//Sensor 2
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_2, 0); //Turn off Coil 2
        GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 8); //Turn on Coil 3
        GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_2);//Disable this interrupt till projectile is out of barrel
        GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_2);//Clear the interrupt from pin 2
    }
    else if (GPIOIntStatus(GPIO_PORTA_BASE, false) & GPIO_PIN_3){//Sensor 3
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0); //Turn off Coil 3
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 1); //Turn on Coil 4
            GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_3);//Disable this interrupt till projectile is out of barrel
            GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_3);//Clear the interrupt from pin 3
        }
    else if (GPIOIntStatus(GPIO_PORTA_BASE, false) & GPIO_PIN_4){//Sensor 4
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_0, 0); //Turn off Coil 4
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 32); //Turn on Coil 5
            GPIOIntDisable(GPIO_PORTA_BASE, GPIO_INT_PIN_4);//Disable this interrupt till projectile is out of barrel
            GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_4);//Clear the interrupt from pin 4
        }
    else if (GPIOIntStatus(GPIO_PORTA_BASE, false) & GPIO_PIN_5){//Sensor 5
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_5, 0); //Turn off Coil 5
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 64); //Turn on Coil 6
            delayMicroSeconds(50);
            GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_6, 0); //Turn on Coil 6
            GPIOIntEnable(GPIO_PORTA_BASE, GPIO_INT_PIN_7|GPIO_INT_PIN_2|GPIO_INT_PIN_3|GPIO_INT_PIN_4|GPIO_INT_PIN_5|GPIO_INT_PIN_6);//Enable this interrupt to launch another projectile
            GPIOIntClear(GPIO_PORTA_BASE, GPIO_INT_PIN_5);//Clear the interrupt from pin 5
        }



}

