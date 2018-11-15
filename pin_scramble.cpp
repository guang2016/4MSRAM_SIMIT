#include "tester.h"


PIN_SCRAMBLE( your_scramble_set_name )
{
	/*SCRAMBLE_MAP( PS1 )	
	{
		SCRAMBLE( CLK,			t_d0	)
	}*/

	SCRAMBLE_MAP( PS1 )	/*  Default pin scramble map  */
	{
		SCRAMBLE( A0,			t_x0	)
		SCRAMBLE( A1,			t_x1	)
		SCRAMBLE( A2,			t_x2	)
		SCRAMBLE( A3,			t_x3	)
		SCRAMBLE( A4,			t_x4	)
		SCRAMBLE( A5,			t_x5	)
		SCRAMBLE( A6,			t_x6	)
		SCRAMBLE( A7,			t_x7	)
		SCRAMBLE( A8,			t_x8	)
		SCRAMBLE( A9,			t_x9	)

		SCRAMBLE( A10,			t_y0	)
		SCRAMBLE( A11,			t_y1	)
		SCRAMBLE( A12,			t_y2	)
		SCRAMBLE( A13,			t_y3	)
		SCRAMBLE( A14,			t_y4	)
		SCRAMBLE( A15,			t_y5	)
		SCRAMBLE( A16,			t_y6	)
		SCRAMBLE( A17,			t_y7	)

		SCRAMBLE( DQ0,			t_d0	)
		SCRAMBLE( DQ1,			t_d1	)
		SCRAMBLE( DQ2,			t_d2	)
		SCRAMBLE( DQ3,			t_d3	)
		SCRAMBLE( DQ4,			t_d4	)
		SCRAMBLE( DQ5,			t_d5	)
		SCRAMBLE( DQ6,			t_d6	)
		SCRAMBLE( DQ7,			t_d7	)
		SCRAMBLE( DQ8,			t_d8	)
		SCRAMBLE( DQ9,			t_d9	)
		SCRAMBLE( DQ10,			t_d10	)
		SCRAMBLE( DQ11,			t_d11	)
		SCRAMBLE( DQ12,			t_d12	)
		SCRAMBLE( DQ13,			t_d13	)
		SCRAMBLE( DQ14,			t_d14	)
		SCRAMBLE( DQ15,			t_d15	)

		SCRAMBLE( CE,			t_cs1	)
		SCRAMBLE( WE,			t_cs2	)
		SCRAMBLE( OE,			t_cs3	)
		SCRAMBLE( CFGC,			t_cs4	)
	}


	SCRAMBLE_MAP( PS2 )	/*  Default pin scramble map  */
	{
		SCRAMBLE( A0,			t_x0	)
		SCRAMBLE( A1,			t_x1	)
		SCRAMBLE( A2,			t_x2	)
		SCRAMBLE( A3,			t_x3	)
		SCRAMBLE( A4,			t_x4	)
		SCRAMBLE( A5,			t_x5	)
		SCRAMBLE( A6,			t_x6	)
		SCRAMBLE( A7,			t_x7	)
		SCRAMBLE( A8,			t_x8	)
		SCRAMBLE( A9,			t_x9	)

		SCRAMBLE( A10,			t_y0	)
		SCRAMBLE( A11,			t_y1	)
		SCRAMBLE( A12,			t_y2	)
		SCRAMBLE( A13,			t_y3	)
		SCRAMBLE( A14,			t_y4	)
		SCRAMBLE( A15,			t_y5	)
		SCRAMBLE( A16,			t_y6	)
		SCRAMBLE( A17,			t_y7	)
	}

	SCRAMBLE_MAP( PS4 ) //Test Mode Configuration
	{
		SCRAMBLE( CFGD,			t_d32	)

		SCRAMBLE( CE,    		t_cs1   )
		SCRAMBLE( WE,    		t_cs2   )
		SCRAMBLE( OE,			t_cs3	)
		SCRAMBLE( CFGC,			t_cs4	)		
	}

	SCRAMBLE_MAP( PS5 )	/*  Default fastest pin scramble map  */
	{
		SCRAMBLE( A0,			t_x0	)
		SCRAMBLE( A1,			t_x1	)
		SCRAMBLE( A2,			t_x2	)
		SCRAMBLE( A3,			t_x3	)
		SCRAMBLE( A4,			t_x4	)
		SCRAMBLE( A5,			t_x5	)
		SCRAMBLE( A6,			t_x6	)
		SCRAMBLE( A7,			t_x7	)
		SCRAMBLE( A8,			t_x8	)
		SCRAMBLE( A9,			t_x9	)

		SCRAMBLE( A10,			t_y0	)
		SCRAMBLE( A11,			t_y1	)
		SCRAMBLE( A12,			t_y2	)
		SCRAMBLE( A13,			t_y3	)
		SCRAMBLE( A14,			t_y4	)
		SCRAMBLE( A15,			t_y5	)
		SCRAMBLE( A16,			t_y6	)
		SCRAMBLE( A17,			t_y7	)

		SCRAMBLE( DQ0,			t_d0	)
		SCRAMBLE( DQ1,			t_d1	)
		SCRAMBLE( DQ2,			t_d2	)
		SCRAMBLE( DQ3,			t_d3	)
		SCRAMBLE( DQ4,			t_d4	)
		SCRAMBLE( DQ5,			t_d5	)
		SCRAMBLE( DQ6,			t_d6	)
		SCRAMBLE( DQ7,			t_d7	)
		SCRAMBLE( DQ8,			t_d8	)
		SCRAMBLE( DQ9,			t_d9	)
		SCRAMBLE( DQ10,			t_d10	)
		SCRAMBLE( DQ11,			t_d11	)
		SCRAMBLE( DQ12,			t_d12	)
		SCRAMBLE( DQ13,			t_d13	)
		SCRAMBLE( DQ14,			t_d14	)
		SCRAMBLE( DQ15,			t_d15	)

		SCRAMBLE( CE,			t_cs1	)
		SCRAMBLE( WE,			t_cs2	)
		SCRAMBLE( OE,			t_cs3	)
		SCRAMBLE( CFGC,			t_cs4	)
		SCRAMBLE( CLKVCO,		t_cs5	)
	}


	SCRAMBLE_MAP( PS6 )	/*  Default voltage stress pin scramble map  */
	{
		SCRAMBLE( A0,			t_x0	)
		SCRAMBLE( A1,			t_x1	)
		SCRAMBLE( A2,			t_x2	)
		SCRAMBLE( A3,			t_x3	)
		SCRAMBLE( A4,			t_x4	)
		SCRAMBLE( A5,			t_x5	)
		SCRAMBLE( A6,			t_x6	)
		SCRAMBLE( A7,			t_x7	)
		SCRAMBLE( A8,			t_x8	)
		SCRAMBLE( A9,			t_x9	)

		SCRAMBLE( A10,			t_y0	)
		SCRAMBLE( A11,			t_y1	)
		SCRAMBLE( A12,			t_y2	)
		SCRAMBLE( A13,			t_y3	)
		SCRAMBLE( A14,			t_y4	)
		SCRAMBLE( A15,			t_y5	)
		SCRAMBLE( A16,			t_y6	)
		SCRAMBLE( A17,			t_y7	)


		SCRAMBLE( DQ0,			t_d0	)
		SCRAMBLE( DQ1,			t_d1	)
		SCRAMBLE( DQ2,			t_d2	)
		SCRAMBLE( DQ3,			t_d3	)
		SCRAMBLE( DQ4,			t_d4	)
		SCRAMBLE( DQ5,			t_d5	)
		SCRAMBLE( DQ6,			t_d6	)
		SCRAMBLE( DQ7,			t_d7	)
		SCRAMBLE( DQ8,			t_d8	)
		SCRAMBLE( DQ9,			t_d9	)
		SCRAMBLE( DQ10,			t_d10	)
		SCRAMBLE( DQ11,			t_d11	)
		SCRAMBLE( DQ12,			t_d12	)
		SCRAMBLE( DQ13,			t_d13	)
		SCRAMBLE( DQ14,			t_d14	)
		SCRAMBLE( DQ15,			t_d15	)

		SCRAMBLE( CE,			t_cs1	)
		SCRAMBLE( WE,			t_cs2	)
		SCRAMBLE( OE,			t_cs3	)

//		SCRAMBLE( CLK,			t_cs5	)
	}
}