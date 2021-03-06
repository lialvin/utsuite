// Copyright (c) 2016-2018 The Ulord Core Foundation
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

#include "PoW.h"
#include "common.h"
#include "my_rand48_r.h"
#include "oneWayFunction.h"

int main(int argc, const char *argv[]) {

	

	uint8_t input[256]={"0000002084b3706ff179c104f6fc08657288ea326648a1816601446c71a9a821000000001290a45b26bf99a93bfca631bd8c1a7cb2ee1c31c7d335826fa73150c534046b9b52d5b1fe5211c00000000ef94de24342d74a9bd520ff41eb456b2da3fe1b6ec1fe1d985af2018"};
	uint8_t output[256]={""};
	uint8_t * Maddr= malloc( 5*1024*1024);
	//memset(input, 0, 256*sizeof(uint8_t));
	//memcpy(input, mess, messLen*sizeof(char));
	
	// Test for oneWayFunction
    //	initOneWayFunction();
	//testOneWayFunction(mess, messLen, 50000);
    
   // void powFunction(uint8_t *input, uint32_t inputLen, uint8_t *Maddr, uint8_t *output);
        powFunction(input, 128, Maddr, output); 

	view_data_u8("PoW", output, 32); 
        
//	testPowFunction(input, messLen, 5000);
	
	// powNistTest("./powNistTest");
	
	/* uint8_t Maddr[WORK_MEMORY_SIZE], output[OUTPUT_LEN];
	memset(Maddr, 0, WORK_MEMORY_SIZE*sizeof(uint8_t));
	
	for (int i = 0; i < 100; ++i)
		powFunction(input, messLen, Maddr, output);
	view_data_u8("PoW", output, OUTPUT_LEN); */
	
	return 0;
}
