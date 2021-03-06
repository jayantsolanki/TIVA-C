#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.c"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"

#define LED_PERIPH SYSCTL_PERIPH_GPIOF
#define LED_BASE GPIO_PORTF_BASE
#define RED_LED GPIO_PIN_1
#define BLUE_LED GPIO_PIN_2
#define GREEN_LED GPIO_PIN_3

#define Button_PERIPH SYSCTL_PERIPH_GPIOF
#define ButtonBase GPIO_PORTF_BASE
#define Button1 GPIO_PIN_4
#define Button2 GPIO_PIN_0
// LOCK_F and CR_F - used for unlocking PORTF pin 0
#define LOCK_F (*((volatile unsigned long *)0x40025520))
#define CR_F   (*((volatile unsigned long *)0x40025524))

  uint32_t value1=0,value2=0;
  uint8_t status1=0, status2=0;
  uint8_t sw1status=0;
  uint8_t sw2status=0;

/*

* Function Name: switchPinConfig()

* Input: none

* Output: none

* Description: Set PORTF Pin 0 and Pin 4 as input. Note that Pin 0 is locked.

* Example Call: switchPinConfig();

*/
void switchPinConfig(void)
{
	// Following two line removes the lock from SW2 interfaced on PORTF Pin0 -- leave this unchanged
	LOCK_F=0x4C4F434BU;
	CR_F=Button1|Button2;

	// GPIO PORTF Pin 0 and Pin4
	GPIODirModeSet(GPIO_PORTF_BASE,Button1,GPIO_DIR_MODE_IN); // Set Pin-4 of PORT F as Input. Modifiy this to use another switch
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, Button1);
	GPIOPadConfigSet(GPIO_PORTF_BASE,Button1,GPIO_STRENGTH_12MA,GPIO_PIN_TYPE_STD_WPU);
	GPIODirModeSet(GPIO_PORTF_BASE,Button2,GPIO_DIR_MODE_IN); // Set Pin-4 of PORT F as Input. Modifiy this to use another switch
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, Button2);
	GPIOPadConfigSet(GPIO_PORTF_BASE,Button2,GPIO_STRENGTH_12MA,GPIO_PIN_TYPE_STD_WPU);
}

int main(void)
{

  SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);

  SysCtlPeripheralEnable(LED_PERIPH);
  SysCtlDelay(3);
  switchPinConfig();
  //GPIOPinTypeGPIOInput(ButtonBase, Button);
  //GPIOPadConfigSet(ButtonBase ,Button,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);

  GPIOPinTypeGPIOOutput(LED_BASE, RED_LED|BLUE_LED|GREEN_LED);

  while(1){
    value1= GPIOPinRead(ButtonBase,Button1);
    value2= GPIOPinRead(ButtonBase,Button2);
    if(value1 ==0 && status1==0){ //for glowing LED
    	sw1status++;
    	if(sw1status==1)
        	GPIOPinWrite(LED_BASE,RED_LED, 2);
    	else if(sw1status==2)
    		GPIOPinWrite(LED_BASE,BLUE_LED, 4);
    	else if(sw1status==3)
    		GPIOPinWrite(LED_BASE,GREEN_LED, 8);
    	else if(sw1status==4){
    		sw1status=0;
    		continue;
    	}
    	status1=1;

    }
    else if(value1==0x10){
    	GPIOPinWrite(LED_BASE,RED_LED|BLUE_LED|GREEN_LED, 0);
    	status1=0;
    }

    	if((value2 & Button2)==0 && status2==0){//for SW2 button
        	sw2status++;
        	if(sw2status==4){
        		sw2status=0;
        		continue;
        	}
        	status2=1;

        }
        else if((value2 & Button2)!=0){
        	status2=0;
        }

    //GPIOPinWrite(LED_BASE,RED_LED, state);
    SysCtlDelay(1000000);
  }

}
