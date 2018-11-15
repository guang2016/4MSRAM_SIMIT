/*
#include "tester.h"

DUT_PIN( A0		){}
DUT_PIN( A1		){}
DUT_PIN( A2		){}
DUT_PIN( A3		){}
DUT_PIN( A4		){}
DUT_PIN( A5		){}
DUT_PIN( A6		){}
DUT_PIN( A7		){}
DUT_PIN( A8		){}
DUT_PIN( A9		){}
DUT_PIN( A10	){}
DUT_PIN( A11	){}
DUT_PIN( A12	){}
DUT_PIN( A13	){}
DUT_PIN( A14	){}
DUT_PIN( A15	){}
DUT_PIN( A16	){}
DUT_PIN( A17	){}

DUT_PIN( DQ0	){}
DUT_PIN( DQ1	){}
DUT_PIN( DQ2	){}
DUT_PIN( DQ3	){}
DUT_PIN( DQ4	){}
DUT_PIN( DQ5	){}
DUT_PIN( DQ6	){}
DUT_PIN( DQ7	){}
DUT_PIN( DQ8	){}
DUT_PIN( DQ9	){}
DUT_PIN( DQ10	){}
DUT_PIN( DQ11	){}
DUT_PIN( DQ12	){}
DUT_PIN( DQ13	){}
DUT_PIN( DQ14	){}
DUT_PIN( DQ15	){}

DUT_PIN( DUMMY_DQ1	){}
DUT_PIN( DUMMY_DQ3	){}
DUT_PIN( DUMMY_DQ4	){}
DUT_PIN( DUMMY_DQ6	){}

DUT_PIN( DUMMY_DQ9	){}
DUT_PIN( DUMMY_DQ11	){}
DUT_PIN( DUMMY_DQ12	){}
DUT_PIN( DUMMY_DQ14	){}

DUT_PIN( CE		){}
DUT_PIN( WE		){}
DUT_PIN( OE		){}
DUT_PIN( ZZ		){}
DUT_PIN( LB		){}
DUT_PIN( UB		){}

DUT_PIN( CLKVCO ){}
DUT_PIN( CFGD	){}
DUT_PIN( CFGC	){}
DUT_PIN( VBG	){}

DUT_PIN( DUMMY_ZZ ){}	//DUMMY_VBG
DUT_PIN( DUMMY_VBG ){}	//DUMMY_VBG
DUT_PIN( DUMMY_A3 ){}	//DUMMY_VBG
DUT_PIN( DUMMY_A11 ){}	//DUMMY_VBG
DUT_PIN( DUMMY_CFGC ){}	//DUMMY_VBG

DUT_PIN( DUMMY_DQ0 ){}	//DUMMY_VBG
DUT_PIN( DUMMY_OE ){}	//DUMMY_VBG

DUT_PIN( DUMMY_A9 ){}	//DUMMY_VBG

DUT_PIN( VDD	){}
PIN_ASSIGNMENTS( tester_pins )
{
    SITES_PER_CONTROLLER(1)

	ASSIGN_1DUT(	CFGC		,   b_62	)
	ASSIGN_1DUT(	DUMMY_CFGC	,   b_19	)

	ASSIGN_1DUT(	A4			,   b_51	)

	ASSIGN_1DUT(	DUMMY_A3	,   b_20	)
	ASSIGN_1DUT(	A3			,   b_56	)

	ASSIGN_1DUT(	A2			,   b_52	)
	ASSIGN_1DUT(	A1			,   b_21	)
	ASSIGN_1DUT(	A0			,   b_53	)
	ASSIGN_1DUT(	CE			,   b_22	)

	ASSIGN_1DUT(	DQ0			,   b_32	)
	ASSIGN_1DUT(	DUMMY_DQ0	,   b_54	)

	ASSIGN_1DUT(	DUMMY_DQ1	,   b_23	)
	ASSIGN_1DUT(	DQ1			,   b_47	)

	ASSIGN_1DUT(	DQ2			,   b_55	)

	ASSIGN_1DUT(	DUMMY_DQ3	,   b_24	)
	ASSIGN_1DUT(	DQ3			,   b_48	)

	ASSIGN_1DUT(	DUMMY_DQ4	,   b_25	)
	ASSIGN_1DUT(	DQ4			,   b_49	)
	
	ASSIGN_1DUT(	DQ5			,   b_57	)

	ASSIGN_1DUT(	DUMMY_DQ6	,   b_26	)
	ASSIGN_1DUT(	DQ6			,   b_50	)

	ASSIGN_1DUT(	DQ7			,   b_58	)
	
	ASSIGN_1DUT(	WE			,   b_27	)
	ASSIGN_1DUT(	A17			,   b_59	)
	ASSIGN_1DUT(	A16			,   b_28	)
	ASSIGN_1DUT(	A15			,   b_60	)
	ASSIGN_1DUT(	A14			,   b_29	)
	ASSIGN_1DUT(	A13			,   b_61	)
	ASSIGN_1DUT(	CLKVCO 		,   b_30	)

	ASSIGN_1DUT(	DUMMY_VBG	,   b_3		)
	ASSIGN_1DUT(	VBG			,   b_63	)

	ASSIGN_1DUT(	A12			,   b_35	)

	ASSIGN_1DUT(	A11			,   b_1		)
	ASSIGN_1DUT(	DUMMY_A11	,   b_4		)

	ASSIGN_1DUT(	A10			,   b_36	)

	ASSIGN_1DUT(	DUMMY_A9	,   b_5		)
	ASSIGN_1DUT(	A9			,   b_17	)

	ASSIGN_1DUT(	A8			,   b_37	)
	
	ASSIGN_1DUT(	DUMMY_ZZ	,   b_6		)
	ASSIGN_1DUT(	ZZ			,   b_15	)
	
	ASSIGN_1DUT(	DQ8			,   b_38	)

	ASSIGN_1DUT(	DUMMY_DQ9	,   b_7	)
	ASSIGN_1DUT(	DQ9			,   b_33	)
	
	ASSIGN_1DUT(	DQ10		,   b_39	)
	
	ASSIGN_1DUT(	DUMMY_DQ11	,   b_8	)
	ASSIGN_1DUT(	DQ11		,   b_34	)
	
	ASSIGN_1DUT(	DUMMY_DQ12	,   b_9		)
	ASSIGN_1DUT(	DQ12		,   b_40	)

	ASSIGN_1DUT(	DQ13		,   b_41	)
	
	ASSIGN_1DUT(	DUMMY_DQ14	,   b_10	)
	ASSIGN_1DUT(	DQ14		,   b_46	)

	ASSIGN_1DUT(	DQ15		,   b_42	)


	ASSIGN_1DUT(	LB			,   b_11	)
	ASSIGN_1DUT(	UB			,   b_43	)
	ASSIGN_1DUT(	OE			,   b_2		)
	ASSIGN_1DUT(	DUMMY_OE	,   b_12	)
	ASSIGN_1DUT(	A7			,   b_44	)
	ASSIGN_1DUT(	A6			,   b_13	)
	ASSIGN_1DUT(	A5			,   b_45	)
	ASSIGN_1DUT(	CFGD		,   b_14	)

	ASSIGN_1DUT(	VDD		,   b_dps1a	)
}
*/
/*
DUT_PIN( A0 ){}
DUT_PIN( A1 ){}
DUT_PIN( A2 ){}
DUT_PIN( A3 ){}
DUT_PIN( A4 ){}
DUT_PIN( A5 ){}
DUT_PIN( A6 ){}
DUT_PIN( A7 ){}
DUT_PIN( A8 ){}
DUT_PIN( A9 ){}
DUT_PIN( A10 ){}
DUT_PIN( A11 ){}
DUT_PIN( A12 ){}
DUT_PIN( A13 ){}
DUT_PIN( A14 ){}
DUT_PIN( A15 ){}
DUT_PIN( A16 ){}
DUT_PIN( A17 ){}

DUT_PIN( DQ0 ){}
DUT_PIN( DQ1 ){}
DUT_PIN( DQ2 ){}
DUT_PIN( DQ3 ){}
DUT_PIN( DQ4 ){}
DUT_PIN( DQ5 ){}
DUT_PIN( DQ6 ){}
DUT_PIN( DQ7 ){}
DUT_PIN( DQ8 ){}
DUT_PIN( DQ9 ){}
DUT_PIN( DQ10 ){}
DUT_PIN( DQ11 ){}
DUT_PIN( DQ12 ){}
DUT_PIN( DQ13 ){}
DUT_PIN( DQ14 ){}
DUT_PIN( DQ15 ){}

DUT_PIN( CE ){}
DUT_PIN( WE ){}
DUT_PIN( OE ){}
DUT_PIN( ZZ ){}
DUT_PIN( LB ){}
DUT_PIN( UB ){}

DUT_PIN( CLKVCO ){}
DUT_PIN( CFGD ){}
DUT_PIN( CFGC ){}
DUT_PIN( VBG ){}

DUT_PIN( VDD ){}

PIN_ASSIGNMENTS( tester_pins )
{
    SITES_PER_CONTROLLER(1)

	ASSIGN_1DUT(	CFGC		,   b_1	)
	ASSIGN_1DUT(	A4			,   b_2	)
	ASSIGN_1DUT(	A3			,   b_3	)
	ASSIGN_1DUT(	A2			,   b_4	)
	ASSIGN_1DUT(	A1			,   b_5	)
	ASSIGN_1DUT(	A0			,   b_6	)
	ASSIGN_1DUT(	CE			,   b_7	)
	ASSIGN_1DUT(	DQ0			,   b_64	)
	ASSIGN_1DUT(	DQ1			,   b_63	)
	ASSIGN_1DUT(	DQ2			,   b_62	)
	ASSIGN_1DUT(	DQ3			,   b_61	)
	ASSIGN_1DUT(	DQ4			,   b_60	)
	ASSIGN_1DUT(	DQ5			,   b_59	)
	ASSIGN_1DUT(	DQ6			,   b_58	)
	ASSIGN_1DUT(	DQ7			,   b_57	)
	ASSIGN_1DUT(	WE			,   b_8		)
	ASSIGN_1DUT(	A17			,   b_9		)
	ASSIGN_1DUT(	A16			,   b_10	)
	ASSIGN_1DUT(	A15			,   b_11	)
	ASSIGN_1DUT(	A14			,   b_12	)
	ASSIGN_1DUT(	A13			,   b_13	)
	ASSIGN_1DUT(	CLKVCO  	,   b_14	)

	ASSIGN_1DUT(	VBG			,   b_32	)
	ASSIGN_1DUT(	A12			,   b_31	)
	ASSIGN_1DUT(	A11			,   b_30	)
	ASSIGN_1DUT(	A10			,   b_29	)
	ASSIGN_1DUT(	A9			,   b_28	)
	ASSIGN_1DUT(	A8			,   b_27	)
	ASSIGN_1DUT(	ZZ			,   b_26	)
	ASSIGN_1DUT(	DQ8			,   b_56	)
	ASSIGN_1DUT(	DQ9			,   b_55	)
	ASSIGN_1DUT(	DQ10		,   b_54	)
	ASSIGN_1DUT(	DQ11		,   b_53	)
	ASSIGN_1DUT(	DQ12		,   b_52	)
	ASSIGN_1DUT(	DQ13		,   b_51	)
	ASSIGN_1DUT(	DQ14		,   b_50	)
	ASSIGN_1DUT(	DQ15		,   b_49	)
	ASSIGN_1DUT(	LB			,   b_25	)
	ASSIGN_1DUT(	UB			,   b_24	)
	ASSIGN_1DUT(	OE			,   b_23	)
	ASSIGN_1DUT(	A7			,   b_22	)
	ASSIGN_1DUT(	A6			,   b_21	)
	ASSIGN_1DUT(	A5			,   b_20	)
	ASSIGN_1DUT(	CFGD		,   b_19	)

	ASSIGN_1DUT(	VDD			,   b_dps1a	)

}

//////////////////////////////////////////////SIMIT////////////////////////////////////////////
#include "tester.h"

DUT_PIN( A0 ){}
DUT_PIN( A1 ){}
DUT_PIN( A2 ){}
DUT_PIN( A3 ){}
DUT_PIN( A4 ){}
DUT_PIN( A5 ){}
DUT_PIN( A6 ){}
DUT_PIN( A7 ){}
DUT_PIN( A8 ){}
DUT_PIN( A9 ){}
DUT_PIN( A10 ){}
DUT_PIN( A11 ){}
DUT_PIN( A12 ){}
DUT_PIN( A13 ){}
DUT_PIN( A14 ){}
DUT_PIN( A15 ){}
DUT_PIN( A16 ){}
DUT_PIN( A17 ){}

DUT_PIN( DQ0 ){}
DUT_PIN( DQ1 ){}
DUT_PIN( DQ2 ){}
DUT_PIN( DQ3 ){}
DUT_PIN( DQ4 ){}
DUT_PIN( DQ5 ){}
DUT_PIN( DQ6 ){}
DUT_PIN( DQ7 ){}
DUT_PIN( DQ8 ){}
DUT_PIN( DQ9 ){}
DUT_PIN( DQ10 ){}
DUT_PIN( DQ11 ){}
DUT_PIN( DQ12 ){}
DUT_PIN( DQ13 ){}
DUT_PIN( DQ14 ){}
DUT_PIN( DQ15 ){}

DUT_PIN( CE ){}
DUT_PIN( WE ){}
DUT_PIN( OE ){}
DUT_PIN( ZZ ){}
DUT_PIN( LB ){}
DUT_PIN( UB ){}

DUT_PIN( CLKVCO ){}	
DUT_PIN( CFGD ){}	
DUT_PIN( CFGC ){}	
DUT_PIN( VBG ){}	

DUT_PIN( VDD1 ){}	//VCCD
DUT_PIN( VDD2 ){}	//VCCD

PIN_ASSIGNMENTS( tester_pins )
{
    SITES_PER_CONTROLLER(1)

	ASSIGN_1DUT(	CFGC		,   a_32	)
	ASSIGN_1DUT(	A4			,   a_30	)
	ASSIGN_1DUT(	A3			,   a_31	)
	ASSIGN_1DUT(	A2			,   a_28	)
	ASSIGN_1DUT(	A1			,   a_29	)
	ASSIGN_1DUT(	A0			,   a_26	)
	ASSIGN_1DUT(	CE			,   a_27	)
	ASSIGN_1DUT(	DQ0			,   a_64	)
	ASSIGN_1DUT(	DQ1			,   a_63	)
	ASSIGN_1DUT(	DQ2			,   a_62	)
	ASSIGN_1DUT(	DQ3			,   a_61	)
	ASSIGN_1DUT(	DQ4			,   a_60	)
	ASSIGN_1DUT(	DQ5			,   a_59	)
	ASSIGN_1DUT(	DQ6			,   a_58	)
	ASSIGN_1DUT(	DQ7			,   a_57	)
	ASSIGN_1DUT(	WE			,   a_22	)
	ASSIGN_1DUT(	A17			,   a_23	)
	ASSIGN_1DUT(	A16			,   a_20	)
	ASSIGN_1DUT(	A15			,   a_21	)
	ASSIGN_1DUT(	A14			,   a_18	)
	ASSIGN_1DUT(	A13			,   a_19	)
	ASSIGN_1DUT(	CLKVCO  	,   a_17	)

	ASSIGN_1DUT(	VBG			,   a_33	)
	ASSIGN_1DUT(	A12			,   a_35	)
	ASSIGN_1DUT(	A11			,   a_34	)
	ASSIGN_1DUT(	A10			,   a_37	)
	ASSIGN_1DUT(	A9			,   a_36	)
	ASSIGN_1DUT(	A8			,   a_39	)
	ASSIGN_1DUT(	ZZ			,   a_38	)
	ASSIGN_1DUT(	DQ8			,   a_56	)
	ASSIGN_1DUT(	DQ9			,   a_55	)
	ASSIGN_1DUT(	DQ10		,   a_54	)
	ASSIGN_1DUT(	DQ11		,   a_53	)
	ASSIGN_1DUT(	DQ12		,   a_52	)
	ASSIGN_1DUT(	DQ13		,   a_51	)
	ASSIGN_1DUT(	DQ14		,   a_50	)
	ASSIGN_1DUT(	DQ15		,   a_49	)
	ASSIGN_1DUT(	LB			,   a_41	)
	ASSIGN_1DUT(	UB			,   a_42	)
	ASSIGN_1DUT(	OE			,   a_43	)
	ASSIGN_1DUT(	A7			,   a_44	)
	ASSIGN_1DUT(	A6			,   a_45	)
	ASSIGN_1DUT(	A5			,   a_46	)
	ASSIGN_1DUT(	CFGD		,   a_16	)

	ASSIGN_1DUT(	VDD1		,   a_dps1a	)
	ASSIGN_1DUT(	VDD2		,   a_dps2a	)
}
*/

#include "tester.h"

DUT_PIN( A0		){}
DUT_PIN( A1		){}
DUT_PIN( A2		){}
DUT_PIN( A3		){}
DUT_PIN( A4		){}
DUT_PIN( A5		){}
DUT_PIN( A6		){}
DUT_PIN( A7		){}
DUT_PIN( A8		){}
DUT_PIN( A9		){}
DUT_PIN( A10	){}
DUT_PIN( A11	){}
DUT_PIN( A12	){}
DUT_PIN( A13	){}
DUT_PIN( A14	){}
DUT_PIN( A15	){}
DUT_PIN( A16	){}
DUT_PIN( A17	){}

DUT_PIN( DQ0	){}
DUT_PIN( DQ1	){}
DUT_PIN( DQ2	){}
DUT_PIN( DQ3	){}
DUT_PIN( DQ4	){}
DUT_PIN( DQ5	){}
DUT_PIN( DQ6	){}
DUT_PIN( DQ7	){}
DUT_PIN( DQ8	){}
DUT_PIN( DQ9	){}
DUT_PIN( DQ10	){}
DUT_PIN( DQ11	){}
DUT_PIN( DQ12	){}
DUT_PIN( DQ13	){}
DUT_PIN( DQ14	){}
DUT_PIN( DQ15	){}

DUT_PIN( DUMMY_DQ1	){}
DUT_PIN( DUMMY_DQ3	){}
DUT_PIN( DUMMY_DQ4	){}
DUT_PIN( DUMMY_DQ6	){}

DUT_PIN( DUMMY_DQ9	){}
DUT_PIN( DUMMY_DQ11	){}
DUT_PIN( DUMMY_DQ12	){}
DUT_PIN( DUMMY_DQ14	){}

DUT_PIN( CE		){}
DUT_PIN( WE		){}
DUT_PIN( OE		){}
DUT_PIN( ZZ		){}
DUT_PIN( LB		){}
DUT_PIN( UB		){}

DUT_PIN( CLKVCO ){}
DUT_PIN( CFGD	){}
DUT_PIN( CFGC	){}
DUT_PIN( VBG	){}

DUT_PIN( VDD	){}
PIN_ASSIGNMENTS( tester_pins )
{
    SITES_PER_CONTROLLER(1)

	ASSIGN_1DUT(	CFGC		,   b_19	)

	ASSIGN_1DUT(	A4			,   b_51	)

	ASSIGN_1DUT(	A3			,   b_20	)

	ASSIGN_1DUT(	A2			,   b_52	)
	ASSIGN_1DUT(	A1			,   b_21	)
	ASSIGN_1DUT(	A0			,   b_53	)
	ASSIGN_1DUT(	CE			,   b_22	)

	ASSIGN_1DUT(	DQ0			,   b_54	)

	ASSIGN_1DUT(	DUMMY_DQ1	,   b_23	)
	ASSIGN_1DUT(	DQ1			,   b_47	)

	ASSIGN_1DUT(	DQ2			,   b_55	)

	ASSIGN_1DUT(	DUMMY_DQ3	,   b_24	)
	ASSIGN_1DUT(	DQ3			,   b_48	)

	ASSIGN_1DUT(	DUMMY_DQ4	,   b_25	)
	ASSIGN_1DUT(	DQ4			,   b_49	)
	
	ASSIGN_1DUT(	DQ5			,   b_57	)

	ASSIGN_1DUT(	DUMMY_DQ6	,   b_26	)
	ASSIGN_1DUT(	DQ6			,   b_50	)

	ASSIGN_1DUT(	DQ7			,   b_58	)
	
	ASSIGN_1DUT(	WE			,   b_27	)
	ASSIGN_1DUT(	A17			,   b_59	)
	ASSIGN_1DUT(	A16			,   b_28	)
	ASSIGN_1DUT(	A15			,   b_60	)
	ASSIGN_1DUT(	A14			,   b_29	)
	ASSIGN_1DUT(	A13			,   b_61	)
	ASSIGN_1DUT(	CLKVCO 		,   b_30	)

	ASSIGN_1DUT(	VBG			,   b_3		)

	ASSIGN_1DUT(	A12			,   b_35	)

	ASSIGN_1DUT(	A11			,   b_4		)

	ASSIGN_1DUT(	A10			,   b_36	)

	ASSIGN_1DUT(	A9			,   b_5		)

	ASSIGN_1DUT(	A8			,   b_37	)
	
	ASSIGN_1DUT(	ZZ			,   b_6		)
	
	ASSIGN_1DUT(	DQ8			,   b_38	)

	ASSIGN_1DUT(	DUMMY_DQ9	,   b_7		)
	ASSIGN_1DUT(	DQ9			,   b_33	)
	
	ASSIGN_1DUT(	DQ10		,   b_39	)
	
	ASSIGN_1DUT(	DUMMY_DQ11	,   b_8	)
	ASSIGN_1DUT(	DQ11		,   b_34	)
	
	ASSIGN_1DUT(	DUMMY_DQ12	,   b_9		)
	ASSIGN_1DUT(	DQ12		,   b_40	)

	ASSIGN_1DUT(	DQ13		,   b_41	)
	
	ASSIGN_1DUT(	DUMMY_DQ14	,   b_10	)
	ASSIGN_1DUT(	DQ14		,   b_46	)

	ASSIGN_1DUT(	DQ15		,   b_42	)


	ASSIGN_1DUT(	LB			,   b_11	)
	ASSIGN_1DUT(	UB			,   b_43	)
	ASSIGN_1DUT(	OE			,   b_12	)
	ASSIGN_1DUT(	A7			,   b_44	)
	ASSIGN_1DUT(	A6			,   b_13	)
	ASSIGN_1DUT(	A5			,   b_45	)
	ASSIGN_1DUT(	CFGD		,   b_14	)

	ASSIGN_1DUT(	VDD			,   b_dps1a	)
}


