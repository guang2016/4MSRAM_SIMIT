#include "tester.h"
#include "patterns.h"
#include "pattern1_MAG2.h"
#include "math.h"
#include "stdio.h"
#include "file.h"

extern void system_initial_internal_vbg();
extern void system_initial_external_vbg();
extern void timing();
extern DWORD trim_initial();
extern void Power_Down();

extern int SET_Time_Verify( int basic_clk_time, int clk_step_time,int clk_step_num);
extern float SET_Vbg_Verify( float min_vbg, float vbg_step_height, int vbg_step_num);

extern int trim_cfg_data ;
extern void Trim_Read_Reference( int read_reference );
extern void Trim_SET_First_Width( int SET_first_width );
extern void Trim_SET_Second_Width( int SET_second_width );
extern void Trim_SET_First_Height( int SET_height );
extern void Trim_RESET_Width( int RESET_width );
extern void Trim_RESET_Height( int RESET_height );

extern  void IV_test_function( PinList *all_PinList, double sweep_start, double sweep_stop, double sweep_step ,int X_address,int Y_address )  ;
extern  void VI_test_function( PinList *all_PinList, double sweep_start, double sweep_stop, double sweep_step ,int X_address,int Y_address )  ; 

int x_coordinate = 0x7fffffff, y_coordinate = 0x7fffffff;

float external_vbg = 3.75;
FLOAT_VARIABLE( uv_external_vbg, 0 , "External_VBG") { external_vbg = uv_external_vbg ;}

int Tset_Time = 500;
INT_VARIABLE( uv_external_clk_time, 0 , "External_CLK_Time") { Tset_Time = uv_external_clk_time ;}


enum e_log_type 
{ 
	LOG_I, LOG_V, LOG_T
};

CString cnvt_result( double value, e_log_type log_type )
{
	CString cstr;
	switch( log_type )
	{
	case LOG_V: // Convert to voltage
		if ( fabs( value ) >= ( 1 V ) )
		{
			cstr.Format( "%8.3f v", value / ( 1 V ) );
		}
		else
		{
			cstr.Format( "%8.0f mv",value / ( 1 MV ) );
		}
		break;
	case LOG_I: // Convert to current
		if( fabs( value ) >= ( 1 MA ) )
		{
			cstr.Format( "%8.3f ma", value / ( 1 MA ) );
		}
		else if ( fabs( value ) >= ( 1 UA ) )
		{
			cstr.Format( "%8.3f ua", value / ( 1 UA ) );
		}
		else
		{
			cstr.Format( "%8.0f na", value / ( 1 NA ) );
		}
		break;
	case LOG_T: break;
	default:cstr.Format( "#############" );
	}
	return ( cstr );
}

void pdatalog( PinList* pPinList)
{
	// Determine the mode of the parametric test last executed
	e_log_type log_type;
	switch( parametric_mode())
	{
	case 0:log_type = LOG_I; break;
	case 2:log_type = LOG_I; break;
	case 1:log_type = LOG_V; break;
	default: output("Error: invalid mode: %d",parametric_mode());
	}
	
	// Output datalog header
	output( "\n--------------------------------------------------------------------------" );
	output( "		<B><COLOR=PURPLE>TestBlock = %-15s</B></COLOR> TestNumber = %d",current_test_block(),test_number() );
	output( "--------------------------------------------------------------------------\n" );
	
	if ( measure() == TRUE )
	{
		output( "----------------------------------------------" );
		output( "Tester DUT Pin Pin Measured" );
		output( " Pin Pin Name Result Value " );
		output( "----------------------------------------------" );
	}
	else
	{
		output( "-----------------------------------" );
		output( "Tester DUT Pin Pin " );
		output( " Pin Pin Name Result" );
		output( "-----------------------------------" );
	}
	
	CArray<double,double> meas_results;
	CArray<PFState,PFState> pf_results;
	// Get/save the current Active DUTs Set
	DutNumArray active_duts;
	int count = active_duts_get(&active_duts);
	// For each active DUT...
	
	for( int dut = 0; dut < count; dut++)
	{
		DutNum dut_num = active_duts[ dut ];
		active_duts_enable( dut_num ); // Enable one DUT at a time
		// Check the pinlist for either dps pins or signal pins
		if( all_dps( pPinList ))
		{
			Dps_pf( pf_results );
			if( measure())
				Dps_meas( meas_results );
		}
		else if( all_hv( pPinList ))
		{
			Hv_pf( pf_results );
			if( measure())
				Hv_meas( meas_results );
		}
		else
		{
			Pin_pf( pf_results );
			if( measure())
				Pin_meas( meas_results );
		}
		
		output("Dut-%d : %s", active_duts[dut]+1,result_get(active_duts[dut]) ? "PASS" : "FAIL");
		int size = pf_results.GetSize(); // Get num of results
		// Loop through all tested pins and output results
		for ( int pin = 0; pin < size; pin++)
		{
			DutPin * dpin;
			pin_info( pPinList, pin, &dpin);
			TesterPin tpin;
			pin_info( dpin, dut_num, &tpin );
			CString pin_name = resource_name( dpin );
			if( measure() )
				output("t_%-4d %5d %-14s %4s %s",( tpin + 1 ),0,pin_name,pf_results[ pin ] ? "PASS" : "FAIL",cnvt_result( meas_results[ pin ], log_type ) );
			else
				output( "t_%-4d %5d %-14s %4s",( tpin + 1 ),0,pin_name,pf_results[ pin] ? "PASS" : "FAIL");
		}
	}
	// Restore Active_DUTs_Set
	active_duts_enable( active_duts );
}

void pdatalog( PinList* pPinList, double Bit_line_current[1] )
{
	// Determine the mode of the parametric test last executed
	e_log_type log_type;
	switch( parametric_mode())
	{
	case 0:log_type = LOG_I; break;
	case 2:log_type = LOG_I; break;
	case 1:log_type = LOG_V; break;
	default: output("Error: invalid mode: %d",parametric_mode());
	}
	
	// Output datalog header
	output( "\n--------------------------------------------------------------------------" );
	output( "		<B><COLOR=LIME>TestBlock = %-15s<B></COLOR> TestNumber = %d",current_test_block(),test_number() );
	output( "--------------------------------------------------------------------------\n" );
	
	if ( measure() == TRUE )
	{
		output( "----------------------------------------------" );
		output( "Tester DUT Pin Pin Measured" );
		output( " Pin Pin Name Result Value " );
		output( "----------------------------------------------" );
	}
	else
	{
		output( "-----------------------------------" );
		output( "Tester DUT Pin Pin " );
		output( " Pin Pin Name Result" );
		output( "-----------------------------------" );
	}
	
	CArray<double,double> meas_results;
	CArray<PFState,PFState> pf_results;
	// Get/save the current Active DUTs Set
	DutNumArray active_duts;
	int count = active_duts_get(&active_duts);
	// For each active DUT...
	
	for( int dut = 0; dut < count; dut++)
	{
		DutNum dut_num = active_duts[ dut ];
		active_duts_enable( dut_num ); // Enable one DUT at a time
		// Check the pinlist for either dps pins or signal pins
		if( all_dps( pPinList ))
		{
			Dps_pf( pf_results );
			if( measure())
				Dps_meas( meas_results );
		}
		else if( all_hv( pPinList ))
		{
			Hv_pf( pf_results );
			if( measure())
				Hv_meas( meas_results );
		}
		else
		{
			Pin_pf( pf_results );
			if( measure())
				Pin_meas( meas_results );
		}
		
		output("Dut-%d : %s", active_duts[dut]+1,result_get(active_duts[dut]) ? "PASS" : "FAIL");
		int size = pf_results.GetSize(); // Get num of results
		// Loop through all tested pins and output results
		for ( int pin = 0; pin < size; pin++)
		{
			DutPin * dpin;
			pin_info( pPinList, pin, &dpin);
			TesterPin tpin;
			pin_info( dpin, dut_num, &tpin );
			CString pin_name = resource_name( dpin );
			if( measure() )
				output("t_%-4d %5d %-14s %4s %s",( tpin + 1 ),0,pin_name,pf_results[ pin ] ? "PASS" : "FAIL",cnvt_result( meas_results[ pin ], log_type ) );
			else
				output( "t_%-4d %5d %-14s %4s",( tpin + 1 ),0,pin_name,pf_results[ pin] ? "PASS" : "FAIL");
		}
	}
	// Restore Active_DUTs_Set
	Bit_line_current[0]=meas_results[0];
	active_duts_enable( active_duts );
}



TEST_BLOCK( Open_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	disconnect( ALL_DUMMY_PINS_pl );

	ipar_force( -100 UA	);
	vpar_high( -0.350 V	);
	vpar_low( -0.750 V );
	test_result= partest( pass_nivl, ALLPINS_pl );
	pdatalog( ALLPINS_pl );
	
	
	ipar_force( -100 UA	);
	vpar_high( -0.35 V );
	vpar_low( -0.750 V );
	test_result = partest( pass_nivl, DPS_pl );
	pdatalog( DPS_pl );	 
	
	return MULTI_DUT;
}

TEST_BLOCK( Short_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	vpar_force( 100 MV );
	ipar_high( 1 UA );
	ipar_low( -1 UA );
	test_result = partest( pass_nicl, ALLPINS_pl );
	
	pdatalog( ALLPINS_pl );
	
	return MULTI_DUT;
}

TEST_BLOCK( Power_Test )
{
	BOOL test_result;
	measure( TRUE );
	CArray<double,double> vcc_current;

	system_initial_internal_vbg();
	
	timing();
	
	#ifdef _DRIVER_H_
		TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
	#endif
	
	//DWORD CFG_dat;
	data_reg_width( 36 );
	FILE *fp = fopen( TQV3_4MSRAM_Power_Test_File, "a+" );
	//int x_coordinate = 0x7fffffff,y_coordinate = 0x7fffffff;
	//fprintf( fp, "%5d	%5d	%5d	%5d	%5d", x_coordinate, y_coordinate, xaddress, yaddress); 
	//TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
	///////////////////////////////////////////////////////////////////////	1
	Delay( 500 MS);
	pin_dc_state_set( ZZ_pl, t_vih, TRUE );
	dps_current_high( 2 MA, DPS_pl );
	dps_current_low( 0 MA, DPS_pl );
	test_result = test_supply( pass_nicl, DPS_pl );
	Dps_meas(vcc_current);
	pdatalog( DPS_pl );
	//	fprintf(fp, "%d  %d  zz_high VDD33_1:%8.3f MA  VDD33_2:%8.3f MA", x_coordinate,y_coordinate,vcc_current[0]/1000000,vcc_current[1]/1000000);
	fprintf(fp, "%5d  %5d  zz_high VDD33_1:%8.3f MA ", x_coordinate,y_coordinate,vcc_current[0]/1000000);
	///////////////////////////////////////////////////////////////////////  2
	Delay( 500 MS);
	pin_dc_state_set( ZZ_pl, t_vil, TRUE );
	//pin_dc_state_set( CLKVCO_pl, t_vil );
	dps_current_high( 10 MA, DPS_pl );
	dps_current_low( 0 MA, DPS_pl );
	test_result = test_supply( pass_nicl, DPS_pl );
	Dps_meas(vcc_current);
	pdatalog( DPS_pl );
	fprintf(fp, "  zz_low VDD33_1:%8.3f MA ",vcc_current[0]/1000000);
	///////////////////////////////////////////////////////////////////////	 3
	Delay( 500 MS);
	pin_dc_state_set( ZZ_pl, t_vih, TRUE );
	pin_dc_state_set( CE_pl, t_vih, TRUE );
	//pin_dc_state_set( CLKVCO_pl, t_vil );
	dps_current_high( 10 MA, DPS_pl );
	dps_current_low( 0 MA, DPS_pl );
	test_result = test_supply( pass_nicl, DPS_pl );
	Dps_meas(vcc_current);
	pdatalog( DPS_pl );
	fprintf(fp, "  CE_high VDD33_1:%8.3f MA ",vcc_current[0]/1000000);
	///////////////////////////////////////////////////////////////////////  4
	Delay( 500 MS);
	pin_dc_state_set( CE_pl, t_vil, TRUE );
	//pin_dc_state_set( CLKVCO_pl, t_vil );
	dps_current_high( 10 MA, DPS_pl );
	dps_current_low( 0 MA, DPS_pl );
	test_result = test_supply( pass_nicl, DPS_pl );
	Dps_meas(vcc_current);
	pdatalog( DPS_pl );
	fprintf(fp, "  CE_low VDD33_1:%8.3f MA \n",vcc_current[0]/1000000 );
	
	fclose(fp);

	return MULTI_DUT;
}


TEST_BLOCK( Vref_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	FILE *fp = fopen( VBG_Test_File, "a+" );
	
	CArray<double,double> bitline_current;
	
	//	optima_VBG_TRIM_BITs = 0x0;
	//	optima_VBG = 0.0;
	
	numx( 10 );
	numy( 8 );
	data_reg_width( 36 );
	x_fast_axis( TRUE );
	
	double minimum_current = 1000;
	double vref = 0.0;
	DWORD CFG_dat = trim_initial();
	CFG_dat = 0x4bd94839;
	int CFG_dat_Array[ 16 ] =
	{
			0x0,	0x8,	0x4,	0xc,
			0x2,	0xa,	0x6,	0xe,
			0x1,	0x9,	0x5,	0xd,
			0x3,	0xb,	0x7,	0xf,	
	};
	
	for( double power_voltage = 2.5; power_voltage < 3.31; power_voltage = power_voltage + 0.2 )
	{
		vih( power_voltage V, INPUT_LOW_pl );
		vih( power_voltage V, INPUT_HIGH_pl );
		vih( power_voltage V, DATA_pl );
		
		dps( power_voltage V, DPS_pl );
		
		for( int vbg_trim = 0; vbg_trim < 16; vbg_trim++ )
		{
			minimum_current = 1000;
			vref = 0.0;
			CFG_dat = 0x4bd94839 + CFG_dat_Array[ vbg_trim ] * 0x80;
			dmain( CFG_dat );
			funtest( CFG_set_PAT, finish );
			
			//fprintf( fp, "%5d	%5d	", x_coordinate, y_coordinate );
			fprintf( fp, "Power Voltage = %10.6f	", power_voltage );
			for( int i = 0 ; i <= 100 ; i++ )
			{
				vpar_force( ( 0.95 + i * 0.005 ) V );
				ipar_high( 20 UA  );
				ipar_low( -1 UA );
				test_result = partest( pass_nicl, VBG );
				Pin_meas( bitline_current );
				//output( "%f V %f uA", 0.9 + i * 0.01, bitline_current[ 0 ] / 1000 );
				if( fabs( bitline_current[ 0 ] ) < minimum_current )
				{
					minimum_current = fabs( bitline_current[ 0 ] );
					vref = 0.95 + i * 0.005;
				}
			}
			
			fprintf( fp, "Vbg_trim = %3d Vref = %10.6f V\n ", vbg_trim, vref );
			
			/*
			if( ( power_voltage > 2.69 ) && ( power_voltage < 2.71 ) )
			{
			if( fabs( 1.25 - vref ) < fabs( 1.25 - optima_VBG ) )
			{
			optima_VBG = vref;
			optima_VBG_TRIM_BITs = CFG_dat_Array[ vbg_trim ];
			}
			}
			fprintf( fp, "optima_VBG_TRIM_BITs = %3d, optima_VBG = %10.6f V\n", optima_VBG_TRIM_BITs, optima_VBG );
			*/
		}
	}
	
	fclose( fp );

	return MULTI_DUT;
}

TEST_BLOCK( Decoder_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	int data=0;
	int xaddress,yaddress;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	DWORD CFG_dat = trim_initial();
	CFG_dat = 0xe8594cb9;        		
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 500 MS);
	
	for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			dmain( data );
			test_result = funtest( int_clk_write_one_address_pat, finish );
			//data=data+1;
		}
	}
	
	return MULTI_DUT;
}


TEST_BLOCK( Dc_Set_Array_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	DWORD CFG_dat;
	int xaddress,yaddress;
	CArray<double,double> bitline_current_dc_set;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );
//	ecr_config_set( 10, 8, DATA_pl );
//	data_strobe( 0xffff ); 	
//	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( TQV3_4MSRAM_Dc_Set_Array_Test_File, "a+" );
	
	//CFG_dat = 0x4bd94acbd;   //Resout enable
	
	CFG_dat = 0x4bd97cb9 | 0x00000004;
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	Delay( 1000 MS );
	
	for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			if( ( xaddress == 0 ) && ( yaddress == 0 ) )
				
				Delay( 10 MS );
			
			///////////////////////////////Ini///////////////////////////////////////
			
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			Delay( 50 MS );
			
			#ifdef _DRIVER_H_
				TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
			#endif
			
			//ipar_force( 200 UA );
			//vpar_high( 2 V );
			//vpar_low( -0.5 V );
			//test_result= partest( pass_nivl, DATA_pl );
			
			//ipar_force( 150 UA );
			//vpar_high( 2 V );
			//vpar_low( -0.5 V );
			//test_result= partest( pass_nivl, DATA_pl );
			
			ipar_force( 100 UA );
			vpar_high( 2 V );
			vpar_low( -0.5 V );
			test_result= partest( pass_nivl, DATA_pl );
			
			ipar_force( 50 UA );
			vpar_high( 2 V );
			vpar_low( -0.5 V );
			test_result= partest( pass_nivl, DATA_pl );
			
			vpar_force( 0.3 V );
			ipar_high( 2000 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( bitline_current_dc_set );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp, "%5d	%5d	%5d	%5d	%5d", x_coordinate, y_coordinate, xaddress, yaddress, i );
				fprintf( fp, "%12.2f	", bitline_current_dc_set[ i ]);
				if( bitline_current_dc_set[ i ] < 30 )
					bitline_current_dc_set[ i ] = 30;
				fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current_dc_set[ i ] )		);
				fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current_dc_set[ i ] ) ) / log( 10 ) );
				fprintf( fp, "\n" );
			}
		}
	}
	fclose(fp);
	
	return MULTI_DUT;
}

TEST_BLOCK( Write_0xffff_Test )
{
	BOOL test_result;
	measure( TRUE );
	
//	system_initial_internal_vbg();
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );

//	ecr_config_set( 10, 8, DATA_pl );
//	data_strobe( 0xffff ); 	
//	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	CFG_dat = trim_initial();
	//CFG_dat = 0xf85beca8; //0.88mA@100ns 0.4mA@1500ns 190k
	CFG_dat = 0xf3dd4ca9; //rst 50ns/1ma--- set 1.5us/f(225ns/0)/0.76ma 
//	CFG_dat = 0xf3dd4cb8; //rst 50ns/1ma--- set 1.5us/0.76ma 
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	xmain( xmax() );
	ymain( ymax() );
	dmain( 0xffff );
	count( 1, amax() );
	test_result = funtest( int_clk_write_pat, finish );

	return MULTI_DUT;
}


TEST_BLOCK( Read_0xffff_Test )
{
	BOOL test_result;
	measure( TRUE );
	
//	system_initial_internal_vbg();
	system_initial_external_vbg();
	timing();
	
	int total_error_num_1,total_error_num_2;
	float bit_yield,word_yield;
	DWORD CFG_dat;
	
//	FILE *fp = fopen( TQV3_4MSRAM_Read_0xffff_Test_File, "a+" );
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );

	ecr_config_set( 10, 8, DATA_pl );
	data_strobe( 0xffff ); 	
	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	CFG_dat = trim_initial();
	//CFG_dat = 0xf85beca8; //0.88mA@100ns 0.4mA@1500ns 190k
	CFG_dat = 0xc0dbcca8; //190K
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	ecr_all_clear();
	xmain( xmax() );
	ymain( ymax() );
	dmain( 0xffff );
	count( 1, amax() );
	test_result = funtest( int_clk_read_pat, fullec );
		
	total_error_num_1 = ecr_main_ram_scan(t_hsb1, 0,xmax(),0,ymax(),0xffff,1024*256,FALSE);
	total_error_num_2 = ecr_main_ram_scan(t_hsb1, 0,xmax(),0,ymax(),0xffff,1024*256*16,TRUE);
	bit_yield = 1-(total_error_num_2/1024.0/256/16);
	word_yield = 1-(total_error_num_1/1024.0/256);
		
//	fprintf(fp," %8x ",CFG_dat);		
//	fprintf( fp,"%12d %12d ",total_error_num_1,total_error_num_2);
//	fprintf( fp,"%12.7f %12.7f ",word_yield,bit_yield);
//	fclose(fp);

	output("%d %12.7lf\n",total_error_num_2,bit_yield);

	return MULTI_DUT;
}

TEST_BLOCK( Write_0x0_Test )
{
	BOOL test_result;
	measure( TRUE );
	
//	system_initial_internal_vbg();
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );

//	ecr_config_set( 10, 8, DATA_pl );
//	data_strobe( 0xffff ); 	
//	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	CFG_dat = trim_initial();
//	CFG_dat = 0xf85beca8; //0.88mA@100ns 0.4mA@1500ns 190k
	CFG_dat = 0xf3dd4ca9; //rst 50ns/1ma--- set 1.5us/0.76ma
//	CFG_dat = 0xf3dd4cb8; //rst 50ns/1ma--- set 1.5us/0.76ma
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	xmain( xmax() );
	ymain( ymax() );
	dmain( 0x0 );
	count( 1, amax() );
	test_result = funtest( int_clk_write_pat, finish );

	return MULTI_DUT;
}

TEST_BLOCK( Read_0x0_Test )
{
	BOOL test_result;
	measure( TRUE );
	
//	system_initial_internal_vbg();
	system_initial_external_vbg();
	timing();
	
	int total_error_num_1,total_error_num_2;
	float bit_yield,word_yield;
	DWORD CFG_dat;
	
//	FILE *fp = fopen( TQV3_4MSRAM_Read_0x0_Test_File, "a+" );
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );

	ecr_config_set( 10, 8, DATA_pl );
	data_strobe( 0xffff ); 	
	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	CFG_dat = trim_initial();
	//CFG_dat = 0xf85beca8; //0.88mA@100ns 0.4mA@1500ns 190k
	CFG_dat = 0xe23bbca8; 
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	ecr_all_clear();
	xmain( xmax() );
	ymain( ymax() );
	dmain( 0x0 );
	count( 1, amax() );
	test_result = funtest( int_clk_read_pat, fullec );
		
	total_error_num_1 = ecr_main_ram_scan(t_hsb1, 0,xmax(),0,ymax(),0xffff,1024*256,FALSE);
	total_error_num_2 = ecr_main_ram_scan(t_hsb1, 0,xmax(),0,ymax(),0xffff,1024*256*16,TRUE);
	bit_yield = 1-(total_error_num_2/1024.0/256/16);
	word_yield = 1-(total_error_num_1/1024.0/256);
		
//	fprintf(fp," %8x ",CFG_dat);		
//	fprintf( fp,"%12d %12d ",total_error_num_1,total_error_num_2);
//	fprintf( fp,"%12.7f %12.7f ",word_yield,bit_yield);
//	fclose(fp);

	output("%d %12.7lf\n",total_error_num_2,bit_yield);

	return MULTI_DUT;
}


TEST_BLOCK( Resistance_Distribution_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	DWORD CFG_dat;
	int xaddress,yaddress;
	CArray<double,double> bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );

//	ecr_config_set( 10, 8, DATA_pl );
//	data_strobe( 0xffff ); 	
//	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( TQV3_4MSRAM_Resistance_Distribution_Test_File, "a+" );	
	
	CFG_dat = 0x4bd97cb9 | 0x00000004;
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	Delay( 20 MS );

	pin_dc_state_set( DATA_pl, t_vil );
	
	for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			if( ( xaddress == 0 ) && ( yaddress == 0 ) )
				
				Delay( 20 MS );
			
			///////////////////////////////Ini///////////////////////////////////////
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			Delay( 20 MS );
			
			#ifdef _DRIVER_H_
				TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
			#endif
			
			//Delay( 50 MS );
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( bitline_current_ini );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp, "%5d	%5d	%5d	%5d	%5d", x_coordinate, y_coordinate, xaddress, yaddress, i );
				fprintf( fp, "%12.2f	", bitline_current_ini[ i ]);
				if( bitline_current_ini[ i ] < 30 )
					bitline_current_ini[ i ] = 30;
				fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current_ini[ i ] )		);
				fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current_ini[ i ] ) ) / log( 10 ) );
				fprintf( fp, "\n" );
			}
		}
	}

	fclose(fp);
	
	return MULTI_DUT;
}


TEST_BLOCK( Rst_Shmoo_Test )
{
	BOOL test_result;
	measure( TRUE );
	
//	system_initial_internal_vbg();
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	int xaddress,yaddress;
	CArray<double,double> bitline_current;
	
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );
	/*
	ecr_config_set( 10, 8, DATA_pl );
	data_strobe( 0xffff ); 	
	ecr_all_clear();
	*/	
	
	
	FILE *fp = fopen( Rst_Shmoo_Test_File, "a+" );
	
	for( int k = 0; k < 16; k++ )
	{
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		CFG_dat = 0xf3dd4ca8;                  
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		Delay( 50 MS);
		
        xmain( xmax() );
		ymain( ymax() );
		dmain( 0xffff );
		count( 1, amax() );
        test_result = funtest( int_clk_write_pat, finish );
		
		CFG_dat = CFG_dat | 0x00000004;
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		Delay( 20 MS );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				///////////////////////////////read_res///////////////////////////////////////
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				Delay( 50 MS );
				
				#ifdef _DRIVER_H_
					TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
				#endif
				
				vpar_force( 0.3 V );
				ipar_high( 2000 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current );
				
				for( int i = 0 ; i < 16 ; i++)
				{
					fprintf( fp, "%5d	%5d	%5d	%5d	%5d	", x_coordinate, y_coordinate, xaddress, yaddress,i );
					fprintf( fp, "%12.2f	", bitline_current[ i ]);
					if( bitline_current[ i ] < 30 )
						bitline_current[ i ] = 30;
					//fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current[ i ] ) );
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
		fprintf( fp, "CFG_dat %10x Set %5d Finished!\n",CFG_dat,k );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		CFG_dat = trim_initial();
		
		switch( k )
		{
		case 0:    CFG_dat = 0x43194ca8;	break;         //Rst:50ns,640uA(basic 20uA)
		case 1:    CFG_dat = 0x43994ca8;	break;         //Rst:50ns,760uA
		case 2:    CFG_dat = 0x43594ca8;	break;         //Rst:50ns,880uA
		case 3:    CFG_dat = 0x43d94ca8;	break;         //Rst:50ns,1000uA
			
		case 4:    CFG_dat = 0x4b194ca8;	break;         //Rst:100ns,640uA
		case 5:    CFG_dat = 0x4b994ca8;	break;         //Rst:100ns,760uA
		case 6:    CFG_dat = 0x4b594ca8;	break;         //Rst:100ns,880uA
		case 7:    CFG_dat = 0x4bd94ca8;	break;         //Rst:100ns,1000uA
			
		case 8:    CFG_dat = 0x47194ca8;	break;         //Rst:200ns,640uA
		case 9:    CFG_dat = 0x47994ca8;	break;         //Rst:200ns,760uA
		case 10:   CFG_dat = 0x47594ca8;	break;         //Rst:200ns,880uA
		case 11:   CFG_dat = 0x47d94ca8;	break;         //Rst:200ns,1000uA
			
		case 12:   CFG_dat = 0x4f194ca8;	break;         //Rst:500ns,640uA
		case 13:   CFG_dat = 0x4f994ca8;	break;         //Rst:500ns,760uA
		case 14:   CFG_dat = 0x4f594ca8;	break;         //Rst:500ns,880uA
		case 15:   CFG_dat = 0x4fd94ca8;	break;         //Rst:500ns,1000uA
			
		default:   CFG_dat = 0x43d94ca8;	break;		   //Rst:50ns,1000uA
			
		/*
		case 0:    CFG_dat = 0x43294cb9;	break;         //Rst:50ns,640uA(basic 25.7uA)
		case 1:    CFG_dat = 0x43a94cb9;	break;         //Rst:50ns,760uA
		case 2:    CFG_dat = 0x43694cb9;	break;         //Rst:50ns,880uA
		case 3:    CFG_dat = 0x43e94cb9;	break;         //Rst:50ns,1000uA
		
		case 4:    CFG_dat = 0x4b294cb9;	break;         //Rst:100ns,640uA
		case 5:    CFG_dat = 0x4ba94cb9;	break;         //Rst:100ns,760uA
		case 6:    CFG_dat = 0x4b694cb9;	break;         //Rst:100ns,880uA
		case 7:    CFG_dat = 0x4be94cb9;	break;         //Rst:100ns,1000uA
		  
		case 8:    CFG_dat = 0x47294cb9;	break;         //Rst:200ns,640uA
		case 9:    CFG_dat = 0x47a94cb9;	break;         //Rst:200ns,760uA
		case 10:   CFG_dat = 0x47694cb9;	break;         //Rst:200ns,880uA
		case 11:   CFG_dat = 0x47e94cb9;	break;         //Rst:200ns,1000uA
			
		case 12:   CFG_dat = 0x4f294cb9;	break;         //Rst:500ns,640uA
		case 13:   CFG_dat = 0x4fa94cb9;	break;         //Rst:500ns,760uA
		case 14:   CFG_dat = 0x4f694cb9;	break;         //Rst:500ns,880uA
		case 15:   CFG_dat = 0x4fe94cb9;	break;         //Rst:500ns,1000uA
			  
		default:   CFG_dat = 0x47294cb9;				   //Rst:200ns,640uA
		*/
			
			/*
			case 0:    CFG_dat = 0x43094cb9;	break;         //Rst:50ns,640uA(basic 33.3uA)
			case 1:    CFG_dat = 0x43894cb9;	break;         //Rst:50ns,760uA
			case 2:    CFG_dat = 0x43494cb9;	break;         //Rst:50ns,880uA
			case 3:    CFG_dat = 0x43c94cb9;	break;         //Rst:50ns,1000uA
			
			  case 4:    CFG_dat = 0x4b094cb9;	break;         //Rst:100ns,640uA
			  case 5:    CFG_dat = 0x4b894cb9;	break;         //Rst:100ns,760uA
			  case 6:    CFG_dat = 0x4b494cb9;	break;         //Rst:100ns,880uA
			  case 7:    CFG_dat = 0x4bc94cb9;	break;         //Rst:100ns,1000uA
			  
				case 8:    CFG_dat = 0x47094cb9;	break;         //Rst:200ns,640uA
				case 9:    CFG_dat = 0x47894cb9;	break;         //Rst:200ns,760uA
				case 10:   CFG_dat = 0x47494cb9;	break;         //Rst:200ns,880uA
				case 11:   CFG_dat = 0x47c94cb9;	break;         //Rst:200ns,1000uA
				
				  case 12:   CFG_dat = 0x4f094cb9;	break;         //Rst:500ns,640uA
				  case 13:   CFG_dat = 0x4f894cb9;	break;         //Rst:500ns,760uA
				  case 14:   CFG_dat = 0x4f494cb9;	break;         //Rst:500ns,880uA
				  case 15:   CFG_dat = 0x4fc94cb9;	break;         //Rst:500ns,1000uA
				  
					default:   CFG_dat = 0x47094cb9;				   //Rst:200ns,640uA
			*/
		}
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		Delay( 50 MS);
		
        xmain( xmax() );
		ymain( ymax() );
		dmain( 0x0 );
		count( 1, amax() );
        test_result = funtest( int_clk_write_pat, finish );
		
		CFG_dat = CFG_dat | 0x00000004;
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		Delay( 20 MS );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				///////////////////////////////read_res///////////////////////////////////////
				
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				
				Delay( 50 MS );
				
				#ifdef _DRIVER_H_
					TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
				#endif
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current );
				
				for( int i = 0 ; i < 16 ; i++)
				{
					fprintf( fp, "%5d	%5d	%5d	%5d	%5d	", x_coordinate, y_coordinate, xaddress, yaddress,i );
					fprintf( fp, "%12.2f	", bitline_current[ i ]);
					if( bitline_current[ i ] < 30 )
						bitline_current[ i ] = 30;
					//fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current[ i ] ) );
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
		fprintf( fp, "CFG_dat %10x Rst %5d Finished!\n",CFG_dat,k );
		//////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	
	fclose(fp);
	
	return MULTI_DUT;
	
}

TEST_BLOCK( Set_Shmoo_Test )
{
	measure( TRUE );
	BOOL test_result;
	
//	system_initial_internal_vbg();
	system_initial_external_vbg();
	timing();

	DWORD CFG_dat;
	CArray<double,double> bitline_current;
	int xaddress,yaddress;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );

	/*
	ecr_config_set( 10, 8, DATA_pl );
	data_strobe( 0xffff ); 	
	ecr_all_clear();
	*/
	
	CFG_dat = trim_initial();
	
	FILE *fp = fopen( Set_Shmoo_Test_File, "a+" );
	
	for( int k = 0;  k < 16; k++ )
	{
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		CFG_dat = 0xf3d94fa8;		//Rst:50ns,1mA
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		Delay( 10 MS);
		
        xmain( xmax() );
		ymain( ymax() );
		dmain( 0x0 );
		count( 1, amax() );
        test_result = funtest( int_clk_write_pat, finish );
		
		CFG_dat = CFG_dat | 0x00000004;
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		Delay( 10 MS );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				///////////////////////////////////////////////////////////////////////////
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				Delay( 10 MS );
				
				#ifdef _DRIVER_H_
	  				TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
				#endif
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current );
				
				for( int i = 0 ; i < 16 ; i++)
				{
					fprintf( fp, "%5d	%5d	%5d	%5d	%5d	", x_coordinate, y_coordinate, xaddress, yaddress,i );
					fprintf( fp, "%12.2f	", bitline_current[ i ]);
					if( bitline_current[ i ] < 30 )
						bitline_current[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current[ i ] ) );
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
		fprintf( fp, "config %12x ",CFG_dat );
		fprintf( fp, "rst %5d Finished!\n",k );
				
		Delay( 10 MS);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );	
		
		switch( k )
		{
			case 0:		CFG_dat = 0x50d94fa8;	break;   //Set=100*5+300ns/400ua
			case 1:		CFG_dat = 0x52d94fa8;	break;   //Set=100*5+300ns/520ua
			case 2:		CFG_dat = 0x51d94fa8;	break;
			case 3:		CFG_dat = 0x53d94fa8;	break;

			case 4:	    CFG_dat = 0x70d94fa8;	break;   //Set=100*5+500ns/400ua
			case 5:     CFG_dat = 0x72d94fa8;	break;   //Set=100*5+500ns/520ua
			case 6:	    CFG_dat = 0x71d94fa8;	break;
			case 7:	    CFG_dat = 0x73d94fa8;	break;

			case 8:		CFG_dat = 0xd0d94fa8;	break;   //Set=200*5+300ns/400ua
			case 9:		CFG_dat = 0xd2d94fa8;	break;   //Set=200*5+300ns/520ua
			case 10:	CFG_dat = 0xd1d94fa8;	break;
			case 11:	CFG_dat = 0xd3d94fa8;	break;

			case 12:	CFG_dat = 0xf0d94fa8;	break;   //Set=200*5+500ns/400ua
			case 13:	CFG_dat = 0xf2d94fa8;	break;   //Set=200*5+500ns/520ua
			case 14:	CFG_dat = 0xf1d94fa8;	break;
			case 15:	CFG_dat = 0xf3d94fa8;	break;
			
			default:	CFG_dat = 0xf3d94fa8;	         //Set=200*5+500ns/520ua
		}
		
		#ifdef _DRIVER_H_
			TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
			fprintf( fp, "%6d	%6d  ",  x_coordinate, y_coordinate);
		#endif

		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		Delay( 10 MS );
		
		xmain( xmax() );
		ymain( ymax() );
		dmain( 0xffff );
		count( 1, amax() );
        test_result = funtest( int_clk_write_pat, finish );
		
		CFG_dat = CFG_dat | 0x00000004;
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		Delay( 10 MS );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for(  xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for(  yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				///////////////////////////////read_res///////////////////////////////////////
				
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				
				Delay( 10 MS );
				
				#ifdef _DRIVER_H_
						TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
				#endif
				
				vpar_force( 0.3 V );
				ipar_high( 2000 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current );
				
				for( int i = 0 ; i < 16 ; i++)
				{
					fprintf( fp, "%5d	%5d	%5d	%5d	%5d	", x_coordinate, y_coordinate, xaddress, yaddress,i );
					fprintf( fp, "%12.2f	", bitline_current[ i ]);
					if( bitline_current[ i ] < 30 )
						bitline_current[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current[ i ] ) );
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
		fprintf( fp, "config %12x ",CFG_dat );
		fprintf( fp, "set %5d Finished!\n",k );		
		
	}
	
	fclose(fp);
	
	return MULTI_DUT;
}


TEST_BLOCK( Set_Short_Width_Shmoo_Test )
{
    measure( TRUE );
    BOOL test_result;
    
//   system_initial_internal_vbg();
	system_initial_external_vbg();
    timing();

    DWORD CFG_dat;
    CArray<double,double> bitline_current;
    int xaddress,yaddress;
    
    numx( 10 );
    numy( 8 );
    data_reg_width ( 36 );
    x_fast_axis( TRUE );
    
    CFG_dat = trim_initial();
    
    FILE *fp = fopen( Set_Short_Width_Shmoo_Test_File, "a+" );
    
    for( int k = 0;  k < 16; k++ )
    {   
        pin_dc_state_set( ZZ, t_vih, TRUE );
        pin_dc_state_set( UB_pl, t_vil, TRUE );
        pin_dc_state_set( LB_pl, t_vil, TRUE );
        
        CFG_dat = 0x43d94fa8;       //Rst:50ns,1mA
//		CFG_dat=CFG_dat & ffffffef external VBG
        dmain( CFG_dat );
        funtest( CFG_set_PAT, finish );
        
        Delay( 10 MS);
        
        xmain( xmax() );
        ymain( ymax() );
        dmain( 0x0 );
        count( 1, amax() );
        test_result = funtest( int_clk_write_pat, finish );
        
        CFG_dat = CFG_dat | 0x00000004;
        dmain( CFG_dat );
        funtest( CFG_set_PAT, finish );
        Delay( 10 MS );
        
        pin_dc_state_set( DATA_pl, t_vil );
        
        for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
        {
            for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
            {
                xmain( xaddress );
                ymain( yaddress );
                test_result = funtest( address_pat, finish );
                Delay( 10 MS );
                
                ///////////////////////////////////////////////////////////////////////////
                
                pin_dc_state_set( WE,   t_vih);
                pin_dc_state_set( CE,   t_vil);
                pin_dc_state_set( ZZ,   t_vih);
                pin_dc_state_set( WE,   t_vil);
                
                Delay( 10 MS );
                
                #ifdef _DRIVER_H_
                    TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
                #endif
                
                vpar_force( 0.3 V );
                ipar_high( 200 UA );
                ipar_low( -0.5 UA );
                test_result = partest( pass_nicl, DATA_pl );
                Pin_meas( bitline_current );
                
                for( int i = 0 ; i < 16 ; i++)
                {
                    fprintf( fp, "%5d   %5d %5d %5d %5d ", x_coordinate, y_coordinate, xaddress, yaddress,i );
                    fprintf( fp, "%12.2f    ", bitline_current[ i ]);
                    if( bitline_current[ i ] < 30 )
                        bitline_current[ i ] = 30;
                    fprintf( fp, "%12.2f    ", 300000 / fabs( bitline_current[ i ] ) );
                    fprintf( fp, "%12.2f    ", log( 300000000 / fabs( bitline_current[ i ] ) ) / log( 10 ) );
                    fprintf( fp, "\n" );
                }
            }
        }
        fprintf( fp, "config %12x ",CFG_dat );
        fprintf( fp, "rst %5d Finished!\n",k );

		Delay( 10 MS);
        ///////////////////////////////////////////////////////////////////////////////////////////////////////
        pin_dc_state_set( ZZ, t_vih, TRUE );
        pin_dc_state_set( UB_pl, t_vil, TRUE );
        pin_dc_state_set( LB_pl, t_vil, TRUE ); 
        
        switch( k )
        {
            case 0:     CFG_dat = 0x00d94fb8;   break;   //Set=25*5+100ns/400ua
            case 1:     CFG_dat = 0x02d94fb8;   break;   //Set=25*5+100ns/520ua
            case 2:     CFG_dat = 0x01d94fb8;   break;   //Set=25*5+100ns/640ua
            case 3:     CFG_dat = 0x03d94fb8;   break;   //Set=25*5+100ns/760ua

            case 4:     CFG_dat = 0x20d94fb8;   break;   //Set=25*5+200ns/400ua
            case 5:     CFG_dat = 0x22d94fb8;   break;   //Set=25*5+200ns/520ua
            case 6:     CFG_dat = 0x21d94fb8;   break;   //Set=25*5+200ns/640ua
            case 7:     CFG_dat = 0x23d94fb8;   break;   //Set=25*5+200ns/760ua

            case 8:     CFG_dat = 0x80d94fb8;   break;   //Set=50*5+100ns/400ua
            case 9:     CFG_dat = 0x82d94fb8;   break;   //Set=50*5+100ns/520ua
            case 10:     CFG_dat = 0x81d94fb8;   break;   //Set=50*5+100ns/640ua
            case 11:     CFG_dat = 0x83d94fb8;   break;   //Set=50*5+100ns/760ua
            
            case 12:     CFG_dat = 0xa0d94fb8;   break;   //Set=50*5+200ns/400ua
            case 13:     CFG_dat = 0xa2d94fb8;   break;   //Set=50*5+200ns/520ua
            case 14:     CFG_dat = 0xa1d94fb8;   break;   //Set=50*5+200ns/640ua
            case 15:     CFG_dat = 0xa3d94fb8;   break;   //Set=50*5+200ns/760ua
                        
            default:    CFG_dat = 0xa0d94fb8;   break;   //Set=50*5+200ns/400ua
        }
        
        #ifdef _DRIVER_H_
            TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
            fprintf( fp, "%6d   %6d  ",  x_coordinate, y_coordinate);
        #endif

		pin_dc_state_set( ZZ, t_vih, TRUE );
        pin_dc_state_set( UB_pl, t_vil, TRUE );
        pin_dc_state_set( LB_pl, t_vil, TRUE ); 

		CFG_dat=CFG_dat & 0xffffffef ; //external VBG
        
		dmain( CFG_dat );
        funtest( CFG_set_PAT, finish );
        Delay( 10 MS );
        
        xmain( xmax() );
        ymain( ymax() );
        dmain( 0xffff );
        count( 1, amax() );
        test_result = funtest( int_clk_write_pat, finish );
        
        CFG_dat = CFG_dat | 0x00000004;
        dmain( CFG_dat );
        funtest( CFG_set_PAT, finish );
        Delay( 10 MS );
        
        pin_dc_state_set( DATA_pl, t_vil );
        
        for(  xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
        {
            for(  yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
            {
                
                xmain( xaddress );
                ymain( yaddress );
                test_result = funtest( address_pat, finish );
                Delay( 10 MS );
                
                ///////////////////////////////read_res///////////////////////////////////////
                
                
                pin_dc_state_set( WE,   t_vih);
                pin_dc_state_set( CE,   t_vil);
                pin_dc_state_set( ZZ,   t_vih);
                pin_dc_state_set( WE,   t_vil);
                
                
                Delay( 10 MS );
                
                #ifdef _DRIVER_H_
                        TPEGetCurrentXY( 0, &x_coordinate, &y_coordinate );
                #endif
                
                vpar_force( 0.3 V );
                ipar_high( 2000 UA );
                ipar_low( -0.5 UA );
                test_result = partest( pass_nicl, DATA_pl );
                Pin_meas( bitline_current );
                
                for( int i = 0 ; i < 16 ; i++)
                {
                    fprintf( fp, "%5d   %5d %5d %5d %5d ", x_coordinate, y_coordinate, xaddress, yaddress,i );
                    fprintf( fp, "%12.2f    ", bitline_current[ i ]);
                    if( bitline_current[ i ] < 30 )
                        bitline_current[ i ] = 30;
                    fprintf( fp, "%12.2f    ", 300000 / fabs( bitline_current[ i ] ) );
                    fprintf( fp, "%12.2f    ", log( 300000000 / fabs( bitline_current[ i ] ) ) / log( 10 ) );
                    fprintf( fp, "\n" );
                }
            }
        }
        fprintf( fp, "config %12x ",CFG_dat );
        fprintf( fp, "set %5d Finished!\n",k );

        //////////////////////////////////////////////////////////////////////////////////////////////////////

        
    }
    
    fclose(fp);
    
    return MULTI_DUT;
}


TEST_BLOCK( Drift_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	DWORD CFG_dat;
	int xaddress,yaddress;
	CArray<double,double> bitline_current_ini;

	int delay_time_1,delay_time_2,delay_time_3;
	delay_time_1=10;
	delay_time_2=100;
	delay_time_3=1000;
	int total_time=0;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );
//	ecr_config_set( 10, 8, DATA_pl );
//	data_strobe( 0xffff ); 	
//	ecr_all_clear();
	
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( Drift_Test_File, "a+" );	
	
	CFG_dat = 0x4bd97cb9 | 0x00000004;
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	Delay( 20 MS );	
	//Delay( 50 MS );



	////////////////////////////////////////////////////////////////////////////////////////Phase_1
	for(int j = 1 ;j < 11 ; j++ )
	{
		total_time+=delay_time_1;

		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				if( ( xaddress == 0 ) && ( yaddress == 0 ) )
					
					Delay( 20 MS );
				
				///////////////////////////////Ini///////////////////////////////////////
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				Delay( 20 MS );
				
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current_ini );
				
				for( int i = 0 ; i < 16 ; i++)
				{
					fprintf( fp, "%7d ",  total_time);
					fprintf( fp, " %5d %5d %5d %5d %5d",  x_coordinate, y_coordinate, xaddress, yaddress, i );
					fprintf( fp, "%12.2f	", bitline_current_ini[ i ]);
					if( bitline_current_ini[ i ] < 30 )
						bitline_current_ini[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current_ini[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current_ini[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
				
		Delay( delay_time_1 S );
				
		//output("%d\n",j);
	}
			
	/////////////////////////////////////////////////////////////////////////////////////////Phase_2
	for( j = 1 ;j < 10 ; j++ )
	{
		total_time+=delay_time_2;

		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				if( ( xaddress == 0 ) && ( yaddress == 0 ) )
					
					Delay( 20 MS );
				
				///////////////////////////////Ini///////////////////////////////////////
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				Delay( 20 MS );
				
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current_ini );
				
				for( int i = 0 ; i < 16 ; i++)
				{
					fprintf( fp, "%7d ",  total_time);
					fprintf( fp, " %5d %5d %5d %5d %5d",  x_coordinate, y_coordinate, xaddress, yaddress, i );
					fprintf( fp, "%12.2f	", bitline_current_ini[ i ]);
					if( bitline_current_ini[ i ] < 30 )
						bitline_current_ini[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current_ini[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current_ini[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
				
		Delay( delay_time_2 S );
				
		//output("%d\n",j);
	}

	/////////////////////////////////////////////////////////////////////////////////////////Phase_3			
/*	for( j = 1 ;j < 10 ; j++ )
	{
		total_time+=delay_time_3;

		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				if( ( xaddress == 0 ) && ( yaddress == 0 ) )
					
					Delay( 20 MS );
				
				///////////////////////////////Ini///////////////////////////////////////
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				Delay( 20 MS );
				
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( bitline_current_ini );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%7d ",  total_time);
					fprintf( fp, " %5d %5d %5d %5d %5d",  x_coordinate, y_coordinate, xaddress, yaddress, i );
					fprintf( fp, "%12.2f	", bitline_current_ini[ i ]);
					if( bitline_current_ini[ i ] < 30 )
						bitline_current_ini[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( bitline_current_ini[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( bitline_current_ini[ i ] ) ) / log( 10 ) );
					fprintf( fp, "\n" );
				}
			}
		}
				
		Delay( delay_time_3 S );
				
		//output("%d\n",j);
	}
*/

	fclose(fp);
	return MULTI_DUT;
}

TEST_BLOCK( IV_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	DWORD CFG_dat;

	int xaddress,yaddress;
	
	CArray<double,double> current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
		
	FILE *fp = fopen( IV_Test_File, "a+" );

	CFG_dat = trim_initial();
	CFG_dat = 0xcadbecb8;        //adjust				
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	CFG_dat = CFG_dat | 0x00000004;      
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
    pin_dc_state_set( DATA_pl, t_vil );
		
	for( xaddress = 2 ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = 2 ; yaddress < Y_Stop ; yaddress+=32 )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 200 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			for( int force_i = 0; force_i<= 50; force_i++ )      
			{
				ipar_force( force_i UA );
				vpar_high( 3 V );                  //adjust range
				vpar_low( -1 V );
				partest( pass_nivl, DATA_pl );
				//if(!force_i)
				//	pdatalog(DATA_pl);
				Pin_meas( current_result );
				
				fprintf( fp, "%5d	%5d ", xaddress, yaddress );
				//fprintf( fp," %10x	", CFG_dat );
				fprintf( fp, "%4d	",  force_i );
				
				for( int i = 0 ; i < 16; i++)
				{
					fprintf( fp, "%12.2f", current_result[i]/1000000 );
				}
				fprintf( fp, "\n" );
				
			}
		}
	}
	
	fclose( fp );
	return MULTI_DUT;
}


TEST_BLOCK( VI_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_internal_vbg();
	timing();
	
	DWORD CFG_dat;

	int xaddress,yaddress;
	
	CArray<double,double> current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
		
	FILE *fp = fopen( VI_Test_File, "a+" );

	CFG_dat = trim_initial();
	CFG_dat = 0xcadbecb8;        //adjust				
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	CFG_dat = CFG_dat | 0x00000004;      
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
    pin_dc_state_set( DATA_pl, t_vil );
		
	for( xaddress = 1 ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = 1 ; yaddress < Y_Stop ; yaddress+=32)
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 100 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			for( double force_v = 0; force_v<0.61; force_v=force_v+0.01 )      
			{
				vpar_force( force_v V );
				ipar_high( 20 UA );                  //adjust range
				ipar_low( -1 UA );
				partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				fprintf( fp, "%5d	%5d ", xaddress, yaddress );
				//fprintf( fp," %10x	", CFG_dat );
				fprintf( fp, "%12.3f	",  force_v );
				
				for( int i = 0 ; i < 16; i++)
				{
					fprintf( fp, "%12.2f", current_result[i] );
				}
				fprintf( fp, "\n" );
				
			}
		}
	}
	
	fclose( fp );

	return MULTI_DUT;
}

TEST_BLOCK( External_Rst_Height_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	
	FILE *fp2 = fopen( External_Rst_Height_Test_File, "a+" );
	
	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	vil( 0.00 V, VBG_pl );
	vih( external_vbg V, VBG_pl );
	
	CFG_dat = 0x83db4cc9;    // extern VBG CLK(MLC)
//	CFG_dat = 0xc09b4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns(STS)
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	xmain( 0 );
	ymain( 0 );
	dmain( 0x0 );
	count( 3, amax());
	test_result = funtest( fastest_write_test_pattern, finish );
	
	RE_CFG_dat = CFG_dat | 0x00000004;      
	dmain( RE_CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	pin_dc_state_set( DATA_pl, t_vil );
				
	for(  xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for(  yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 10 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( current_result );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp2, "%5d	%5d	 %12.2f  %5d", xaddress, yaddress,external_vbg, i );
				fprintf( fp2, "%12.2f	", current_result[ i ]);
				if( current_result[ i ] < 30 )
					current_result[ i ] = 30;
				fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
				fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
				
				fprintf( fp2, "\n" );
			}					
		}
	}
	
	fprintf( fp2, "VBG %12.2f RST_Current %12.2f CFG_dat %10x \n",external_vbg , (external_vbg/3.75)*1 ,CFG_dat );
	
	fclose(fp2);
	
	return MULTI_DUT;

}


TEST_BLOCK( External_Set_Height_Test )  
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	
	FILE *fp2 = fopen( External_Set_Height_Test_File, "a+" );
	
	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	vil( 0.00 V, VBG_pl );
	vih( external_vbg V, VBG_pl );
	
	CFG_dat = 0x80db4cc9;    // extern VBG CLK (set:7 clock/rst :1 clock)
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	xmain( 0 );
	ymain( 0 );
	dmain( 0xffff );
	count( 3, amax());
	test_result = funtest( fastest_write_test_pattern, finish );
	
	RE_CFG_dat = CFG_dat | 0x00000004;      
	dmain( RE_CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	pin_dc_state_set( DATA_pl, t_vil );
				
	for(  xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for(  yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 10 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( current_result );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp2, "%5d	%5d	 %12.2f  %5d", xaddress, yaddress,external_vbg, i );
				fprintf( fp2, "%12.2f	", current_result[ i ]);
				if( current_result[ i ] < 30 )
					current_result[ i ] = 30;
				fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
				fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
				
				fprintf( fp2, "\n" );
			}					
		}
	}

	fprintf( fp2, "VBG %12.2f CFG_dat %10x \n",external_vbg ,CFG_dat );
					
	fclose(fp2);
	
	return MULTI_DUT;

}

TEST_BLOCK(RST_Different_Vbg_Shmoo_Test_1)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( RST_Different_Vbg_Shmoo_Test_FILE_1, "a+" );
//	FILE *fp2 = fopen( RST_Different_Vbg_Shmoo_Test_Set_Resistance_FILE_1, "a+" );
	FILE *fp3 = fopen( RST_Different_Vbg_Shmoo_Test_FILE_V_I_1, "a+" );
	FILE *fp4 = fopen( RST_Different_Vbg_Shmoo_Test_FILE_I_V_1, "a+" );
	
	for(double ex_vbg= 3.75; ex_vbg< 4.25; ex_vbg=ex_vbg+0.4)
	{	
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( 3.75 V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0xc01b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns   xie 1
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0xffff );
		count( 3, amax());
		test_result = funtest( fastest_write_test_pattern, finish );
/*
		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );

		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.64/3.75)*ex_vbg,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
				
			}

		}
*/		
		Delay( 50 MS);

		//////////////////////////////
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		//////////////////////////////////////
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( ex_vbg V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0xc0db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)      
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0x0 );
		count( 3, amax());
		test_result = funtest( fastest_write_test_pattern, finish );
		
		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.64/3.75)*ex_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
			}

		}
		/////////////////////////////////////////V-I///////////////////////////////////////////////////////
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 100 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				for( double force_v = 0; force_v<0.61; force_v=force_v+0.01 )      
				{
					vpar_force( force_v V );
					ipar_high( 20 UA );                  //adjust range
					ipar_low( -1 UA );
					partest( pass_nicl, DATA_pl );
					Pin_meas( current_result );
					
					fprintf( fp3, "%5d	%5d ", xaddress, yaddress );
					fprintf( fp3," %12.2f %10x	", ex_vbg, CFG_dat );
					fprintf( fp3, "%12.3f	",  force_v );
					
					for( int i = 0 ; i < 16; i++)
					{
						fprintf( fp3, "%12.2f", current_result[i] );
					}
					fprintf( fp3, "\n" );
					
				}
			}
		}
		//////////////////////////////////////I-V////////////////////////////////////////////////////
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 100 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				for( int force_i = 0; force_i<= 50; force_i++ )      
				{
					ipar_force( force_i UA );
					vpar_high( 3 V );                  //adjust range
					vpar_low( -1 V );
					partest( pass_nivl, DATA_pl );
					//if(!force_i)
					//	pdatalog(DATA_pl);
					Pin_meas( current_result );
					
					fprintf( fp4, "%5d	%5d ", xaddress, yaddress );
					fprintf( fp4," %12.2f %10x	", ex_vbg, CFG_dat );
					fprintf( fp4, "%4d	",  force_i );
					
					for( int i = 0 ; i < 16; i++)
					{
						fprintf( fp4, "%12.2f", current_result[i]/1000000 );
					}
					fprintf( fp4, "\n" );
					
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////

	}	
	fclose(fp);
//	fclose(fp2);
	fclose(fp3);
	fclose(fp4);

	return MULTI_DUT;

}

TEST_BLOCK(RST_Different_Vbg_Shmoo_Test_9)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( RST_Different_Vbg_Shmoo_Test_FILE_9, "a+" );
	FILE *fp2 = fopen( SET_Before_RST_Different_Vbg_Shmoo_Test_FILE_9, "a+" );
	
	for(double ex_vbg= 2.131; ex_vbg< 2.35; ex_vbg=ex_vbg+0.042)
	{	
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( 3.75 V, VBG_pl );
		
		//	CFG_dat = trim_initial();
//		CFG_dat = 0xc21b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns    xie 1
		CFG_dat = 0xc2dd4ca9;   //rst 50ns/1ma
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( xmax() );
		ymain( ymax() );
		dmain( 0xffff );
		count( 1, amax() );
		test_result = funtest( int_clk_write_pat, finish );
		
		Delay( 50 MS);


		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );

		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.52/3.75)*3.75,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
				
			}

		}


		////////////////////////////////////
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		/////////////////////////////////////

		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( ex_vbg V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0xc09b4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0x0 );
		count( 3, amax());
		test_result = funtest( fastest_write_test_pattern, finish );
		
		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.88/3.75)*ex_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
			}

		}
	}

	fclose(fp);
	fclose(fp2);

	return MULTI_DUT;

}

TEST_BLOCK( Staircase_Down_Pulset_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	
	FILE *fp = fopen( Staircase_Down_Pulset_Test_File, "a+" );
	
	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	vil( 0.00 V, VBG_pl );
	vih( external_vbg V, VBG_pl );
	
	CFG_dat = 0x83db4cc9;    // extern VBG CLK (set:7 clock/rst :1 clock)
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	Delay( 50 MS);
	
	xmain( 0 );
	ymain( 0 );
	dmain( 0xffff );
//	test_result = funtest( staircase_down_external_write_test_pattern, finish );
//	test_result = funtest( single_external_write_test_pattern, finish );
//	test_result = funtest( double_staircase_down_external_write_test_pattern, finish );	
	test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish ); //没有用fastest_write_test_pattern时需要把count3屏蔽
	
	RE_CFG_dat = CFG_dat | 0x00000004;      
	dmain( RE_CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	pin_dc_state_set( DATA_pl, t_vil );
				
	for(  xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for(  yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 10 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( current_result );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp, "%5d	%5d	 %12.2f  %5d", xaddress, yaddress,external_vbg, i );
				fprintf( fp, "%12.2f	", current_result[ i ]);
				if( current_result[ i ] < 30 )
					current_result[ i ] = 30;
				fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
				fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
				
				fprintf( fp, "\n" );
			}					
		}
	}

	fprintf( fp, "VBG %12.2f CFG_dat %10x \n",external_vbg ,CFG_dat );
					
	fclose(fp);
	
	return MULTI_DUT;

}

TEST_BLOCK(SET_Different_Vbg_Shmoo_Test)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( SET_Different_Vbg_Shmoo_Test_FILE, "a+" );
	FILE *fp2 = fopen( RST_Before_SET_Different_Vbg_Shmoo_Test_FILE, "a+" );
	
//	for(double ex_vbg= 2.5; ex_vbg< 2.6; ex_vbg=ex_vbg+0.5)
	for(double ex_vbg= 2.0; ex_vbg< 2.1; ex_vbg=ex_vbg+0.5)	
	{	
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( 3.75 V, VBG_pl );
		
		//	CFG_dat = trim_initial();
//		CFG_dat = 0xc01b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns    xie 1
		CFG_dat = 0xf3dd4ca9;   //rst 50ns/1ma
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( xmax() );
		ymain( ymax() );
		dmain( 0x0 );
		count( 1, amax() );
		test_result = funtest( int_clk_write_pat, finish );

		Delay( 50 MS);

		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,3.75,(1/3.75)*3.75,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
				
			}

		}

		////////////////////////////////////
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		/////////////////////////////////////

		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( ex_vbg V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0x83db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
//		CFG_dat = 0x80db4cc9;		
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0xffff );
		count( 3, amax());																			//没有用fastest_write_test_pattern时需要把count3屏蔽
		test_result = funtest( fastest_write_test_pattern, finish ); //jiyizhong 7times clk
//		test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish );		//没有用fastest_write_test_pattern时需要把count3屏蔽
//		test_result = funtest( double_staircase_down_external_write_test_pattern, finish );			//没有用fastest_write_test_pattern时需要把count3屏蔽

		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.76/3.75)*ex_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
			}

		}
	}
	
	fclose(fp);
	fclose(fp2);

	return MULTI_DUT;

}

TEST_BLOCK(Single_Step_SET_Different_Vbg_Shmoo_Test)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( Single_Step_SET_Different_Vbg_Shmoo_Test_FILE, "a+" );
	FILE *fp2 = fopen( RST_Before_Single_Step_SET_Different_Vbg_Shmoo_Test_FILE, "a+" );
	
	for(double ex_vbg= 0.85; ex_vbg< 4.3; ex_vbg=ex_vbg+0.42)
	{	
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( 3.75 V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0xf3dd4ca9;     //rst 50ns/1ma set 1.5us/0.76ma  only extern bandgap used
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( xmax() );
		ymain( ymax() );
		dmain( 0x0 );
		count( 1, amax() );
		test_result = funtest( int_clk_write_pat, finish );
		
		Delay( 50 MS);

		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,3.75,(1/3.75)*3.75,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
				
			}

		}

		////////////////////////////////////
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		/////////////////////////////////////

		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( ex_vbg V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0x835b4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns    
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0x0 );
		count( 3, amax());
		test_result = funtest( fastest_write_test_pattern, finish );
		
		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.88/3.75)*ex_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
			}

		}
	}	
	fclose(fp);
	fclose(fp2);

	return MULTI_DUT;

}

TEST_BLOCK( Voltage_Stress_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	//DWORD CFG_dat;
	DWORD RE_CFG_dat;
	
	int xaddress,yaddress ;
	
	int time_num = 10; //1000,100000,10000000
	
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( Voltage_Stress_Test_File, "a+" );

	//////////////////////////////////////////////////////////////////////////////////////测完所有地址来读电阻
/*
	for( int num = 1; num<11; num= num+1)  
	{
		for(double V_force = 0.8; V_force<0.9; V_force=V_force+0.1 )
		{	
			
			RE_CFG_dat = 0xf3dd4ca9 | 0x00000004;      //open
			dmain( RE_CFG_dat );
			funtest( CFG_set_PAT, finish );
			
			pin_dc_state_set( DATA_pl, t_vil );  
			vil( 0.0 V, DATA_pl );
			vih( V_force V, DATA_pl );	     // force  V 
			
			for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
			{
				for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
				{
					xmain( xaddress );
					ymain( yaddress );
					test_result = funtest( address_pat, finish );
					
					Delay( 10 MS );
					
					pin_dc_state_set( WE,	t_vih);
					pin_dc_state_set( CE,	t_vil);
					pin_dc_state_set( ZZ,	t_vih);
					pin_dc_state_set( WE,	t_vil);
					
					xmain( xaddress );
					ymain( yaddress );
					dmain(0xffff);
					count( 2, time_num-1);
					test_result = funtest( force_V_pat, finish );  
					
				}
			}
			
			
			///////////////////////////////////////////////////////////////////////////////R_distribution
			pin_dc_state_set( DATA_pl, t_vil );
			
			for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
			{
				for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
				{
					xmain( xaddress );
					ymain( yaddress );
					test_result = funtest( address_pat, finish );
					Delay( 10 MS );
					
					pin_dc_state_set( WE,	t_vih);
					pin_dc_state_set( CE,	t_vil);
					pin_dc_state_set( ZZ,	t_vih);
					pin_dc_state_set( WE,	t_vil);
					
					vpar_force( 0.3 V );
					ipar_high( 200 UA );
					ipar_low( -0.5 UA );
					test_result = partest( pass_nicl, DATA_pl );
					Pin_meas( current_result );
					
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%10d	%5d %5d	  %10x  %5d", num * time_num ,xaddress, yaddress,RE_CFG_dat,i );
						fprintf( fp, "%12.2f	", current_result[ i ]);
						if( current_result[ i ] < 30 )
							current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					
				}
				
			}
		}
	}
*/	

	//////////////////////////////////////////////////////////////////////////////////////测完一个地址立即读电阻

	for(double V_force = 0.8; V_force<0.9; V_force=V_force+0.1 )
	{	
		
		RE_CFG_dat = 0xf3dd4ca9 | 0x00000004;      //open
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );  
		vil( 0.0 V, DATA_pl );
		vih( V_force V, DATA_pl );	     // force  V 
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				for( int num = 1; num<11; num= num+1)  
				{
					xmain( xaddress );
					ymain( yaddress );
					test_result = funtest( address_pat, finish );
					
					Delay( 10 MS );
					
					pin_dc_state_set( WE,	t_vih);
					pin_dc_state_set( CE,	t_vil);
					pin_dc_state_set( ZZ,	t_vih);
					pin_dc_state_set( WE,	t_vil);

					//pin_dc_state_set( DATA_pl, t_vih ); 
					
					xmain( xaddress );
					ymain( yaddress );
					dmain(0xffff);
					count( 2, time_num-1);
					test_result = funtest( force_V_pat, finish ); 
					
					//////////////////////////////////////////////////////////////////////R_Distribution
					pin_dc_state_set( DATA_pl, t_vil );
					
					vpar_force( 0.3 V );
					ipar_high( 200 UA );
					ipar_low( -0.5 UA );
					test_result = partest( pass_nicl, DATA_pl );
					Pin_meas( current_result );
					
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%10d	%5d %5d	  %10x  %5d", num * time_num ,xaddress, yaddress,RE_CFG_dat,i );
						fprintf( fp, "%12.2f	", current_result[ i ]);
						if( current_result[ i ] < 30 )
							current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					
				}
			}
			
		}
		
	}
	
	fclose(fp);

	return MULTI_DUT;
}



TEST_BLOCK(old_SET_Different_Clk_Time_Shmoo_Test) //but work noraml
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( SET_Different_Clk_Time_Shmoo_Test_FILE, "a+" );
	FILE *fp2 = fopen( RST_Before_SET_Different_Clk_Time_Shmoo_Test_FILE, "a+" );
	
	
	
	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	vil( 0.00 V, VBG_pl );
	vih( 3.75 V, VBG_pl );
	
	//CFG_dat = trim_initial();
	//CFG_dat = 0xc01b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns    xie 1
	CFG_dat = 0xf3dd4ca9;   //rst 50ns/1ma
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	xmain( xmax() );
	ymain( ymax() );
	dmain( 0x0 );
	count( 1, amax() );
	test_result = funtest( int_clk_write_pat, finish );
	
	Delay( 50 MS);
	
	RE_CFG_dat = CFG_dat | 0x00000004;      
	dmain( RE_CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	pin_dc_state_set( DATA_pl, t_vil );
	
	for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 10 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( current_result );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,3.75,(1/3.75)*3.75,CFG_dat,i );
				fprintf( fp2, "%12.2f	", current_result[ i ]);
				if( current_result[ i ] < 30 )
					current_result[ i ] = 30;
				fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
				fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
				
				fprintf( fp2, "\n" );
			}
			
		}
		
	}

	////////////////////////////////////
	system_initial_external_vbg();
		
	pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	/////////////////////////////////////	

	
	pin_dc_state_set( VBG_pl, t_vih, TRUE );
	vil( 0.00 V, VBG_pl );
	vih( external_vbg V, VBG_pl );           //fixed current
	
	//	Tset_Time 需要大于10
	
	cycle(TSET7, Tset_Time NS);//extern_clock_write
	settime(TSET7,	CLKVCO_pl,			RTZ,	4 + (Tset_Time / 10)			NS, 4 + (Tset_Time / 10) + (Tset_Time / 2) NS);
	settime(TSET7,	CE,					NRZ,	1 + (Tset_Time / 20)			NS); 
	settime(TSET7,	WE,					NRZ,	1 + (Tset_Time / 20)			NS);
	settime(TSET7,	OE,					NRZ,	1 + (Tset_Time / 20)			NS);
	settime(TSET7,	ADDRESS_pl,			NRZ,	1 + (Tset_Time / 20)			NS);
	settime(TSET7,	DATA_pl,			NRZ,	1 + (Tset_Time / 20)			NS);
	
	//	CFG_dat = trim_initial();
	CFG_dat = 0x83db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
	dmain( CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	xmain( 0 );
	ymain( 0 );
	dmain( 0xffff );
	count( 3, amax());																			//没有用fastest_write_test_pattern时需要把count3屏蔽
	test_result = funtest( fastest_write_test_pattern, finish ); //jiyizhong 7times clk
	//test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish );	//没有用fastest_write_test_pattern时需要把count3屏蔽
	//test_result = funtest( double_staircase_down_external_write_test_pattern, finish );		//没有用fastest_write_test_pattern时需要把count3屏蔽
	
	RE_CFG_dat = CFG_dat | 0x00000004;      
	dmain( RE_CFG_dat );
	funtest( CFG_set_PAT, finish );
	
	pin_dc_state_set( DATA_pl, t_vil );
	
	
	for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			Delay( 10 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( current_result );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp, "%5d	%5d	 %5d %7.5f %12.5f %10x  %5d", xaddress, yaddress,Tset_Time,external_vbg,(0.76/3.75)*external_vbg,CFG_dat,i );
				fprintf( fp, "%12.2f	", current_result[ i ]);
				if( current_result[ i ] < 30 )
					current_result[ i ] = 30;
				fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
				fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
				
				fprintf( fp, "\n" );
			}
			
		}
		
	}
	
	
	fclose(fp);
	fclose(fp2);
	
	return MULTI_DUT;

}



TEST_BLOCK( Cycle_Test )
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
//	double external_vbg = 3.75;
	int cycle_times=10000000; //100,10000,1000000,100000000
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( RST_Resistance_Distribution_Test_FILE, "a+" );
	FILE *fp2 = fopen( SET_Resistance_Distribution_Test_FILE, "a+" );
	
	
	for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
	{
		for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
		{
			for( int k=1 ; k < 11 ; k+=1 )
			{

				system_initial_external_vbg();
		
				pin_dc_state_set( ZZ, t_vih, TRUE );
				pin_dc_state_set( UB_pl, t_vil, TRUE );
				pin_dc_state_set( LB_pl, t_vil, TRUE );
				
				pin_dc_state_set( VBG_pl, t_vih, TRUE );
				vil( 0.00 V, VBG_pl );
				vih( external_vbg V, VBG_pl );
				
				//	CFG_dat = trim_initial();
//				CFG_dat = 0x82db4cc9;   //rst 50ns/1ma   3.61V下set配置为0（0.52mA/3.75v）才能实现set电流为0.5mA，且保证rst电流为0.9mA
				CFG_dat = 0x80db4cc9; 
				dmain( CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				xmain( xaddress );
				ymain( yaddress );
				count( 3, cycle_times );
				count( 4, 0 );			//pattern 中地址不变
				test_result = funtest( reset_set_01_cycle_pat, finish );
				
				
				Delay( 10 MS);
				
				///////////////////////////////////////////////////////////////////////RST后读电阻
				system_initial_external_vbg();
				pin_dc_state_set( ZZ, t_vih, TRUE );
				pin_dc_state_set( UB_pl, t_vil, TRUE );
				pin_dc_state_set( LB_pl, t_vil, TRUE );

				pin_dc_state_set( VBG_pl, t_vih, TRUE );
				vil( 0.00 V, VBG_pl );
				vih( external_vbg V, VBG_pl );
				
				CFG_dat = 0x80db4cc9;
//				CFG_dat = 0xf3dd4ca9;
				dmain( CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				xmain( xaddress );
				ymain( yaddress );
				dmain( 0x0 );
//				count( 1, 0 );
//				test_result = funtest( int_clk_write_pat, finish );

				count( 3, 0);
				test_result = funtest( fastest_write_rst_test_pattern, finish );
				
				RE_CFG_dat = CFG_dat | 0x00000004;      
				dmain( RE_CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				pin_dc_state_set( DATA_pl, t_vil );
				
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%10d %5d	%5d	 %12.5f  %12.5f %10x  %5d", k * cycle_times,xaddress, yaddress,external_vbg,(1/3.75)*external_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
				
				///////////////////////////////////////////////////////////////////////SET后读电阻
				system_initial_external_vbg();
		
				pin_dc_state_set( ZZ, t_vih, TRUE );
				pin_dc_state_set( UB_pl, t_vil, TRUE );
				pin_dc_state_set( LB_pl, t_vil, TRUE );

				pin_dc_state_set( VBG_pl, t_vih, TRUE );
				vil( 0.00 V, VBG_pl );
				vih( external_vbg V, VBG_pl );

				CFG_dat = 0x80db4cc9;   
				dmain( CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				xmain( xaddress );
				ymain( yaddress );
				dmain( 0xffff );
				count( 3, 0);
				test_result = funtest( fastest_write_test_pattern, finish );
				
				RE_CFG_dat = CFG_dat | 0x00000004;      
				dmain( RE_CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				pin_dc_state_set( DATA_pl, t_vil );
				
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				Delay( 10 MS );
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for(  i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%10d %5d	%5d	%12.5f  %12.5f %10x  %5d",k * cycle_times ,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
			}
		}
		
	}
	
	fclose(fp);
	fclose(fp2);
	
	return MULTI_DUT;
	
}

TEST_BLOCK(SET_Different_Clk_Time_Shmoo_Test)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( SET_Different_Clk_Time_Shmoo_Test_FILE, "a+" );
	FILE *fp2 = fopen( RST_Before_SET_Different_Clk_Time_Shmoo_Test_FILE, "a+" );
	
	
	for(int Tset_Time= 14; Tset_Time< 43; Tset_Time=Tset_Time + 2)
//	for(int Tset_Time= 70; Tset_Time< 90; Tset_Time=Tset_Time + 10)
	{
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );

		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( 3.75 V, VBG_pl );
		
		//CFG_dat = trim_initial();
		//CFG_dat = 0xc01b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns    xie 1
		CFG_dat = 0xf3dd4ca9;   //rst 50ns/1ma
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( xmax() );
		ymain( ymax() );
		dmain( 0x0 );
		count( 1, amax() );
		test_result = funtest( int_clk_write_pat, finish );
		
		Delay( 50 MS);
		
		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,3.75,(1/3.75)*3.75,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
				
			}
			
		}
		
		////////////////////////////////////
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		/////////////////////////////////////	
		
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( external_vbg V, VBG_pl );           //fixed current
		
		//	Tset_Time 需要大于10
		
		cycle(TSET7, Tset_Time NS);//extern_clock_write
		settime(TSET7,	CLKVCO_pl,			RTZ,	4 + (Tset_Time / 10)			NS, 4 + (Tset_Time / 10) + (Tset_Time / 2) NS);
		settime(TSET7,	CE,					NRZ,	1 + (Tset_Time / 20)			NS); 
		settime(TSET7,	WE,					NRZ,	1 + (Tset_Time / 20)			NS);
		settime(TSET7,	OE,					NRZ,	1 + (Tset_Time / 20)			NS);
		settime(TSET7,	ADDRESS_pl,			NRZ,	1 + (Tset_Time / 20)			NS);
		settime(TSET7,	DATA_pl,			NRZ,	1 + (Tset_Time / 20)			NS);
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0x03db4cc9;
//		CFG_dat = 0x83db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0xffff );
		count( 3, amax());																			//没有用fastest_write_test_pattern时需要把count3屏蔽
		test_result = funtest( fastest_write_test_pattern, finish ); //jiyizhong 7times clk
		//test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish );	//没有用fastest_write_test_pattern时需要把count3屏蔽
		//test_result = funtest( double_staircase_down_external_write_test_pattern, finish );		//没有用fastest_write_test_pattern时需要把count3屏蔽
		
		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					//fprintf( fp, "%5d	%5d	 %5d   %7d  %7.5f  %12.5f  %10x  %5d", xaddress, yaddress,Tset_Time,Tset_Time * 7,external_vbg,(0.76/3.75)*external_vbg,CFG_dat,i );
					fprintf( fp, "%5d	%5d	 %5d   %7.5f  %7.5f  %12.5f  %10x  %5d", xaddress, yaddress,Tset_Time,Tset_Time * 4.5,external_vbg,(0.76/3.75)*external_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
			}
			
		}
	}	

	fclose(fp);
	fclose(fp2);

	return MULTI_DUT;

}


TEST_BLOCK(SET_Different_Vbg_Shmoo_Slope_Version_Test)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> current_result,bitline_current_ini;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( SET_Different_Vbg_Shmoo_Slope_Version_Test_FILE, "a+" );
	FILE *fp2 = fopen( RST_Before_SET_Different_Vbg_Shmoo_Slope_Version_Test_FILE, "a+" );
	
//	for(double ex_vbg= 2.5; ex_vbg< 2.6; ex_vbg=ex_vbg+0.5)
	for(double ex_vbg= 2.5; ex_vbg< 2.6; ex_vbg=ex_vbg+0.5)	
	{	
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		
		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( 3.75 V, VBG_pl );
		
		//	CFG_dat = trim_initial();
//		CFG_dat = 0xc01b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns    xie 1
		CFG_dat = 0xf3dd4ca9;   //rst 50ns/1ma
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( xmax() );
		ymain( ymax() );
		dmain( 0x0 );
		count( 1, amax() );
		test_result = funtest( int_clk_write_pat, finish );

		Delay( 50 MS);

		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,3.75,(1/3.75)*3.75,CFG_dat,i );
					fprintf( fp2, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp2, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp2, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp2, "\n" );
				}
				
			}

		}

		////////////////////////////////////
		system_initial_external_vbg();
		
		pin_dc_state_set( ZZ, t_vih, TRUE );
		pin_dc_state_set( UB_pl, t_vil, TRUE );
		pin_dc_state_set( LB_pl, t_vil, TRUE );
		/////////////////////////////////////

		pin_dc_state_set( VBG_pl, t_vih, TRUE );
		vil( 0.00 V, VBG_pl );
		vih( ex_vbg V, VBG_pl );
		
		//	CFG_dat = trim_initial();
		CFG_dat = 0x83db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
//		CFG_dat = 0x80db4cc9;		
		dmain( CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		xmain( 0 );
		ymain( 0 );
		dmain( 0xffff );
		count( 4, amax());																			//没有用fastest_write_test_pattern时需要把count3屏蔽
		test_result = funtest( fastest_write_test_pattern_slope_version_pattern, finish ); //jiyizhong 7times clk
//		test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish );		//没有用fastest_write_test_pattern时需要把count3屏蔽
//		test_result = funtest( double_staircase_down_external_write_test_pattern, finish );			//没有用fastest_write_test_pattern时需要把count3屏蔽

		RE_CFG_dat = CFG_dat | 0x00000004;      
		dmain( RE_CFG_dat );
		funtest( CFG_set_PAT, finish );
		
		pin_dc_state_set( DATA_pl, t_vil );
		
		
		for( xaddress = X_Start ; xaddress < X_Stop ; xaddress+=X_Step )
		{
			for( yaddress = Y_Start ; yaddress < Y_Stop ; yaddress+=Y_Step )
			{
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( current_result );
				
				for( int i = 0 ; i < 16 ; i++)
				{	
					fprintf( fp, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,ex_vbg,(0.76/3.75)*ex_vbg,CFG_dat,i );
					fprintf( fp, "%12.2f	", current_result[ i ]);
					if( current_result[ i ] < 30 )
						current_result[ i ] = 30;
					fprintf( fp, "%12.2f	", 300000 / fabs( current_result[ i ] )		);
					fprintf( fp, "%12.2f	", log( 300000000 / fabs( current_result[ i ] ) ) / log( 10 ) );
					
					fprintf( fp, "\n" );
				}
				
			}

		}
	}
	
	fclose(fp);
	fclose(fp2);

	return MULTI_DUT;

}

TEST_BLOCK(SET_Program_Verify_Test)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;
	
	CArray<double,double> rst_current_result,set_current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( SET_Program_Verify_Test_FILE, "a+" );
	FILE *fp2 = fopen( RST_Before_SET_Program_Verify_Test_FILE, "a+" );
		
	for( xaddress = X_Start_Verify ; xaddress < X_Stop_Verify ; xaddress+=X_Step_Verify )
	{
		for( yaddress = Y_Start_Verify ; yaddress < Y_Stop_Verify ; yaddress+=Y_Step_Verify )
		{
//			for( int k = 0 ; k < 5 ; k++)
//			{
			system_initial_external_vbg();
			pin_dc_state_set( ZZ, t_vih, TRUE );
			pin_dc_state_set( UB_pl, t_vil, TRUE );
			pin_dc_state_set( LB_pl, t_vil, TRUE );

			pin_dc_state_set( VBG_pl, t_vih, TRUE );
			vil( 0.00 V, VBG_pl );
			vih( 3.75 V, VBG_pl );
			
			//CFG_dat = trim_initial();
			//CFG_dat = 0xc01b4cc9;    // extern VBG CLK I = 0.64(1)0.88(5)1(d)0.76(9)   width  50ns    xie 1
			CFG_dat = 0x80db4cc9;   //rst 50ns/1ma
			dmain( CFG_dat );
			funtest( CFG_set_PAT, finish );
			
			xmain( xaddress );
			ymain( yaddress );
			dmain( 0x0 );
			count( 3, 0 );
			test_result = funtest( fastest_write_rst_test_pattern, finish );
			
			//Delay( 50 MS);
			
			RE_CFG_dat = CFG_dat | 0x00000004;      
			dmain( RE_CFG_dat );
			funtest( CFG_set_PAT, finish );
			
			pin_dc_state_set( DATA_pl, t_vil );
			
			
			xmain( xaddress );
			ymain( yaddress );
			test_result = funtest( address_pat, finish );
			//Delay( 10 MS );
			
			pin_dc_state_set( WE,	t_vih);
			pin_dc_state_set( CE,	t_vil);
			pin_dc_state_set( ZZ,	t_vih);
			pin_dc_state_set( WE,	t_vil);
			
			vpar_force( 0.3 V );
			ipar_high( 200 UA );
			ipar_low( -0.5 UA );
			test_result = partest( pass_nicl, DATA_pl );
			Pin_meas( rst_current_result );
			
			for( int i = 0 ; i < 16 ; i++)
			{	
				fprintf( fp2, "%5d	%5d	 %12.2f  %12.5f %10x  %5d", xaddress, yaddress,3.75,(1/3.75)*3.75,CFG_dat,i );
				fprintf( fp2, "%12.2f	", rst_current_result[ i ]);
				if( rst_current_result[ i ] < 30 )
					rst_current_result[ i ] = 30;
				fprintf( fp2, "%12.2f	", 300000 / fabs( rst_current_result[ i ] )		);
				fprintf( fp2, "%12.2f	", log( 300000000 / fabs( rst_current_result[ i ] ) ) / log( 10 ) );
				
				fprintf( fp2, "\n" );
			}
			
			
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////SET
			

			
			for( int k = 0 ; k < 20 ; k++)
			{
				system_initial_external_vbg();
				pin_dc_state_set( ZZ, t_vih, TRUE );
				pin_dc_state_set( UB_pl, t_vil, TRUE );
				pin_dc_state_set( LB_pl, t_vil, TRUE );			
			
				pin_dc_state_set( VBG_pl, t_vih, TRUE );
				vil( 0.00 V, VBG_pl );
				vih( external_vbg V, VBG_pl );           //fixed vbg/current
				 
				int Tset_Time = SET_Time_Verify(10,2,k);
				//SET_Time_Verify(10,2,k);    	//	total需要大于10
				
				//	CFG_dat = trim_initial();
				CFG_dat = 0x80db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
				dmain( CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				xmain( xaddress );
				ymain( yaddress );
				dmain( 0xffff );
				count( 3, 0);																			//没有用fastest_write_test_pattern时需要把count3屏蔽
				test_result = funtest( fastest_write_test_pattern, finish ); //jiyizhong 7times clk
				//test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish );	//没有用fastest_write_test_pattern时需要把count3屏蔽
				//test_result = funtest( double_staircase_down_external_write_test_pattern, finish );		//没有用fastest_write_test_pattern时需要把count3屏蔽
				
				
				RE_CFG_dat = CFG_dat | 0x00000004;      
				dmain( RE_CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				pin_dc_state_set( DATA_pl, t_vil );
				
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				//Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( set_current_result );
				
				///////////////////////////////////////////////////////////////Judge
				
				int range_1 = 0;						//<5.5
				int range_2 = 0;						//5.5-6.0
				int range_3 = 0;						//>6.0
				
				float min_range = 948;					//related to log_r-5.5
				float max_range = 300;					//related to log_r-6.0
				
				for ( int i = 0; i < 16; i++)
				{
					if (set_current_result[i] > min_range)
						range_1 = range_1 + 1;
					else if (set_current_result[i] > max_range)
						range_2 = range_2 + 1;
					else
						range_3 = range_3 + 1;
				}

//				output("%d %d %d %d %d\n",k,range_1,range_2,range_3,Tset_Time);

				
				if(range_1 > 0)
				{
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%5d %5d	%5d	 %12.2f  %12.5f %10x  %5d", k,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
						fprintf( fp, "%12.2f	", set_current_result[ i ]);
						if( set_current_result[ i ] < 30 )
							set_current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( set_current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( set_current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					break;
				}	
				else if (range_3 < 2)
				{
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%5d %5d	%5d	 %12.2f  %12.5f %10x  %5d", k,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
						fprintf( fp, "%12.2f	", set_current_result[ i ]);
						if( set_current_result[ i ] < 30 )
							set_current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( set_current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( set_current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					break;
				}
				else if (k == 14)
				{
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%5d %5d	%5d	 %12.2f  %12.5f %10x  %5d", k,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
						fprintf( fp, "%12.2f	", set_current_result[ i ]);
						if( set_current_result[ i ] < 30 )
							set_current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( set_current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( set_current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					break;
				}

			}
			
		}
	}
	
	fclose(fp);
	fclose(fp2);
	
	return MULTI_DUT;
	
}

TEST_BLOCK(Verify_Test)
{
	BOOL test_result;
	measure( TRUE );
	
	system_initial_external_vbg();
	timing();
	
	DWORD CFG_dat;
	DWORD RE_CFG_dat;
	int xaddress,yaddress;

	int Verify_Time = 20;
	
	CArray<double,double> rst_current_result,set_current_result;
	
	numx( 10 );
	numy( 8 );
	data_reg_width ( 36 );
	x_fast_axis( TRUE );	
	
    pin_dc_state_set( ZZ, t_vih, TRUE );
	pin_dc_state_set( UB_pl, t_vil, TRUE );
	pin_dc_state_set( LB_pl, t_vil, TRUE );
	
	FILE *fp = fopen( Verify_Test_FILE, "a+" );
		
	for( xaddress = X_Start_Verify ; xaddress < X_Stop_Verify ; xaddress+=X_Step_Verify )
	{
		for( yaddress = Y_Start_Verify ; yaddress < Y_Stop_Verify ; yaddress+=Y_Step_Verify )
		{
			for( int k = 0 ; k <= Verify_Time ; k++)
			{
				
				float Vbg_Voltage = SET_Vbg_Verify(1.0,0.1,k);
				int Tset_Time = SET_Time_Verify(10,2,k);
				//SET_Time_Verify(10,2,k);    	//	total需要大于10
				
				//	CFG_dat = trim_initial();
				CFG_dat = 0x80db4cc9;    // extern VBG CLK I = 0.64(1)0.76(9)0.88(5)1(d)   width  50ns   
				dmain( CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				xmain( xaddress );
				ymain( yaddress );
				dmain( 0xffff );
				count( 3, 0);																			//没有用fastest_write_test_pattern时需要把count3屏蔽
				test_result = funtest( fastest_write_test_pattern, finish ); //jiyizhong 7times clk
				//test_result = funtest( quadruple_staircase_down_external_write_test_pattern, finish );	//没有用fastest_write_test_pattern时需要把count3屏蔽
				//test_result = funtest( double_staircase_down_external_write_test_pattern, finish );		//没有用fastest_write_test_pattern时需要把count3屏蔽
				
				
				RE_CFG_dat = CFG_dat | 0x00000004;      
				dmain( RE_CFG_dat );
				funtest( CFG_set_PAT, finish );
				
				pin_dc_state_set( DATA_pl, t_vil );
				
				xmain( xaddress );
				ymain( yaddress );
				test_result = funtest( address_pat, finish );
				//Delay( 10 MS );
				
				pin_dc_state_set( WE,	t_vih);
				pin_dc_state_set( CE,	t_vil);
				pin_dc_state_set( ZZ,	t_vih);
				pin_dc_state_set( WE,	t_vil);
				
				vpar_force( 0.3 V );
				ipar_high( 200 UA );
				ipar_low( -0.5 UA );
				test_result = partest( pass_nicl, DATA_pl );
				Pin_meas( set_current_result );
				
				///////////////////////////////////////////////////////////////Judge
				
				int range_1 = 0;						//<5.5
				int range_2 = 0;						//5.5-6.0
				int range_3 = 0;						//>6.0
				
				float max_current_range = 948;					//related to log_r-5.5
				float min_current_range = 300;					//related to log_r-6.0
				
				for ( int i = 0; i < 16; i++)
				{
					if (set_current_result[i] > max_current_range)
						range_1 = range_1 + 1;
					else if (set_current_result[i] > min_current_range)
						range_2 = range_2 + 1;
					else
						range_3 = range_3 + 1;
				}

//				output("k=%d r1=%d r2=%d r3=%d vbg=5.5f% time=%d\n",k,range_1,range_2,range_3,Vbg_Voltage,Tset_Time * 7);

				
				if(range_1 > 0)
				{
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%5d %5d	%5d	 %12.2f  %12.5f %10x  %5d", k,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
						fprintf( fp, "%12.2f	", set_current_result[ i ]);
						if( set_current_result[ i ] < 30 )
							set_current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( set_current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( set_current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					break;
				}	
				else if (range_3 < 2)
				{
					for( int i = 0 ; i < 16 ; i++)
					{	
						fprintf( fp, "%5d %5d	%5d	 %12.2f  %12.5f %10x  %5d", k,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
						fprintf( fp, "%12.2f	", set_current_result[ i ]);
						if( set_current_result[ i ] < 30 )
							set_current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( set_current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( set_current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					break;
				}
				else if (k == 14)
				{
					for( int i = Verify_Time ; i < 16 ; i++)
					{	
						fprintf( fp, "%5d %5d	%5d	 %12.2f  %12.5f %10x  %5d", k,xaddress, yaddress,external_vbg,(0.4/3.75)*external_vbg,CFG_dat,i );
						fprintf( fp, "%12.2f	", set_current_result[ i ]);
						if( set_current_result[ i ] < 30 )
							set_current_result[ i ] = 30;
						fprintf( fp, "%12.2f	", 300000 / fabs( set_current_result[ i ] )		);
						fprintf( fp, "%12.2f	", log( 300000000 / fabs( set_current_result[ i ] ) ) / log( 10 ) );
						
						fprintf( fp, "\n" );
					}
					break;
				}

			}
			
		}
	}
	
	fclose(fp);
	
	return MULTI_DUT;
	
}