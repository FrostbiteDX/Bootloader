#include <stdio.h>
#include "stm32loaderCWrapper.h"


int main()
{
    uint8_t result;
    result = stm32loaderCWrapper_stm32_init();
    
    if(1 == result)
    {
        printf("Everything's fine\n");
    }
    else
    {
        printf("Whop Whop ohhhhh\n");
    }
    
    return 0;
}
