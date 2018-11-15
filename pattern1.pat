///////////////////////////////////////////////////////////////////////////////////
PATTERN( CFG_set_PAT )
@{	
	count( 1, 31);
@}
%CFG_loop:		
	datgen	shldr,mainmain
	chips	cs1f,cs2f,cs3f,cs4pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset2,ps4
	count	count1,decr,aon
	mar		cjmpnz,CFG_loop
%mar done

///////////////////////////////////////////////////////////////////////////////////
PATTERN( CFG_get_PAT )
@{	
	xmain( 0x0 );
	ymain( ymax() );
	count( 1, 31);
@}
%CFG_loop:
	xalu	xmain,xcare,cmeqmax,increment,dxmain,oxmain
	yalu	ymain,xcare,con,increment,dymain,oymain		
	datgen	shldr,mainmain
	chips	cs1f,cs2f,cs3f,cs4pt			//cs1 CS;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset3,ps4,adhiz
	count	count1,decr,aon
	mar		cjmpnz,CFG_loop,read
%mar done


///////////////////////////////////////////////////////////////////////////////////
PATTERN( address_pat )
@{	
@}
%read_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	pinfunc	tset6,ps2
%mar done

///////////////////////////////////////////////////////////////////////////////////
PATTERN( int_clk_read_pat )
@{	
@}
%read_loop:		
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	datgen	mainmain
	chips	cs1pt,cs2f,cs3pt,cs4f			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset4,ps1,adhiz
	count	count1,decr,aon
	mar		cjmpnz,read_loop,read
%mar done

///////////////////////////////////////////////////////////////////////////////////
PATTERN( int_clk_write_pat )
@{	
@}
%write_loop:		
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	datgen	mainmain
	chips	cs1pt,cs2pt,cs3f,cs4f			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset5,ps1
	count	count1,decr,aon
	mar		cjmpnz,write_loop
%mar done


///////////////////////////////////////////////////////////////////////////////////
PATTERN( reset_set_cycle_pat )
@{	
	dmain( 0x0    );
	dbase( 0xffff );
@}
%init_loop:
   	xalu	xbase,xcare,coff,hold,dxmain,oxmain
	yalu	ybase,xcare,coff,hold,dymain,oymain
%reset_loop:		
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	datgen	mainmain
	chips	cs1pt,cs2pt,cs3f,cs4f			//cs1 CS;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset5,ps1
	count	count1,decr,aon
	mar		cjmpnz,reset_loop
%switch_loop:
   	xalu	xbase,xcare,coff,hold,dxmain,oxmain
	yalu	ybase,xcare,coff,hold,dymain,oymain
%set_loop:		
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	datgen	basebase
	chips	cs1pt,cs2pt,cs3f,cs4f			//cs1 CS;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset5,ps1
	count	count1,decr,aon
	mar		cjmpnz,set_loop
%finish_loop:
	count	count2,decr,aon
	mar		cjmpnz,init_loop
%mar done
/////////////////////////////////////////////////////////////////////

PATTERN( int_clk_write_one_address_pat )
@{	
@}
%write_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1pt,cs2pt,cs3f,cs4f			//cs1 CS;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset5,ps1
%mar done

///////////////////////////////////////////////////////////////////////////////////
PATTERN( reset_set_single_cell_cycle_pat )
@{	
	dmain( 0x0    );
	dbase( 0xffff );
@}
%init_loop:
   	xalu	xbase,xcare,coff,hold,dxmain,oxmain
	yalu	ybase,xcare,coff,hold,dymain,oymain
%reset_loop:		
	xalu	xbase,xcare,coff,hold,dxmain,oxmain
	yalu	ybase,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1pt,cs2pt,cs3f,cs4f			//cs1 CS;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset5,ps1
	count	count1,decr,aon
	mar		cjmpnz,set_loop
%set_loop:		
	xalu	xbase,xcare,coff,hold,dxmain,oxmain
	yalu	ybase,xcare,coff,hold,dymain,oymain
	datgen	basebase
	chips	cs1pt,cs2pt,cs3f,cs4f			//cs1 CS;cs2 WE;cs3 OE;cs4 CFGC
	pinfunc	tset5,ps1
	count	count1,decr,aon
	mar		cjmpnz,another_loop
%another_loop:
	count	count2,decr,aon
	mar		cjmpnz,init_loop
%mar done



PATTERN( old_fastest_write_test_pattern )
@{	
	count( 1, 9);
	count( 2, 6);
	count( 3, 0);
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
%write_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%write_clock_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_test_start
%mar done


PATTERN( fastest_read_test_pattern )
@{	
	count( 1, 9);
	count( 2, 10);
	count( 3, amax());
@}

%read_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5
	count	count1,decr,aon
	mar		cjmpnz,read_test_start
%read_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3t,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5
	mar		cjmpnz,read_clock_loop
%read_clock_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3t,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5
	count	count2,decr,aon
	mar		cjmpnz,read_clock_loop
%read_test:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3t,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5,adhiz
	count	count2,decr,aon
	mar		cjmpnz,read_done,read
%read_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3t,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5
	mar		cjmpnz,read_test_done
%read_test_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3t,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5
	mar		cjmpnz,another_loop
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3t,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset8,ps5
	count	count3,decr,aon
	mar		cjmpnz,read_start
%mar done


PATTERN( staircase_down_external_write_test_pattern )	// Mainly three different clocks but you can change the numbers of different clocks (the count_numbers need to be carefully controlled)	
@{	
	count( 1, 5);
	count( 2, 1);
	count( 3, 3);
	count( 4, 0);
	count( 5, amax());
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_clock_loop_1:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset9,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop_1
%write_clock_loop_2:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset10,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_clock_loop_2
%write_clock_loop_3:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset11,ps5
	count	count4,decr,aon
	mar		cjmpnz,write_clock_loop_3
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count5,decr,aon
	mar		cjmpnz,write_test_start
%mar done


PATTERN( single_external_write_test_pattern )				//only 1 clock during WE low
@{	
	count( 1, 5);
	count( 5, amax());
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_clock_loop_1:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset9,ps5
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count5,decr,aon
	mar		cjmpnz,write_test_start
%mar done


PATTERN( double_staircase_down_external_write_test_pattern )	// only 3 clock during WE low (but 2 pulse)
@{	
	count( 1, 5);
	count( 2, 1);
	count( 5, amax());
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_clock_loop_1:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset9,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop_1
%write_clock_loop_2:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset10,ps5
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count5,decr,aon
	mar		cjmpnz,write_test_start
%mar done


PATTERN( quadruple_staircase_down_external_write_test_pattern )	// only 5 clock during WE low (but 4 pulse)
@{	
	count( 1, 5);
	count( 2, 1);
	count( 3, 2);
	count( 5, amax());
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_clock_loop_1:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset9,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop_1
%write_clock_loop_2:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset10,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_clock_loop_2
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt						//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count5,decr,aon
	mar		cjmpnz,write_test_start
%mar done


PATTERN( fastest_write_test_pattern )                      //需要把count3放到Test Block中
@{	
	count( 1, 1);
	count( 2, 50);
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%write_clock_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_start
%mar done


PATTERN( debug_time_test_pattern )
@{	
	count( 1, 1);
	count( 2, 5);
	count( 3, 1);
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%write_clock_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset7,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset7,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_start
%mar done

PATTERN( force_V_pat )
@{	

@}
%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f		//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK
	pinfunc	tset12,ps6
	count	count2,decr,aon
	mar		cjmpnz,write_test_start

%mar done


PATTERN( reset_set_01_cycle_pat )
@{	
	count( 1,20);
	count( 2,20);
	dmain( 0x0    );
	dbase( 0xffff );
@}

%init_config:
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt				//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK	
	pinfunc	tset13,ps5

%reset_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt				//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK			
	pinfunc	tset13,ps5
	count	count1,decr,aon
	mar		cjmpnz,reset_loop

%reset_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt				//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK			
	pinfunc	tset13,ps5

%set_config:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	basebase
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt				//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK				
	pinfunc	tset14,ps5

%set_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	basebase
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt				//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK				
	pinfunc	tset14,ps5
	count	count2,decr,aon
	mar		cjmpnz,set_loop

%set_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	basebase
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt				//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK			
	pinfunc	tset14,ps5

%count_loop:		
	count	count3,decr,aon                         //cycle numbers
	mar		cjmpnz,init_config

%another_address_loop:
	xalu	xmain,xcare,cmeqmax,increment,dxmain,oxmain
	yalu	ymain,xcare,con,increment,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLK   
	pinfunc	tset13,ps5
	count	count4,decr,aon                      //address number
	mar		cjmpnz,init_config
%mar done



PATTERN( fastest_write_test_pattern_slope_version_pattern )                      //需要把count3放到Test Block中
@{	
	count( 1, 1);
	count( 2, 1);
	count( 3, 10);
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset15,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset15,ps5
%write_clock_loop_1:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset5,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop_1
%write_clock_loop_2:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset16,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_clock_loop_2
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset15,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset15,ps5
	count	count4,decr,aon
	mar		cjmpnz,write_start
%mar done


PATTERN( fastest_write_rst_test_pattern )                      //需要把count3放到Test Block中
@{	
	count( 1, 1);
	count( 2, 50);
@}

%write_test_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1f,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset13,ps5
	count	count1,decr,aon
	mar		cjmpnz,write_test_start
%write_start:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset13,ps5
%write_clock_loop:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2t,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset13,ps5
	count	count2,decr,aon
	mar		cjmpnz,write_clock_loop
%write_done:		
	xalu	xmain,xcare,coff,hold,dxmain,oxmain
	yalu	ymain,xcare,coff,hold,dymain,oymain
	datgen	mainmain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	pinfunc	tset13,ps5
%another_loop:
	xalu	xmain,xcare,con,increment,dxmain,oxmain
	yalu	ymain,xcare,cmeqmax,increment,dymain,oymain
	chips	cs1t,cs2f,cs3f,cs4f,cs5pt			//cs1 CE;cs2 WE;cs3 OE;cs4 CFGC;cs5 CLKVCO
	datgen	mainmain
	pinfunc	tset13,ps5
	count	count3,decr,aon
	mar		cjmpnz,write_start
%mar done