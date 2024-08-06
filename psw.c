/*
* File:   psw.c
* Description: File containing the functions to update the PSW flags
* Author: Abdulla Sadoun
*
* - Date: 04/08/2024
*
*/

#include "xm23p.h"

void updatePSW(unsigned int src, unsigned int dst, unsigned int tempResult, int wbbuff){
/*
function to update the PSW flags
- it updates the PSW flags based on the result of the operation
- it updates the Zero (Z) flag
- it updates the Negative (N) flag
- it updates the Carry (C) flag
- it updates the Overflow (V) flag
*/
    // Clear the PSW flags
     PSW.z = CLEAR;
     PSW.n = CLEAR;
     PSW.c = CLEAR;
     PSW.v = CLEAR;// check if this should be cleared too
     
     // Check if the operation was on a byte or word
     int mask = (wbbuff == WORD) ? 0xFFFF : 0xFF;

    // Update the Zero (Z) flag
     if ((tempResult & mask) == CLEAR) {
          PSW.z = SET;
    }

     // Update the Negative (N) flag
    if ((tempResult & (mask + SET) >> SET) != CLEAR) {
        PSW.n = SET;
    }

    // Update the Carry (C) flag - In the context of CMP, carry flag is set if src < dst
    if (src < dst) {
        PSW.c = SET;
    }
    
}