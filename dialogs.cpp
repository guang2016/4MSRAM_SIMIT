////////////////////////////////////////////////////////////////////////////////
//
// Variables are created by using the following macro, with XXX replaced by
// BOOL, INT, FLOAT, CSTRING, or ONEOF:
//
//		XXX_VARIABLE( name, initial_value, prompt ) { body }
//
// This will create a global variable named 'name' of the following types:
//
//	XXX		C++ type	initial_value
//	===		========	=============
//	BOOL	BOOL		should evaluate to BOOL (can include function calls, etc)
//	INT		int			should evaluate to int (can include function calls, etc)
//	FLOAT	float		must evaluate to float (can include function calls, etc)
//	CSTRING	CString		must evaluate to a char *, LPCTSTR, or CString
//	ONEOF	CString		must be a string containing a comma separated list of legal values
//
// 'prompt' is the string displayed by the user interface in the "User Variable" dialog.
//
// 'body' is any C++ code, which can use 'oldval' to refer to the previous value.
// It will get evecuted any time the variable is changed by set_value or a Dialog.
//
//
////////////////////////////////////////////////////////////////////////////////
// Dialogs are create by using the following macros:
//
//	DIALOG( name ) {
//		CONTROL( id1, name1 )
//		CONTROL( id2, name2 )
//		...
//	}
//
// Where 'name' is the same as the name of a dialog template created by the Developer
// Studio dialog editor.  Each id must be from a control in the template, and each name
// must be defined with one of the XXX_VARIABLE macros above, or VOID_VARIABLE( name, prompt ).
//
// Before building a Dialog, it is recommended to create the XXX_VARIABLE statements
// that your dialog will control, because each type of variable can only be connected to
// certain types of controls, as follows:
//
//	XXX		Control type			Notes
//	===		============			=====
//	BOOL	CheckBox				1
//	INT		Edit					2
//	FLOAT	Edit					2
//	CSTRING	Edit					2
//	CSTRING	Static Text				1, 2, 3, 4
//	ONEOF	ListBox					1
//	ONEOF	ComboBox				1, 3, 5
//	VOID	Button
//
//	Notes:
//		1: Style:Notify must not be set
//		2: No immediate mode
//		3: Use dialog editor to make it big enough
//		4: Must change id from default IDC_STATIC
//		5: Style:DropDown incompatible with immediate mode
//
// Note: To create a dialog template, click Insert/Resource/Dialog. Double click on the
// background of the template, and set the ID to whatever name you want IN DOUBLE QUOTES.
// It is important that the name of your DIALOG and the name of your template match.
// (You can verify this by going to resource view tab in the project workspace window).
// After creating the template, simple drag and drop controls onto your template.  It can
// be useful to set the ID of the controls to something mnemonic, to make your 'CONTROL'
// statements more legible.
//
// The predefined IDs IDOK and IDCANCEL can also be added in a CONTROL statement.  The
// handlers for the associated variable then takes all responsibility for ending the
// dialog.  In either case you might want to do an "update_values( current_dialog( ) )".
// To actually end the dialog, you must call 'focus( varaible )'.  For example:
//
//		VOID_VARIABLE( onOK, "Dismiss current dialog successfully" ) {
//			if ( update_values( current_dialog( ) ) ) {
//				// all Variables' handler functions said they we individually ok.
//				if ( relationship between variables is ok )
//					focus( variable );
//			}
//		}
//
//
// Dialog Editor Tips:
//		- In general, the default styles are appropriate.  Test any non-default styles carefully!
//		- Control-Z is undo.
//		- To insert a bitmap onto an image, perform the following steps:
//			step 1: Insert a bitmap into your project.  Click on Insert/Resource/Import, and select a file.
//			step 2: Create a "Picture" control in your dialog.  It's size doesn't matter.
//			step 3: Double-click on the outline of the new control to bring up its property editor
//					In the "General" tab, set Type to Bitmap, and select the new one in the "Image" combo box.
//


#include "tester.h"
#include "resource.h"
#include <math.h>

/////////////////////////////////
//////// Dialog "Simple" ////////
/////////////////////////////////

CSTRING_VARIABLE( SM_static, "Count = 0", "" ) {
	static int count = 0;
	SM_static.Format( "Count = %d", ++count );
}

BOOL_VARIABLE( SM_check, TRUE, "a BOOL variable" ) {
	TRACE( "SM_check is now %s\n", ( SM_check ? "TRUE" : "FALSE" ) );
}

INT_VARIABLE( SM_int, 1024, "an int variable" ) {
	TRACE( "SM_int is now %d\n", SM_int );
}

FLOAT_VARIABLE( SM_float, 3.14159, "an approximation of pi" ) {
	TRACE( "SM_float is now %f\n", SM_float );
}

CSTRING_VARIABLE( SM_string, "Edit me", "a CString variable" ) {
	TRACE( "SM_string is now %s\n", SM_string );
}

ONEOF_VARIABLE( SM_combo, "Why, What, When, Where, Who", "a bunch of questions" ) {
	TRACE( "SM_combo wants to know %s\n", SM_combo );
}

ONEOF_VARIABLE( SM_listbox, "Red, Orange, Yellow, Green, Black, Indigo, Violet", "the colors of the rainbox" ) {
	TRACE( "SM_listbox is now %s\n", SM_listbox );
}

VOID_VARIABLE( SM_more, "a command" ) {
	AfxMessageBox(
		"\"Control Type\" refers to pop ups in the \"controls\" when editing a dialog\n"
		"\"Declaration\" refers to the macro name needed to declare the controlled variable."
		);

}

DIALOG( Simple )
{
	CONTROL( IDC_SM_static,		SM_static )
	CONTROL( IDC_SM_check,		SM_check )
	CONTROL( IDC_SM_int,		SM_int )
	CONTROL( IDC_SM_float,		SM_float )
	CONTROL( IDC_SM_string,		SM_string )
	CONTROL( IDC_SM_combo,		SM_combo )
	CONTROL( IDC_SM_listbox,	SM_listbox )
	CONTROL( IDC_SM_more,		SM_more )
}


////////////////////////////////////
//////// Dialog "Immediate" ////////
////////////////////////////////////

DIALOG( Immediate )
{
	// Uses the same variables as "Simple"
	IMMEDIATE_CONTROL( IDC_IM_check,	SM_check )
	IMMEDIATE_CONTROL( IDC_IM_combo,	SM_combo )
	IMMEDIATE_CONTROL( IDC_IM_listbox,	SM_listbox )
}


//////////////////////////////////
//////// Dialog "Dynamic" ////////
//////////////////////////////////


EXTERN_DIALOG( Dynamic )

CSTRING_VARIABLE( DY_carsx, "", "what car you last chose" ) { }

ONEOF_VARIABLE( DY_cars, "honda, toyota, nissan, ford, chevy", "car makes" ) {
	DY_carsx = DY_cars;
	update_control( current_dialog( ), DY_carsx );
}

CSTRING_VARIABLE( DY_bikesx, "", "what bike you last chose" ) { }

ONEOF_VARIABLE( DY_bikes, "honda, kawi, ducati, yamaha", "motorcycle makes" ) {
	DY_bikesx = DY_bikes;
	update_control( Dynamic, DY_bikesx );
}

BOOL_VARIABLE( DY_state, FALSE, "a state variable" ) {
	// Don't do message box if in non-immediate mode...
	extern BOOL DY_enable;
	if ( DY_enable )
		AfxMessageBox( vFormat( "DY_state is now %d", DY_state ) );
}

CSTRING_VARIABLE( DY_message, "Immediate mode is off", "a message" ) { }

BOOL_VARIABLE( DY_enable, FALSE, "Controls immediate mode of other controls" )
{
	VERIFY( immediate( Dynamic, DY_cars,  DY_enable ) );
	VERIFY( immediate( Dynamic, DY_bikes, DY_enable ) );
	VERIFY( immediate( Dynamic, DY_state, DY_enable ) );

	DY_message.Format( "Immediate mode is %s", ( DY_enable ? "on" : "off" ) );
	update_control( Dynamic, DY_message );
}

DIALOG( Dynamic )
{
	CONTROL( IDC_DY_cars,    DY_cars )
	CONTROL( IDC_DY_carsx,   DY_carsx )
	CONTROL( IDC_DY_bikes,   DY_bikes )
	CONTROL( IDC_DY_bikesx,  DY_bikesx )
	CONTROL( IDC_DY_state,   DY_state )
	CONTROL( IDC_DY_message, DY_message )
	IMMEDIATE_CONTROL( IDC_DY_enable, DY_enable )
}

/////////////////////////////////
//////// Dialog "Update" ////////
/////////////////////////////////

void show( VariableProxy v ) {
	TRACE( "Variable %s is now %s\n", get_name( v ), get_value( v ) );
}

INT_VARIABLE( UP_int, 1, "an int" ) {
	show( variable );
}

CSTRING_VARIABLE( UP_string, "", "a string" ) {
	show( variable );
}

CSTRING_VARIABLE( UP_log, "", "" ) { }

VOID_VARIABLE( UP_doInt, "update the int" )
{
	update_value( current_dialog( ), UP_int );
	UP_int *= 2;
	update_control( current_dialog( ), UP_int );
}

VOID_VARIABLE( UP_doString, "update the string" )
{
	update_value( current_dialog( ), UP_int );
	UP_int += 1;
	UP_string = vFormat( "sqrt(%d) = %f", UP_int, sqrt( (float)UP_int ) );
	UP_log += UP_string + "\r\n";
	update_controls( current_dialog( ) );
}

#define MY_CONTROL( name )		CONTROL( IDC_##name, name )
DIALOG( Update )
{
	MY_CONTROL( UP_int )
	MY_CONTROL( UP_string )
	MY_CONTROL( UP_doInt )
	MY_CONTROL( UP_doString )
	MY_CONTROL( UP_log )
}


/////////////////////////////////////
//////// Dialog "RangeCheck" ////////
/////////////////////////////////////

INT_VARIABLE( FO_2i, 1, "a two digit integer" )
{
	if ( abs( FO_2i ) > 99 ) {
		AfxMessageBox( vFormat( "Must be between -99 and 99, not %d", FO_2i ) );
		FO_2i = oldval;
		focus( FO_2i );
	}
}

VOID_VARIABLE( FO_change, "changes the integer" )
{
	// Get it from the edit box
	update_value( current_dialog( ), FO_2i );


	// The two different branches of this conditional are functionally identical,
	// Use the style you like better...
	static int which = 0;
	if ( ++which % 2 ) {

		// set_value also always does an invoke...
		set_value( FO_2i, FO_2i * 2 );

	} else {
		// change it
		FO_2i *= 2;

		// call the handler to do range checking
		invoke( FO_2i );
	}

	// Note that if FO_2i's handler calls 'focus', then we won't get here.

	// put FO_2i back into its edit box
	TRACE( "Actually updating FO_2i\n" );
	update_control( current_dialog( ), FO_2i );
}

DIALOG( RangeCheck )
{
	MY_CONTROL( FO_2i )
	MY_CONTROL( FO_change )
}


//////////////////////////////////
//////// Dialog "Dismiss" ////////
//////////////////////////////////

BOOL_VARIABLE( DM_mode, FALSE, "Really Exit" ) { }

VOID_VARIABLE( DM_ok,    "IDOK handler" ) {
	if ( DM_mode )
		// Calls to Focus in a IDOK handler cause invoke( ) to return IDOK
		focus( DM_ok );
	AfxMessageBox( "IDOK swallowed by handler" );
}

VOID_VARIABLE( DM_cancel, "IDCANCEL handler" ) {
	if ( DM_mode )
		// Calls to Focus in a IDCANCEL handler cause invoke( ) to return IDCANCEL
		focus( DM_cancel );
	AfxMessageBox( "IDCANCEL swallowed by handler" );
}

DIALOG( Dismiss )
{
	IMMEDIATE_CONTROL( IDC_DM_mode, DM_mode )

	CONTROL( IDOK,       DM_ok )
	CONTROL( IDCANCEL,   DM_cancel )
}

////////////////////////////////
//////// Dialog "Reset" ////////
////////////////////////////////

SNAPSHOT( snapshot )
{
	VARIABLES( Reset )
	REMOVE_VARIABLE( RE_persist )
}

CSTRING_VARIABLE( RE_persist, "I am persistent", "doesn't get reset" ) { }

CSTRING_VARIABLE( RE_string, "Resetable string", "" ) { }
INT_VARIABLE( RE_int,		 2121, "" ) { }
FLOAT_VARIABLE( RE_float,	 9.99, "" ) { }

BOOL_VARIABLE( RE_bool,		 TRUE, "" ) { }

ONEOF_VARIABLE( RE_oneof,	 "green eggs, ham, spam, tofu, carrots, sardines, peas", "dinner choices" ) { }

VOID_VARIABLE( RE_restore, "restore" ) {
	restore( snapshot, FALSE );
	update_controls( current_dialog( ) );
}

VOID_VARIABLE( RE_save, "save" ) {
	if ( update_values( current_dialog( ) ) )
		save( snapshot );
}

DIALOG( Reset )
{
	MY_CONTROL( RE_restore )
	MY_CONTROL( RE_save )

	MY_CONTROL( RE_persist )

	MY_CONTROL( RE_string )
	MY_CONTROL( RE_int )
	MY_CONTROL( RE_float )
	MY_CONTROL( RE_bool )
	MY_CONTROL( RE_oneof )
}

//////////////////////////////////
//////// Dialog "Runtime" ////////
//////////////////////////////////

// To Do
//	 add dialog that has:
//	 variable, runtime, generic value printer
//	 resource_all_names, resource_find, set_value, get_value
//	 for_each, invoke, etc


////////////////////////////////
//////// Dialog "Win32" ////////
////////////////////////////////

ONEOF_VARIABLE( W32_victim, "abc, 123, xyz, 999", "" ) { }

BOOL_VARIABLE( W32_enable, TRUE, "" ) {
	EXTERN_DIALOG( Win32 ); 
	EnableWindow( get_HWND( Win32, W32_victim ), W32_enable );
}

BOOL_VARIABLE( W32_show,   TRUE, "" ) {
	ShowWindow( get_HWND( current_dialog( ), W32_victim ), ( W32_show ? SW_SHOW : SW_HIDE ) );
}

VOID_VARIABLE( W32_ok, "" ) {
	W32_enable = W32_show = TRUE;
	focus( variable );
}

VOID_VARIABLE( W32_cancel, "" ) {
	W32_enable = W32_show = TRUE;
	focus( variable );
}

DIALOG( Win32 )
{
	CONTROL( IDOK,		W32_ok )
	CONTROL( IDCANCEL,	W32_cancel )

	CONTROL( IDC_W32_victim, W32_victim )
	IMMEDIATE_CONTROL( IDC_W32_enable, W32_enable )
	IMMEDIATE_CONTROL( IDC_W32_show,   W32_show )
}


//////////////////////////////////
//////// Dialog "Countdown" ////////
//////////////////////////////////

CSTRING_VARIABLE( CD_static, "", "Seconds to go till blast off" )
{
	if ( CD_static == "t = 1" ) {
		AfxMessageBox( "Blast off!" );
	}
}

VOID_VARIABLE( CD_ok, "IDOK handler" )
{
	for ( int i = 10; i > 0; --i ) {
		CD_static = vFormat( "t = %d", i );
		update_control( current_dialog( ), CD_static );
		Sleep( 1000 );
	}
	CD_static = "";
	focus( CD_ok );
}

DIALOG( BlastOff )
{
	CONTROL( IDC_CD_static, CD_static )
	CONTROL( IDOK, CD_ok )
}

//////////////////////////////////
//////// Dialog "Chooser" ////////
//////////////////////////////////

ONEOF_VARIABLE( CH_combo, resource_all_names( "Dialog" ), "all of the dialogs" ) {
	if ( CH_combo == "Chooser" )
		AfxMessageBox( "Can't choose chooser" );
	else
		invoke( Dialog_find( CH_combo ) );
}

DIALOG( Chooser )
{
	IMMEDIATE_CONTROL( IDC_CH_combo, CH_combo )
}
