#include "main.h"

bool bitmask32_has_flag(uint param, uint flag) {
	return ((param & flag) > 0);
}


uint bitmask32_set_flags(uint n, ... ) {
	va_list ptr;
    va_start(ptr, n);
	bitmask32_t	mask = 0;

    for (int i = 0; i < n; i++)
        mask += va_arg(ptr, uint);
 
    va_end(ptr);
    return mask;
}

// source: 
// https://stackoverflow.com/questions/109023/count-the-number-of-set-bits-in-a-32-bit-integer
int bitmask32_count_bits(bitmask32_t i)
{
     i = i - ((i >> 1) & 0x55555555);        
     i = (i & 0x33333333) + ((i >> 2) & 0x33333333);  
     i = (i + (i >> 4)) & 0x0F0F0F0F;        
     i *= 0x01010101;                        
     return  i >> 24;               
}


void bitmask32_add_flag(bitmask32_t* mask, uint flag) {
	assert(bitmask32_count_bits(flag) == 1 && 
			"Flag cannot be empty or bigger than one parameter at a time");
	if (!bitmask32_has_flag(*mask,flag)) {
		(*mask) += flag;
	}
}
