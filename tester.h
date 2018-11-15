// *************************
// ***** tester.h file *****
// *************************

#include <afxwin.h>
#pragma warning( disable: 4244 )	// conversion from const double to float

#include "TestProgApp/public.h"

#include "pin_lists.h"

#define PASS TRUE
#define FAIL FALSE

#define NORMAL_DPS 3.3

/*
#define X_Start 256
#define X_Step 1
#define X_Stop 265

#define Y_Start 137
#define Y_Step 1
#define Y_Stop 146
*/


#define X_Start 36
#define X_Step 32
#define X_Stop 1024

#define Y_Start 20
#define Y_Step 8
#define Y_Stop 256


#define X_Start_Verify 200
#define X_Step_Verify 1
#define X_Stop_Verify 232

#define Y_Start_Verify 100
#define Y_Step_Verify 1
#define Y_Stop_Verify 132





//#define normal_vcc 3.3
//int rst_shmoo_cycle_test_step