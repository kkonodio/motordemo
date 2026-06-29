#include "key.h"
#include <reg52.h>
sbit KEY_STOP = P1^7; 
sbit KEY_ADD  = P1^6; 
void key_init(void)
{
    KEY_STOP = 1;
    KEY_ADD  = 1;
}

unsigned char key_read(void)
{
    static unsigned char last = 0;
    static unsigned char cnt  = 0;
    static unsigned char key_released = 1;  
    unsigned char cur = 0;
    KEY_STOP = 1;
    KEY_ADD  = 1;
    if (KEY_STOP == 0) cur = 1;
    if (KEY_ADD  == 0) cur = 2;
    if (cur == last && cur != 0)
    {
        cnt++;
        if (cnt >= 3 && key_released)  
        {
            key_released = 0;
            cnt = 0;                 
            return cur;
        }
    }
    else
    {
        cnt = 0;
        last = cur;
        if (cur == 0)
            key_released = 1;          
    }
    return 0;
}