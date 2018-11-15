#include "tester.h"

void Power_Down()
{
	pin_dc_state_set( VBG_pl,	t_vil, TRUE );

	pin_dc_state_set( UB_pl,	t_vih, TRUE );
	pin_dc_state_set( LB_pl,	t_vih, TRUE );
	
	dps( 0 V, DPS_pl );

	pin_dc_state_set( INPUT_HIGH_pl, t_vil );
	pin_dc_state_set( INPUT_LOW_pl, t_vil );
}

void timing()
{
	tgmode( 3 );
	
	cycle(TSET2, 2000 NS);//INTERNAL_REGISTER_WRITE
	settime(TSET2,	CFGC_pl,			RTZ,	500			NS, 1500 NS);
	settime(TSET2,	CFGD_pl,			NRZ,	250			NS);
	settime(TSET2,	CE,					NRZ,	250			NS);
	settime(TSET2,	WE,					NRZ,	250			NS);
	settime(TSET2,	OE,					NRZ,	250			NS);
	
	cycle(TSET3, 2000 NS);//INTERNAL_REGISTER_READ
	settime(TSET3,	CFGC_pl,			RTZ,	500			NS, 1500 NS);
	settime(TSET3,	CFGD_pl,			STROBE,	1900		NS,	1920 NS);
	settime(TSET3,	CE,					NRZ,	250			NS);
	settime(TSET3,	WE,					NRZ,	250			NS);
	settime(TSET3,	OE,					NRZ,	250			NS);
	
	cycle(TSET4, 3000 NS);//INTERNAL_CLK_READ
	settime(TSET4,	ADDRESS_pl,			NRZ,	20			NS);
	settime(TSET4,	CE,					RTO,	250			NS,	 2950 NS);
	settime(TSET4,	WE,					NRZ,	20			NS);
	settime(TSET4,	OE,					RTO,	1300		NS,	 2850 NS);
	settime(TSET4,	DATA_pl,			STROBE,	2750	    NS,	 2760 NS);
	
	cycle(TSET5, 3000 NS);//INTERNAL_CLK_WRITE
	settime(TSET5,	ADDRESS_pl,			NRZ,	20			NS);
	settime(TSET5,	CE,					RTO,	50			NS,	 2900 NS);
	settime(TSET5,	WE,					RTO,	650			NS,	 2800 NS);
	settime(TSET5,	OE,					NRZ,	20			NS);
	settime(TSET5,	DATA_pl,			NRZ,	20			NS);
	
	cycle(TSET6, 1000 NS);//Address
	settime(TSET6,	ADDRESS_pl,			NRZ,	20			NS);

	cycle(TSET7, 10 NS);//extern_clock_write
	settime(TSET7,	CLKVCO_pl,			RTZ,	4			NS, 9 NS);
	settime(TSET7,	CE,					NRZ,	1			NS); 
	settime(TSET7,	WE,					NRZ,	1			NS);
	settime(TSET7,	OE,					NRZ,	1			NS);
	settime(TSET7,	ADDRESS_pl,			NRZ,	1			NS);
	settime(TSET7,	DATA_pl,			NRZ,	1			NS);

	cycle(TSET8, 180 NS);//extern_clock_read
	settime(TSET8,	CLKVCO_pl,			RTZ,	40			NS, 130 NS);
	settime(TSET8,	CE,					NRZ,	20			NS);
	settime(TSET8,	WE,					NRZ,	20			NS);
	settime(TSET8,	OE,					NRZ,	20			NS);
	settime(TSET8,	ADDRESS_pl,			NRZ,	20			NS);
	settime(TSET8,	DATA_pl,			STROBE,	20			NS,90	NS);

	cycle(TSET9, 200 NS);//staircase_down_external_write_clock_loop_1
	settime(TSET9,	CLKVCO_pl,			RTZ,	50			NS, 150 NS);
	settime(TSET9,	CE,					NRZ,	15			NS);
	settime(TSET9,	WE,					NRZ,	15			NS);
	settime(TSET9,	OE,					NRZ,	15			NS);
	settime(TSET9,	ADDRESS_pl,			NRZ,	15			NS);
	settime(TSET9,	DATA_pl,			NRZ,	15			NS);

	cycle(TSET10, 400 NS);//staircase_down_external_write_clock_loop_2
	settime(TSET10,	CLKVCO_pl,			RTZ,	100			NS, 300 NS);
	settime(TSET10,	CE,					NRZ,	50			NS);
	settime(TSET10,	WE,					NRZ,	50			NS);
	settime(TSET10,	OE,					NRZ,	50			NS);
	settime(TSET10,	ADDRESS_pl,			NRZ,	50			NS);
	settime(TSET10,	DATA_pl,			NRZ,	50			NS);

	cycle(TSET11, 180 NS);//staircase_down_external_write_clock_loop_3
	settime(TSET11,	CLKVCO_pl,			RTZ,	30			NS, 120 NS);
	settime(TSET11,	CE,					NRZ,	10			NS);
	settime(TSET11,	WE,					NRZ,	10			NS);
	settime(TSET11,	OE,					NRZ,	10			NS);
	settime(TSET11,	ADDRESS_pl,			NRZ,	10			NS);
	settime(TSET11,	DATA_pl,			NRZ,	10			NS);

	cycle(TSET12, 100 NS);//extern_clock_write
//	settime(TSET12,	CLK_pl,				RTZ,	10			NS, 60 NS);  4000
	settime(TSET12,	CE,					NRZ,	5			NS);
	settime(TSET12,	WE,					NRZ,	5			NS);
	settime(TSET12,	OE,					NRZ,	5			NS);
	settime(TSET12,	ADDRESS_pl,			NRZ,	5			NS);
	settime(TSET12,	DATA_pl,			NRZ,	5			NS);

	cycle(TSET13, 50 NS);//extern_clock_cycle_rst
	settime(TSET13,	CLKVCO_pl,			RTZ,	15			NS, 40 NS);
	settime(TSET13,	CE,					NRZ,	5			NS); 
	settime(TSET13,	WE,					NRZ,	5			NS);
	settime(TSET13,	OE,					NRZ,	5			NS);
	settime(TSET13,	ADDRESS_pl,			NRZ,	5			NS);
	settime(TSET13,	DATA_pl,			NRZ,	5			NS);

	cycle(TSET14, 12 NS);//extern_clock_cycle_set
	settime(TSET14,	CLKVCO_pl,			RTZ,	4			NS, 10 NS);
	settime(TSET14,	CE,					NRZ,	2			NS); 
	settime(TSET14,	WE,					NRZ,	2			NS);
	settime(TSET14,	OE,					NRZ,	2			NS);
	settime(TSET14,	ADDRESS_pl,			NRZ,	2			NS);
	settime(TSET14,	DATA_pl,			NRZ,	2			NS);

	cycle(TSET15, 20 NS);//extern_clock_write_slope_version_loop_1
	settime(TSET15,	CLKVCO_pl,			RTZ,	5			NS, 15 NS);
	settime(TSET15,	CE,					NRZ,	1			NS); 
	settime(TSET15,	WE,					NRZ,	1			NS);
	settime(TSET15,	OE,					NRZ,	1			NS);
	settime(TSET15,	ADDRESS_pl,			NRZ,	1			NS);
	settime(TSET15,	DATA_pl,			NRZ,	1			NS);

	cycle(TSET16, 8 NS);//extern_clock_write_slope_version_loop_2
	settime(TSET16,	CLKVCO_pl,			RTZ,	3			NS, 7 NS);
	settime(TSET16,	CE,					NRZ,	1			NS); 
	settime(TSET16,	WE,					NRZ,	1			NS);
	settime(TSET16,	OE,					NRZ,	1			NS);
	settime(TSET16,	ADDRESS_pl,			NRZ,	1			NS);
	settime(TSET16,	DATA_pl,			NRZ,	1			NS);
	
	
}

DWORD trim_initial()
{
	DWORD CFG_DATA = 0;
	DWORD cfg[32];
	int i;
	cfg[0]=1; //1 SW0	   :
	cfg[1]=1; //1 SW1
	cfg[2]=1; //0 S0W0
	cfg[3]=0; //0 S0W1
	
	cfg[4]=1; //1 RSW0     :
	cfg[5]=0; //1 RSW1	
	cfg[6]=0; //1 S0H0
	cfg[7]=0; //1 S0H1	
	
	cfg[8]=1; //0 RSH0		
	cfg[9]=1; //0 RSH1
	cfg[10]=0;//0 IWR0		:
	cfg[11]=1;//1 IWR1
	
	cfg[12]=1;//1 RW0		:READ Width
	cfg[13]=1;//0 RW1
	cfg[14]=0;//0 IRD0		:READ Resistance Reference
	cfg[15]=1;//1 IRD1
	
	cfg[16]=0;//0 IRD2
	cfg[17]=1;//1 IRD3
	cfg[18]=0;//0 VRCK0		:VCO
	cfg[19]=0;//0 VRCK1
	
	cfg[20]=1;//1 VRCK2
	cfg[21]=1;//1 VRC0
	cfg[22]=0;//0 VRC1		:BGN
	cfg[23]=0;//0 VRC2
	
	cfg[24]=1;//1 VRC3
	cfg[25]=0;//0 CLK0       :
	cfg[26]=1;//1 CLK1
	cfg[27]=1;//1 Bandgap	(可以外加电压)
	
	cfg[28]=1;//1 ZZ	
	cfg[29]=0;//RESOUT
	cfg[30]=0;//VRST
	cfg[31]=1;//ECCE
	
	for( i = 0 ; i < 32 ; i++ )
	{
		CFG_DATA = CFG_DATA + ( cfg[ i ] << ( 31-i ) );
	}
	
	//output( "CFG_DATA = %8X", CFG_DATA );
	
	return CFG_DATA;	
}

void system_initial_internal_vbg()
{
	disconnect( ALL_DUMMY_PINS_pl );
	dps_connect( DPS_pl );
	
	pin_dc_state_set( INPUT_LOW_pl, t_vih );
	pin_dc_state_set( INPUT_HIGH_pl, t_vil );
	
	pin_dc_state_set( DATA_pl, t_vil );	
	
	pin_dc_state_set( VBG_pl, t_tristate, TRUE );
	pin_dc_state_set( CLKVCO_pl, t_vil );
	
	vil( 0.00 V, VBG_pl );
	vih( 1.25 V, VBG_pl );
	
	vil( 0.0 V, INPUT_LOW_pl );
	vih( NORMAL_DPS V, INPUT_LOW_pl );
	
	vil( 0.0 V, INPUT_HIGH_pl );
	vih( NORMAL_DPS V, INPUT_HIGH_pl );
	
	vil( 0.0 V, DATA_pl );
	vih( NORMAL_DPS V, DATA_pl );
	
	vol( 1.5 V, ALLPINS_pl );
	voh( 1.6 V, ALLPINS_pl );
	
	iol( 1.0 UA, ALLPINS_pl );
	ioh( 1.0 UA, ALLPINS_pl );
	
	dps( NORMAL_DPS V, DPS_pl );
}

void system_initial_external_vbg()
{
	disconnect( ALL_DUMMY_PINS_pl );

	dps( 0 V, DPS_pl );
	dps_connect( DPS_pl );	
	
	pin_dc_state_set( INPUT_LOW_pl, t_vih );
	pin_dc_state_set( INPUT_HIGH_pl, t_vil );

	pin_dc_state_set( DATA_pl, t_vil );	

	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	pin_dc_state_set( CLKVCO_pl, t_vil );

	vil( 0.00 V, VBG_pl );
	vih( 3.75 V, VBG_pl );

	vil( 0.0 V, INPUT_LOW_pl );
	vih( NORMAL_DPS V, INPUT_LOW_pl );

	vil( 0.0 V, INPUT_HIGH_pl );
	vih( NORMAL_DPS V, INPUT_HIGH_pl );

	vil( 0.0 V, DATA_pl );
	vih( NORMAL_DPS V, DATA_pl );

	vol( 1.5 V, ALLPINS_pl );
	voh( 1.6 V, ALLPINS_pl );

	iol( 1.0 UA, ALLPINS_pl );
	ioh( 1.0 UA, ALLPINS_pl );

	dps( NORMAL_DPS V, DPS_pl );
}
