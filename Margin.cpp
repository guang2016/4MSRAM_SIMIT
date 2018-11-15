#include "tester.h" 

extern void system_initial_internal_vbg();
extern void system_initial_external_vbg();

extern int trim_cfg_data= 0xffd90cb8;

void Trim_Read_Reference( int read_reference )
{
	
	trim_cfg_data = trim_cfg_data & 0xfffc3fff;
	switch( read_reference )
	{
		case 15: trim_cfg_data = trim_cfg_data + 0x0003c000; break; // 190K
		case 14: trim_cfg_data = trim_cfg_data + 0x0001c000; break; // 180k
		case 13: trim_cfg_data = trim_cfg_data + 0x0002c000; break; // 170k
		case 12: trim_cfg_data = trim_cfg_data + 0x0000c000; break; // 160k
		case 11: trim_cfg_data = trim_cfg_data + 0x00034000; break; // 150k
		case 10: trim_cfg_data = trim_cfg_data + 0x00014000; break; // 140k
		case  9: trim_cfg_data = trim_cfg_data + 0x00024000; break; // 130k
		case  8: trim_cfg_data = trim_cfg_data + 0x00004000; break; // 120k
		case  7: trim_cfg_data = trim_cfg_data + 0x00038000; break; // 110k
		case  6: trim_cfg_data = trim_cfg_data + 0x00018000; break; // 100k
		case  5: trim_cfg_data = trim_cfg_data + 0x00028000; break; //  90k
		case  4: trim_cfg_data = trim_cfg_data + 0x00008000; break; //  80k
		case  3: trim_cfg_data = trim_cfg_data + 0x00030000; break; //  70k
		case  2: trim_cfg_data = trim_cfg_data + 0x00010000; break; //  60k
		case  1: trim_cfg_data = trim_cfg_data + 0x00020000; break; //  50k
		case  0: trim_cfg_data = trim_cfg_data + 0x00000000; break; //  40k
		default: trim_cfg_data = trim_cfg_data + 0x00018000; break; // 100k
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
 void Trim_SET_First_Width( int SET_first_width )
 {
	trim_cfg_data = trim_cfg_data & 0xcfffffff;
	switch( SET_first_width )
	{
		case  3: trim_cfg_data = trim_cfg_data + 0x30000000; break; // 500ns
		case  2: trim_cfg_data = trim_cfg_data + 0x10000000; break; // 300ns
		case  1: trim_cfg_data = trim_cfg_data + 0x20000000; break; // 200ns
		case  0: trim_cfg_data = trim_cfg_data + 0x00000000; break; // 100ns
		default: trim_cfg_data = trim_cfg_data + 0x20000000; break; // 200ns
	}
}
		
		  
void Trim_SET_Second_Width( int SET_second_width )
{
	trim_cfg_data = trim_cfg_data & 0x3fffffff;
	switch( SET_second_width )
	{
		case  3: trim_cfg_data = trim_cfg_data + 0xc0000000; break; // 200ns
		case  2: trim_cfg_data = trim_cfg_data + 0x40000000; break; // 100ns
		case  1: trim_cfg_data = trim_cfg_data + 0x80000000; break; // 50ns
		case  0: trim_cfg_data = trim_cfg_data + 0x00000000; break; // 25ns
		default: trim_cfg_data = trim_cfg_data + 0x40000000; break; // 100ns
	}
}

void Trim_SET_First_Height( int SET_height )
{
	trim_cfg_data = trim_cfg_data & 0xfcffffff;
	switch( SET_height )
	{
		case  3: trim_cfg_data = trim_cfg_data + 0x03000000; break; // 760uA
		case  2: trim_cfg_data = trim_cfg_data + 0x01000000; break; // 640uA
		case  1: trim_cfg_data = trim_cfg_data + 0x02000000; break; // 520uA
		case  0: trim_cfg_data = trim_cfg_data + 0x00000000; break; // 400uA
		default: trim_cfg_data = trim_cfg_data + 0x02000000; break; // 760uA
	}
}

void Trim_RESET_Width( int RESET_width )
{
	
	trim_cfg_data = trim_cfg_data & 0xf3ffffff;
	switch( RESET_width )
	{
		case  3: trim_cfg_data = trim_cfg_data + 0x0c000000; break; // 500ns
		case  2: trim_cfg_data = trim_cfg_data + 0x04000000; break; // 200ns
		case  1: trim_cfg_data = trim_cfg_data + 0x08000000; break; // 100ns
		case  0: trim_cfg_data = trim_cfg_data + 0x00000000; break; // 50ns
		default: trim_cfg_data = trim_cfg_data + 0x08000000; break; // 100ns
	}
}

void Trim_RESET_Height( int RESET_height )
{
	trim_cfg_data = trim_cfg_data & 0xff3fffff;
	switch( RESET_height )
	{
		case  3: trim_cfg_data = trim_cfg_data + 0x00c00000; break; // 1000uA
		case  2: trim_cfg_data = trim_cfg_data + 0x00400000; break; // 980uA
		case  1: trim_cfg_data = trim_cfg_data + 0x00800000; break; // 760uA
		case  0: trim_cfg_data = trim_cfg_data + 0x00000000; break; // 640uA
		default: trim_cfg_data = trim_cfg_data + 0x08000000; break; // 640uA
	}
}

/*
void Trim_VBG_Mode( int VBG_Mode )
{
	CFG_dat = CFG_dat & 0xffffffef;
	if( VBG_Mode == 1 )
		CFG_dat = CFG_dat + 0x10;
}
*/

int SET_Time_Verify( int basic_clk_time, int clk_step_time,int clk_step_num)
{
	int Tset_Toatl_Time = basic_clk_time + ( clk_step_time * clk_step_num );
	
	cycle(TSET7, Tset_Toatl_Time NS);//extern_clock_write
	settime(TSET7,	CLKVCO_pl,			RTZ,	3 + (Tset_Toatl_Time / 10)			NS, 3 + (Tset_Toatl_Time / 10) + (Tset_Toatl_Time / 2) NS);
	settime(TSET7,	CE,					NRZ,	1 + (Tset_Toatl_Time / 20)			NS); 
	settime(TSET7,	WE,					NRZ,	1 + (Tset_Toatl_Time / 20)			NS);
	settime(TSET7,	OE,					NRZ,	1 + (Tset_Toatl_Time / 20)			NS);
	settime(TSET7,	ADDRESS_pl,			NRZ,	1 + (Tset_Toatl_Time / 20)			NS);
	settime(TSET7,	DATA_pl,			NRZ,	1 + (Tset_Toatl_Time / 20)			NS);

	return Tset_Toatl_Time;
}


float SET_Vbg_Verify( float min_vbg, float vbg_step_height, int vbg_step_num)
{
	float Total_Vbg = min_vbg + ( vbg_step_height * vbg_step_num );
	
	system_initial_external_vbg();
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );			
			
	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	vil( 0.00 V, VBG_pl );
	vih( Total_Vbg V, VBG_pl );           //fixed vbg/current

	return Total_Vbg;
}