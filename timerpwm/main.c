#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/debug.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "inc/hw_gpio.h"
#include "driverlib/rom.h"
#define PWM_FREQUENCY 55
volatile uint8_t mode=0,flag1=0, flag2=0, count=0;
int main(void)
{
	volatile uint32_t ui32Load;
	volatile uint32_t ui32PWMClock;
	volatile uint8_t ui8Adjust;
	ui8Adjust = 1;
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_OSC_MAIN|SYSCTL_XTAL_16MHZ);
	SysCtlPWMClockSet(SYSCTL_PWMDIV_64);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	ROM_GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);
	ROM_GPIOPinConfigure(GPIO_PF1_M1PWM5);
	ROM_GPIOPinConfigure(GPIO_PF2_M1PWM6);
	ROM_GPIOPinConfigure(GPIO_PF3_M1PWM7);

	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

	ROM_GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	//gen 2 out 5 for red, gen 3 out 6 for blue, gen 3 out 7 for green
	ui32PWMClock = SysCtlClockGet() / 64;
	ui32Load = (ui32PWMClock / PWM_FREQUENCY) - 1;
	//red
	PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, ui32Load);
	//blue
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
	//green
	PWMGenConfigure(PWM1_BASE, PWM_GEN_3, PWM_GEN_MODE_DOWN);
	PWMGenPeriodSet(PWM1_BASE, PWM_GEN_3, ui32Load);
	//red
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 1);
	PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_2);
	//blue
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, 1);
	PWMOutputState(PWM1_BASE, PWM_OUT_6_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	//green
	PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7, 1);
	PWMOutputState(PWM1_BASE, PWM_OUT_7_BIT, true);
	PWMGenEnable(PWM1_BASE, PWM_GEN_3);
	while(1)
	{
		//mode selection
		/*if(mode!=0 && (ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00 || ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)){
					//SysCtlDelay(10000000);
					if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00 && ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
						mode=3;//green
						SysCtlDelay(40000000);
					}

				}*/

		/*if(mode!=1 && ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
			SysCtlDelay(10000000);
			if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
				if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
						mode=1;//red
						SysCtlDelay(4000000);

				}

		}*/
		//////////////red
		if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00 && flag2==0){
			if(flag1==1){

				if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00){
					mode=2;//mode 2 set
					flag1=0;
				}
				else //if opposite button not pressed yet then cancel mode change
					flag1=0;
				SysCtlDelay(4000000);

			}
			else {
				//increase intensity
				count++;
				SysCtlDelay(3000000);
				//continue;
			}
			if(mode!=1){
				SysCtlDelay(4000000);
				if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
					flag2=1; //button2 pressed
					continue;
				}
			}

		}

		if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00 && flag1==0){
			if(flag2==1){
				if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
					mode=1;//mode 1 set
					flag2=0;
				}
				else
					flag2=0;
				SysCtlDelay(4000000);
				continue;
			}
			else{
				//decrease intensity
				count--;
				SysCtlDelay(3000000);
				//continue;
			}
			if(mode!=2){
				SysCtlDelay(4000000);
					if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
						flag1=1; //button2 pressed
			}

		}
		///////////////////blue
		/*if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00 && flag1==0){
			if(mode!=2){
				SysCtlDelay(4000000);
				flag1=1;
			}
			else{
				//decrease intensity
			}
		}
		if(flag1==1){
			if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
				mode=2;
				flag1=0;
				SysCtlDelay(4000000);
			}
		}*/
		///////////////////green
		/*if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00 && flag2==0){
			if(mode!=2){
				SysCtlDelay(4000000);
				flag2=1;
			}
			else{

			}
		}
		if(flag2==1){
			if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00){
				mode=2;
				flag2=0;
				SysCtlDelay(4000000);
			}
		}*/


		/*if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_4)==0x00)
		{

			ui8Adjust--;
			if (ui8Adjust < 1)
			{
				ui8Adjust = 1;
			}
			ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);
		}
		if(ROM_GPIOPinRead(GPIO_PORTF_BASE,GPIO_PIN_0)==0x00)
		{
			ui8Adjust++;
			if (ui8Adjust > 254)
			{
				ui8Adjust = 254;
			}
			ROM_PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6, ui8Adjust * ui32Load / 1000);
		}*/
		//SysCtlDelay(100000);
	}
}
