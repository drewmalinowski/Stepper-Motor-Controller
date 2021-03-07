#include <plib.h>
#include <stdio.h>
#include "CerebotMX7cK.h"
#include "Stepper_Motor.h"

unsigned int buttons, dir, mode, sm_code, step_delay = 20, step_period = 20;

int main(void)
{
    initialize_system();
    while(1)
    {
        
    }
    return(1);
}

void initialize_system()
{
    unsigned int dummy;
    Cerebot_mx7cK_setup();
    PORTSetPinsDigitalOut(IOPORT_B, SM_LEDS);
    LATBCLR = SM_LEDS;
    OpenTimer1(T1_ON | T1_SOURCE_INT | T1_PS_1_1, (T1_TICK-1));
    mT1SetIntPriority(2);
    mT1SetIntSubPriority(0);
    mT1IntEnable(1);
    
   
    
    mCNOpen(CN_ON, (CN8_ENABLE | CN9_ENABLE), 0);
    mCNSetIntPriority(1);
    mCNSetIntSubPriority(0);
    dummy = PORTReadBits(IOPORT_G, BTN1 | BTN2);
    mCNIntEnable(1);
    
    INTEnableSystemMultiVectoredInt();
    INTEnableInterrupts();
}

void __ISR(_CHANGE_NOTICE_VECTOR, IPL1) CNIntHandler(void)
{
    LATBSET = LEDC;
    sw_msDelay(20);
    buttons = read_buttons();
    decode_buttons();
    LATBCLR = LEDC;
    mCNClearIntFlag();    
}

void __ISR(_TIMER_1_VECTOR, IPL2) Timer1Handler(void)
{
    LATBINV = LEDA;
    step_delay = step_delay - 1;
    if (step_delay == 0)
    {
        
    sm_code = stepper_state_machine(dir, mode);
    output_to_stepper_motor(sm_code);
    step_delay = step_period;   
    }
    mT1ClearIntFlag();   
}

int read_buttons(void)
{
    int buttons;
    buttons = PORTG & (BTN1 | BTN2);
    return buttons;
}

int decode_buttons(void)
{
    switch(buttons)
    {
        case BTN1 :
            dir = CCW;
            mode = FS;
            step_period = 40;
            break;
            
        case BTN2 :
            dir = CW;
            mode = HS;
            step_period = 20;
            break;
        
        case (BTN1 & BTN2) :
            dir = CCW;
            mode = HS;
            step_period = 30;
            break;
            
        case (BTN1 | BTN2) :
            dir = CW;
            mode = FS;
            step_period = 24;
            break;    
    } 
}

int stepper_state_machine(unsigned int dir, unsigned int mode)
{
    enum {S0_5, S1, S1_5, S2, S2_5, S3, S3_5, S4};
    static unsigned int State = 0;
    const unsigned int sm_code[] = {0x0A, 0x08, 0x09, 0x01, 0x05, 0x04, 0x06, 0x02};
    
    switch(State)
    {
        case S1:
            if (dir == CW)
            {
                if (mode == HS) State = S1_5;
                else State = S2;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S0_5;
                else State = S4;
            }
            break;
            
        case S2:
            if (dir == CW)
            {
                if (mode == HS) State = S2_5;
                else State = S3;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S1_5;
                else State = S1;
            }
            break;
            
        case S3:
            if (dir == CW)
            {
                if (mode == HS) State = S3_5;
                else State = S4;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S2_5;
                else State = S2;
            }
            break;
        
        case S4:
            if (dir == CW)
            {
                if (mode == HS) State = S0_5;
                else State = S1;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S3_5;
                else State = S3;
            }
            break;
            
        case S0_5:
            if (dir == CW)
            {
                if (mode == HS) State = S1;
                else State = S1_5;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S4;
                else State = S3_5;
            }
            break;
            
        case S1_5:
            if (dir == CW)
            {
                if (mode == HS) State = S2;
                else State = S2_5;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S1;
                else State = S0_5;
            }
            break;
            
        case S2_5:
            if (dir == CW)
            {
                if (mode == HS) State = S3;
                else State = S3_5;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S2;
                else State = S1_5;
            }
            break;
            
        case S3_5:
            if (dir == CW)
            {
                if (mode == HS) State = S4;
                else State = S0_5;
            }
            else if (dir == CCW)
            {
                if (mode == HS) State = S3;
                else State = S2_5;
            }
            break;
    }
    return sm_code[State];
}

void output_to_stepper_motor(unsigned int sm_code)
{
    unsigned int temp;
    temp = LATB;
    temp = temp & (~BitMask);
    temp = temp | (sm_code << 7);
    LATB = temp;   
}

/*

void Timer1_delay(int *step_delay, int *button_delay)
{
    while(!mT1GetIntFlag());
    mT1ClearIntFlag();
    // LATBINV = LEDC;
    *button_delay = *button_delay - 1;
    *step_delay = *step_delay - 1;
}

 */

void sw_msDelay(unsigned int mS)
{
    int i;
    while (mS --)
    {
        for (i = 0; i < COUNTS_PER_MS; i++)
        {
            // do nothing
        }
        LATBINV = LEDA;
    }
}
