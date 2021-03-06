#include "tester.h"


PINLIST( ADDRESS_pl )
{
	PINS(	A0, 	A1, 	A2, 	A3, 	A4, 	A5, 	A6, 	A7		)
	PINS(	A8, 	A9, 	A10,	A11,	A12,	A13,	A14,	A15		)
	PINS2(	A16,	A17	)
}

PINLIST( DATA_pl )
{
	PINS(	DQ8, 	DQ9, 	DQ10,	DQ11,	DQ12,	DQ13,	DQ14,	DQ15	)
	PINS(	DQ0, 	DQ1, 	DQ2, 	DQ3, 	DQ4, 	DQ5, 	DQ6, 	DQ7		)

}

PINLIST( DUMMY_DATA_pl )
{
	PINS(	DUMMY_DQ1, 	DUMMY_DQ3, 	DUMMY_DQ4, 	DUMMY_DQ6, 	DUMMY_DQ9, 	DUMMY_DQ11, 	DUMMY_DQ12, 	DUMMY_DQ14	)
//	PINS1(	DUMMY_DQ0	)
}

/*
PINLIST( DUMMY_VBG_pl )
{
	PINS1(	DUMMY_VBG	)
}

PINLIST( DUMMY_ADDRESS_pl )
{
	PINS3(	DUMMY_A3 ,DUMMY_A9 , DUMMY_A11	)
}

PINLIST( DUMMY_ZZ_pl )
{
	PINS1(	DUMMY_ZZ	)
}

PINLIST( DUMMY_CFGC_pl )
{
	PINS1(	DUMMY_CFGC	)
}

PINLIST( DUMMY_OE_pl )
{
	PINS1(	DUMMY_OE	)
}
*/
PINLIST( ALL_DUMMY_PINS_pl )
{
	INCLUDE_PINLIST( DUMMY_DATA_pl )
//	INCLUDE_PINLIST( DUMMY_VBG_pl )
//	INCLUDE_PINLIST( DUMMY_ADDRESS_pl )
//	INCLUDE_PINLIST( DUMMY_ZZ_pl )
//	INCLUDE_PINLIST( DUMMY_CFGC_pl )
//	INCLUDE_PINLIST( DUMMY_OE_pl )
}

PINLIST( CE_pl )
{
	PINS1( CE )
}

PINLIST( WE_pl )
{
	PINS1( WE )
}

PINLIST( OE_pl )
{
	PINS1( OE )
}

PINLIST( ZZ_pl )
{
	PINS1( ZZ )
}

PINLIST( LB_pl )
{
	PINS1( LB )
}

PINLIST( UB_pl )
{
	PINS1( UB )
}

PINLIST( CLKVCO_pl )
{
	PINS1( CLKVCO )
}

PINLIST( CFGC_pl )
{
	PINS1( CFGC )
}

PINLIST( CFGD_pl )
{
	PINS1( CFGD )
}

PINLIST( VBG_pl )
{
	PINS1( VBG )
}

PINLIST( INPUT_LOW_pl )
{
	PINS6(	LB,		UB,		WE,		OE,		CE,		ZZ )
}

PINLIST( INPUT_HIGH_pl )
{
	INCLUDE_PINLIST( ADDRESS_pl )
	INCLUDE_PINLIST( CLKVCO_pl )
	INCLUDE_PINLIST( CFGC_pl )
	INCLUDE_PINLIST( CFGD_pl )
}

PINLIST( INPUT_pl )
{
	INCLUDE_PINLIST( INPUT_HIGH_pl )
	INCLUDE_PINLIST( INPUT_LOW_pl )
}

PINLIST( ALLPINS_pl )
{
	INCLUDE_PINLIST( INPUT_pl )
	INCLUDE_PINLIST( VBG_pl )
	INCLUDE_PINLIST( DATA_pl )
}


PINLIST( DPS_pl )
{
//	PINS2(	VDD1,	VDD2 )
	PINS1(	VDD )
}
