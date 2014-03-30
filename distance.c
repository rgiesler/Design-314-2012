#include "sfr_r827.h"

distance_init()
{
    int3ic = 0x07;
    int3en = 1;
	int3f0 = 1;
	int3f1 = 1;
}