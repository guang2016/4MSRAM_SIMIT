#include "tester.h"


SEQUENCE_TABLE( sample_sequence_table )
{
	SEQUENCE_TABLE_INIT

	/////////////////////////////////////////////////////////////////////////////	Before Experiment Test

	TEST( Open_Test,							NEXT,			open_fail	)
//	TEST( Short_Test,							NEXT,			NEXT		)
//	TEST( Power_Test,							NEXT,			power_fail	)
//	TEST( Vref_Test,						   	NEXT,			NEXT		)
//	TEST( Decoder_Test,							NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////	Foundation Test

//	TEST( Dc_Set_Array_Test,				    NEXT,			NEXT		)	

	TEST( Write_0xffff_Test,					NEXT,			NEXT		)
//	TEST( Read_0xffff_Test,						NEXT,			NEXT		)
	TEST( Write_0x0_Test,						NEXT,			NEXT		)
//	TEST( Read_0x0_Test,						NEXT,			NEXT		)

	TEST( Resistance_Distribution_Test,			NEXT,			NEXT		)	

	/////////////////////////////////////////////////////////////////////////////	Internal Shmoo Test

//	TEST( Rst_Shmoo_Test,						NEXT,           NEXT		)
//	TEST( Set_Shmoo_Test,						NEXT,			NEXT		)
//	TEST( Set_Short_Width_Shmoo_Test,			NEXT,           NEXT		)
	
	/////////////////////////////////////////////////////////////////////////////	Yield/Bitmap Test

//	TEST( Write_Read_Chip_Test,				NEXT,			NEXT			)

//	TEST( Write_0x0_Get_Error_Address_Test,		NEXT,			NEXT		)
//	TEST( Write_0xffff_Get_Error_Address_Test,	NEXT,			NEXT		)
	
	/////////////////////////////////////////////////////////////////////////////	Margin Test

//	TEST( Set_Margin_Test,						NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////	Cycle Test

//	TEST( Cycle_Endurance_Test,					NEXT,			NEXT		)
	TEST( Cycle_Test,							NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////	Drift Test

//	TEST( Drift_Test,							NEXT,			NEXT		)		//Total_Time=10000s/2.7h

	/////////////////////////////////////////////////////////////////////////////	Electrical characterization Test

//	TEST( IV_Test,								NEXT,			NEXT		)
//	TEST( VI_Test,								NEXT,			NEXT		) 

	/////////////////////////////////////////////////////////////////////////////	External CLK/VBG Test	(Same Clock)

//	TEST( External_Rst_Height_Test,				NEXT,			NEXT		)		//Different VBG CLK 手动输入VBG / Set Default Value 0/3.75V
	TEST( External_Set_Height_Test,				NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////	Staircase_Down_Pulse External CLK/VBG Test	(Different Clock)

//	TEST( Staircase_Down_Pulset_Test,				NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////	STS/I-V/V-I Test

//	TEST( RST_Different_Vbg_Shmoo_Test_1,			NEXT,			NEXT		)	//rst,i-v,v-i地址不能相同 因为iv会set单元
//	TEST( RST_Different_Vbg_Shmoo_Test_9,			NEXT,			NEXT		)

	TEST( SET_Different_Vbg_Shmoo_Test,				NEXT,			NEXT		)
//	TEST( Single_Step_SET_Different_Vbg_Shmoo_Test,	NEXT,			NEXT		)
	
//	TEST( SET_Different_Vbg_Shmoo_Slope_Version_Test,	NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////  Voltage_Stress

//	TEST( Voltage_Stress_Test,						NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////  CLK_Tset_Time

	TEST( SET_Different_Clk_Time_Shmoo_Test,		NEXT,			NEXT		)

	/////////////////////////////////////////////////////////////////////////////  Verify
	
//	TEST( SET_Program_Verify_Test,					NEXT,			NEXT		)
	TEST( Verify_Test,								NEXT,			NEXT		)
	

	BIN( pass_bin, STOP)
	
	BINL( open_fail,					open_fail_bin,				STOP)
	BINL( short_fail,					short_fail_bin,				STOP)
	BINL( leakage_fail,					leakage_fail_bin,			STOP)
	BINL( power_fail,					power_fail_bin,				STOP)

}
TEST_BIN( open_fail_bin )					{ }
TEST_BIN( short_fail_bin )					{ }
TEST_BIN( leakage_fail_bin )				{ }
TEST_BIN( power_fail_bin )					{ }
TEST_BIN( pass_bin )						{ }
