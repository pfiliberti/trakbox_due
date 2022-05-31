/**********************************************************************/
/* Tracking code for Southern NET VersaBoard 8051                     */
/*          8031/51/52 CPU  1991/09/29                                */
/*          (C) JG6MCG,Yasunari Gen/Southern NET 1991                 */
/*              JA6FTL,Sueo Asato                                     */
/*            14,Wadomari,Kagoshima,891-91,JAPAN                      */
/*            Released under MIT License, located here:               */
/* https://github.com/pfiliberti/trakbox_due/blob/20df6d80123f2a027c8e4ec960ef88d0ae74d38b/TrakboxOriginal/Firmware/Source/LICENSE */
/**********************************************************************/
/******* version up history **********/
/* Added LCD module driver via 8255 port A&B     30,Oct         */
/* Port-A: DATA Port-B bit0 - 2 : RS,R/W,E   			*/
/* LCD driver was installed GETL.A51 assembler   11,Dec         */
/* LCD driver port changed to 8052 P1  for VersaBoard  10,Dec   */
/* LED indicater supported					*/
/*		Port-C bit5: AOS indicator			*/
/*		Port-C bit7: Brake output			*/
/* Satellite selection from hard SW supported			*/
/*		Port-B bit0-3  4bit binary SW (0-F)		*/
/* Tracking On/Off hard SW supported				*/
/*		Port-B bit4					*/
/* Realtime display format changed to full screen   14,Dec      */
/* Satellite selection by R-ladder & ADC support   27,Dec       */
/* 		Port-B bit5 pull down for this selection        */
/* Mic click doppler puls support                28,Dec 	*/
/*		Port-C bit4   not completed    			*/
/* modefy LPT port baud rate generation  Timer-2(8052)->        */
/*                Timer-0 , supported in 8051     11,Jan,1992   */
/* support using preveous year's kepel                          */
/* Support YAESU CAT command     		15,Jan          */
/* Support north rotor 0 position  		15,Jan	  	*/
/* Support back space key in in correct miss	15,Jan		*/
/* Support ICOM CI-V                            06,Feb		*/
/*         ICOM baud rate = 1200bps             12,Feb          */
/* Support NASA format kepele                   20.Feb          */
/* Support mic click doppler correction with PLL 20.Feb         */
/*         PLL eror voltage in ADC CH3, UP:portCbit4,DWN:bit6   */
/* Support F2 (uplink freq) and mode set                        */
/* Modified Doppler calculation with the equation               */
/*          suggested by Tanikawa,JR1HAL     Ver.1.30a 12.Mar   */
/* Doppler conpensation command +/-/0 support v1.30d   18.Mar   */
/* Support YAESU FMN switch option for wide filter     22.Mar   */
/* Bug fix  lock-up at 180deg position.  v1.40a        08.Apr   */
/* Minor change in calc_sat_sched() scroll option            08.Apr   */
/* minor change in radioport config. full.AOS track    12.apr   */
/* Freq/mode file upload supported.      v1.50         15.Apr   */
/* Station element file upload support   v1.50a        17.Apr   */
/* CAT/Mic-click can be defined for each sat.  v1.50b  18.Apr   */
/* minor enhansment in select sat list '*'mark v1.50b  18.Apr   */
/* bug correction in station elements read routine v1.50c       */
/* minor change in statellite list display routine              */
/* bug fix in T calc. in leep year.                v1.50d 30.Apr*/
/* bug fix in mic_click option in rotoir_con()     v2.00  2.May */
/* minor change in RTC init. routine (4x4as)       v2.01 10.May */
/* bug fix in local_lon->local_lon_c conversion    v2.01a 25.Jun*/
/* minor mods to RTC init. mandatoly reset insystem reset 21.Aug*/
/* support YAESU other than 736 support.			*/
/* support antenna parking after LOS                      21.Aug*/
/* Rotator limit set supported.  				*/
/* support multiple sat. tracking		   v2.02e 10.Sep*/
/* minor change in FREQ.DAT uploading, now defined              */
/*  satellites are selected for the list automatically    15.Sep*/
/* modified BS handling in get_fval() routine             16.Sep*/
/* support COM port 1200bps for modem link                23.Sep*/
/* host mode support.                                     28.Sep*/
/* simplified rotor_con2() and many other improvement     14.Oct*/
/* use On CPU RAM for speed up                                  */
/* minor change in YAESU CAT dummy freq. set              17.Oct*/
/* 1min RTC interrupt for priority tracking               18.Oct*/
/* Add main/sub select for ICOM CAT                       28.Oct*/
/* Added display [phase] in LCD settable from station           */
/* element   Antenna start option SP or NP                01.Nov*/
/* returned t poling methode in 60sec interrupt           03.Nov*/
/* COM port b.rate     1200/2400/4800/9600 support        08.Nov*/
/* User selectable Tracking start EL value                12.Nov*/
/* Direct rotor control from key-board                    19.Nov*/
/* MA display by user selectable in FREQ.DATfile          21.Nov*/
/* Freq offset for converter F0,F1 supported.             22.Nov*/
/* SW engaged AOS introduced 8051 P16                     23.Nov*/
/* Bug in skew handling fixed               (v2.09B)      23.Nov*/
/* Support step pulse generation for FT-726 (v2.10)       26.Nov*/
/* Preset antenna control                   (v2.11)       29.Nov*/
/* V3.00 release                                       93 04.Mar*/
/*   UP/DWNlink freq "round function"                           */
/* obj. sat. change with SPC,BS key support               11.Apr*/
/* tracking from Host mode                   v3.02        15.apr*/
/* Track, Stop  in real tracking mode        v3.02        15.Apr*/
/* kepele data append function	             v3.03	  18.Apr*/
/* rotator watch dog			     v3.04        25.Apr*/
/* P16_ON_EL support */
/* bug fix in ADC value display (ka7cmf)     v3.04c       09.May */
/* bug fix in priority track routine. DL1CR  v3.04e       21 May */
/* bug fix in doppler compensation ("0")                  21 May */
/* mode set command once again at el > 0.0     v3.04f	  25 May */
/* general release as v3.00a                              01 june*/
/* parking function in tracking1() support     v3.00c     27 June*/
/* Antenna position monitor in real tracking   v3.00d     25 Aug */
/* ICOM reverse addressing		       v3.01beta  10 Oct */
/* KENWOOD 711/811 combination support                           */
/*                 reverse port 		v3.01b2		*/
/* p_16_off value introduced 			v3.01b2		*/
/* station data upload from menu-4 introduced   v3.01b3         */
/* Out of range warning removed                           29 Oct */
/* bug fix in micclick pulse direction                    03 Nov */
/* bug fix in initial freq set command in mic-click       15 Dec */
/* bug fix in RS COM port initialization routine          31 Dec */
/* bug fix in RTC init routine                            31 Dec */
/* TRX mode/freq set support in parking mode              31 Dec */
/* Error mode timeout time    v3.10a                  5 Jan 1994 */ 
/* Change the mic_step_con() button -> Down          10 Jan 94   */
/* Host mode ?A out format change                    15 Jan 94   */
/* Function to disable Rotor Error introduced        15 Jan 94   */
/* YAESU filter W/N selection introduced             16 Jan 94   */
/* err_timeout configurable from station data file   22 Jan 94   */
/* TRX Parking condition uploadable                  23 Jan 94   */
/* Hardware +-50Hz correction support                26 Jan 94   */
/* Antenna position mon. SW CN9-7 -> P1.4 changed    28 Jan 94   */
/* Hardware +-50Hz Slew support                      10 Feb 94   */
/* four Uplink freq support		             19 Feb 94   */
/* Four Uplink freq set from edit menu		     04 Mar 94   */
/* Bug fix in NASA format uploading                  09 Mar 94   */
/* Minor change at priority track ->next AOS >0.0    13 Mar 94   */
/* fstep_ini() improved for UP/DWN pulse             21 Mar 94   */
/* bug fix at priority tracking                      21 Mar 94   */
/* Uplink channel memory function		     24 Apr 94   */
/* AMSAT format removed  			     24 Apr 94   */
/* data memory used for code size compression v3.20e 14 May 94   */
/* & mode as default				     15 May 94   */
/* mic click pulse width introduced		     15 May 94   */
/* Rotor error mode default OFF                      15 May 94   */
/* Flip mode support			v3.30	     25 May 94   */
/* TRX parking condition display                     14 June 94  */
/* minor change at multi track changeover            20 Jun  94  */
/* Flip OFF in parking action           v3.30b       25 Jun  94  */
/* Bug fix in ICOM CAT control          v3.30c       01 Jul  94  */
/* Minor change of parking action/SW    v3.30d       08 Jul  94  */
/* Bug fix of conflicting od parking and flip action 09 Jul  94  */
/* REG_MAX changed to 60                v3.30e       21 Aug  94  */
/* Function to assign all sats. in RS buffer         21 Aug  94  */
/* Host mode call from real time tracking            22 Aug  94  */
/* bug fix USN -> signed adc_dat        v3.30f       31 Aug  94  */
/* Correction of flip detection in North start  v3.30g 18 Sep  94  */
/* Mic click pulse width configuration enable   v3.30g 19 Sep  94  */
/* Bug fix in 711/811 control routine           v3.30g 27 Sep  94  */
/* Bug correction in north start flip           v3.30h 09 Aug  94  */
/* Bug fix in leep year decision                v3.30i 04 Jan  95  */
/* ICOM id display optimize                            07 Mar  95  */
/* add return value to getline()                v3.30k 09 Mar  95  */
/* ICOM CAT modification (wait time)		v3.30m 08 Aug  96  */
/* The year of 2000 support 			v3.30n 17 Nov  96  */
/*  G2 table for 1990 - 2010 added              v3.40a 01 Dec  96  */
/* Bug fix in Flip mode ( |= ( & ~xx))		v3.40a 09 Jan  97  */
/* Timeout value for err_timeout configurable	v3.40a 17 Jan  97  */
/* Default kepele of 1997			v3.40a 18 Jan  97  */
/* Bug fix in ICOM 970/821 CAT routine 		v3.40c 03 Apr  97  */
/* FT-736 UV/VU f change dummy 1.2Ghz ->50Mhz  change              */
/* Fix for 1200 MHz freq set in YAESU cat       v3.40c 08 Apr  97  */
/* Fix 1200Mhz capable in host mode freq set	v3.40c 08 Apr  97  */
/* Fix 2400MHz(USN long value) in freq set      v3.40d 10 May  97  */
/* freq. offset capable in host mode		v3.40d 11 May  97  */
/* Code size optimize				v3.40e 15 June 97  */
/* icom_wait = 400 is the best			v3.40e 16 Aug  97  */
/* debug in day_of_year (USN char -> int)	v3.40e 14 Sep  97  */
/* configurable LPT port			v3.40f 25 Sep  97  */
/* for YAESU FT-847 test v3.40FX 21 Aug 1998 */
/* _icom_wait(), so IC970 and 821 are unified   v3.50  01 Oct  98  */
/* bug fix in kepele append function */
/* parking function in main menu display 28 Oct. 98 */
/* official release 				v3.50a 03 Dec  98  */
/* set_date("00/01/01 00:00:00"); in initialization  v3.50b */
/* bug fix in months(),get_epday() endless loop in for loop */
/* 350cx:	LCD by C-routine, CAT port unified
		i_wait(100 for ICOM) configurable
		lpt_wait(0:for KENWOOD,ICOM,10 for YAESU=fixed)	Jan 14 1999 */
/*	1200-19200 CAT support x4 b.r. in assembler Jan.15 1999  */
/*	ICOM_SEPARATE introduced 		Jan.17 1999 */
/*	removed 0x07 command in ICOM_SEPARATE */
/* 1999 kepele as default */
/* i_wait cnfiguable from STATION.DAT */
/*	bug fix in 	icom_reverse option	v3.50c	Fb.07 1999 */ 
/* K0 problem 			99-04-20  */
/* start 350d version		jan.03 2000 */
/*		minor change in kepele file read line[2] =='A'   Jan.05 2000 */

/* V3.60a  support Y2020 */

/*******************************************************************/

/*** C51 compiler Directives ***/

#ifdef NOTDEF
#pragma OPTIMIZE(5,SPEED)
#pragma LARGE
#pragma NOIV
#endif

#include <reg51.h>              /* define 8051 registors */
#include <stdio.h>              /* define I/O functions */
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <absacc.h>

/***********************************/
/*  RS COM CHAR                    */
/***********************************/

#define    USN     unsigned
#define    UCHAR   USN char
#define    UINT    USN int
#define	   ULONG	USN long
#define	   CLR	   0x1a
#define    ESC     'q'			/* 0x1b */
#define    STX     0x02
#define    ETX     0x03
#define    CR      0x0d
#define    LF      0x0a
#define    BS      0x08
#define    BEEP4    0x07
#define    DEL     0x7F
#define    TAB     0x09
#define    ENT     'E'
#define    EOFF    0x1a 
#define    LEFT_A	'4'
#define	   RIGHT_A	'6'
#define	   UP_A		'8'
#define	   DWN_A	'2'
#define    POINT_STOP   '5'

#define SATMAX     60		/* MAX STRUCT SIZE */
#define REG_MAX    60           /* Regist max sat number */
#define SEL_MAX    5		/* number in multi tracking display */
#define NO         0
#define YES        1
#define RS_BSIZE   20000        /* RS232C buffer size */

/***********************************************************/
/*     USER CONFIGURATION VALUE                            */
/***********************************************************/

#define    TRK_FRQ      10     /* tracking ref freq(20hz) for SBB mode */
#define    TRK_FRQ_FM   200    /* Tracking ref freq 200Hz for FM mode */
#define    TRK_CORREC	100    /* manual correction step 100Hz  */
#define	   STEP_P_WIDTH 3000  /* auto step pulse width */
#define    ADC_TO_A   2.84166  /* 1023 / 360deg */
#define    ADC_TO_E   5.68333  /* 1023 / 180deg */
#define    AZ_DEAD_BAND   3.0; /* Dead band for Azimuth */
#define	   EL_DEAD_BAND	  1.0; /* Dead band for elevation */
#define    AZ_MINI     3       /* default mini. adc value for AZ */
#define    AZ_MAX     837      /* default Max. adc value for AZ */
#define    EL_MINI     1       /* default mini. adc valu  for EL */
#define    EL_MAX     460      /* default Max. adc valu  for EL */

#define	   ERR_TIMEOUT	120	/* sec. rotator error timeout time */
#define	   ERR_RECOVER_TIME	61  /* Auto recover from error mode */

#define    EL_BLINK     -3.0   /* led2 brinking deg.            */
#define    TRACK_START  -2.0   /* Tracking start EL value  */
#define	   P16_ON	-5.0   /* p16_on_el value */
#define	   P16_OFF	-7.0   /* p16_off_el value */

#define	   Y_WAIT	4	/* yaesu inter character wait 4msec */

#define    LL9      27.3910         /* default LATITUDE (JA6FTL)*/
#define    WW9      128.6570     /* default LONGTUDE East */
#define    HH9      50.0            /* default ANTENNA height [m]*/
#define    TOH      0.0416666666
#define    TOM      0.0006944444
#define    TOS      0.000011574074

#define    ADC_CEN  550

/***********************/
/*         ADC         */
/***********************/

#define    AZ_PORT    0        /* adc channel for azimus   */
#define    EL_PORT    1        /* adc channel for elvetion */
#define    MODE_PORT  2        /* select sat 5V/16         */
#define    PLL_ADC    3        /* adc channel for modem lock ref V */

/************************/
/*	8051 Port-1	*/
/************************/
				/* P0-2: LCD cont, P5:LP2, P7:LP1 */
#define	   P13	       0x08	
#define	   P14	       0x10	/* P1.4 Antenna position monitor SW */
#define	   P16	       0x40     /* P1.6 AOS/Power control   */

/***********************/
/*     8255 B port     */      /* IN port */
/***********************/
			       /* Satellite selection binary SW  bit0-3 */
#define    ANT_CONT    0x10    /* Antenna control SW bit         bit4  */
#define    SEL_DIP_ADC 0x20    /* R ladder selection             bit5  */
#define    MODEM_LOCK  0x40    /* modem lock detect (active low) bit6  */
#define    RSP1200B    0x80    /* RS232C port 1200bps set (low active)  bit7 */


#define	   ANT_MONI    0x80    /* Antenna position monitow SW           bit7 */

#define	   PLUS_50     0x20	/* +50 correction bit-5 */
#define	   MINUS_50    0x80	/* -50 correction bit-7 */

/***********************/
/*     8255 C port     */
/***********************/

#define	   AZ_CCW      0x01	/*  CCW				bit0 */
#define    AZ_CW       0x02	/*  CW				bit1 */
#define    EL_DWN      0x04     /*  Down			bit2 */
#define    EL_UP       0x08     /*  UP				bit3 */
#define    UP_PULSE    0x10    /* Mic step up pulse start        bit4   */
#define    AOS_LED     0x20    /* AOS LED indicator              bit5   */
#define    DWN_PULSE   0x40    /* Mic step down pulse start      bit6   */
#define	   AZ_ROTOR_HIGH 0x80  /* Azimuth rotor high speed       bit7   */
#define    PULSE_END   0xaf    /* mask for pulse end 10101111           */

/***********************/
/*     other chip      */
/***********************/

#define    NEC_ADC     0xe200  /* ADC base address */
#define    RTC         0xe000  /* RTC base address */
#define    PPI         0xe100  /* 8255 base address */
#define    LED_PORT    PPI+2

#define    RTC_S1      RTC            /*         Second    */
#define    RTC_S10     RTC+1
#define    RTC_MI1     RTC+2          /*         Minute    */
#define    RTC_MI10    RTC+3
#define    RTC_H1      RTC+4          /*         Hour      */
#define    RTC_H10     RTC+5
#define    RTC_D1      RTC+6          /*         Day       */
#define    RTC_D10     RTC+7
#define    RTC_MO1     RTC+8          /*         Month     */
#define    RTC_MO10    RTC+9
#define    RTC_Y1      RTC+0xa        /*         Year      */
#define    RTC_Y10     RTC+0xb
#define    RTC_CD      RTC+0xd
#define    RTC_CE      RTC+0xe
#define    RTC_CF      RTC+0xf


/***********************/
/*    Switch  SW1      */
/***********************/

#define		MIC_CLICK	0x0001	/* mic_click  */
#define		HOST_FCONV	0x0002  /* host mode f offset 97-05-11 */
#define		TRACK_ID	0x0004	/* Antenna control status */
#define		ROTOR_ZERO	0x0008	/* Rotator zero point S/N */	
#define		ANT_PARK	0x0010	/* Antenna Parking enable */
#define		ROTATE_L	0x0020	/* Rotator range limit enable */
#define		YAESU_FMN	0x0040	/* YAESU FM narrow filter status */
#define		FULL_O_CAT	0x0080	/* Full orbit cat enable */

#define		RUN_MODE	0x0100  /* Run mode terminal:0/standalone:1 */
#define		MULTI_MODE	0x0200  /* multi sat tracking mode */
#define		BREAK_M		0x0400  /* brek flag from multi track modfe */
#define		PRIORITY_TRK	0x0800  /* Priority tracking flag */
#define	 	HOST_MODE	0x1000  /* Host mode flag */
#define		DOPPL_UPL	0x2000  /* Uplink doppler correct */
#define		DOPPL_DWN	0x4000  /* Downlink doppler correct */
#define		KEPELE_FMT	0x8000  /* Kepler element format NASA:0 AMSAT:1 */


/***********************/
/*     SW2 Flag        */
/***********************/

#define		FLAG_STEP	0x0001	/* Flag for step contro init routine */
#define		FLAG_AOS_CAT    0x0002  /* Freq set at AOS in mic click mode */
#define		STEP_CON	0x0004  /* Auto freq. compensation on mic click */
#define		STEP_PORLAR	0x0008	/* step porlarity */
#define		DOP_ROUND_D	0x0010  /* Doppler round for downlink */
#define		_711811		0x0020  /* KENWOOD 711/811 combo */
#define		P16_ON_FLAG	0x0040  /* P16 On flag */
#define		MODE_CMD_FLAG   0x0080  /* mode set at EL=0.0 again */
#define		MIC_CAT_FLAG	0x0100  /* CAT command at mic click flag */
#define		PARK_TRX_MODE   0x0200  /* TRX mode/freq. flag */
#define         PARK_TRX_FLAG   0x0400  /*   "" "" Flag */
#define         ERR_MODE_OFF    0x0800  /* Rotor error mode disable flag */
#define         SEL_SAT_ADC     0x1000  /* R-radder/ADC for sat select */
#define         FLIP_ON         0x2000  /* Flip action flag */
#define         FLIP_MODE       0x4000  /* Flip mode flag */ 
#define         FLIP_CALC_FLAG  0x8000  /* Calculation done flag */
 
/****************/
/*	SW3     */
/****************/

#define		ICOM821		0x0001	/* IC-821 selected  */
#define	 	YAESU736	0x0002    /* FT-736 */
#define		YAESU847	0x0004	/* FT-847 */

#define		ICOM_SEPARATE	0x0010

/***************************/
/*   SW for each satellite */
/***************************/

#define	DPLR_MODE	0x0001	/* 1:mic-click, 0:CAT */
#define	MA_MARK		0x0002  /* LCD MA display 1:yes, 0:no */
#define	ICOM_R_ADDR	0x0004	/* ICOM reverse address flag */
#define	KEN_REVS_PORT	0x0008  /* Kenwood 711/811 reverse port */
#define SELECTED_MRK	0x0010  /* selected from sat list mark */

/*************************************************************/
/*		Calculation constants           	     */
/*************************************************************/

#define    G0      7.536979e13
#define    G1      1.0027379093
#define    P11     3.14159265
#define    P2      6.283185307
#define    P0      0.017453292

#define    R0      6378.16
#define    F       0.003352891
#define    F2      0.001676445

#define    NON         0
#define    STANDALONE  1
#define    TERMINAL    0

#define    KENWOOD     0	     /* Rig maker name */
#define    YAESU       1
#define    ICOM	       2
#define	   ICOM_821    3

#define    ICOM_R_AD   0x2E	    /* ICOM CAT default destination address */ 
#define    ICOM_T_AD   0x2E	    /* ICOM TX default destination address */
#define    CAT	        0	    
#define    MIC          1

#define    NONE         0	    /* TRX mode */
#define    LSB		1
#define    USB		2
#define    CW		3
#define    FM		4

#define    NON_ST      ' '           /* NO selcet */
#define    TRACK_ST    '*'
#define    MULTI_ST    '!'	     /* multi satellite tracking mark */
#define    PRIORITY_ST    '&'           /* priority track mark */

#define    SATNAME     1
#define    CATALOG     2
#define    ELEMENT     3
#define    ECCN        4
#define    ETIME       5
#define    EREV        6
#define    INCL        7
#define    RAOF        8
#define    ARG         9
#define    ANOMALY     10
#define    MOTION      11
#define    DECAY       12
#define    NON_INS     0 

#define     _300B 	0
#define    _1200B	1	/* COM port baud rate */
#define    _2400B	2
#define    _4800B	3
#define    _9600B	4
#define   _19200B       5

sbit	RS    =	0x92;
sbit	R_W   =	0x91;
sbit	ENABLE=	0x90;	

/*****************/
/* ASM PARAMETER */
/*****************/

UINT	i_wait;

static code USN char *greeting[]={
	"\r\n\r\n\r\n\t****************************************\r\n",
	"\t*        TrakBox  8052  v3.60a         *\r\n",
	"\t*   (C) JAMSAT/JA6FTL   Sep.22 2013    *\r\n",
	"\t****************************************\r\n",
	"  TrakBox 8052  \0",  		  /* Title display for LED */
	"  version 3.60a \0",
	"(C)JAMSAT/JA6FTL\0"
};

UCHAR	d1,p1;

USN  short  SW;		   		/* s/w switch */
USN  short  SW2;		   	/* Flag */
USN  short  SW3;

float  local_inf[4];	/* 0:local_lat, 1:local_lon, 2:local_lin_c, 3:local_high */

data USN short  adc_data;
data	 short  adc_az,adc_el;

USN char   pulse_w,pulse_i;	/* mic click pilse width and interval */
USN short  pll_lock;	   /* ADC read value in modem lock */
USN short  pll_reff;           /* ADC read value one click up */

data   USN int   tm_year;
data   USN char  tm_mon,tm_day,tm_hour,tm_min,tm_sec;
data   USN char  time_ch[13];
USN short  rs_buff_mark;	   /* RS buffer max point */

/************************/
/*  Array variables     */
/************************/

static USN char	baud_tab[6] =		 /* COM port baud rate */
	{0x40,0xD0,0xE8,0xF4,0xFA,0xFD}; /* 300,1200,2400,4800,9600,19200 */
	
static USN char lpt_baud_tab[5] =	 /* LPT port baud rate */
	{0x40,0xA0,0xD0,0xE8,0xF4};	 /* 1200,2400,4800,9600,19200 */
	
USN char baud_rate,lpt_baud_rate;

static USN char day_tab[2][13]={
	{0,31,28,31,30,31,30,31,31,30,31,30,31},
	{0,31,29,31,30,31,30,31,31,30,31,30,31}
	};

static float G2_tbl[30]={   /* Greenwitch Sideral Time table  added 04-01-20 */
        /*1991*/  0.275442,0.274779,0.276854,0.276191,
        /*1995*/  0.275528,0.274865,0.276940,0.276277,0.275613,
        /*2000*/  0.274950,0.277025,0.276362,0.275699,0.275036,
        /*2005*/  0.277111,0.276448,0.275785,0.275122,0.277196,
        /*2010*/  0.276533,0.275870,0.275207,0.277282,0.276619,
        /*2015*/  0.275956,0.275293,0.277368,0.276704,0.276041,
        /*2020*/  0.275378};

#ifdef NOTDEF
static float G2_tbl[20]={   /* Greenwitch Sideral Time table  1996-12-01 */
        0.275442,       /* 1991 */
        0.274779,
        0.276854,
        0.276191,
        0.275528,       /* 1995 */
        0.274865,
        0.276940,
        0.276277,
        0.275613,
        0.274950,       /* 2000 */
        0.277025,
        0.276362,
        0.275699,
        0.275036,
        0.277111,       /* 2005 */
        0.276448,
        0.275785,
        0.275122,
        0.277196,
        0.276533 };      /* 2010 */
#endif

float   PRESET_AZ[5];	/* preset antenna control Az */
float   PRESET_EL[5];

USN char    O_name[SATMAX][20];
USN short   Y02[SATMAX];         /* epoch Year */
float   T02[SATMAX];
float   M02[SATMAX];
float   N02[SATMAX];
float   I02[SATMAX];
float   E02[SATMAX];
ULONG   K02[SATMAX];	/* epoch # */
float   W02[SATMAX];
float   O02[SATMAX];
float   N12[SATMAX];        /* Decay rate  */
float   F01[SATMAX];        /* Downlink Freq */
float   F02[SATMAX][4];        /* Uplink Freq */
USN char UPLINK_CH[SATMAX];	/* Uplink channel 0-4 */
float   F0CONV[SATMAX];	    /* F0 correction for converter */
float   F1CONV[SATMAX];     /* F1 correction for converter */
float	CORRECT_MIC[SATMAX]; /* Doppler correction value for mic click mode */
USN char  MDU[SATMAX];        /* Uplink mode */
USN char  MDD[SATMAX];        /* Dwnlink mode */
USN short   SAT_FLAG_SW[SATMAX];  /* Flag SW for each satellite */

char	sat_list[REG_MAX+1];	/* Control SAT LIST */

USN char   local_call[20];             /* Use LOCAL CALLsign */
USN char   rs_buf[RS_BSIZE];           /* MSG BUFF */


/*************************/
/* Kepele variables      */
/*************************/

float      G2,I0,O0,E0,W0,M0,N0,K0;
float      TX0,NX0;
float      FREQ1;          /* Base freq for doppler calculation DOWN LINK */
float	   FREQ2;	   /* '''''                               UP LINK */
float      T,D,DA;
float      Y2;

/**********************/
/* ROTOR value        */
/**********************/

float  ELDEG;                   /* Elevation range for 1 deg */
float  AZDEG;                   /* Azimuth   range for 1 deg */

USN short  az_mini;                 /* ROTATOR MINI VALUE    */
USN short  az_max;                  /*         MAX           */
USN short  el_mini;                 /* ELEVATION MINI        */
USN short  el_max;                  /*           MAX         */

float  skew_db[5];  /* 0:AZ_skew 1:EL-skew 2:AZ-dbnd 3:EL_dbnd */

float  AZ_setv;                 /* Set AZ deg.           */
float  EL_setv;                 /* Set EL deg            */

float  AZ_park;
float  EL_park;

float  rotor_lim[5]; /* 0:AZ_lim_min,1:AZ_lom_max,2:EL_lim_min,3:EL_lim_max */

float  trk_start_el;
float  p16_on_el;
float  p16_off_el;

/************************/
/* CAT variables        */
/************************/

USN char   rig_maker;		/* Rig variation for CAT command */ 
USN char   yaesu_tx_cmd;	/* YAESU freq set command char */
USN char   yaesu_rx_cmd;	/* YAESU freq set command char */
USN char   i_rx_address;	/* ICOM RX destination address  */
USN char   i_tx_address;   	/* ICOM TX destination address  */
USN char   icom_r;
USN char   icom_t;

USN long   trk_frq_d;           /* temporaly freq for command step */
USN long   trk_frq_u;
    long   trk_frq_correct;	        /* manual track frequency correction by +,- */

float  frq_park[3];		/* 0:RX park freq, 1:TX park freq  */

USN long   trk_frq_band;		/* 20/200 Hz  */

USN char  trx_mode[4];    /* 0:up_mode,1:dwn_mode,2:up_park_mode,3:dwn_park_mode*/

/**************************/
/*  Calculation variables */
/**************************/

long  RFD,RFU;	 

float C[13];
float A,E,E1,S1,C1,O,W,Q;
float M9,M,S3,C3,R3,M1,R,R9;
float X,X0,X9,X5,Y,Y0,Y9,Y5,Z,Z0,Z1,Z9,Z5;
float G7,S7,C7,L8,S9,C9,S8,C8,S5;
float R5,Z8,X8,Y8,C5,E9,B5,A9,L5,W5,F9U,F9D;
float VX9,VY9;

/**********************/
/* MISC variables     */
/**********************/
USN  long   DX,mem_DX_az,mem_DX_el;	/* sec of the year */
USN short  err_timeout,err_recover_tm;

USN char   sat_mx;         /* RAM stored max number */
USN char   ld_sat;         /* sat number for load to selection menu */
char      obj_sat;        /* Tracking object satellite (0-14) */
USN char  m_track_num;    /* satellites number of multi tracking mode */

USN long   fd0_los;		/* Freq. at LOS for step control */
USN long   fdx_los;		/* Freq. for refference */
USN long   step_size;	/* step freq value */

USN char   zzzcall[10];		   /* RAM backup fail check ID */

/***********************/
/* PROTO TYPE          */
/* Function proto type */
/* for ASM code        */
/***********************/

void   init_io(void) small;
void   read_tm(void) small;
void   set_date(char *) small;
void   read_adc(USN char) small;
void   tm_hold(void) small;

/**********************************************************/
/*     extern Assembler code        1991/10/22            */
/**********************************************************/

void   spout(UCHAR,UCHAR);                 /* Serial printer out */
void   spout2(char);		    /* Serial printer out-2 */
void   icom_wait(UCHAR);
/*
void   lcd_cmd(unsigned char);
void   lcd_data(unsigned char);
*/

/***************************************/
/*      My code                        */
/***************************************/
void   lcd_data(UCHAR);
void   lcd_cmd(UCHAR);

void   main(void) small;
void   print_lcd(char *) small;
void   months(void) small;
void   send(char *) small;
USN    int day_of_year(USN short,USN char,USN char);
void   sel_sat(char);
void   set_def(void);
void   clr_buff(void) small;
void   clr_rs_buff(void) small;
void   main_loop(char);
void   tracking1();
void   tracking2();
void   display_1();
void   display_2();
USN char sel_main_menu(void);
USN char sel_ant_menu();
void   time_set(void);

void   radio_p_conf(void);
short  disp_sat(void); 
short  disp_list_sat(void);
void   calc_sat_sched(short);
void   get_epday(USN short,USN char,USN char,USN char,USN char,USN char);

void   month_day(USN short,USN short,USN short *,USN char *,USN char *);
void   to_hour(float,USN char *,USN char *,USN char *);

void   set_first(void) small;
void   set_stn_inf(void);		/* set station data */

char   disp_yn(char *) small;
float  get_fval(char *,short *);
short  get_val(char *,short *);
void   sys_config(void);
void   calib_ant(void);
void   value_ant(void);

USN char  update_menu(void);
void   up_station(void);
USN char  config_menu(void);
void   kep_up_sat(void);

void   rotor_con1(short);
void   rotor_con2(void);
void   move_az_cw(void) small;
void   move_az_ccw(void) small;
void   move_az_non(void) small;
void   move_el_up(void) small;
void   move_el_dn(void) small;
void   move_el_non(void) small;

void   set_park();
void   set_dbnd(void);
void   param_list(void);
void   edit_sat_data(short);

float  pow(float,float);
float  ipower(float,short);
float  floor(float);
float  ceil(float);

int    getline (char *);

void   adc_moni(void) small;

void   disp_ant(void);
void   set_ant(void);
void   point_ant(void);
void   preset_ant(void);

void   sendp(char *) small;
void   read_kep(void);
void   read_f_minf(void);      /* read freq/mode file */

/*	short  perser(char *);	*/

void   add_list(void);
void   sel_obj(void);          /* select satellite tracking obj */
void   colect_list(void);

void   rig_mode_set(char);
void   rig_set_d(char);	       /* downlink freq set CAT */
void   rig_set_u(char);	       /* uplink freq set CAT */
/*
void   yaesu_cat_on() small;
void   yaesu_cat_off() small;
*/
void   yaesu_cmd(UCHAR,UCHAR) small;
void   f_step_con(char);       /* mic click control with PLL */
void   step_ini();	       /* Auto step pulse generate at AOS */
USN char  read_mode(void) small;        /* binary SW and R-ADC read & mode   */
void   disp_adc(void) small;
void   mic_step_con(void);
void   host_mode(void);		/* host mode  92-09-28 */
void   rotor_error();
void   el_az_deg_set();
void   flip_check(char);
void   wait(USN int);
void   reset_timeout();
void   icom_cat_header(USN char);
void   go_parking(char);	/* 98-10-28 */

/**********************************************************************/
/*             TRAKBOX MAIN PROGRAM                                   */
/**********************************************************************/

void main() small
{
USN 	char	sel=0,mode;
	short	num;

           init_io();
           if(strcmp(zzzcall,"YORON") != 0){
	   	set_first(); /* RAMS backup fail check */
		init_io();
		set_date("13/01/01 00:00:00");
	   }
           if(SW2 & STEP_PORLAR) XBYTE[PPI+2] |= ~PULSE_END;
	   else XBYTE[PPI+2] &= PULSE_END;
	   
	   wait(30000);		      /* wait for slow LCD display */
           print_lcd(greeting[4]);    /* Title display 1992/09/25 */
           lcd_cmd(0xc0);	      /* LCD line-2 */
	   print_lcd(greeting[6]);    /* copy right */

	   reset_timeout();	/* include months()  */

           if(T > 396) send("\n\r\x07Too old elements. \r\n");

           mode = read_mode();

        if(mode){			/* standalone mode */
	       SW |= RUN_MODE;          /* set run_mode SW */
               send("\n\r\x07");
               for(;;){
		       fd0_los = 0L;
		       XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
		       mode = read_mode();		       
               	       obj_sat = mode - 1;             /*  save */
		       if((sat_list[obj_sat] < 0) || (mode == 0)){
        	       		mode = read_mode();
                 		continue;
        	       }
		       if(SW & MULTI_MODE) tracking1();
		       else tracking2();
	       }
        }  	/* standalone mode end */

        else{    	/* else terminal mode */
           SW &= ~RUN_MODE;  	       	       /* reset run_mode SW */
	   for(;;){
	       reset_timeout();
	       XBYTE[PPI+2] &= 0xf0; 	       /* AZ/EL STOP */
	       fd0_los = 0L;
               SW &= ~MULTI_MODE;

	       SW2 |=  PARK_TRX_FLAG;

               if(SW & HOST_MODE) sel = 7;     /* if host mode */
	       else  sel = sel_main_menu();    /* display main menu */

               switch (sel){
                   case    1:
		   case   's':
                       send("\r\n\t***  Tracking object satellite  ***\r\n");
	       	       num = disp_list_sat();
		       if((num != -1) && (num < ld_sat)) tracking2();
	       	       break;
                   case    2:
		       send("\r\n\r\n\t***  Select object satellite  ***\r\n");
        	       num = disp_list_sat();
               	       if(num != -1) calc_sat_sched(sat_list[num]);
                       break;
                   case    3:
                       kep_up_sat();
                       break;
                   case    4:
                       up_station();
                       break;
                   case    5:
                       time_set();
                       break;
                   case    6:
                       sys_config();
                       break;
		   case    7:
		       host_mode();
		       break;
		   case    8:
		       point_ant();
		       break;
		   case   'm':
		       SW |= MULTI_MODE;
		       tracking1();
		       break;
		   case   '0':
		   	/*	copy right 	*/
			break;
               }  /* switch end */
           }  /* for loop end */
         }   /* if terminal mode end */
}

void   init_io() small
{

   XBYTE[NEC_ADC+1] = 0x02;            /* ADC INIT 1/4 clock sampling Binary */
   XBYTE[PPI+3] = 0x82;                /* PA:out PB:in PC:out */
   XBYTE[PPI+0] = 0x00;                /* PA */

   SCON = 0x52;    /* SCON */                  /* setup serial port control */
   TMOD = 0x22;    /* TMOD */                  /* hardware  */
   TCON = 0x50;    /* TCON */
   PCON = 0x80;	   /* SMOD =1 */

   TR1  = TR0 = 0;
   
   if((XBYTE[PPI+1] & RSP1200B) == 0){
   	 TH1 = 0xe8; /* RS port 1200bps */
   }
   else if(baud_rate < _300B || baud_rate > _19200B){
   	 baud_rate = _9600B;
	 TH1 = 0xfa;		/* default 9600bps */
   }

   else TH1 = baud_tab[baud_rate];	/* COM port baud rate
   					  0:300, 1:1200, 2:2400, 3:4800
					  4:9600 bps 5:19200    */
		/* TH1 = 256 - (2~SMOD x OSC_f)/(384 x baud_rate) */

#ifdef NOTDEF
   TH0 =  0x41;    /* LPT BAUD RATE 4800 & 1200bps for ICOM       */
  		   /* 256-(sys_clock/12) / (baud rate)  */ 

			/* TH0  4800:0x40, 9600:0xA0, 19200:0xD0, 38400:0xE8 */
#endif

   TH0 = lpt_baud_tab[lpt_baud_rate];

   TR1 = TR0 = 1;   
/**********
   T2CON = 0x04;                          
   TH2 = RCAP2H = 0xff;         8052 Timer-2 baud rate =4800         
   TL2 = RCAP2L = 0x3f;
   1200:FD00,4800:FF40,9600:FFA0,19200:FFD0,38400:FFE8
**********/

   if((XBYTE[RTC_CF] & 0x04) != 0x04){   	/* 24/12 flag check */ 
	   send("\x07");	  /* beep */
     	   XBYTE[RTC_CF] = 0x07;  /* RTC Reset test0,reset0,24hr1,stop1 0111 */
	   XBYTE[RTC_CF] = 0x04;  /* RTC Reset reset0,24hr1 0100 */
           XBYTE[RTC_CD] &= 0xfe;
	   XBYTE[RTC_CF] = 0x04; 
   }
                                       /*  LCD init  */
   lcd_cmd(0x38); 		       /*  LCD module SHARP-LM16257 */
   lcd_cmd(0x38);
   lcd_cmd(0x0C);		       /*  DB0-DB7 : 8255 port-A */
   lcd_cmd(0x06);
   lcd_cmd(0x01);

   XBYTE[RTC_CD] &= 0xfb; 	       /* IRQ FLAG reset */
}

void   lcd_cmd(ch)      /* new for 80c251 */
USN char ch;
{
        RS = 0;        /* RS */
        R_W = 0;       /* R_W */
        XBYTE[PPI] = ch;
        wait(10);
        ENABLE = 1;         /* ENABLE */
        ENABLE = 0;
}

void   lcd_data(ch)
USN char ch;
{
        RS = 1;
        R_W = 0;
        XBYTE[PPI] = ch;
        wait(10);       
        ENABLE = 1;
        ENABLE = 0;
}
        
void   set_first() small
{
       send("\r\n\tData in RAM was lost !! Set default parameter ! \x07\r\n");

	   clr_buff();
	   set_def();		 /* set default Kepele data  */
	   
	   memcpy(local_call,"NOCALL\0",7);
           memcpy(zzzcall,"YORON\0",6);

           if((XBYTE[PPI+1] & RSP1200B) == 0) TH1 = 0xe8; /* RS port 1200bps */
	   else baud_rate = _9600B;	/* COM port 9600bps */
	   lpt_baud_rate  = 2;		/* 4800bps */
	  
     	   XBYTE[RTC_CF] = 0x07;  /* RTC Reset test0,reset0,24hr1,stop1 0111 */
	   XBYTE[RTC_CF] = 0x04;  /* RTC Reset reset0,24hr1 0100 */
           XBYTE[RTC_CD] &= 0xfe;
	   XBYTE[RTC_CF] = 0x04; 

           local_inf[0] = LL9;      /*   set default parameter  */
           local_inf[1] = WW9;
              if(local_inf[1] <= 0 ) local_inf[2]= -1.0 * local_inf[1];
              else 		  local_inf[2] = 360.0 - local_inf[1];
	   local_inf[3] = HH9;
           skew_db[0] = 0.0;
           skew_db[1] = 0.0;
           skew_db[2] = AZ_DEAD_BAND;
           skew_db[3] = EL_DEAD_BAND;

	   az_mini = AZ_MINI;    	/* ADC value */
           az_max  = AZ_MAX;
           el_mini = EL_MINI;
           el_max  = EL_MAX;

	   AZ_park = 0.0;		/* Azimuth Parking position */    
	   EL_park = 0.0;		/* Elevation Parking Position */
	   frq_park[0] = frq_park[1] = 0;
	   trx_mode[0] = trx_mode[1] = trx_mode[2] = trx_mode[3] = FM; /* FM */

	   rotor_lim[0] = 0.0;		/* Az limit min. */
	   rotor_lim[1] = 360.0;		/*          max. */
	   rotor_lim[2] = 0.0;		/* EL limit min. */
	   rotor_lim[3] = 90.0;		/*          max. */
	   trk_start_el = TRACK_START;  /* = -2.0 default */
	   p16_on_el = P16_ON;	 	/* = -4.0 default */
	   p16_off_el = P16_OFF;	/* = -6.0 default */
	   
           obj_sat = 0;                 /* default track obj */
	   m_track_num = 3;		/* default multi tracking number */

	   trk_frq_correct = 0;		/* manual correction +/- */   

	   trk_frq_band = (long)TRK_FRQ; /*  20Hz default */
	   rig_maker    = KENWOOD;	/* CAT port */

	   icom_r = i_rx_address = ICOM_R_AD;	/* ICOM RX destination address */
	   icom_t = i_tx_address = ICOM_T_AD;	/* ICOM TX destination address */ 

           yaesu_tx_cmd = 0x2E;	/* FT-736 */
	   yaesu_rx_cmd = 0x1E;

	   i_wait = 50;	/* 50msec */

	   step_size    = 100;		/*  step freq. of mic click */
	   pulse_w      = 1;		/* n*1000 */
	   pulse_i      = 2;		/* n*1000 */

           pll_lock     = ADC_CEN;      /*  TS790 default  */
	   pll_reff     = 560;          /*  1KHz off point */
	   err_timeout = ERR_TIMEOUT;		/*  Error mode timeout time */
	   err_recover_tm = ERR_RECOVER_TIME;	/*  recovery time from error mode */


#ifdef NOTDEF
	   SW &= ~MIC_CLICK;		/* Mic click not selected */
           SW &= ~HOST_FCONV;           /* hostmode freq offset */
           SW &= ~TRACK_ID;		/* track NO */
	   SW &= ~ROTOR_ZERO;           /* rotor 0 = South  */
	   
	   SW &= ~ANT_PARK;		/* No antenna parking */
	   SW &= ~ROTATE_L;		/* Rotator limit not selected */
	   SW |=  YAESU_FMN;	        /* YAESU FM narrow filter selected */	   
	   SW &= ~FULL_O_CAT;	        /* full orbit cat not selected */   

	   SW &= ~RUN_MODE;		/* terminal mode */
	   SW &= ~MULTI_MODE;		/* multi-tracking mode not selected  */
	   SW &= ~BREAK_M;	/*  break from s-track routine in m-track flag */
	   SW |= PRIORITY_TRK;  /*  priority tracking flag */

	   SW &= ~HOST_MODE;    /* Host mode **/
	   SW |= DOPPL_UPL;	/* uplink Doppler correction ON */
	   SW |= DOPPL_DWN;	/* downlink Doppler correction ON */
	   /** KEPELE_FMT */
#endif

	   SW = 0x6840;		/* 0110 1000 0100 0000 */

#ifdef NOTDEF
	   SW2 |= FLAG_STEP;	/* set  flag for AOS/LOS detection */
	   SW2 |=  FLAG_AOS_CAT; /* Freq set at AOS in mic click mode */
	   SW2 &= ~STEP_CON;	/* auto step control flag */
	   SW2 &= ~STEP_PORLAR;  /* normal porlarity */

	   SW2 &= ~DOP_ROUND_D;  /* Doppler round by 5KHz */
	   SW2 &= ~_711811;      /* KENWOOD 711/811 combo not selected */
	   SW2 &= ~P16_ON_FLAG;  /* P16 On flag */	   
	   SW2 |=  MODE_CMD_FLAG;
	   
	   SW2 |=  MIC_CAT_FLAG;
	   SW2 &= ~PARK_TRX_MODE; /* TRX mode/freq. set */
	   SW2 |=  PARK_TRX_FLAG;
	   SW2 |=  ERR_MODE_OFF;

	   SW2 &= ~SEL_SAT_ADC;
	   SW2 &= ~FLIP_MODE;
	   SW2 &= ~FLIP_ON;
	   SW2 &= ~FLIP_CALC_FLAG;
#endif
	   SW2 =  0x0d83; 		/* 0000 1101 1000 0011 */	   

	   SW3 |= YAESU736;

	   el_az_deg_set();
}

void    el_az_deg_set()
{
        AZDEG                     = (float)(abs(az_max - az_mini) / 360.0);
        if(SW2 & FLIP_MODE) ELDEG = (float)(abs(el_max - el_mini) / 180.0);
        else                ELDEG = (float)(abs(el_max - el_mini) / 90.0);
}

void 	clr_buff() small   /* clear the whole data area */
{
data USN char  i;
	   for(i=0;i<SATMAX;i++){
		 Y02[i]= M02[i]= N02[i]= I02[i] = E02[i] = 
		 W02[i]= O02[i]= N12[i]= F01[i]= UPLINK_CH[i] =
		 F02[i][0]=F02[i][1]=F02[i][2]=F02[i][3]= 
		 CORRECT_MIC[i] = F0CONV[i]= F1CONV[i]= 0;		 
		 K02[i]= T02[i]= 1;		 
		 MDU[i] = MDD[i] = '\0';

		 SAT_FLAG_SW[i] &= ~ICOM_R_ADDR;
		 SAT_FLAG_SW[i] &= ~DPLR_MODE;
		 SAT_FLAG_SW[i] &= ~MA_MARK;
		 SAT_FLAG_SW[i] &= ~SELECTED_MRK;
                 memset(O_name[i],'\0',20);
	   }
	   rs_buff_mark = 0;
           for(i=0;i<REG_MAX;i++) sat_list[i] = -1 ; 
	   for(i=0;i<5;i++){PRESET_AZ[i] = PRESET_EL[i]= 0;}
}

void 	clr_rs_buff() small     /* clear RS-232C buffer */
{
data USN short i;
           for(i=rs_buff_mark;i<RS_BSIZE;i++) rs_buf[i] = '\0';   /* BUFF CLR*/
}

void	wait(n)
USN	int	n;
{
data USN	int	i;
	for(i=0;i<n+1;i++) continue;
}	

void	reset_timeout()
{
	months();
	mem_DX_az = mem_DX_el = DX;
}

USN char read_mode() small        /* Binary SW read & ADC READ_MODE */
{
       if((SW2 & SEL_SAT_ADC) == 0){              /* binary SW */
                return((XBYTE[PPI+1] ^ 0xff) & 0x0f);
       }
       else{ 
       		read_adc(MODE_PORT); 		  /* ADC binary SW */      
                return ((adc_data >> 6) & 0x0f);
       }
}

void   send(s) small
char   *s;
{
data  USN char  ch;
   for(;;){
       ch = *s++;
       if(ch == '\0') break;
       putchar(ch);
   }
}

void   print_lcd(s) small
char   *s;
{
data USN char ch;
   for(;;){
       ch = *s++;
       if(ch == '\0') break;
       lcd_data(ch);
   }
}

void   sendp(s) small                 /* out to LP port */
char   *s;
{
data USN char  ch;

   for(;;){
       ch = *s++;
       if(ch == '\0') break;
       spout(ch,0);
   }
}

void   sendp2(s)                   /* out to Alternate LP port */
char   *s;
{
data  USN char  ch;

   for(;;){
       ch = *s++;
       if(ch == '\0') break;
       spout2(ch);
   }
}

int   getline(char *line)
{
	USN char    ch;
	USN int cnt = 0;
 
       for(;;){
           ch = getchar();
           if(ch == CR || ch == LF) break;
	   if((ch == BS) || (ch == DEL) && cnt != 0){/* BS support 15,Jan 91 */
		*line-- = '\0';
		cnt--;
		continue;
	   }
           *line++ = ch;
           cnt++;
       }
       *line++ = '\0';
       return cnt;
}

short  get_val(msg,size)
char   *msg;
short  *size;
{
USN    char    temp[80];

       send(msg);
       getline(temp);
       *size = 0;
       *size = strlen(temp); 
       return (atoi(temp));
}

float  get_fval(msg,size)
char   *msg;
short  *size;
{
USN    char    temp[80];

       send(msg);
       getline(temp);
       *size = 0;
       *size = strlen(temp); 
       return (atof(temp));
}

#ifdef NOTDEF
float pow(val1,val2)
float val1,val2;
{
     float v1,v2,res;

	if(val2 <= 32767.0 && val2 >= -32768.0 && val2 ==(int)val2)
		res = ipower(val1,val2);
	else if(val1 > 0){
           v1 = log(val1);
           v2 = v1 *val2;
           res = exp(v2);
	}
	else res = 0.0;
	return res;
}

float ipower(x,n)
float x;
short n;
{
	USN int	abs_n;
	float	r,res;
	
	abs_n = abs(n);
	r = 1;
	while(abs_n != 0){
		if(abs_n &1 ) r *= x;
		x *= x;abs_n >>= 1;
	}
	if(n >= 0) res = r;
       else res =  1/r;
       return res;
}

float  floor(val)
float  val;
{
	float val1;

           val1 = (float)((long) val * 1.0);	 /* 92/01/06 */
	   return (val1);
}

float  ceil(val)
float val;
{
	float val1,res;
           val1 = (float)((int) val * 1.0);
	if(fabs(val - val1) > 0.000001){
           res = val1+1.0;
        }
        else res = val1;
        return res;
}
#endif

char  disp_yn(s) small
char   *s;
{
USN	char    ch;
           send(s);
           send(" [Y/N] ? ");
           ch = getchar();
           if((ch | 0x20) == 'y') return 1;  
	   else if((ch | 0x20) == 'n') return 0;
           else return -1;  
}

void   tm_hold() small			/*  RTC hold  */
{
       for(;;){                                /* HOLD ON */
           XBYTE[RTC_CD] |= 0x01;
           if((XBYTE[RTC_CD] & 0x02) == 0) break;      /* test busy */
           XBYTE[RTC_CD] &= 0xfe;
           wait(10);
       }
}

void   read_tm() small                /* read RTC */
{
data USN	char	i;
data USN	short	rtc_address;

	tm_hold();
	for(i=0;i<0x0C;i++){
		rtc_address = RTC_Y10 - i;
		time_ch[i] = 0x30 | XBYTE[rtc_address] & 0x0F;
	}
	time_ch[0x0C] = '\0';

	XBYTE[RTC_CD] &= 0xfe;   /* time run */
}

void   set_date(s) small             /* RTC SET */
USN char   *s;
{
data USN     char    i,j,k;

        j=16;k=0;
        for(i=0;i<0x0C;i++){
                XBYTE[RTC+i] = s[j] - 0x30;
                j--;k++;
                if(k>=2){j--;k=0;}
        }
}

void   read_adc(ch) small           /* Read ADC & store (adc_data) */
USN char  ch;
{
       data USN short   adata,data_h,data_l;
       USN char   port;

       port = (USN char)(ch & 0x07);    /* MAX CH = 8 */
       XBYTE[NEC_ADC] = port;           /* select channel */

       wait(30);

       data_h = (USN short) (XBYTE[NEC_ADC+1]& 0xff);
       data_l = XBYTE[NEC_ADC] & 0xff;
       adata = data_h << 8;
       adata |= data_l;
       adc_data = (adata >> 6) & 0x03ff;  /* Result */
}

void   adc_moni() small
{
       send("\r\n\t==== ADC value and antenna direction ==== \r\n\r\n");

       for(;;){
           if(RI){         /* ANY KEY IN ?? */
               getchar();
               return ;                    /* Break */
           }
	   disp_ant();
       }
}

USN char  sel_main_menu()		/* Main menu display */
{
USN  char    ch;

           send(greeting[0]);
           send(greeting[1]);
           send(greeting[2]);
           send(greeting[3]);
	   lcd_cmd(0x80);
           print_lcd(greeting[4]);
           lcd_cmd(0xc0);
	   print_lcd(greeting[5]);
	   
           send("\r\n \t------ MAIN MENU ------\r\n\r\n");
           send("\t1. Realtime Tracking.\r\n");
           send("\t2. Display Schedule.\r\n");
           send("\t3. Update Satellite Elements.\r\n");
           send("\t4. Update Station Elements.\r\n");
           send("\t5. Update Realtime Clock.\r\n");
           send("\t6. Configuration.\r\n");
	   send("\t7. Host mode.\r\n");
	   send("\t8. Direct Antenna control.\r\n\r\n");

	   send("\r\tSelect [1-8,S,M]: ");

	   for(;;){
		if(RI){
			ch= getchar();
			if(ch >= '0' && ch <= '8') return (ch - '0');
			else if((ch | 0x20) == 's') return 's';
			else if((ch | 0x20) == 'm') return 'm';
			else send("\r\tSelect [1-8,S,M]: ");
		}
		if((SW & ANT_PARK) && (SW & TRACK_ID)) go_parking(1);
	   }
}

USN char update_menu()
{
USN  char    ch;

           send("\r\n\r\n\t   ========= UPDATE MENU ========\r\n");
           send("\t1. EDIT:   View/Edit satellite Elements by hand.\r\n");
           send("\t2. SELECT: Select Satellites from the database.\r\n");
           send("\t3. RELOAD: Clear database & read in new Elements.\r\n");
           send("\t4. APPEND: Append new Elements.\r\n");
	   send("\t5. MULTI TRACK: Configure multi tracking sat. number.\r\n\r\n");
           for(;;){
               send("\r\t\tSelect: ");
               ch = getchar();
               if(ch >= '1' && ch <= '5' ) return (ch -'0');
               if((ch | 0x20) == ESC) return ESC;
           }
}

void   kep_up_sat()     /*      Kepele data mentanance   */
{
char         num,chk;
USN char    i,sel;

short   cs;
   for(;;){
	switch(sel = update_menu()){
		case	ESC: return;
		case	1:    /* View & Edit */
			send("\r\n\t***  Select object satellite  ***\r\n");
			num = disp_list_sat();
			if(num != -1) edit_sat_data(sat_list[num]);
			break;

		case	2:   /* select */
			chk = disp_yn("\x07\r\n\tSelected list will be lost. Are you sure ?");
               		if(chk <= 0) continue;
               		for(i=0;i<SATMAX;i++) SAT_FLAG_SW[i] &= ~SELECTED_MRK;

			send("\r\n\r\n\t1. Select    : Select sats. by hand.\r\n");
			send("\t2. Select All: Select all sats. in the database.\r\n");
			send("\t3. Upload    : Upload selection data file.\r\n");

                        send("\r\t\tSelect: ");
			switch(getchar()){
				case	'1':
					add_list();
					break;
				case	'2':
					for(i=0;i<sat_mx;){
						sat_list[i] = i;
						SAT_FLAG_SW[i] |= SELECTED_MRK;
						i++;
						ld_sat = i;
					}
					colect_list();
					break;
				case	'3':
					read_f_minf();   /* Read freq/mode info */
           				break;
			}
			break;
		case	3:
		case	4:
			if(sel == 3){
				chk = disp_yn("\x07\r\n\tStored data in memory will be lost. Are you sure ?");
				if(chk <= 0) continue;
				rs_buff_mark = 0;
			}
			else if((sel == 4) && (sat_mx >= 60)){
				send("\x07\r\n No room for append !\r\n");
				break;
			}
			send("\r\n\tFile format NASA (N) :  ");
			getchar();	/* dummy */
			for(i=0;i<SATMAX;i++) SAT_FLAG_SW[i] &= ~SELECTED_MRK; /* clear the * mark*/
               		read_kep();
			break;
		case	5:
again1:        		sel = get_val("\r\n\tHow many satellites configure for multi tracking mode ? (2 - 5)  ",&cs);
               		if((sel < 2 || sel > 5) || (sel > sat_mx)){
                        	send("\r\n\tOut of range !\x07");
                        	goto again1;
               		}
			else m_track_num = sel;
			break;
	}	/* switch end */
   }	/*  for loop end */
}

void   up_station()		/* Station elements update */
{
       short   cs,baudval;
USN    char    ch,tmp[10];
       float   newf;
USN    short   temp;
USN    char    *msg[] ={
	         "South",
	         "North"};
USN    char    *BAUDmsg[] ={		/* COM port Baud rate display */
	 	 " 300",
		 "1200",
		 "2400",
		 "4800",
		 "9600",
		 "19200"};

USN    char   *Smsg[] ={                       /* Station Elements msg */
               "         Callsign",
               "         Latitude",
               "        Longitude",
               "   Antenna height",
	       "Rotor start (S/N)",
               "    COM port baud"};

        send("\r\n\r\n\t   ========= Station Data ========\r\n");
        send("\t1. EDIT:   View/Edit station elements by hand.\r\n");
        send("\t2. RELOAD: Clear database & read in new data.\r\n\r\n");

        for(;;){
               send("\r\t\tSelect: ");
               ch = getchar();
               if((ch == '1') || (ch == '2')) break;
               if((ch | 0x20) == ESC) return;
        }
	
	if(ch == '2') set_stn_inf();
	else if(ch == '1'){
	
           putchar(CLR);	/* clear of line */
           send("\r\n\r\n\t========= Station Elements ========\r\n\r\n");
           printf("%15s:  %s\t",Smsg[0],local_call);
           if(getline(tmp))  strcpy(local_call,tmp);

           printf("%15s:  %+5.3f",Smsg[1],local_inf[0]);
           newf = get_fval("\t",&cs);
           if(cs != 0) local_inf[0] = newf;

           printf("%15s:  %+5.3f",Smsg[2],local_inf[1]);
           newf = get_fval("\t",&cs);
           if(cs != 0) local_inf[1] = newf;

	   if(local_inf[1] < 0 ) local_inf[2] = -1.0 * local_inf[1]; /* to west lon */
	   else if(local_inf[1] >=0 ) local_inf[2] = 360.0 - local_inf[1];

           printf("%15s:  %5.2f",Smsg[3],local_inf[3]);
           newf = get_fval("\t",&cs);
           if(cs != 0) local_inf[3] = newf;
           
	   temp = SW;		/* copy SWITCH */
	   printf("%15s:  %s\t ",Smsg[4],msg[(temp & ROTOR_ZERO) >> 3]);
           getline(tmp);
           if((tmp[0] | 0x20) == 'n') SW |= ROTOR_ZERO;
	   else if((tmp[0] | 0x20) == 's') SW &= ~ROTOR_ZERO;

           if((tmp[1] | 0x20) == 'a') SW2 |= SEL_SAT_ADC;
	   else SW2 &= ~SEL_SAT_ADC;

	   printf("%15s:  %s",Smsg[5],BAUDmsg[baud_rate]);
	   baudval = get_val("\t",&cs);

		switch(baudval){
			case	300: baud_rate = 0;break;
			case	1200: baud_rate = 1;break;
			case	2400: baud_rate = 2;break;
			case	4800: baud_rate = 3;break;
			case	9600: baud_rate = 4;break;
			case	19200: baud_rate = 5;break;
		}
	}	   
}

/*************************************************/
/*     Set real time clock                       */
/*************************************************/

void time_set()
{
USN   char    temp[30],ch;

           send("\r\n\tEnter 'Y' for zero sec. adjustment");
           send("\r\n\tEnter 'Q' for exit");
	   send("\r\n\tEnter 'Ret.' for Real Time Clock set\r\n\r\n");
	  
        for(;;){	   
	   	months();
	   	send("\tPresent Time:UTC ");
           	printf("%4d/%02bd/%02bd %02bd:%02bd:%02bd\r",tm_year,
                tm_mon,tm_day,tm_hour,tm_min,tm_sec);
	        if(RI){
	   		ch = getchar();
	   		if((ch | 0x20) == 'y') { 
				XBYTE[RTC_CD] |= 0x08;
	        		return; 
	        	}
	   		else if((ch | 0x20) == 'q') return;
	   		else break; 
	    	}
	}	   
	   months();
           send("\r\n\r\n\tSet realtime Clock.\r\n\tPresent Time:UTC ");
           printf("%4d/%02bd/%02bd %02bd:%02bd:%02bd\r\n",tm_year,
               tm_mon,tm_day,tm_hour,tm_min,tm_sec);
           send("\tEnter with format [YY/MM/DD HH:MM:SS] ");

	   temp[2] = temp[5] =0;
           getline(temp);
           if((temp[2] | temp[5]) == '/' && (temp[11] | temp[14]) == ':'){
	     	   XBYTE[RTC_CF] = 0x07;  /* Reset test0,reset0,24hr,stop1 */
		   XBYTE[RTC_CF] = 0x04;  /* RTC Reset reset0,24hr, 0100 */
	           XBYTE[RTC_CD] &= 0xfe;
		   XBYTE[RTC_CF] = 0x04; 
	           printf("\r\n\tSet time: %s\r\n",temp);
        	   send("\r\n\t   ENTER when ready.");
           	   ch = getchar();
            	   set_date(temp);
           }
           else{
               send("\r\n\x07 ! Format Error !");
	       time_set();               
	   }
}

void   edit_sat_data(s_num)     /* DIPLAY SAT DATA & EDIT */
short s_num;
{
       short   cs,ys;
USN    char    s_txt[100],tmp[20];
       float   newf;

USN    char    *DPLmsg[] = {
			"CAT ",
			"Mic-click "};
USN    char    *MAmsg[] = {
       			"No",
			"Yes"};
USN    char    *Dmsg[] = {                       /* DATA MSG */
               "       Satellite  :",
               "   Epoch Time T0  :",
               "    Epoch Rev K0  :",
               " Mean Anomaly M0  :",
               "  Mean Motion N0  :",
               "  Inclination I0  :",
               " Eccentricity E0  :",
               "  Arg perigee W0  :",
               "     R.A.A.N. O0  :",
               "        Decay N1  :",
               " Downlink Frq.F1  :",
               "   Uplink Frq.F2  :",
               "   Downlink mode  :",
	       "     Uplink mode  :",
	       "   Epoch Year Y0  :",	/* #16 */
	       "Doppler correct.  :",   /* #17 CAT/Mic click */
	       "  LCD MA display  :",   /* #18 Phase value display */
	       "   Offset for F1  :",   /* 17 */
	       "   Offset for F2  :",   /* 18 */
	       "     F1 slew(Hz)  :",   /* 19 */
               };
USN   char   *Mdmsg[] = {	/*  TX/RX mode  msg */
               "none",
	       "LSB ",
	       "USB ",
	       "CW  ",
	       "FM  "};

           send("\r\n\r\n\t\t====== Satellite data ======\r\n");

           printf("\t%s %-s\t\t",Dmsg[0],O_name[s_num]);
           if(getline(tmp))
		strcpy(O_name[s_num],tmp);

           printf("\t%s %d",Dmsg[14],Y02[s_num]);
           ys = (short)get_fval("\t\t\tY0:",&cs);
           if(cs != 0) Y02[s_num]= ys;

           printf("\t%s %6.5f",Dmsg[1],T02[s_num]);
           newf = get_fval("\t\tT0:",&cs);
           if(cs != 0) T02[s_num]=  newf;

           printf("\t%s %6ld",Dmsg[2],K02[s_num]);
           newf = get_fval("\t\tK0:",&cs);
           if(cs != 0) K02[s_num]= (ULONG)newf;

           printf("\t%s %6.5f",Dmsg[3],M02[s_num]);
           newf = get_fval("\t\tM0:",&cs);
           if(cs != 0) M02[s_num]=  newf;

           printf("\t%s %6.5f",Dmsg[4],N02[s_num]);
           newf = get_fval("\t\tN0:",&cs);
           if(cs != 0) N02[s_num]=  newf;

           printf("\t%s %6.5f",Dmsg[5],I02[s_num]);
           newf = get_fval("\t\tI0:",&cs);
           if(cs != 0) I02[s_num]=  newf;

           printf("\t%s %6.5f",Dmsg[6],E02[s_num]);
           newf = get_fval("\t\tE0:",&cs);
           if(cs != 0) E02[s_num]=  newf;

           printf("\t%s %6.5f",Dmsg[7],W02[s_num]);
           newf = get_fval("\t\tW0:",&cs);
           if(cs != 0) W02[s_num]=  newf;

           printf("\t%s %6.5f",Dmsg[8],O02[s_num]);
           newf = get_fval("\t\tO0:",&cs);
           if(cs != 0) O02[s_num]=  newf;

           printf("\t%s %e",Dmsg[9],N12[s_num]);
           newf = get_fval("\tN1:",&cs);
           if(cs != 0) N12[s_num]=  newf;

           printf("\t%s %6.3f",Dmsg[10],F01[s_num]);
           newf = get_fval("\t\tF1:",&cs);
           if(cs != 0) F01[s_num]=  newf;

           printf("\t%s %6.0f",Dmsg[19],CORRECT_MIC[s_num]);
           newf = get_fval("\t\tFx:",&cs);
           if(cs != 0) CORRECT_MIC[s_num]=  newf;

	   printf("\t%s %6.3f",Dmsg[17],F0CONV[s_num]);
           newf = get_fval("\t  F1OFFSET:",&cs);
           if(cs != 0) F0CONV[s_num]=  newf;

           printf("\t%s %6.3f %6.3f %6.3f %6.3f\r\n\t\t\t    ",Dmsg[11],
	   	F02[s_num][0],F02[s_num][1],F02[s_num][2],F02[s_num][3]);
/**
           newf = get_fval("\tF2_1:",&cs);
           if(cs != 0) F02[s_num][0]=  newf;
**/     
           if(getline(s_txt))
		   sscanf(s_txt,"%f %f %f %f",&F02[s_num][0],&F02[s_num][1],
		   		&F02[s_num][2],&F02[s_num][3]);
  
           printf("\t%s %6.3f",Dmsg[18],F1CONV[s_num]);
           newf = get_fval("\t  F2OFFSET:",&cs);
           if(cs != 0) F1CONV[s_num]=  newf;
	
	   printf("\t%s %s\t\t   ",Dmsg[12],Mdmsg[MDD[s_num]]);
           if(getline(tmp)){
		switch(tmp[0] | 0x20){
			case 'l': MDD[s_num] = LSB;break;
			case 'u': MDD[s_num] = USB;break;
			case 'c': MDD[s_num] = CW;break;
			case 'f': MDD[s_num] = FM;break;
			case 'n': MDD[s_num] = NONE;
		}
	   }
	   printf("\t%s %s\t\t   ",Dmsg[13],Mdmsg[MDU[s_num]]);
	   if(getline(tmp)){
		switch(tmp[0] | 0x20){
			case 'l': MDU[s_num] = LSB;break;
			case 'u': MDU[s_num] = USB;break;
			case 'c': MDU[s_num] = CW;break;
			case 'f': MDU[s_num] = FM;break;
			case 'n': MDU[s_num] = NONE;
		}
	   }
	   printf("\t%s %s\t\t   ",Dmsg[15],DPLmsg[(SAT_FLAG_SW[s_num] & DPLR_MODE)]);
	   if(getline(tmp)){
	       if((tmp[0] | 0x20) == 'c') SAT_FLAG_SW[s_num] &= ~DPLR_MODE; /* CAT */
	       else if((tmp[0] | 0x20) == 'm') SAT_FLAG_SW[s_num] |= DPLR_MODE; /* mic click */ 
	   }
	   printf("\t%s %s\t\t\t   ",Dmsg[16],MAmsg[(SAT_FLAG_SW[s_num] & MA_MARK) >> 1]);

	   if(getline(tmp)){
	       if((tmp[0] | 0x20) == 'y') SAT_FLAG_SW[s_num] |= MA_MARK; /* Phase display */
	       else if((tmp[0] | 0x20) == 'n') SAT_FLAG_SW[s_num] &= ~MA_MARK; 
	   }
}

USN char   config_menu()
{
USN  char    ch;

           send("\r\n\r\n\t======= Configuration Menu  ======\r\n");
           send("\t0. Display all configured values\r\n");
           send("\t1. Calibration of antenna span value\r\n");
           send("\t2. Set ADC span value\r\n");
           send("\t3. Set Dead Band and Skew value\r\n");
           send("\t4. Set antenna Parking and Limit position\r\n");
           send("\t5. Monitor ADC value & Antenna position\r\n");
           send("\t6. Direct antenna control\r\n");
           send("\t7. Radio CAT port configuration\r\n");
           send("\t8. Mic-click configuration\r\n");
           send("\t9. Set default parameter\r\n\r\n");

           for(;;){
               send("\r\t\tSelect: ");
               ch = getchar();
               if(ch <= '9' ) return (ch -'0');
               if((ch | 0x20) == ESC) return ESC;
           }
}

void   sys_config()		/*  System configuration */
{
       USN char	sel;
       
	for(;;){
       		sel = config_menu();
       		if((sel | 0x20) == ESC ) return;
                switch(sel){
                   case    0:		/* parameter list */
                       param_list();
                       break;
                   case    1:  	        /* Anntena cal.   */
                       calib_ant();
                       break;
                   case    2:           /* Set ADC span value */
                       value_ant();
                       break;
                   case    3:		/* Set dead band */
                       set_dbnd();
                       break;
                   case    4:		/* Set Parking value */
                       set_park();
                       break;
                   case    5:		/* ADC monitor */
                       adc_moni();
                       break;
                   case    6:		/* Direct antenna control */
                       point_ant();
                       break;
                   case    7:		/* Radio port */
		       radio_p_conf();
                       break;
                   case    8:		/* Mic click configuration */
               		 mic_step_con();
                       break;
	           case    9:		/* Clear RAM and set default value */
                       for(;;){
		                send("\x07\r\n\tStored data in memory will be lost. Are you sure ?");
				if((getchar() | 0x20) == 'y'){
			 		set_first(); 
				        send("\r\n\tConfigure your station elements ? [Y/N]  ");
					if((getchar() | 0x20) == 'y'){
						set_stn_inf();
						break;
					}
					else break;	
			 	}
				else break;
		       }
               }
	}
}

void radio_p_conf()	/* Radio CAT port configuration */
{

       USN char ch,tmp[20];
	int	lpt_baudval = 0;
	short   cs,temp;
	
	       send("\r\n\n\tCAT Rig KENWOOD,YAESU,ICOM [K/Y/I] ?  ");
               ch = getchar();

                   if((ch | 0x20) == 'k'){
		         rig_maker = KENWOOD;
                         send("\r\n\tIs the CAT RIG TS-711 & TS-811 combination ? [Y/N] ");
			 ch = getchar();
			 if((ch | 0x20) == 'y'){
			        send("\r\n\t  You must use alternate COM port for Uplink TRX");
			 	send("\r\n\t  Alternate COM port is 8031 P1.5/CN3-6 ");     
				SW2 |= _711811;
			 }
			 else SW2 &= ~_711811;
		   }

                   else if((ch | 0x20) == 'y'){ 
                         rig_maker = YAESU;
                         send("\r\n\tSelect TRX 1:FT-736  2:FT-847  3:Other  ");
                         switch(getchar()){
                                case '1':
                                        yaesu_tx_cmd = 0x2E;
                                        yaesu_rx_cmd = 0x1E;
                                        SW3 |= YAESU736 ;
					SW3 &= ~YAESU847;
					break;
                                case '2':
                                        yaesu_tx_cmd = 0x21;
                                        yaesu_rx_cmd = 0x11;
                                        SW3 |= YAESU847;
					SW3 &= ~YAESU736;
					break;
                                case '3':
                                        yaesu_tx_cmd = yaesu_rx_cmd = 0x01;
                         		SW3 &= ~YAESU736;
					SW3 &= ~YAESU847;
			 }
                        send("\r\n\tSelect FMN mode ? [Y/N] ");
                        ch = getchar();
                        if(( ch | 0x20 ) == 'y') SW |= YAESU_FMN;
                        else SW &= ~YAESU_FMN;
		   }
                   else if((ch | 0x20) == 'i'){
			 rig_maker = ICOM;
                         printf("\r\n\tEnter destination Address for RX (%BX) ",i_rx_address);
                         if(getline(tmp)) sscanf(tmp,"%hx",&i_rx_address);
 
                         printf("\tEnter destination Address for TX (%BX) ",i_tx_address);
                         if(getline(tmp)) sscanf(tmp,"%hx",&i_tx_address);

			 printf("\tSeparated TRX ? [Y/N]  ");
			 if((getchar() | 0x20 ) == 'y')	SW3 |= ICOM_SEPARATE;
			 else SW3 &= ~ICOM_SEPARATE;

			 printf("\r\n\tWait time for command [%d] ",i_wait);
			 if(temp=get_val(" [1-255] ",&cs)) i_wait =(UINT)temp;
                   }    /* ICOM end */

		lpt_baudval = get_val("\r\n\tPort baud rate ? [1200/2400/4800/9600/19200] ",&cs);

		switch(lpt_baudval){
			case	4800: lpt_baud_rate = 2;break;
			case    9600: lpt_baud_rate = 3;break;
			case   19200: lpt_baud_rate = 4;break;
			case	1200: lpt_baud_rate = 0;break;
			case	2400: lpt_baud_rate = 1;break;
		}
		
		TR0 = 0;
		TH0 = lpt_baud_tab[lpt_baud_rate];
		TR0 = 1;

	  send("\r\n\tUplink Doppler correction ? [Y/N]  ");
	  switch(getchar() | 0x20){
	  	case 'y': SW |= DOPPL_UPL;break;
		case 'n': SW &= ~DOPPL_UPL;
	  }
	  send("\r\n\tDownlink Doppler correction ? [Y/N/R]  ");
	  switch(getchar() | 0x20){
		case 'r': {SW |= DOPPL_DWN; SW2 |= DOP_ROUND_D;break;}
		case 'y': {SW |= DOPPL_DWN; SW2 &= ~DOP_ROUND_D;break;}
		case 'n': {SW &= ~DOPPL_DWN; SW2 &= ~DOP_ROUND_D;} 
	  } 
	  
	  send("\r\n\tSelect Full orbit or AOS CAT control ? [F/A]  ");
	  switch(getchar() | 0x20){
	  	case 'f': SW |= FULL_O_CAT;break;
		case 'a': SW &= ~FULL_O_CAT; 
	  }
}	

void display_1()		/* 5 satellite display */
{
data    USN char   i;

           printf("\r\n\r\n\r\n\t ========== Realtime Tracking [%4d/%02bd/%02bd] =========\r\n",tm_year,tm_mon,tm_day);
           send("\r\n  'E'Toggle rotor control  'P'Toggle priority track 'Q'Return to main menu\r\n\r\n");
           send("  HH:MM:SS    ");
	   for(i=0;i<m_track_num;i++){
	   	printf("%-13s",O_name[sat_list[i]]);
	   }
           for(i=m_track_num;i<SEL_MAX;i++){
	   	send("  -          "); 
	   }
    	   send("\r\n");
}


void display_2()                /* one satellite display */
{
           printf("\r\n\r\n\t ========== Realtime Tracking [%4d/%02bd/%02bd] =========\r\n",tm_year,tm_mon,tm_day);
           send("\r\n  'E' Toggle rotor control  'Y' RTC zero adjust  'Q' Return to main menu");
           send("\r\n  'I' Initialize the TRX    'T' Tune TRX   'SPC/BS'  Next/Previous. sat.");
           send("\r\n  '1/2/3/4' Uplink channel  '+/-/0' Doppler compensation  '?' obj. sat.");
           if(SW & MULTI_MODE)
           send("\r\n  'N' Track next satellite  'P' Toggle priority tracking\r\n");
           else send("\r\n");
           printf("  [Satellite: %-9s] \r\n",O_name[sat_list[obj_sat]]);
           printf("     UTC     Azim.   Elev.   Range      Alt.   Lat.   Long.  Phase   Doppler\r\n");
           printf("  HH:MM:SS   deg     deg      Km        Km     deg     deg   <256>      Hz\r\n"); 

}

void tracking1() 	/* multi satellite tracking */
{
data USN     int 	i;
	char    sel,sel1,lcd_act_sat,mode;
USN 	char	lcd_text[30],mark = NON_ST;
	float	lcdsat_az,lcdsat_el;
   	float	ctr_az[SEL_MAX],ctr_el[SEL_MAX];

	for(i=0;i<SEL_MAX;i++) ctr_az[i] = ctr_el[i] = '\0';
        sel = sel1 = 0;
  	SW2 |= PARK_TRX_FLAG;

        if(SW & RUN_MODE){     /* if standalone mode */
                sel1 = read_mode() - 1;       		/* Read select SW */
                if((sel1 < 0) || (sel1 >= m_track_num))	sel1 = 0;
	}
        else{		       /* terminal mode */
		printf("\r\n\r\n\t***  Select object satellite for LCD display (1 - %bd) ***\r\n",m_track_num);
           
		disp_list_sat();
                if(obj_sat == -1) return;
		if((sat_list[obj_sat] < 0) || (obj_sat >= m_track_num)){
	       		printf("\x07\r\n\tOut of number ! (1 - %d)  #1 was selected for LCD display\r\n",m_track_num);		
			obj_sat = 0;
		}
        	sel1 = obj_sat;
	}
	if(m_track_num > sat_mx) m_track_num = sat_mx;
        lcd_act_sat = sat_list[sel1];	/* copy LCD display satellite */

	reset_timeout();
	display_1();

	XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */

/***********************************/
/*   Tracking loop                 */
/***********************************/

	for(;;){  
	    if(SW & TRACK_ID){
		if(SW & PRIORITY_TRK) mark = PRIORITY_ST;
		else mark = MULTI_ST;
	    }
	    else mark = NON_ST;

	    i=0;
	    while(i<m_track_num){
		sel = sat_list[i];
		if(sel < 0 || sel > SATMAX) continue;
		sel_sat(sel);  		/* read elements */
		months();
		main_loop(sel);
		
		ctr_az[i] = A9; 	/* save the results */
		ctr_el[i] = E9;

		if(sel == lcd_act_sat){
			lcdsat_az = A9;
			lcdsat_el = E9;
		}
		
	        if((P1 & P14) == 0) disp_ant();
		else{
		        printf("%c %02bd:%02bd:%02bd ",mark,tm_hour,tm_min,tm_sec);/* UTC */
	  
	        	printf("%5.1f/%-6.1f%6.1f/%-6.1f%6.1f/%-6.1f%6.1f/%-6.1f",
		            ctr_az[0],ctr_el[0],ctr_az[1],ctr_el[1],ctr_az[2],ctr_el[2],ctr_az[3],ctr_el[3]);

		        printf("%6.1f/%-6.1f\r",ctr_az[4],ctr_el[4]);

			lcd_cmd(0x80);	       /* set DD RAM to line-1 top */

	               	sprintf(lcd_text,"%02bd:%02bd:%02bd%c%-7s",tm_hour,tm_min,tm_sec,mark,O_name[lcd_act_sat]);
        		print_lcd(lcd_text);
       			lcd_cmd(0xc0);            /* set DD RAM to line-2 top */
    	                sprintf(lcd_text,"AZ %3.1f EL%2.1f    ",lcdsat_az,lcdsat_el);
       			print_lcd(lcd_text); 
		}
		if(SW & RUN_MODE){  /* standalone mode */

			mode = read_mode();     /* sel sat is 1-15  Only */
			if(mode == 0){
				SW &= ~MULTI_MODE;
				return;
			}
                   	mode--;
			if(mode >= m_track_num){			       
				lcd_act_sat = sat_list[0];
			}
                   	else if(sat_list[mode] != lcd_act_sat){
                       		lcd_act_sat = sat_list[mode];
                       		break;     /*    sel sat & re-start */
			}
			
                        if((XBYTE[PPI+1]^ 0xff) & ANT_CONT){  /* check ant cont SW */
                                SW |= TRACK_ID;
                        }
                        else {
                                SW &= ~TRACK_ID;
                        }
		} /* if standalone mode end */
		
	        else if(RI){     /* ANY KEY IN from 232c port ? */
			switch(getchar() | 0x20){
				case ESC:
					XBYTE[LED_PORT] &= ~AOS_LED; /*AOS-LED OFF*/
		        	        return;
				case 'e':
        	                   	if(SW & TRACK_ID) SW &= ~TRACK_ID;
					else SW |=  TRACK_ID;
					break;
				case 'p':
					if(SW & PRIORITY_TRK){
						SW &= ~PRIORITY_TRK;
						mark = MULTI_ST;
					}
					else{
				        	SW |= PRIORITY_TRK;
		       				mark = PRIORITY_ST;
					}
					break;
	       		}
	    	}  /* any key in ? loop end */

		/************* go tracking2 ***************/

		if((E9 > p16_on_el) && (SW & TRACK_ID)){ 
			obj_sat = i;
               		send("\x07");			
			SW &= ~BREAK_M;
                	tracking2();
			if(SW & BREAK_M) return;
		}

                else if(E9 >= EL_BLINK){  	/* blink */
                	if(XBYTE[RTC_S1] & 0x01){   
                   		XBYTE[LED_PORT] |=  AOS_LED; /*AOS-LED ON */
                	}
                	else{                       
                   		XBYTE[LED_PORT] &= ~AOS_LED; /*AOS-LED OFF*/
                	}
		}		
		
		else{	/* E9 < EL_BLINK */
		 	XBYTE[LED_PORT] &= ~AOS_LED; /*AOS-LED OFF*/
			if((SW & ANT_PARK) && (SW & TRACK_ID)){
				go_parking(sel);
			}				
			else{
				XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
				reset_timeout();
			}
		}
 		if((SW2 & P16_ON_FLAG) && (ctr_el[obj_sat] < p16_off_el)){
	     		P1 |= P16;		/* pull up P1.6 */
	     		SW2 &= ~P16_ON_FLAG;
		}	     
	    i++;
	    } /* while loop end */

	}  /* end loop */
}

void tracking2()		/* one satellite tracking */
{
USN short   i;
    char    sel1,act_sat,mode,obj_sat_mem;
USN char    lcd_text[30], mark,no_mark = NON_ST,mark_ = TRACK_ST;
   
   if(SW & MULTI_MODE){
   	 XBYTE[RTC_CE] = 0x0a;      /* RTC interrupt D1 1min = 1010 */
	 if(SW & PRIORITY_TRK) mark_ = PRIORITY_ST;
	 else mark_ = MULTI_ST; 
   }
   else{
   	 mark_ = TRACK_ST;
   	 XBYTE[RTC_CE] |= 0x01;      /* no RTC interrupt */
   }

   XBYTE[LED_PORT] &=  ~AOS_LED;          /*AOS-LED OFF*/   

   for(;;){
	   if((SW & RUN_MODE) && ((SW & MULTI_MODE) == NO)){
	   	obj_sat = read_mode() -1;
		if(obj_sat < 0 || obj_sat > REG_MAX) continue;
	   }
newsat_found:

   	   SW2 |= (MODE_CMD_FLAG | MIC_CAT_FLAG | FLAG_STEP | PARK_TRX_FLAG);
	   SW2 &= ~(FLIP_ON | FLIP_CALC_FLAG);

 	   XBYTE[RTC_CD] &= 0xfb;      /* IRQ FLAG reset */
	   
	   obj_sat_mem  = obj_sat;	/* copy object satellite */
	   if(sat_list[obj_sat] < 0) continue;
           act_sat = sat_list[obj_sat];
           sel_sat(act_sat);            /* Read target elements */
	   trk_frq_correct = 0;		/* manual correction value 0 */

	   if(rig_maker == YAESU){
	   		yaesu_cmd(0x00,0x00);
			if(SW3 & YAESU736){  /* FT-736 only  */
				spout(0xC9,Y_WAIT);spout(0x60,Y_WAIT);spout(0x00,Y_WAIT);   /* 92-10-17 */
        	        	spout(0x00,Y_WAIT);spout(yaesu_rx_cmd,Y_WAIT);/* dummy 1296.0MHz */
				spout(0x05,Y_WAIT);spout(0x10,Y_WAIT);spout(0x00,Y_WAIT);   /* 92-10-17 */
                		spout(0x00,Y_WAIT);spout(yaesu_rx_cmd,Y_WAIT);/* dummy 51.0000MHz */
			}
			else if(SW3 & YAESU847)	yaesu_cmd(0x00,0x4E);	/* satellite ON */
	   		yaesu_cmd(0x00,0x80);
	   }
           trk_frq_d = RFD;		/* dummy */	   
           trk_frq_u = RFU;		/* dummy */

	   rig_mode_set(act_sat);
           
	   if(FREQ2 > 0.1) rig_set_u(act_sat);   
           if(FREQ1 > 0.1) rig_set_d(act_sat);	/* set downlink freq at first */
	   
           trk_frq_band = (long)TRK_FRQ;      /* default 20Hz step */
	   if(trx_mode[1] == FM) trk_frq_band = (long)TRK_FRQ_FM; /* 200Hz step  */

	   reset_timeout();
	   display_2();
	   XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
	   
/*******************************************************/
/*      Sub Tracking culculation Loop                  */
/*******************************************************/
           for(;;){			   /* Tracking loop          */
               
	       if(SW & TRACK_ID) mark = mark_; 
	       else mark = NON_ST;

               if(SW & RUN_MODE){        /* if standalone mode */
		   mode = read_mode();
		   if((SW & MULTI_MODE) == 0){  /* if not multi mode */
			if(mode == 0){		/* toggle the mode */
				SW |= MULTI_MODE;
				return;
			}
                   	mode--;
			if(mode != obj_sat){
                       		obj_sat = mode;
                       		break;           /* sel sat & start */
			}
		   }	
                   else if(mode == 0){           /* toggle the mode */
			SW &= ~MULTI_MODE;
			return;
		   }

                   if((XBYTE[PPI+1]^ 0xff) & ANT_CONT){ /* check cont SW */
                           SW |= TRACK_ID;
                           mark = mark_;
                   }
                   else {
                           SW &= ~TRACK_ID;
                           mark = no_mark;
                   }
               }     /* standalone mode end */

               else if(RI){     /* ANY KEY IN from 232c port ? */
		        switch(getchar() | 0x20){

			case '1':UPLINK_CH[act_sat] = 0;
				 FREQ2 = F02[act_sat][0];break;
			case '2':UPLINK_CH[act_sat] = 1;
				 FREQ2 = F02[act_sat][1];break;
			case '3':UPLINK_CH[act_sat] = 2;
				 FREQ2 = F02[act_sat][2];break;
			case '4':UPLINK_CH[act_sat] = 3;
				 FREQ2 = F02[act_sat][3];break;
#ifdef NOTDEF  			
			case 'h':
				XBYTE[PPI+2] &= 0xF0;	/* stop motor */
				SW |= HOST_FCONV;
				host_mode();
				SW &= ~HOST_FCONV;
			        reset_timeout();
				break;
#endif	
			case ' ':
				obj_sat++;
				if(obj_sat >= ld_sat) obj_sat--;
				break;

			case (BS | 0x20):
				obj_sat--;
				if(obj_sat < 0) obj_sat++;
				break;
				
			case 'q': 	/* return  */
		                XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
				XBYTE[LED_PORT] &= ~AOS_LED; /* off AOS-LED */ 
                                SW2 &= ~FLIP_ON; 
				SW |= BREAK_M;
		           	return;     
			case 'n': /* next sat in multi track */
				if(SW & MULTI_MODE){
		       			XBYTE[LED_PORT] &= ~AOS_LED; /* off AOS-LED */ 
					display_1();
					return;
				}
				break;

			case 'p': /* toggle priority tracking */
				if(SW & MULTI_MODE){
					if(SW & PRIORITY_TRK){
						SW &= ~PRIORITY_TRK;
						mark_ = MULTI_ST;
					}
					else{
						SW |= PRIORITY_TRK;
						mark_ = PRIORITY_ST;
					}
				}
				break;
			case 'e': /* track ON/OFF */
                           	if((SW & TRACK_ID) == NO){
                               		SW |= TRACK_ID;
                               		mark = mark_;
				}
                           	else {
                               		SW &= ~TRACK_ID;
                               		mark = no_mark;
				}
				break;
			case 'y': /* RTC zero adjust */
				reset_timeout();
		       		XBYTE[RTC_CD] |= 0x08;
				break;
				
			case '+': /* Doppler compensation + */
		       		trk_frq_correct += TRK_CORREC;
				break;
				
			case '-': /*   -    */
		       		trk_frq_correct -= TRK_CORREC;
				break;
				
			case '0': /*   to zero shift */
		       		trk_frq_correct  = 0;
				break;		       

			case '?': /*  return sat name.  */
				printf("\r\n%s\r\n",O_name[sat_list[obj_sat]]);
				break;
			case 't': /* Tune the cat TRX */
		   		rig_set_u(act_sat);		
		       		rig_set_d(act_sat);		
				break;
			case 'i': /* Init the CAT TRX */
				rig_mode_set(act_sat);
		   		rig_set_u(act_sat);		
		       		rig_set_d(act_sat);		
				break;   
			}    /* switch end */
			
               }    /* Any key in ? loop end */
	     if (obj_sat_mem != obj_sat) goto newsat_found; /* 96-08-04 */

             months();
             main_loop(act_sat);        /*  Culculation sat   */ 
             
	     AZ_setv = A9;
             EL_setv = E9; 

             rotor_con1(act_sat);        /* ROTOR  CONTROL */

	     if((P1 & P14) == 0) disp_ant();
	     else{ 
		     printf("%c %02bd:%02bd:%02bd",
		                     mark,tm_hour,tm_min,tm_sec);/* UTC */
        
	     	     printf("%8.2f%8.2f%9.1f%9.1f%7.1f%8.1f%7.1f%+6.0f",
		                     A9,E9,R5,R-R0,L5,W5,M9,F9D); 
		     printf("/%-5.0f \r",(float)trk_frq_correct); 


	   	     lcd_cmd(0x80);	       /* set DD RAM to line-1 top */
        	     sprintf(lcd_text,"%02bd:%02bd:%02bd%c%-5s  ",
		                       tm_hour,tm_min,tm_sec,mark,O_name[sat_list[obj_sat]]);
	    	     print_lcd(lcd_text);

             	     lcd_cmd(0xc0);            /* set DD RAM to line-2 top */
	     
	             if(SAT_FLAG_SW[sat_list[obj_sat]] & MA_MARK) 
		     	  sprintf(lcd_text,"A%3.0f E%2.0f P%3.1f    ",A9,E9,M9);
        	     else sprintf(lcd_text,"AZ %3.1f EL%2.1f     ",A9,E9);
             	     print_lcd(lcd_text); 
	     }

	     if(E9 >= p16_on_el){
	     		P1 &= ~P16; 	/* P16 pull down => ON state */
	     		SW2 |= P16_ON_FLAG;
        
   			if((SW2 & FLIP_MODE) && (SW & TRACK_ID) && 
			   ((SW2 & FLIP_CALC_FLAG) == 0)) flip_check(act_sat);
	     }
 	     else if(SW & MULTI_MODE){ 
			XBYTE[PPI+2] &= 0xF0;  /* stop rotator */
        		display_1();
			send("\x07");			
			SW2 |= PARK_TRX_FLAG;
			SW2 &= ~FLIP_ON;
			return; /*  if multi mode && LOS return to tracking1() */
	     }

	     else if((SW2 & P16_ON_FLAG) && (E9 <= p16_off_el)){
	     		P1 |= P16;		/* pull up P1.6 */
	   		trk_frq_correct = 0;
			SW2 &= ~(P16_ON_FLAG | FLIP_CALC_FLAG | FLIP_ON);
	     }

	     if((SW2 & MODE_CMD_FLAG) && (EL_setv > 0.0)){ 
			rig_mode_set(act_sat);  	/* mode cmd again */
		       	rig_set_u(act_sat);
			SW2 &= ~MODE_CMD_FLAG;
	     }		   	 

             if(SW & MIC_CLICK) f_step_con(act_sat);

	     else if((SW & FULL_O_CAT) || (EL_setv > EL_BLINK)){     
		     		/* Full orbit CAT control or AOS */

                   if((labs(trk_frq_u - RFU) > 300l) && (SW & DOPPL_UPL)){ 
	               	trk_frq_u = RFU;     /* uplink freq reffrence 300Hz */
		       	rig_set_u(act_sat);
		   }

		   if((labs(trk_frq_d - RFD) > trk_frq_band) && (SW & DOPPL_DWN)){
                	trk_frq_d = RFD;	/* for next loop refference */
		   	rig_set_d(act_sat);
                   }
	     }

	/* Hardware doppler compensation */

	     if((XBYTE[PPI+1] & PLUS_50) == 0){
			if((XBYTE[PPI+1] & MODEM_LOCK) == 0)
				CORRECT_MIC[act_sat] += 50; 
	     		else if((XBYTE[PPI+1] & MINUS_50) == 0) trk_frq_correct = 0;
		       	else trk_frq_correct += 50;
	     }			
	     else if((XBYTE[PPI+1] & MINUS_50) == 0){
	     		if((XBYTE[PPI+1] & MODEM_LOCK) == 0) 
				CORRECT_MIC[act_sat] -= 50; 
		       	else trk_frq_correct -= 50;
	     }	     

	     if(((DX - mem_DX_az) > (long)err_timeout) ||
		  ((DX - mem_DX_el) > (long)err_timeout)) rotor_error();

             if(SW2 & STEP_PORLAR) XBYTE[PPI+2] |= ~PULSE_END;
	     else XBYTE[PPI+2] &= PULSE_END;  /* Mic pulse end */	    

	/***** Priority tracking  *****/

        /* check of 60 sec interrupt flag of RTC */

	     if((XBYTE[RTC_CD] & 0x04) && (SW & PRIORITY_TRK) && (SW & MULTI_MODE)){
		XBYTE[RTC_CD] &= 0xfb; /* IRQ FLAG D2 reset */
		for(i=0;i<m_track_num;i++){
			sel1 = sat_list[i];
			if(sel1 < 0 || sel1 > SATMAX) continue;
			sel_sat(sel1);  	/* read elements */
			months();
			main_loop(sel1);	/* calculation */
			if(E9 > -2.0){
				if(i < obj_sat){  
					obj_sat = i;
	   				printf("\x07\r\n\r\n  [Prior Satellite %-9s is AOS]\r\n",
						O_name[sat_list[obj_sat]]);
					goto newsat_found;
				}
				else if((i > obj_sat) && (EL_setv < 0) &&
					((SW2 & MODE_CMD_FLAG) == 0)){
					obj_sat = i;
					goto newsat_found;
				}
				else break;
			}
		} /* priority for loop end */
		sel_sat(act_sat);
	     }	
             rotor_con1(act_sat);           /*  rotor control again */

	  }  /* sub tracking loop end */
   }
}

void flip_check(act_sat)
char  act_sat;
{
        float AZ_flip = A9;

	XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */

        for(;;){
             if(N0 >= 10.0) T += 0.0013888888;  /* 120 sec interval */     
	     else T += 0.01388888;   /*  20 min interval */
                                  
             main_loop(act_sat);        /*  Culculation sat   */ 
       	     lcd_cmd(0xc0);            /* set DD RAM to line-2 top */
   	     print_lcd("QRX Flip check..");

             if((SW & ROTOR_ZERO) == 0){        /* South start */
                        if(((AZ_flip > 90) && (AZ_flip < 180) && (A9 > 180)) ||
                           ((AZ_flip < 270) && (AZ_flip > 180) && (A9 < 180))){
                                SW2 |= FLIP_ON;
                                break;
                        }
             }
	     else		/* North Start */
                        if(((AZ_flip > 270) && (AZ_flip < 360) && (A9 < 90)) ||
                           ((AZ_flip < 90) && (AZ_flip > 0) && (A9 >270))){
                                SW2 |= FLIP_ON;
                                break;
	     } 

             if(E9 < p16_on_el){             
                        SW2 &= ~FLIP_ON;
                        break;
             }
             AZ_flip = A9;
        }       /* for loop end */
	
        SW2 |= FLIP_CALC_FLAG;     /* set flip calc done flag */
        months();
        main_loop(act_sat);        /*  Culculation sat   */ 
}

void rotor_error()
{
USN     int	tm,ts;
		
		if(SW2 & ERR_MODE_OFF) return;
		
	     	XBYTE[PPI+2] &= 0xF0;	/* stop motor */
		printf("\x07\r\n\tRotator Error at %02bd:%02bd:%02bd\r\n\tCheck and power on reset for system recovery.\r\n",tm_hour,tm_min,tm_sec);
		lcd_cmd(0x80);
		print_lcd("Rotator Error ! ");lcd_cmd(0xc0);
		print_lcd("Check the system");
		tm = tm_min;
		ts = tm_sec;
		
	     for(;;){
                if(XBYTE[RTC_S1] & 0x01) XBYTE[LED_PORT] |=  AOS_LED; 
                else      XBYTE[LED_PORT] &=  ~AOS_LED; /*AOS-LED OFF*/

		months();
/*		printf("\t%02bd:%02bd:%02bd\r",tm_hour,tm_min,tm_sec);*/
		
		if((int)(tm_min - tm) < 0) tm_min += 60;
		if((tm_sec == ts) && (tm_min - tm) == (UINT)err_recover_tm){
			printf("\x07\tExit from Error mode at %02bd:%02bd:%02bd\r\n\n",tm_hour,tm_min,tm_sec);
			break;
		}
             }
	     reset_timeout();
}

void  f_step_con(act_sat)              /*  Doppler RX freq up down control */
char act_sat;
{       		        /*  92-02-20 */

	short   now_pll;
	short	pll_range = (pll_reff - pll_lock);
USN     short   j,pulse_width = pulse_w * 1000;

#ifdef NOTDEF        
	if(XBYTE[PPI+1] & MODEM_LOCK){   	       /* not locking */
		if(labs(trk_frq_d - RFD) > trk_frq_band){
                	trk_frq_d = RFD;	/* for next loop refference */
		   	rig_set_d(act_sat);
		}
		return;
	}
#endif

	if(EL_setv < 0.0) return; /* check AOS */

        read_adc(PLL_ADC);
        now_pll = (adc_data - pll_lock);
	
        if(abs(now_pll) < abs(pll_range)) return;
        else{
		if(pll_range < 0){	/* positive S-curve */
                	if(now_pll > 0){
				if(SW2 & STEP_PORLAR) XBYTE[PPI+2] &= ~DWN_PULSE;
				else XBYTE[PPI+2] |= DWN_PULSE;    /* Down Pulse start  */
        		}
			else{
				if(SW2 & STEP_PORLAR) XBYTE[PPI+2] &= ~UP_PULSE;
				else XBYTE[PPI+2] |= UP_PULSE;     /* Up Pulse start  */
       			}
		}
		else{			/* Negative S-curve */
                	if(now_pll > 0){
				if(SW2 & STEP_PORLAR) XBYTE[PPI+2] &= ~UP_PULSE;
				else XBYTE[PPI+2] |= UP_PULSE;     /* Up Pulse start  */
        		}
			else{
				if(SW2 & STEP_PORLAR) XBYTE[PPI+2] &= ~DWN_PULSE;
				else XBYTE[PPI+2] |= DWN_PULSE;    /* Down Pulse start  */
       			}
		}
                for(j=0;j<pulse_width;j++) continue;  /* pulse width */
	}
}

void step_ini()             /* Auto step pulse generate at AOS */
{                               /* 92-12-03 */
	USN  short	i,j,pulse_width,pulse_int;
             long       f_diff;
	USN  long       pulse_n;
	
	pulse_width = pulse_w * 1000;
	pulse_int   = pulse_i * 1000;
        f_diff = RFD - fdx_los;

        pulse_n = labs(f_diff/(long)step_size);   /* pulse number */
        printf("\r\nf_los/f_aos/f_diff/pulse:%ld/%ld/%ld/%ld\r\n",fdx_los,RFD,f_diff,pulse_n);

        if(f_diff > 0)      send("UP pulse:");
        else if(f_diff < 0) send("DWN pulse:");
                        
        for(i=0;i< pulse_n;i++){
            if(f_diff > 0){
                if(SW2 & STEP_PORLAR) XBYTE[PPI+2] &= ~UP_PULSE;
                else    XBYTE[PPI+2] |= UP_PULSE;
            }
            else{
                if(SW2 & STEP_PORLAR) XBYTE[PPI+2] &= ~DWN_PULSE;
                else XBYTE[PPI+2] |= DWN_PULSE;
            }
                send("*");
                for(j=0;j<pulse_width;j++) continue;  /* pulse width */

            if(f_diff > 0){
                if(SW2 & STEP_PORLAR) XBYTE[PPI+2] |= UP_PULSE; 
                else    XBYTE[PPI+2] &= ~UP_PULSE;
            }
            else{
                if(SW2 & STEP_PORLAR) XBYTE[PPI+2] |= DWN_PULSE;
                else    XBYTE[PPI+2] &= ~DWN_PULSE;
            }
                send("_");
                for(j=0;j<pulse_int;j++) continue;  /* pulse interval */
        }
        
        send("\r\n");
        SW2 &= ~FLAG_STEP;      /* reset flag, no need again */
}

void   yaesu_cmd(d1,p1) small
USN char d1,p1;
{
                spout(d1,Y_WAIT);
                spout(0x00,Y_WAIT);
                spout(0x00,Y_WAIT);
                spout(0x00,Y_WAIT);
                spout(p1,Y_WAIT);
}

void icom_cat_header(ch)
USN     char    ch;	/* destination address */
{
                spout(0xfe,0);spout(0xfe,0);
                spout(ch,0);spout(0x02,0);
}

void   rig_set_d(sel)          /*  Downlink Doppler correction by CAT  */
char   sel;
{
   char         i;
   USN char     ch,chl,chh;
   USN long     fl;
   USN char     s_txt[40];

        if(rig_maker == KENWOOD) {   /* KENWOOD VFO-B for receive */
                sprintf(s_txt,"FB%011lu;\0",RFD);       /* VFO-B M hz */
                if((SW2 & _711811) && (SAT_FLAG_SW[sel] & KEN_REVS_PORT)){
                        sendp2(s_txt);
                }
                else    sendp(s_txt);     /* Out text to CAT PORT */
        }

        else if(rig_maker == YAESU){  /* YAESU   */
                yaesu_cmd(0x00,0x00);
                fl = RFD/10;
                sprintf(s_txt,"%09Ld\0",fl);
		chl = chh = 0x00;
                if((s_txt[0] == '1') && (s_txt[1] =='2')){
                        chl = (s_txt[2]-0x30) & 0x0f;
                        ch = 0xC0 | chl;        /* 1200Mhz */
                        spout(ch,Y_WAIT);
                }
                else if(s_txt[0] == '0'){
                        chh = (s_txt[1]-0x30) & 0x0f;
                        chl = (s_txt[2]-0x30) & 0x0f;
                        ch = ((chh << 4) & 0xf0) | chl;
                        spout(ch,Y_WAIT);
                }
                for(i=3;i<8;){
                        chl = chh = 0x00;
                        chh = (s_txt[i]-0x30) & 0x0f; /* to bin */
                        chl = (s_txt[i+1]-0x30) & 0x0f; /* to bin */
                        ch = ((chh << 4) & 0xf0) | chl; /* Fre set char */
                        spout(ch,Y_WAIT);
                        i +=2;
                }
                spout(yaesu_rx_cmd,Y_WAIT);                   /* FULLDUP RX  Freq  */
                yaesu_cmd(0x00,0x80);
        }

        else if(rig_maker == ICOM){  /*  ICOM  */
                fl = RFD; 
                sprintf(s_txt,"%010lu\0",fl);

	   if(!(SW3 & ICOM_SEPARATE)){
		icom_cat_header(icom_r);
        	spout(0x07,0);
	/*	spout(((SW3 & ICOM_SEPARATE)? 0x00:0xD1),0);  Sub band access  */
		spout(0xD1,0); /* Sub band access  */
                spout(0xfd,0);
		_icom_wait(i_wait);
	   }
                icom_cat_header(icom_r);
                spout(0x00,0);    /*  freq. set command    */
                for(i=9;i>0;){   /*  send freq figure loop */
                        chl =chh = '\0';
                        chl = (s_txt[i]-0x30) & 0x0f;   /* to bin */
                        chh = (s_txt[i-1]-0x30) & 0x0f; /* to bin */
                        ch = ((chh << 4) & 0xf0) | chl; 
                        spout(ch,0);
                        i -=2;
                }
                spout(0xfd,0);    /*   CI-V tail char     */
		_icom_wait(i_wait);
        }
}

void   rig_set_u(sel)          /*  Doppler correction for uplink by CAT  */
char   sel;
{                           /*  Mar. 28 92  */
   char         i;
   USN char     ch,chl,chh,s_txt[40];
   USN long     fl;

        if(rig_maker == KENWOOD) {   /* KENWOOD VFO-A for receive */
           sprintf(s_txt,"FA%011lu;\0",RFU);       /* VFO-A M hz */

           if(SW2 & _711811){
                if(SAT_FLAG_SW[sel] & KEN_REVS_PORT){
                        sendp("DC0;");
                        sendp(s_txt);
                }
                else{
                        sendp2("DC0;");       /* Main VFO select VFO-A for transmit */
                        sendp2(s_txt);
                }
           }
           else {
                sendp("DC0;");       /* Main VFO select VFO-A for transmit */
                sendp(s_txt);                       /* Out text to CAT PORT */
                sendp("DC1;");       /* return to sub VFO */
           }
        }

        else if(rig_maker == YAESU){  /* YAESU   */
                yaesu_cmd(0x00,0x00);
                fl = RFU/10;
                sprintf(s_txt,"%09Ld\0",fl);
		chl = chh = 0x00;
                if((s_txt[0] == '1') && (s_txt[1] =='2')){
                        chl = (s_txt[2]-0x30) & 0x0f;
                        ch = 0xC0 | chl;        /* 1200Mhz */
                        spout(ch,Y_WAIT);
                }
                else if(s_txt[0] == '0'){
                        chh = (s_txt[1]-0x30) & 0x0f;
                        chl = (s_txt[2]-0x30) & 0x0f;
                        ch = ((chh << 4) & 0xf0) | chl;
                        spout(ch,Y_WAIT);
                }
                for(i=3;i<8;){
                        chl = chh = 0x00;
                        chh = (s_txt[i]-0x30) & 0x0f; /* to bin */
                        chl = (s_txt[i+1]-0x30) & 0x0f; /* to bin */
                        ch = ((chh << 4) & 0xf0) | chl; /* Fre set char */
                        spout(ch,Y_WAIT);
                        i +=2;
                }
                spout(yaesu_tx_cmd,Y_WAIT);                   /* FULLDUP RX  Freq  */
                yaesu_cmd(0x00,0x80);
        }

        else if(rig_maker == ICOM){  /*  ICOM  */
                        fl = RFU; 
                        sprintf(s_txt,"%010lu\0",fl);

		if(!(SW3 & ICOM_SEPARATE)){
                        icom_cat_header(icom_t);
                        spout(0x07,0);
			spout(0xD0,0);  /* main band  */
                        spout(0xfd,0);
			_icom_wait(i_wait);
		}			
                        icom_cat_header(icom_t);
                        spout(0x00,0);              /* Set freq command */
                        for(i=9;i>0;){
                                chl = chh = '\0';
                                chl =(s_txt[i]-0x30) & 0x0f;
                                chh =(s_txt[i-1]-0x30) & 0x0f;
                                ch  =((chh << 4) & 0xf0) | chl;
                                spout(ch,0);
                                i -=2;
                        }
                        spout(0xfd,0);
        		_icom_wait(i_wait);
	}
}

void rig_mode_set(sel)     /* TRX mode set for up/dwn  */
USN char   sel;
{
        USN char    s_txt2[40],s_txt3[40];
              
        if(rig_maker == KENWOOD){
                sprintf(s_txt2,"MD%bd;\0",trx_mode[0]);      /* uplink mode */
                sprintf(s_txt3,"MD%bd;\0",trx_mode[1]);     /* dwn link mode */
                
                if(SW2 & _711811){   /* if 711/811 */
                        if(SAT_FLAG_SW[sel] & KEN_REVS_PORT){
                                sendp("DC0;");  /* Main VFO select */
                                sendp("FN0;");  /* VFO-A select */
                                sendp(s_txt2);
                                sendp2("DC1;");  /* Sub VFO select  */
                                sendp2("FN1;");   /* VFO-B select */
                                sendp2(s_txt3);
                        }
                        else{
                                sendp2("DC0;");  /* Main VFO select */
                                sendp2("FN0;");  /* VFO-A select */
                                sendp2(s_txt2);
                                sendp("DC1;");  /* Sub VFO select  */
                                sendp("FN1;");   /* VFO-B select */
                                sendp(s_txt3);
                        }
                }
                else {
                        sendp("DC0;");  /* Main VFO select */
                        sendp("FN0;");  /* VFO-A select */
                        sendp(s_txt2);
                        sendp("DC1;");  /* Sub VFO select  */
                        sendp("FN1;");   /* VFO-B select */
                        sendp(s_txt3);
                }
        }

        else if(rig_maker == YAESU){
              yaesu_cmd(0x00,0x00);
              if(trx_mode[0] != 0){                /* FULLDUP TX mode  */ 
			switch(trx_mode[0]){
				case LSB: d1 = 0x00;break;
				case USB: d1 = 0x01;break;
				case  FM: 
				default : d1 = 0x08;
			}
	      yaesu_cmd(d1,0x27);
              }
	      switch(trx_mode[1]){		/*  RX mode */
	      		case LSB: d1 = 0x00;break;
			case USB: d1 = 0x01;break;
			case  FM:
			default :
				if(SW & YAESU_FMN) d1 = 0x88;
				else               d1 = 0x08;
	      }
	      yaesu_cmd(d1,0x17);
              yaesu_cmd(0x00,0x80);
        }

        else if(rig_maker == ICOM){

		if(!(SW3 & ICOM_SEPARATE)){
                        icom_cat_header(icom_t);
                        spout(0x07,0);
			spout(0xD0,0); /* main band access */
                        spout(0xfd,0);
			_icom_wait(i_wait);
		}			
                        icom_cat_header(icom_t);
                        spout(0x01,0);   /* Set mode command */
			switch(trx_mode[0]){
				case LSB: spout(0x00,0);break;
				case USB: spout(0x01,0);break;
				case  CW: spout(0x03,0);break;
				case  FM: spout(0x05,0);break;
			}
                        spout(0xfd,0);
			_icom_wait(i_wait);
		
		if(!(SW3 & ICOM_SEPARATE)){
                        icom_cat_header(icom_r);
                        spout(0x07,0);
			spout(0xD1,0); /* Sub band access */
                        spout(0xfd,0);
			_icom_wait(i_wait);
		}	
                        icom_cat_header(icom_r);
                        spout(0x01,0);   /* Set mode command */
			switch(trx_mode[1]){
				case LSB: spout(0x00,0);break;
				case USB: spout(0x01,0);break;
				case  CW: spout(0x03,0);break;
				case  FM: spout(0x05,0);break;
			}
                        spout(0xfd,0);
			_icom_wait(i_wait);
        }
}       

void   months() small
{
USN	int	n,i;

       read_tm();    

       tm_year = (time_ch[0]-0x30) *10 + (time_ch[1]-0x30)+1900;
       if(time_ch[0] != 0x39) tm_year += 100;
       
       tm_mon  = (time_ch[2]-0x30) *10 + (time_ch[3]-0x30);
       tm_day  = (time_ch[4]-0x30) *10 + (time_ch[5]-0x30);
       tm_hour = (time_ch[6]-0x30) *10 + (time_ch[7]-0x30);
       tm_min  = (time_ch[8]-0x30) *10 + (time_ch[9]-0x30);
       tm_sec  = (time_ch[10]-0x30) *10 + (time_ch[11]-0x30);

       DA = day_of_year(tm_year,tm_mon,tm_day);
       DX = DA*86400l+(long)(tm_hour*3600l)+(long)(tm_min*60l)+(long)tm_sec;
       T = DX/86400.0;

       if(n = (tm_year - Y2)){
		for(i=1;i<=n;i++){
			if((tm_year-1)%4 == 0) T += 366.0;
			else T += 365.0;
			if(i>5) break;	/* 98-12-15 */
       		}
       }	
}

USN int  day_of_year(year,month,day)
USN short	year;
USN char	month,day;
{
	USN char i,leep;
	USN int	day_x = day;
	
	leep = year%4 == 0; /*  && year%100 != 0 || year%400 == 0; */
	for(i=1;i<month;i++)
		day_x += day_tab[leep][i];
	return day_x;	      /* 1992/01/06 */
}

void   sel_sat(sel)            /* read object elements for selected sat. */
char  sel;
{
int	g2_sel;

      if(sel < 0) return;
      Y2 = Y02[sel]+1900;	
      if(Y02[sel] < 90) Y2 += 100;	/* 96-11-15 */
      g2_sel = Y2 - 1991;
      if((g2_sel<0) || (g2_sel>19)) g2_sel = 19;      /* 96-12-01 */
      
      TX0 =T02[sel];
      K0 = K02[sel];
      M0 = M02[sel];
      N0 = N02[sel];
      I0 = I02[sel];
      E0 = E02[sel];
      W0 = W02[sel];
      O0 = O02[sel];
      NX0 =  N12[sel];

      FREQ1 = F01[sel];
      FREQ2 = F02[sel][UPLINK_CH[sel]];

      if(fd0_los == 0L) fdx_los =(long)(FREQ1*1000000L);

      trx_mode[0] = MDU[sel];
      trx_mode[1] = MDD[sel];

      if(SAT_FLAG_SW[sel] & DPLR_MODE) SW |= MIC_CLICK;
      else SW &= ~MIC_CLICK;
      
      if(SAT_FLAG_SW[sel] & ICOM_R_ADDR){	/* ICOM reverse addressing */
      		icom_r = i_tx_address;
		icom_t = i_rx_address;
      }
      else{
      		icom_r = i_rx_address;
		icom_t = i_tx_address;
      }

      RFD =(long)(FREQ1*1000000l); /*  for initial set for downlink freq.  */
      RFD += (long) (F0CONV[sel]*1000000l);
      RFU =(long)(FREQ2*1000000l); /*  for initial set for uplink freq. */
      RFU += (long) (F1CONV[sel]*1000000l);

       G2 = G2_tbl[g2_sel];	/* G2 selection */

       S1 = sin(I0*P0);
       C1 = cos(I0*P0);

       L8 = local_inf[0]*P0;
       S9 = sin(L8);
       C9 = cos(L8);
       S8 = sin(-1.0*local_inf[2]*P0);
       C8 = cos(local_inf[2]*P0);

       R9 = R0*(1-F2+F2*cos(2*L8))+local_inf[3]/1000.0;
       L8 = atan(0.993305458*S9/C9);
       Z9 = R9 * sin(L8);
       X9 = R9 * cos(L8)*C8;
       Y9 = R9 * cos(L8)*S8;

       VX9 = -R9*cos(L8)*S8*0.0000729211585; /*  JR1HAL  */
       VY9 =  R9*cos(L8)*C8*0.0000729211585; /* Velocity vector of observer */

}

/**********************************************/
/*   main calculation routine                 */
/**********************************************/

void   main_loop(sat_sel)
char  sat_sel;
{
       float  N,V0,VX,VY,VZ,X1,Y1,VX1,VY1,VZ1;
       float  M5,C0,S0,E2,K2,C2,S2;
       float  K,R8,TXG,Q,Q0,VX0,VY0;
       long   F9DT1,F9DT2,F9DT3;

       TXG = T-TX0;    /* step */

/*       A0 = pow( (G0/(N0*N0)) ,0.3333333333);	*/

       N = N0 + 2*TXG*NX0;       	  /* Mean Motion at T */
       A =  pow((G0/(N*N)) ,0.333333333); /* SMA at Epoch T */
       V0 = A*N*0.00007272205217;	  /* Mean velocity of satellite jr1hal */

       E2 = 1.0- E0*E0;
       E1 = sqrt(E2);
       Q0 = M0/360.0 +K0;

       K2 = 9.95*pow(R0/A,3.5)/(E2*E2);      

       O = O0-TXG*K2*C1;
       S0 = sin(O*P0);
       C0 = cos(O*P0);
                                              
       W =W0+TXG*K2*(2.5*(C1*C1)-0.5);
       S2 = sin(W*P0);    
       C2 = cos(W*P0);

       C[6] = (C2*C0)-(S2*S0*C1);
       C[10] = -1.0 *(S2*C0)-(C2*S0*C1);
       C[7] = (C2*S0)+(S2*C0*C1);
       C[11] = -1.0 *(S2*S0)+(C2*C0*C1);
       C[8] = (S2*S1);
       C[12] = (C2*S1);

       Q = Q0 + N0*TXG + NX0*(pow(TXG,2.0));    /* rev. to include drug */     

       K = floor(Q+0.000001);                   /* Orbit # */
       M9 =(Q-K+0.000001)*256;                  /* Phase */
       M  = (Q-K)*P2;

       E = M + E0 * sin(M) + 0.5*(E0*E0)*sin(2*M);
/*
       lp = 0;
*/
       for(;;){                    /* Kepler */
           S3 = sin(E);
           C3 = cos(E);
           R3 = 1.0 - E0*C3;
           M1 = E- E0*S3;
           M5 = M1 - M;

           if(fabs(M5) < 1e-6) break;
           E = E - M5/R3;
/*
           if(lp++ > 30) {
               sat_az = 0.0;
               sat_el = 0.0;  
               sat_ph = 0.0;
               return ;
           }
*/
       }
                                          
       X0 = A *(C3-E0);	        /* get satellite's XYZ coordinates */
       Y0 = A*E1*S3;
       R  = A*R3;
       
	VX0 = -V0*S3/R3;		/*  JR1HAL  */
	VY0 =  V0*E1*C3/R3;     /* Velocity on XY plane (Km/sec) */          

       X1 = X0 * C[6]+Y0*C[10]; /* Rotate frm orbit plane to inertial   */ 
       Y1 = X0 * C[7]+Y0*C[11]; /* celestial coordinates.               */
       Z1 = X0 * C[8]+Y0*C[12]; 
       
       VX1 = VX0*C[6] + VY0*C[10];	/* JR1HAL */
       VY1 = VX0*C[7] + VY0*C[11];      /* Velocity vector */
       VZ1 = VX0*C[8] + VY0*C[12];		
       
       G7 = T*G1+G2;                  /* Convert to Geocentric coordinates. */
       G7 = (G7 - floor(G7))*P2;
       S7 =  -1.0*sin(G7);
       C7 = cos(G7);

       X  = (X1*C7)-(Y1*S7);
       Y  = (X1*S7)+(Y1*C7);
       Z  = Z1;

	VX = (VX1*C7)-(VY1*S7);		/* JR1HAL */
	VY = (VX1*S7)+(VY1*C7);         /* Velocity vector of satellite */
	VZ = VZ1;

       
       X5 = (X- X9);            /* get vector from observer to satellite  */
       Y5 = (Y- Y9);
       Z5 = (Z- Z9);
       R5 = sqrt(X5*X5+Y5*Y5+Z5*Z5);

       Z8 = (X5*C8*C9) + (Y5*S8*C9)+(Z5*S9);
       X8 = -1.0*(X5*C8*S9)-(Y5*S8*S9)+(Z5*C9);
       Y8 = (Y5*C8)-(X5*S8);
       S5 = Z8/R5;
       C5 = sqrt(1.0 - S5*S5);
       E9 = (atan(S5/C5))/P0;  /* Elevation */
       A9 = (atan(Y8/X8))/P0;
       B5 = Z/R;

       L5 = (atan(B5/(sqrt(1.0-B5*B5))))*57.30; /* SSP Latitude */
       W5 = (atan(Y/X))*57.30;

       if(X <0 ) W5 = 180.0-W5;	/* resolves correct quadrant for W5=SSP_Lon */
       else if(X>0 && Y < 0) W5 = -1.0* W5;
       else if(X>0 && Y > 0) W5 = 360.0- W5;
       else if(X==0 && Y >= 0) W5 = 270.0;
       else if(X==0 && Y < 0) W5 = 90.0;
       
       if(W5 < 180) W5 = -1.0*W5;	/* East Long. => + */
       else if(W5 >= 180) W5 = 360.0 - W5;
       
       if(X8 < 0 ) A9 = A9+180.0;       
       else if(X8 >0 && Y8 < 0) A9 = 360.0 + A9;
       else if(X8 == 0 && Y8 >= 0 ) A9 = 90.0;
       else if(X8 == 0 && Y8 < 0 )  A9 = 270.0;

					/* relative velocity from observer */
	R8 = ((VX-VX9)*X5 + (VY-VY9)*Y5 + VZ*Z5)/R5;	/* JR1HAL */

           F9D = -1*FREQ1* R8*3.33564;	/* Doppler shift (Hz) */       

	   if(SW2 & DOP_ROUND_D){

		F9DT1 = F9D/10000L; 
		F9DT2 = (long)F9D%10000L; 
		F9DT3 = F9DT1 * 10000L;
		
		if(F9DT2 < 2500L) F9D = F9DT3;
		else if(F9DT2 >= 7500) F9D = F9DT3 + 10000L;
		else F9D = F9DT3 + 5000L;
		if(R8 > 0) F9D *= -1;
	   }
	   
	   RFD =(long)(FREQ1*1000000l) + (long)F9D + trk_frq_correct;	
	   				/* Real dwnlink freq.(Hz) */

	   RFD += (long)(F0CONV[sat_sel]*1000000l);
	   RFD += (long)(CORRECT_MIC[sat_sel]);	/* Slew : new in v3.10bb */

           F9U = -1*FREQ2* R8*3.33564;                

	   RFU =(long)(FREQ2*1000000l)-(long)F9U; /* Real uplink freq.(Hz) */
	   RFU += (long)(F1CONV[sat_sel]*1000000l);
       
       if(ceil(T) != ceil(DA)){
               D += 1.0;
               DA += 1.0;
       }

/*       sat_az = A9; 	 Azimuth */
/*       sat_el = E9;	 Elevation */

}

void   set_def()          /* default Kepler elements */
{
USN     char    i;       

      Y02[0] = 13;
      T02[0] = 255.51589744;     /* Epoch Time */
      K02[0] = 77672;           /* Epoch rev. */
      M02[0] = 39.4778;        /* Mean Anomaly */
      N02[0] = 12.53598152;     /* Mean Motion */
      I02[0] = 101.4404;         /* Inclination */
      E02[0] = 0.0012317;       /* Eccentricity */
      W02[0] = 116.2999;        /* Arg. of Perigee */
      O02[0] = 241.8776;         /* R.A.A.N. */
      N12[0] = -2.8e-07;        /* Decay rate */
      F01[0] = 435.120;
      F02[0][0] = 145.900;
      F02[0][1] = 145.975;
      F02[0][2] = 145.900;
      F02[0][3] = 145.900;
      MDD[0] = FM;
      MDU[0] = FM;
      SAT_FLAG_SW[0] &= ~DPLR_MODE;
      SAT_FLAG_SW[0] &= ~MA_MARK;
      F0CONV[0] = F1CONV[0] = 0;
      SAT_FLAG_SW[0] |= SELECTED_MRK;
      memcpy(O_name[0],"AO-07\0",6);

      Y02[1]= 13;
      T02[1]= 254.54205523;
      K02[1]= 77725;
      M02[1]= 323.8264;
      N02[1]= 14.29120366;
      I02[1]= 31.4339;
      E02[1]= 0.0351235;
      W02[1]= 193.9814;
      O02[1]= 298.6696;
      N12[1]= 4.94e-06;
      F01[1]= 435.175;
      F02[1][0]= 145.850;
      F02[1][1]= 145.900;
      F02[1][2]= 145.900;
      F02[1][3]= 145.900;
      MDD[1]= FM;
      MDU[1]= FM;
      SAT_FLAG_SW[1] &= ~DPLR_MODE;
      SAT_FLAG_SW[1] &= ~MA_MARK;
      SAT_FLAG_SW[1] |= SELECTED_MRK;
      F0CONV[1] = F1CONV[1] = 0;
      memcpy(O_name[1],"SO-33\0",6);

      Y02[2] = 13;
      T02[2] = 254.74297939;     /* Epoch Time */
      K02[2] = 84301;           /* Epoch rev. */
      M02[2] = 170.2427;        /* Mean Anomaly */
      N02[2] = 13.53005152;     /* Mean Motion */
      I02[2] = 98.5457;         /* Inclination */
      E02[2] = 0.0349499;       /* Eccentricity */
      W02[2] = 299.9056;        /* Arg. of Perigee */
      O02[2] = 301.6933;        /* R.A.A.N. */
      N12[2] = 1.0e-07;        /* Decay rate */
      F01[2] = 435.910;
      F02[2][0] = 145.870;
      F02[2][1] = 145.850;
      F02[2][2] = 145.890;
      F02[2][3] = 145.910;
      MDD[2] = FM;
      MDU[2] = FM;
      SAT_FLAG_SW[2] &= ~DPLR_MODE;
      SAT_FLAG_SW[2] &= ~MA_MARK;
      F0CONV[2] = F1CONV[2] = 0;
      SAT_FLAG_SW[2] |= SELECTED_MRK;
      memcpy(O_name[2],"FO-29\0",6);

      sat_list[0] = 0;
      sat_list[1] = 1;
      sat_list[2] = 2;

      for(i=3;i<REG_MAX;i++){
                sat_list[i] = i;
      }
            
      sat_mx = ld_sat = REG_MAX;         /* TEST */
}

USN char sel_ant_menu()
{
USN     char    ch;

           send("\r\n\r\n");           
           send("\t1.Direct rotor control from key board.\r\n");
           send("\t2.Enter direction value.\r\n");
           send("\t3.Preset Control.\r\n\r\n");

           for(;;){
               send("\r\tSelect[1-3,Q]: ");
               ch = getchar();
               if(ch >= '1' && ch <= '3' ) return (ch -'0');
               if((ch | 0x20) == ESC) return ESC;
           }
}

void point_ant()          /* Point ANTENNA direction */
{
   short cs;
   float new;
   USN char sel,flag;
   
   for(;;){    
       sel = sel_ant_menu();       
       switch(sel){
       
       case 1:
	        send("\r\n\r\n\tLeft(4), Right(6), Up(8), Dwn(2), Stop(5) or Ret.\r\n\r\n");
		XBYTE[PPI+2] |= AZ_ROTOR_HIGH;       
       		flag = 0;
		for(;;){

	           if(RI){         /* ANY KEY IN ?? */
			switch(getchar()){
				case LEFT_A:	/*left  '2' */
					move_az_ccw();
					XBYTE[LED_PORT] |=  AOS_LED; /*AOS-LED ON */
		        		break;
				case RIGHT_A:	/*right  '6'*/
	       				move_az_cw();
					XBYTE[LED_PORT] |=  AOS_LED; /*AOS-LED ON */
	       				break;
				case UP_A:	/*up  '8'*/
	       				move_el_up();
					XBYTE[LED_PORT] |=  AOS_LED; /*AOS-LED ON */
	       				break;
				case DWN_A:	/*down  '2'*/
	       				move_el_dn();
					XBYTE[LED_PORT] |=  AOS_LED; /*AOS-LED ON */
    	       				break;
				case POINT_STOP:  /* stop '5' */
	       				XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
					XBYTE[LED_PORT] &= ~AOS_LED;  /* AOS-LED OFF */
					break;
				default:
		       			XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
					XBYTE[LED_PORT] &= ~AOS_LED;  /* AOS-LED OFF */
					flag = 1;
	       		}	       
           	   } /* if any key in loop end */
           	   if(flag) break;
	   	   disp_ant();
	   
       		} /* for loop end */
		break;

	case   2:
	       printf("\r\n\r\n\tAzimuth: %2.1f",AZ_setv);
	
	       new = get_fval("\t",&cs);
	       if(cs != 0) AZ_setv = new;
	       printf("\r\n\tElevation: %2.1f",EL_setv);

	       new = get_fval("\t",&cs);
	       if(cs != 0) EL_setv = new;
	       set_ant();
	       break;

	case	3:
		preset_ant();
		break;

	case ESC:
		return;
	}
    } /* for loop end */
}

void set_ant()
{
USN char	ch;

	       SW |= TRACK_ID;
	       XBYTE[LED_PORT] |=  AOS_LED; /*AOS-LED ON */
	       send("\r\n");
	       for(;;){
		   rotor_con2();
		   if((XBYTE[PPI+2] & 0x0F) == 0) break; 
	           if(RI){         /* ANY KEY IN ?? */
	               ch = getchar();
	               break;                      /* Break */
	           }
	           disp_ant();
	       }
	       SW &= ~TRACK_ID;
	       XBYTE[LED_PORT] &= ~AOS_LED;  /* AOS-LED OFF */
	       XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
}

void preset_ant()

{
USN char ch;
    float	new;
    short	cs;
USN short	i;
   
   for(;;){
	printf("\r\n\r\n\t1:%5.1f/%-4.1f 2:%5.1f/%-4.1f 3:%5.1f/%-4.1f 4:%5.1f/%-4.1f",
		PRESET_AZ[0],PRESET_EL[0],PRESET_AZ[1],PRESET_EL[1],PRESET_AZ[2],PRESET_EL[2],
		PRESET_AZ[3],PRESET_EL[3]);

	printf(" 5:%5.1f/%-4.1f",PRESET_AZ[4],PRESET_EL[4]);
	
	send("\r\n\r\n\tSelect[1-5,S,Q]: ");
        ch = getchar();
        if(ch >= '1' && ch <= '5'){
		ch = ch -'0'-1;
		AZ_setv = PRESET_AZ[ch];
		EL_setv = PRESET_EL[ch];
		set_ant();
	}
	else if((ch | 0x20) == 's'){
		for(i=0;i<5;i++){
			printf("\r\n\tPreset position %d  Az:%5.1f",i+1,PRESET_AZ[i]);
			new = get_fval("\t",&cs);
			if(cs != 0) PRESET_AZ[i] = new;

			printf("\t                   El:%5.1f",PRESET_EL[i]);
			new = get_fval("\t",&cs);
			if(cs != 0) PRESET_EL[i] = new;
		}
	
	}
	else if((ch | 0x20) == ESC) return;
   }
}

void disp_ant()
{
USN    char    lcd_text[30];
   float   ch1_d,ch2_d; 
USN short   az_adc_data,el_adc_data;

           read_adc(AZ_PORT); adc_az += (adc_data-az_mini); adc_az >>= 1;
	                      az_adc_data = adc_data;
           read_adc(EL_PORT); adc_el += (adc_data-el_mini); adc_el >>= 1;
			      el_adc_data = adc_data;
           ch1_d = (float)(adc_az / AZDEG);
           ch2_d = (float)(adc_el / ELDEG);

	   if((SW & ROTOR_ZERO) == 0){  /* if South start */
               	if(ch1_d > 180) ch1_d -= 180.0;
           	else  ch1_d += 180.0;
		
	   }
           printf("\tAZ=%4d / %3.1fdeg\tEL=%4d / %3.1fdeg.    \r",az_adc_data,ch1_d,el_adc_data,ch2_d);

/*
	   sprintf(lcd_text," Ant. direction ");
	   lcd_cmd(0x80);
	   print_lcd(lcd_text);
*/
	   sprintf(lcd_text,"AZ %3.1f EL%2.1f    ",ch1_d,ch2_d);             
	   lcd_cmd(0xc0);		/* LCD line-2 */
	   print_lcd(lcd_text); 
}

void  mic_step_con()		/* 92/02/26  */
{
	USN char	ch;
	short	cs,new1;
	float	new;	

	send("\r\n\r\n\tThis menu configures ADC value for the mic-click control\r\n");
	send("\r\n\tSet new value or Measure ADC value ? [S/M]  ");
	ch = getchar();
	if((ch | 0x20) == 's'){
	 	printf("\r\n\tModem locked center ADC value  : %4d",pll_lock);
		new = get_fval("\t",&cs);
		if(cs != 0) pll_lock = new;
		printf("\tReference ADC value         : %4d",pll_reff);
		new = get_fval("\t",&cs);
		if(cs != 0) pll_reff = new;
	}
	else if((ch | 0x20) == 'm'){
		 send("\r\n\tTune RX frequency to modem locked position [Y/N] ");
		 send("\r\n");
		 for(;;){
			if(RI){
				switch(getchar() | 0x20){
					case 'y': break;
					case 'n': return;
				}
			}
		        disp_adc();
		 }  
		 read_adc(PLL_ADC);
		 pll_lock = adc_data;
			 
		 send("\r\n\tPush Down mic-click button two times [Y/N]  ");
		 send("\r\n");
		 for(;;){
			if(RI){
				switch(getchar() | 0x20){
					case 'y': break;
					case 'n': return;
				}
			}
			disp_adc();
		 }     
		 read_adc(PLL_ADC);
		 pll_reff = adc_data;
        }

        new1 = get_val("\r\n\tEnter Step frequency [Hz]:",&cs);
        if(cs != 0) step_size = new1;
	send("\tFrequency set CAT command at AOS ? [Y/N] ");
	ch = getchar();
	if((ch | 0x20 ) == 'y') SW2 |= FLAG_AOS_CAT;
	else if((ch | 0x20) == 'n') SW2 &= ~FLAG_AOS_CAT;
        
	send("\r\n\tAutomatic up pulse generation at AOS ? [Y/N] ");
	switch(getchar() | 0x20){
		case 'y': SW2 |= (STEP_CON | FLAG_STEP);	/* set  flag */
			  break;
		case 'n': SW2 &= ~STEP_CON;
	}
	
	send("\r\n\tPorlarity of step pulse, Normal/Reverse ? [N/R] ");
	switch(getchar() | 0x20){
		case 'r': SW2 |= STEP_PORLAR;
			  XBYTE[PPI+2] |= ~PULSE_END;
			  break;
		case 'n': SW2 &= ~STEP_PORLAR;
			  XBYTE[PPI+2] &= PULSE_END;
	}
}

void disp_adc() small	/* display PLL ADC value  */
{
	read_adc(PLL_ADC);
	printf("\t\tADC value =%4d/1023   \r",adc_data);
}

void   rotor_con1(sel)     /* PRESESSION for ROTATOR CONTROL */
short	sel;
{
       if((SW & TRACK_ID) == NO){
       		XBYTE[PPI+2] &= 0xf0; /* AZ/EL STOP */
		reset_timeout();
	/*	mem_DX_az = mem_DX_el = DX;	*/
			
		if(EL_setv > trk_start_el){
                	if(XBYTE[RTC_S1] & 0x01) XBYTE[LED_PORT] |= AOS_LED; /*AOS-LED ON */
                	else           		XBYTE[LED_PORT] &=  ~AOS_LED; /*AOS-LED OFF*/
		}
		else XBYTE[LED_PORT] &=  ~AOS_LED; /*AOS-LED OFF*/
		return;
       }
       else if(EL_setv < trk_start_el){
           if(EL_setv >= EL_BLINK){ /* AOS LED blinking routine */
                                   /* set initial freq. for mic click */
               if(XBYTE[RTC_S1] & 0x01) XBYTE[LED_PORT] |= AOS_LED;
               else 			XBYTE[LED_PORT] &= ~AOS_LED; /*AOS-LED OFF*/
       	   }
	   if(EL_setv < EL_BLINK){
		XBYTE[LED_PORT] &= ~AOS_LED; /* AOS LED OFF */
		if((SW2 & FLAG_STEP) == 0){ /*  set Step flag and copy freq. */
			fdx_los = fd0_los;
			SW2 |= FLAG_STEP;
		}
	   }
	   if((SW & ANT_PARK) && (EL_setv < p16_on_el)){
			go_parking(sel);
			EL_setv = - 15.0; /* dummy */  
	                return;
	   }
	   else reset_timeout();
           XBYTE[PPI+2] &= 0xF0;     /* stop rotator */
       }

       else{		/* (EL_setv > = trk_start_el) && (TRACK_ID) */
               if(SW & MIC_CLICK){
	       		if((SW2 & MIC_CAT_FLAG) && (FREQ1 > 0.1) && (SW2 & FLAG_AOS_CAT)){
				rig_set_d(sel);
				SW2 &= ~MIC_CAT_FLAG;
			} 
	       		if((SW2 & STEP_CON) && (SW2 & FLAG_STEP)) step_ini(); 
	       }
	       fd0_los = fdx_los = RFD;  /* copy freq. for step control */
	       XBYTE[LED_PORT] |= AOS_LED;	/* AOS LED ON */

	       if(SW & ROTATE_L){		/* if limit SW */

			if(EL_setv > rotor_lim[3]) EL_setv = rotor_lim[3];
			else if(EL_setv < rotor_lim[2]) EL_setv = rotor_lim[2];
			
			if(rotor_lim[1] > rotor_lim[0]){
				if(AZ_setv > rotor_lim[1]) AZ_setv = rotor_lim[1];
				else if(AZ_setv < rotor_lim[0]) AZ_setv = rotor_lim[0];
			}
			else if(rotor_lim[1] < rotor_lim[0]){
				if     ((AZ_setv < rotor_lim[0]) && (AZ_setv > 180.0)) AZ_setv = rotor_lim[0];
				else if((AZ_setv > rotor_lim[1]) && (AZ_setv < 180.0)) AZ_setv = rotor_lim[1];
			}
		}
		rotor_con2();
       }
}

void rotor_con2()  	/* Rotor control */
{
float		AZ_now,EL_now,AZ_set,EL_set;

       read_adc(AZ_PORT); adc_az += (adc_data-az_mini); adc_az >>= 1;
       read_adc(EL_PORT); adc_el += (adc_data-el_mini); adc_el >>= 1;

       AZ_now = (float) adc_az/AZDEG;
       EL_now = (float) adc_el/ELDEG;
       
       if((SW & ROTOR_ZERO) == 0){   /* south start */
		if(AZ_setv < 180.0) AZ_set = AZ_setv + 180.0;
		else AZ_set = AZ_setv - 180.0;
       }
       else AZ_set = AZ_setv;	/* north start */

       EL_set = EL_setv;
       
       AZ_set += skew_db[0];
       EL_set += skew_db[1];

       if(SW2 & FLIP_ON){       /* flip mode 94-05-26 */
                AZ_set += 180.0;        
                EL_set = 180 - EL_set;
       }
       
       if(AZ_set > 360.0)  AZ_set -= 360.0;
       else if(AZ_set < 0) AZ_set += 360.0;

       if(EL_set >= 180.0) EL_set = 180.0;
       else if(EL_set <= 0) EL_set = 0.0;
           
       /* AZ rotor speed control */
       
       if(fabs(AZ_now - AZ_set) > 3*skew_db[2]) XBYTE[PPI+2] |= AZ_ROTOR_HIGH;
       else XBYTE[PPI+2] &= ~AZ_ROTOR_HIGH;

       if(fabs(AZ_now - AZ_set) > skew_db[2]){     /* MOVE AZ */
		if(AZ_set > AZ_now) move_az_cw();
                else        move_az_ccw();
       }
       else  move_az_non();                  /* STOP AZ */
                                   
       /**** Elevation control ****/
       
       if(fabs(EL_now  - EL_set) > skew_db[3]){        /* MOVE EL */
           if(EL_set > EL_now) move_el_up();
           else if(EL_set >= 0.0) move_el_dn();
       }
       else   move_el_non();                   /* STOP EL */
}

void  move_az_cw() small
{
       XBYTE[PPI+2] =  (XBYTE[PPI+2] & 0xfc) | AZ_CW;          
}

void   move_az_ccw() small
{
       XBYTE[PPI+2] =  (XBYTE[PPI+2] & 0xfc) | AZ_CCW;          
}

void   move_az_non() small
{
       mem_DX_az = DX;
       XBYTE[PPI+2] &= 0xfc; /* AZ STOP */
}

void   move_el_up() small
{
       XBYTE[PPI+2] =  (XBYTE[PPI+2] & 0xf3) | EL_UP;          
}

void   move_el_dn() small
{
       XBYTE[PPI+2] =  (XBYTE[PPI+2]  & 0xf3) | EL_DWN;          
}

void   move_el_non() small
{
       mem_DX_el = DX;
       XBYTE[PPI+2] &= 0xf3;                    
}

void   calib_ant()               /* ANTENNA CALIBRATION */ 
{
   char   chk;

       printf("\r\n\r\n  ADC span value   AZ: %04d - %04d  EL: %04d - %04d\r\n",az_mini,az_max,el_mini,el_max);

       chk = disp_yn("\r\n  Set flip mode operation for elevation rotor");
       if(chk == 1) SW2 |= FLIP_MODE;
       else if(chk == 0) SW2 &= ~FLIP_MODE;

       for(;;){
           chk = disp_yn("\r\n  Rotate the antenna to fully CCW and Elevation minimum position");
           if(chk == 1) break;
           else if(chk == 0) return;
       }

       read_adc(AZ_PORT);
       az_mini = adc_data;
       read_adc(EL_PORT);
       el_mini = adc_data;

       for(;;){
           chk = disp_yn("\r\n  Rotate the antenna to fully CW and Elevation maximum position");
           if(chk == 1) break;
           else if(chk == 0) return;
       }
       read_adc(AZ_PORT);
       az_max = adc_data;
       read_adc(EL_PORT);
       el_max = adc_data;

	el_az_deg_set();
}

void   value_ant()          /* ADC value setting */
{
       short   cs;
       char    chk;
       float   newf;

       printf("\r\n\tADC span value   AZ: %04d - %04d ",az_mini,az_max); 
       newf = get_fval("\r\n\tAZ minimum value ? : ",&cs);
       if(cs != 0) az_mini = (short)newf;
       newf = get_fval("\r\tAZ maximum value ? : ",&cs);
       if(cs != 0) az_max = (short)newf;

       chk = disp_yn("\r\n\tSet flip mode operation for elevation rotor");
       if(chk == 1) SW2 |= FLIP_MODE;
       else if(chk == 0) SW2 &= ~FLIP_MODE;
       
       printf("\r\n\tADC span value   EL: %04d - %04d ",el_mini,el_max);
       newf = get_fval("\r\n\tEL mimimum value ? : ",&cs);
       if(cs != 0) el_mini = (short)newf;
       newf = get_fval("\r\tEL maximum value ? : ",&cs);
       if(cs != 0) el_max = (short)newf;
       
       el_az_deg_set();
}

void   set_park()       /* Set Parking value and rotate limitation  */
{
   short cs;
   USN int   css;
   float new;
USN    char   ch,tmp[10];

       send("\r\n\r\n\tSet Antenna Parking position ? [Y/N]  ");
       ch = getchar() | 0x20;
       if(ch == 'y'){
               SW |= ANT_PARK;

	       printf("\r\n\r\n\tAzimuth  Parking  value : %3.1f",AZ_park);
	       new = get_fval("\t",&cs);
	       if(cs != 0) AZ_park = new;
	       printf("\tElevation Parking value : %3.1f",EL_park);
	       new = get_fval("\t",&cs);
	       if(cs != 0) EL_park = new;

	       send("\r\n\tSet TRX mode/freq. in parking ? [Y/N]  ");
	       ch = getchar() | 0x20;
       	       if(ch == 'y'){
			SW2 |= PARK_TRX_MODE;
			printf("\r\n\tRX parking freq : %6.3f",frq_park[0]);
	 	        new = get_fval("\t",&cs);
	 	        if(cs != 0) frq_park[0] = new;
			printf("\tTX parking freq : %6.3f",frq_park[1]);
	 	        new = get_fval("\t",&cs);
	 	        if(cs != 0) frq_park[1] = new;

			printf("\tRX parking mode (FM/USB/LSB) : ");
	   		if(getline(tmp)){
			    switch(tmp[0] | 0x20){
				case 'l': trx_mode[3] = LSB;break;
				case 'u': trx_mode[3] = USB;break;
				case 'f': trx_mode[3] = FM;break;
	   		    }
			}
			printf("\tTX parking mode (FM/USB/LSB) : ");
	   		if(getline(tmp)){
			    switch(tmp[0] | 0x20){
			    	case 'l': trx_mode[2] = LSB;break;
				case 'u': trx_mode[2] = USB;break;
				case 'f': trx_mode[2] = FM;break;
			    }
	   		}
       	       }
       	       else	SW2 &= ~PARK_TRX_MODE;
       }
       else if(ch == 'n') SW &= ~ANT_PARK;
       
       send("\r\n\tSet Antenna rotation limit ? [Y/N]  ");
       ch = (getchar() | 0x20);
       if(ch == 'y'){
               SW |= ROTATE_L;
	       printf("\r\n\r\n\tAzimuth   minmum  value : %3.1f",rotor_lim[0]);
	       new = get_fval("\t",&cs);
	       if(cs != 0) rotor_lim[0] = new;
	       printf("\tAzimuth  maximum  value : %3.1f",rotor_lim[1]);
	       new = get_fval("\t",&cs);
	       if(cs != 0) rotor_lim[1] = new;
	       printf("\tElevation  minmum value : %3.1f",rotor_lim[2]);
	       new = get_fval("\t",&cs);
	       if(cs != 0) rotor_lim[2] = new;
	       printf("\tElevation maximum value : %3.1f",rotor_lim[3]);
	       new = get_fval("\t",&cs);
	       if(cs != 0) rotor_lim[3] = new;
       }
       else if(ch == 'n') SW &= ~ROTATE_L;

       send("\r\n\tSet rotator smoke protection ? [Y/N]  ");
       ch = (getchar() | 0x20);
       if(ch == 'y'){
		SW2 &= ~ERR_MODE_OFF;
       		printf("\r\n\tTimeout value for rotator error [sec] : %3d",err_timeout);
		css = (USN int)get_val("\t",&cs);
		if(cs != 0) err_timeout = css;
       		printf("\r\n\tAuto recovery time from rotator error [1-61min] : %3d",err_recover_tm);
		css = (USN int)get_val("\t",&cs);
		if(cs != 0) err_recover_tm = css; 
       }
       else if(ch == 'n') SW2 |= ERR_MODE_OFF;
}

void   set_dbnd()  /* Dead BAND/ Skew Value set */
{
   short cs;
   float new;
       printf("\r\n\r\n\tAzimuth   Dead Band  : %2.1f",skew_db[2]);
       new = get_fval("\t",&cs);
       if(cs != 0) skew_db[2] = new;
       printf("\tElevation Dead Band  : %2.1f",skew_db[3]);
       new = get_fval("\t",&cs);
       if(cs != 0) skew_db[3] = new;

       printf("\r\n\tAzimuth   Skew value : %2.1f",skew_db[0]);
       new = get_fval("\t",&cs);
       if(cs != 0) skew_db[0] = new;
       printf("\tElevation Skew value : %2.1f",skew_db[1]);
       new = get_fval("\t",&cs);
       if(cs != 0) skew_db[1]= new;
}

void   param_list()        /* all list */
{

USN    char *mk_name[]={
		"KENWOOD",
		"YAESU  ",
                "ICOM "};

USN    char *ps_name[]={
	        "South",
	        "North"};

USN    char *mode_park[]={
       		"none",
		"LSB",
		"USB",
		"CW",
		"FM"};

USN    char *cat_span[]={
       		"AOS",
		"Full"};

USN    char *BAUDmsg[] ={		/* COM port Baud rate display */
		" 300",
		"1200",
		"2400",
		"4800",
		"9600",
		"19200"};

USN    short temp = SW;

       send("\r\n\r\n");
       printf("\tCallsign: %s  Lat: %5.1f  Long: %5.1f  Height: %5.1f\r\n",
               local_call,local_inf[0],local_inf[1],local_inf[3]);
       printf("\tADC span value    AZ: %5d - %5d  EL: %5d - %5d ",az_mini,az_max,el_mini,el_max);
       if(SW2 & FLIP_MODE) printf("(Flip)");
       printf("\r\n");
       printf("\tDead band         AZ: %5.1f          EL: %5.1f \r\n",skew_db[2],skew_db[3]);
       printf("\tSkew              AZ: %5.1f          EL: %5.1f \r\n",skew_db[0],skew_db[1]);
       if (SW & ANT_PARK){
       		printf("\tAntenna parking   AZ: %5.1f          EL: %5.1f \r\n",AZ_park,EL_park);
		if(SW2 & PARK_TRX_MODE){
			printf("\tTRX parking   RX:%6.3f TX:%6.3f    RX mode:%s TX mode:%s\r\n",
			     frq_park[0],frq_park[1],mode_park[trx_mode[3]],mode_park[trx_mode[2]]);
		}
		
		else send("\tTRX  parking      Off\r\n");
       }
       else     printf("\tAntenna parking   Off\r\n");
       if (SW & ROTATE_L){
       		printf("\tRotator limit     AZ: %5.1f - %5.1f  EL: %5.1f - %5.1f \r\n",rotor_lim[0],rotor_lim[1],rotor_lim[2],rotor_lim[3]);
       }
       else     printf("\tRotator limit     Off\r\n");

       printf("\tRotator start EL:%5.1f   P1.6_on EL:%5.1f   P1.6_off EL:%5.1f \r\n",
       					trk_start_el,p16_on_el,p16_off_el);
       
       if(SW2 & ERR_MODE_OFF){
       		send(          "\tRotor smoke protection\tOff\r\n");
       }
       else{
		printf(        "\tTimeout value for rotator error: %3d sec\r\n",err_timeout);
       		if(err_recover_tm < 61){
       			printf("\tAuto recovery time from rotator error: %3d min\r\n",err_recover_tm);
       		}
       }

       printf("\tCAT Rig           %s %s",mk_name[rig_maker],cat_span[(temp & FULL_O_CAT) >> 7]);
       switch(rig_maker){
       		case KENWOOD:
			if (SW2 & _711811) send("  TS711/811");
			break;
		case YAESU  :
			if(SW3 & YAESU736) send("  FT-736");
			else if(SW3 & YAESU847) send("  FT-847");
			if(SW & YAESU_FMN) send("/FMN-Filter");
			break;
		case ICOM   :
			printf("  (%BX)(%BX)(w=%d)",i_rx_address,i_tx_address,i_wait);
			break;
       }
       printf(" %s",BAUDmsg[lpt_baud_rate+1]);
       
       printf("\r\n\tStep control ADC value  Center:%5d  Reff:%5d",
       			pll_lock,pll_reff);
       printf("\r\n\tFrequency step    %ldHz",step_size);
       printf("\r\n\tRotor start       %s",ps_name[(temp & ROTOR_ZERO) >> 3]);
       printf("\r\n\tCOM port          %s Baud 8-N-1\r\n",BAUDmsg[baud_rate]);
       printf("\r\n\t\tPress any key to continue");
       getchar();
}

/*******************************/
/*  display schedule.          */
/*******************************/

void calc_sat_sched(num)
short num;
{
USN   short   y_dsp,day_of_year,yx;
USN   char    mox,dx,hx,mix,sx;
USN   char    ch,i,temp[40], flg = NO, scr_flg = NO;
      short   cs;
USN   char   m_dsp,d_dsp,h_dsp,mi_dsp,s_dsp;
   float   minx = 1.0;
   float   secx = 0.0;
   float   tsp,fx1,n_minx,n_secx,tmp_sat_el;

           sel_sat(num);	/* read the elements. */
           months();
           printf("\r\n%4d/%02bd/%02bd %02bd:%02bd:%02bd\r\n",tm_year,
               tm_mon,tm_day,tm_hour,tm_min,tm_sec);

           send("Starting Day/Time ? [YY/MM/DD HH:MM:SS] ");
	   temp[2] = temp[5] =0;
           getline(temp);
           if((temp[2] | temp[5]) == '/' && (temp[11] | temp[14]) == ':'){
	       yx = (temp[0] - 0x30)*10+(temp[1] - 0x30)+1900;
               if(temp[0] != 0x39) yx += 100; /* 96-11-15 */
	       mox= (temp[3] - 0x30)*10+(temp[4] - 0x30);
               dx = (temp[6] - 0x30)*10+(temp[7] - 0x30);
               hx = (temp[9] - 0x30)*10+(temp[10] - 0x30);
               mix= (temp[12] - 0x30)*10+(temp[13] - 0x30);
               sx = (temp[15] - 0x30)*10+(temp[16] - 0x30);
           }
           else{
               yx =tm_year;
               mox = tm_mon;
               dx = tm_day;
               hx = tm_hour;
               mix = tm_min;
               sx = tm_sec;
           }
           get_epday(yx,mox,dx,hx,mix,sx);	/* set T:start day/time */

           printf("Day of the Year [T] > %5.5f\r\n",T);
           send("Time Step\r\n");

           n_minx = get_fval("\tMin [1]: ",&cs);
           if(cs != 0) minx = n_minx;
	   n_secx = get_fval("\tSec [0]: ",&cs);
           if(cs != 0) secx = n_secx;
	   minx *= 0.0006944443;              /*Day / sec */
           secx *= 0.00001157407;
           tsp = minx+secx;  			 /* tsp: step */

           for(;;){
               printf("  %5s    %5s       %3s  /%3s     %5s%10s\r\n","DATE","TIME","AZ","EL","Phase","Doppler");
               for(i=0;i<22;){
                   if(RI){         /* ANY KEY IN ?? */
                       getchar();
		       scr_flg = NO;
                       break;                      /* Break */
                   }

                   main_loop(num);        /* Calclation of the sat's AZ/EL */ 
                   day_of_year = (short)T;
                   fx1 = T - (short)T;
		   
                   month_day(yx,day_of_year,&y_dsp,&m_dsp,&d_dsp);
                   to_hour(fx1,&h_dsp,&mi_dsp,&s_dsp);

                   if(E9 > -1.0){
                       printf("%4d/%02bd/%02bd %02bd:%02bd:%02bd    %5.1f/%4.1f%9.1f%+9.0f\r\n",
                           y_dsp,m_dsp,d_dsp,h_dsp,mi_dsp,s_dsp,A9,E9,M9,F9D);
		    
		       if( E9 > tmp_sat_el) flg = YES;
                       i++;
                       T += tsp;                    /* step up set time*/
		       tmp_sat_el = E9;         /* temp store */	     
                       continue;
                   }
		   if((E9 < tmp_sat_el) && (flg == YES)){
		   	send("**********\n\r");
			flg = NO;
		   }		   
		   if( tsp > 0.00069444) T += tsp;
		   else T += 0.00069444;            /* step up = 1 min.*/
	     	   tmp_sat_el = E9;
               }   /* 21 line for loop end */

               if(scr_flg == NO){
               		send("\t--- 'Q' for abort 'S' for scroll ---");
			ch = getchar();
			if((ch | 0x20) == 'q') break;
			else if((ch | 0x20) == 's'){
				scr_flg = YES;
				send("\r\n");
			}
	       }
           }
}

void   get_epday(yx,mox,dx,hx,mix,sx)
USN short  yx;
USN char mox,dx,hx,mix,sx;
{
USN     int     n,i;
       float th,tm,ts;
       
       DA = day_of_year(yx,mox,dx);
       th = (float)hx/24.0;
       tm = (float)mix/1440.0;
       ts = (float)sx/86400.0;

       if(n = (yx - Y2)){
                for(i=1;i<=n;i++){
                        if((yx -i)%4 == 0) DA += 366.0;
                        else DA += 365;
                	if(i>5) break;	/* 98-12-15 */
		}
       }       
       if(DA > 397) send("\n\r\x07Too old elements. \r\n");

       T = (float)DA*1.0;             /* to float */
       T += (th + tm + ts);
}

void month_day(yx,day_of_year,year_x,pmonth,pday)
USN     short  yx,day_of_year,*year_x;
USN char	*pmonth,*pday;
{
  USN short     year,yd;
  USN char      leep=1,i;

        year = Y2;
        
year_loop:
       
       leep = year%4 == 0; /* && year%100 != 0 || year%400 == 0; */
       yd = day_of_year;
        for(;;){
           for(i=1;yd >day_tab[leep][i];i++){
               if(i >12) break;
               yd -= day_tab[leep][i];
           }
           if(i<=12) break;
           if(i>12) {
                if(leep) day_of_year -= 366;
                else     day_of_year -= 365;
                year++;
                goto year_loop;
           }
        }
        *pmonth = i;
        *pday = yd;
        *year_x = year; 
}


void   to_hour(tos,th,tm,ts)
float  tos;
USN char  *th,*tm,*ts;
{
       float fx1;
       short   h_dsp,mi_dsp,s_dsp;

           fx1 = tos * 24.0;
           h_dsp =(short) fx1;
           fx1 =fx1 - (float)h_dsp;
           mi_dsp = (short)( fx1*60.0);
           fx1 = fx1 -(float)(mi_dsp/60.0);
           s_dsp = (short)(fx1*3600.0);
           *th = h_dsp;
           *tm = mi_dsp;
           *ts = s_dsp;
}

void   read_kep()      /* Read Kepele with AMSAT/NASA Format */
{		       /* 92-02-26 */
USN   int i,sn=0;
USN   char    k,ch,line[80],tmp[80];
       
       clr_rs_buff();
       send("\r\n\n\tUpload Kepele data file. (EOF:ctrl+Z)\r\n");

       for(i=rs_buff_mark;i<RS_BSIZE-1;i++){
	   ch = getchar();     /* read */
           rs_buf[i] = ch;
           if(ch == EOFF) break;
       }
       rs_buff_mark = i;

       if(rs_buff_mark >= RS_BSIZE-2){
	   	send("\x07\r\n\tRS Buffer Full !\r\n");
       }
       send("\r\n");

                for(i=0;;){
                        for(k=0;k<79;){		/* read first line */
                           ch = rs_buf[i++];
                           if(ch == EOFF) break;
                           if(ch == CR || ch == LF) break;
                           tmp[k++] = ch;
                        }

			if(ch == EOFF) break;	    /* end */
			
                        tmp[k] = '\0';		    /* serch satellite name */
			if((tmp[0] == ' ') || (tmp[0] == '\0')) continue; 
							/* ignore blank line */
			/* if(strlen(tmp) >15) continue;  ignore long line */
                        
                        memset(O_name[sn],'\0',20);
                        memcpy(O_name[sn],tmp,15);  /* SAT name set */
                        
			for(k=0;k<79;){		/* read 2-nd line */	
                           ch = rs_buf[i++];
                           if(ch == EOFF) break;
                           if(ch == CR || ch == LF) break;
                           line[k++] = ch;
                        }

                        if(line[0] != '1') continue;   /* search "1" */
                        if(line[2] == 'A') continue;   /* ignor garvage */
			Y02[sn] = 10*(line[18]-'0')+(line[19]-'0');
                        for(k=0;k<12;k++) tmp[k]= line[20+k];
                        tmp[k]= '\0';
                        T02[sn] = atof(tmp);           /* Epoc time */
                        for(k=0;k<10;k++) tmp[k]= line[33+k];
                        tmp[k]= '\0';
                        N12[sn] = atof(tmp);           /* Decay rate */

                        for(k=0;k<79;){		/* read line-3 */
                           ch =rs_buf[i++];
                           if(ch == EOFF) break;
                           if(ch == CR || ch == LF) break;
                           line[k++] = ch;
                        }
                        if(line[0] != '2') continue;   /* serch  "2" */

                        for(k=0;k<8;k++) tmp[k]= line[8+k];
                        tmp[k]= '\0';
                        I02[sn] = atof(tmp);           /* Inclination */

                        for(k=0;k<8;k++) tmp[k]= line[17+k];
                        tmp[k]= '\0';
                        O02[sn] = atof(tmp);           /* RA of node */

                        for(k=2;k<7;k++) tmp[k]= line[24+k];
                        tmp[k]= '\0';
                        tmp[0]= '0';
                        tmp[1]= '.';
                        E02[sn] = atof(tmp);            /* Eccent  */

                        for(k=0;k<8;k++) tmp[k]= line[34+k];
                        tmp[k]= '\0';
                        W02[sn] = atof(tmp);           /* Arg of perigee */

                        for(k=0;k<8;k++) tmp[k]= line[43+k];
                        tmp[k]= '\0';
                        M02[sn] = atof(tmp);          /* Mean anomary */

                        for(k=0;k<11;k++) tmp[k]= line[52+k];
                        tmp[k]= '\0';
                        N02[sn] = atof(tmp);          /* Mean motion */

                        for(k=0;k<5;k++) tmp[k]= line[63+k];
                        tmp[k]= '\0';

                       K02[sn] = atol(tmp);
                       F01[sn] = 0.0;
                       F02[sn][0] =F02[sn][1] =F02[sn][2] =F02[sn][3] = 0.0;

		       printf("NASA>#%3d\t%s\r\n",sn+1,O_name[sn]);

		       sn++;
                       sat_mx = sn;            /* inc data exist */
                       if(sn >= SATMAX) break;
		       if(ch == EOFF) break; 	    /* if EOF, exit loop */
		}
		read_f_minf();
}

void   add_list()      /* select list from sat_buffer */
{
short	st;
USN	char i,j;
       
       send("\r\n");
       j = 0;
       for(i=0;i<REG_MAX;i++) sat_list[i] = -1;	/* clear */

       for(i=0;i<SATMAX;i++){
           st=disp_sat();
           if(st < 0) {            /* delete from sat list */
               if(j > 0) break;    /* selection stop and return */
               else continue;      /* if none selected, continue */
               }
           else {
               sat_list[i] = st;
               printf("\r\n\tSatellite (%s) selected as #%bd\r\n",O_name[st],j+1); 
               j++;
	   }
           if(j >= REG_MAX) break;
       }
       ld_sat = j;     /* active satellite number */
       colect_list();

}

short   disp_sat()             /* Display all sats in RS buffer  */
{
short	sel,i,exist,cs;

       exist = 0;
       send("\r\n");

       for(i=0;i<SATMAX;i++){
           if(i >= sat_mx) break;
           if(i && (i%4) == 0) send("\r\n");
           if(SAT_FLAG_SW[i] & SELECTED_MRK){
               printf("%2d.*%-14s ",i+1,O_name[i]);
           }
           else{
               printf("%2d. %-14s ",i+1,O_name[i]);
           }    
           exist++;
       }
       sel = get_val("\r\n\tSelect:",&cs);
       if(sel > exist) return -1;     /* TO SUB MENU */
       if(sel > 0 && sel <= SATMAX){
           SAT_FLAG_SW[sel-1] |= SELECTED_MRK;
	   return (sel-1);
       }
       return -1;
}

short disp_list_sat()  /* display list of asigned satellites */
{
short   i,st,cs;
USN   char ch;

       send("\r\n");
       for(i=0;i<REG_MAX;i++){         /* 0- 15 */
           if(sat_list[i] < 0 ) continue;
           if(i > ld_sat) break;    
           if(i && (i%4) == 0) send("\r\n");
           if(i == obj_sat) ch = TRACK_ST;
           else ch = NON_ST;
           printf("%2d.%c%-14s ",i+1,ch,O_name[sat_list[i]]);
           
       }
       send("\r\n");
       if(strncmp(O_name[sat_list[0]],"       ",7) == 0){
       		send("\r\n\t\x07Satellite is not asigned. Go to menu 3-2\r\n");
       		return -1;
       }
       for(;;){
           st = get_val("\r\n\tSelect:",&cs) - 1;
           if(st >= ld_sat) continue;
	   if(st < 0) st = -1;
	   break;
       }
       obj_sat = st;
       return st;
}

void   colect_list()   /* collect effective satllite */
{
USN    char   i,j;

       for(j=0,i=0;i<REG_MAX;i++){
           if(sat_list[i] >= 0){
                   sat_list[j++]= sat_list[i];
           }
       }
       for(i=j;i<REG_MAX;i++){        /* gerbage is del */
               sat_list[i] = -1;
     	       memcpy(O_name[sat_list[i]]," \0",2);
       }
}

void   read_f_minf()      /* Read freq/mode info */
{		          /* 92-04-14 */
USN    int    i;
USN    char   j,k,m=0,n,o,p,q,r,ch,tmp[100],tmp1[20],tmp2[40],tmp21[20],
   	   tmp3[100],tmp31[20],tmp4[8],tmp5[8],tmp6[8],
	   tmp7[6],tmp8[6];
   
       clr_rs_buff();   
       for(i=0;i<REG_MAX;i++) sat_list[i] = -1 ; 
       send("\r\n\n\tUpload freq/mode data file. (EOF:ctrl+Z)\r\n");
       
       for(i=rs_buff_mark;i<RS_BSIZE-1;){
           ch = getchar();     /* read */
           rs_buf[i++] = ch;
           if(ch == EOFF) break;
       }

       if(i >= RS_BSIZE -1 ) send("\x07\r\n\tRS Buffer Full !\r\n");
       send("\r\n");

       if(rs_buf[rs_buff_mark] == EOFF){
		for(m=0;m<sat_mx;){
                        sat_list[m] = m;
                        SAT_FLAG_SW[m] |= SELECTED_MRK;
			m++;
			ld_sat = m;
		}
	        colect_list();
		return;
       }

       m = 0;
       for(i=rs_buff_mark;;){  
		
                for(k=0;k<80;){		/* read one line */
                      ch =rs_buf[i++];
                      if(ch == EOFF) break;
                      if(ch == LF) break;
                      tmp[k++] = ch;
                }
		if(ch == EOFF) break;
                tmp[k] = '\0'; 		/* null character */
		if(tmp[0] == ';')continue;
      		if(tmp[0] == ' ')continue;
      		if(tmp[0] == '\0')continue;

		sscanf(tmp,"%s %s %s %s %s %s %s %s",
		       tmp1,tmp2,tmp3,tmp4,tmp5,tmp6,tmp7,tmp8);

		n = strlen(tmp1);		
		for(j=0;j<sat_mx;j++){

			if((strncmp(O_name[j],tmp1,n) == 0) && (m < REG_MAX)){

				p = 0;
				o = strlen(tmp2);
				
				if(strchr(tmp2,'/')){

					for(k=0;k<o;k++){
						tmp21[k] = tmp2[k];
						p++;
						if(tmp2[k] == '/'){
							tmp21[k] = '\0';
							break;
						}
					}
					F01[j] = atof(tmp21);     /* F1  */
					
					for(k=0;k<o-p;k++) tmp21[k] = tmp2[p+k];
					tmp21[k] = '\0';
					F0CONV[j] = atof(tmp21);
				}
				else{ F01[j] = atof(tmp2);     /* F1  */
					F0CONV[j] = 0;
				}

				p = 0;
				o = strlen(tmp3);
				q = 0;
				
			   for(q=0;q<4;q++){
                                for(k=0;k<o-p;k++){
                                        tmp31[k] = tmp3[k+p];
                                        if((tmp31[k] == ',') || (tmp31[k] == '\0')){
                                                tmp31[k] = '\0';
                                                break;
                                        }
                                }
                                F02[j][q] = atof(tmp31);   
				if(tmp3[k+p] == '\0'){
					for(r=q+1;r<4;r++) F02[j][r] = 0;
					break;
				}
				p += (k+1);
			   }
				p = 0;
                                if(strchr(tmp3,'/')){
                                        for(k=0;k<o;k++){
						p++;
				                if(tmp3[k] == '/') break;
                                        }
					for(k=0;k<o-p;k++) tmp31[k] = tmp3[p+k];
					tmp31[k] = '\0';
					F1CONV[j] = atof(tmp31);
                                }
				else F1CONV[j] = 0;
				
			/**************************/

				switch(tmp4[0] | 0x20){
					case 'l': MDD[j] = LSB;break;
					case 'u': MDD[j] = USB;break;
					case 'c': MDD[j] = CW;break;
					case 'f': MDD[j] = FM;break;
					default : MDD[j] = NONE;
				}
				switch(tmp5[0] | 0x20){
					case 'l': MDU[j] = LSB;break;
					case 'u': MDU[j] = USB;break;
					case 'c': MDU[j] = CW;break;
					case 'f': MDU[j] = FM;break;
					default : MDU[j] = NONE;
				}
							
		                if((tmp6[0] | 0x20) == 'c') SAT_FLAG_SW[j] &= ~DPLR_MODE;
	                	else if((tmp6[0] | 0x20) == 'm') SAT_FLAG_SW[j] |= DPLR_MODE;
				
				if((tmp7[0] | 0x20) == 'm') SAT_FLAG_SW[j] |= MA_MARK;
				else  SAT_FLAG_SW[j] &= ~MA_MARK;

				if(((tmp8[0] | 0x20) == 'i') && ((tmp8[1] | 0x20) == 'r'))
					SAT_FLAG_SW[j] |= ICOM_R_ADDR;
				else    SAT_FLAG_SW[j] &= ~ICOM_R_ADDR;
				
				if(((tmp8[0] | 0x20) == 'k') && ((tmp8[1] | 0x20) == 'r'))
					SAT_FLAG_SW[j] |= KEN_REVS_PORT;
				else    SAT_FLAG_SW[j] &= ~KEN_REVS_PORT;
				
                                sat_list[m] = j;
                                SAT_FLAG_SW[j] |= SELECTED_MRK;
				
                                printf("Satellite (%8s) updated and selected as #%2bd\r\n",O_name[j],m+1);

				tmp8[0] = '\0';
				m++;                                    
				ld_sat = m;
				break;	
		        }  /* if sat_name match */
		}    /* for 0- sat_max loop */
        } 
        colect_list();
}
	   
void	set_stn_inf()   /* upload station information file  04/16/92 */
{
USN    int  i,baudval = 0,lpt_baudval = 0,err_lavel = 0;
USN    char   k,ch,tmp1[80],tmp5[20],tmp6[20],tmp7[20],pulse[5];

       clr_rs_buff();
       send("\r\n\r\n\tUpload Station info. data file. (EOF:ctrl+Z)\r\n");
       
       for(i=rs_buff_mark;i<RS_BSIZE-1;){
           ch = getchar();     /* read from RS-232C port */
           rs_buf[i++] = ch;
           if(ch == EOFF) break;
       }

	for(i=rs_buff_mark;;){  
             for(;;){
                for(k=0;k<60;){		/* read 1-st line */
                      ch = rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
		      tmp1[k++] = ch;
                }
                tmp1[k] = '\0'; 		/* null character */
                if(tmp1[0] == '1') break;       /* search "1" */
	     }
             sscanf(tmp1,"%*c %s %f %f %f %s",
	       local_call,&local_inf[0],&local_inf[1],&local_inf[3],tmp7,&baudval);

             if((tmp7[1] | 0x20) == 'r') SW2 |= DOP_ROUND_D; 
	     else SW2 &= ~DOP_ROUND_D;

             if((tmp7[1] | 0x20) == 'a') SW2 |= SEL_SAT_ADC;
	     else SW2 &= ~SEL_SAT_ADC;

	     if((tmp7[0] | 0x20) == 's') SW &= ~ROTOR_ZERO;
	     else if((tmp7[0] | 0x20) == 'n') SW |= ROTOR_ZERO;		

             if(local_inf[1] < 0 ) local_inf[2] = -1.0*local_inf[1];
             else               local_inf[2] = 360.0 - local_inf[1];

		switch(baudval){
			case	300: baud_rate = 0;break;
			case	1200: baud_rate = 1;break;
			case	2400: baud_rate = 2;break;
			case	4800: baud_rate = 3;break;
			case	19200: baud_rate = 5;break;
			default:	baud_rate = 4;
		}

             for(;;){
                for(k=0;k<60;){		/* read 2-nd line */
                      ch = rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
		      tmp1[k++] = ch;
                }
	        tmp1[k] = '\0';
                if(tmp1[0] == '2') break;     /* search "2" */
	     }
	     sscanf(tmp1,"%*c %d %d %d %d %s",
	     			&az_mini,&az_max,&el_mini,&el_max,tmp5);

		if((tmp5[0] | 0x20) == 'f') SW2 |= FLIP_MODE;
		else			    SW2 &= ~FLIP_MODE;
		el_az_deg_set();
					
             for(;;){
                for(k=0;k<60;){		/* read 3-rd line */
                      ch =rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
                      tmp1[k++] = ch;
                }
	        tmp1[k] = '\0';
                if(tmp1[0] == '3') break;     /* search "3" */
	     }
	     sscanf(tmp1,"%*c %f %f %f %f",&skew_db[2],&skew_db[3],&skew_db[0],&skew_db[1]);
		
             for(;;){
                for(k=0;k<60;){		/* read 4-th line */
                      ch =rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
		      tmp1[k++] = ch;
                }
	        tmp1[k] = '\0';
                if(tmp1[0] == '4') break;   /* search "4" */
	     }
	     sscanf(tmp1,"%*c %s %s %hx %hx %d %d",
	     		tmp5,tmp6,&i_rx_address,&i_tx_address,&lpt_baudval,&i_wait);
		switch(tmp5[0] | 0x20){
			case 'k': 
				rig_maker = KENWOOD;
				if((tmp5[7] | 0x20) == '7') SW2 |= _711811;
				break;
			case 'y': 
				rig_maker = YAESU;
				if((tmp5[5] | 0x20) == 'w') SW &= ~YAESU_FMN;
				else SW |= YAESU_FMN;

				if(tmp5[6] == '8'){	/* FT-847 */
					yaesu_tx_cmd = 0x21;
					yaesu_rx_cmd = 0x11;
					SW3 |= YAESU847;SW3 &= ~YAESU736;
				}
				else if((tmp5[6] | 0x20) == 'o'){  /* other */
					yaesu_tx_cmd = 0x01;
					yaesu_rx_cmd = 0x01;
					SW3 &= ~YAESU847;SW3 &= ~YAESU736;
				}
				else{			/* FT-736 default */
					yaesu_tx_cmd = 0x2E;
					yaesu_rx_cmd = 0x1E;
					SW3 |= YAESU736;SW3 &= ~YAESU847;
				}
				break;
			case 'i': 
				rig_maker = ICOM;
				if((tmp5[4] | 0x20) == 's') SW3 |= ICOM_SEPARATE;
				else SW3 &= ~ICOM_SEPARATE;
				break;
		}
		if((tmp6[0] | 0x20) == 'f') SW |= FULL_O_CAT;
		else if((tmp6[0] | 0x20) == 'a') SW &= ~FULL_O_CAT;

		switch(lpt_baudval){
			case    9600: lpt_baud_rate = 3;break;
			case   19200: lpt_baud_rate = 4;break;
			case	1200: lpt_baud_rate = 0;break;
			case	2400: lpt_baud_rate = 1;break;
			default: lpt_baud_rate = 2;break;
		}

		TR0=0;
		TH0 = lpt_baud_tab[lpt_baud_rate];
		TR0=1;
					
             for(;;){
                for(k=0;k<60;){		/* read 5-th line */
                      ch =rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
		      tmp1[k++] = ch;
                }
	        tmp1[k] = '\0';
                if(tmp1[0] == '5') break;   /* search "5" */
	     }
	     sscanf(tmp1,"%*c %s %f %f %f %f %f",
	        tmp5,&AZ_park,&EL_park,&trk_start_el,&p16_on_el,&p16_off_el);
	        if(tmp5[0] == '1') SW |= ANT_PARK;
		else SW &= ~ANT_PARK;
		
		pulse[0] = tmp5[1]; pulse[1] = '\0';	/* 94-05-15 */
		pulse_w = (short)atoi(pulse)+1;

		pulse[0] = tmp5[2]; pulse[1] = '\0';
		pulse_i = (short)atoi(pulse)+2;

             for(;;){
                for(k=0;k<60;){		/* read 6-th line */
                      ch =rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
		      tmp1[k++] = ch;
                }
	        tmp1[k] = '\0';
                if(tmp1[0] == '6') break;   /* search "6" */
	     }
				/* Parking condition and error timeout */
	     sscanf(tmp1,"%*c %s %f %f %s %s %d %d",
		tmp7,&frq_park[0],&frq_park[1],tmp5,tmp6,&err_timeout,&err_recover_tm);

	        if(tmp7[0] == '1') SW2 |= PARK_TRX_MODE;
		else SW2 &= ~PARK_TRX_MODE;
	        if(tmp7[1] == '1') SW2 &= ~ERR_MODE_OFF;
		else SW2 |= ERR_MODE_OFF;
				
		switch(tmp5[0] | 0x20){
			case 'l': trx_mode[2] = LSB;break;
			case 'u': trx_mode[2] = USB;break;
			case 'c': trx_mode[2] = CW;break;
			case 'f': trx_mode[2] = FM;break;
			default : trx_mode[2] = NONE;
		}
		switch(tmp6[0] | 0x20){
			case 'l': trx_mode[3] = LSB;break;
			case 'u': trx_mode[3] = USB;break;
			case 'c': trx_mode[3] = CW;break;
			case 'f': trx_mode[3] = FM;break;
			default : trx_mode[3] = NONE;
	        }
	     for(;;){
                for(k=0;k<60;){		/* read 7-th line */
                      ch =rs_buf[i++];
                      if(ch == EOFF) goto error1;
		      if(ch == LF) break;
		      tmp1[k++] = ch;
                }
	        tmp1[k] = '\0';
                if(tmp1[0] == '7') break;   /* search "7" */
	     }
	     sscanf(tmp1,"%*c %s %f %f %f %f",
	        tmp5,&rotor_lim[0],&rotor_lim[1],&rotor_lim[2],&rotor_lim[3]);
                if(tmp5[0] == '1') SW |= ROTATE_L;
		else SW &= ~ROTATE_L;

	     send("\r\n\tStation information updated.\r\n");
             return;  /* end */
	}
error1:	send("\x07\r\n\tError ! update not completed.\r\n");
}

void host_mode()	/* Host mode 09-28-92 */

{
 USN  char	size,k,ch,tmp[30],temp[80],temp1[30],temp2[30];
 long		f0_conv,f1_conv;

    SW |= HOST_MODE; 
    if(SW & HOST_FCONV){
    	f0_conv = (long)(F0CONV[sat_list[obj_sat]])*100000L;
    	f1_conv = (long)(F1CONV[sat_list[obj_sat]])*100000L;	
    }
    else f0_conv = f1_conv = 0;
    
    send("\r\n\r\nTrakBox host mode.");
    send("\r\nCommand:MUxxx,MDxxx,FUxxx,FDxxx,AZxxx_ELxx,Sxxx,Txx,Q,?A,?\r\n");
    lcd_cmd(0x80);	        /* set line-1 */
    print_lcd("TrakBox Hostmode\0");
    lcd_cmd(0xc0);		/* LCD line-2 */
    print_lcd("                \0");
           
    for(;;){
	send("\r\nTrakBox>> ");	
	getline(temp);
        size = strlen(temp);
        sscanf(temp,"%s %s",temp1,temp2);
	ch = temp[0];
	switch(ch | 0x20){
	
		case 'q':
			SW &= ~HOST_MODE;
			return;
		case 'f':	/* freq set command 10Hz order */
			if((temp[1] | 0x20) == 'u'){  
				for(k=0;k<9;k++) tmp[k] = temp[2+k];
				tmp[k] = '\0';
				RFU = atol(tmp) + f1_conv;
				RFU *= 10l;	/* 1Hz order */
				rig_set_u(sat_list[obj_sat]);
				
				lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("FU_             \0");
				lcd_cmd(0xc3);		/* LCD line-2 */
				print_lcd(tmp);

			}
			else if((temp[1] | 0x20) == 'd'){
				for(k=0;k<9;k++) tmp[k] = temp[2+k];
				tmp[k] = '\0';
				RFD = atol(tmp) + f0_conv; 
				RFD *= 10l;
				rig_set_d(sat_list[obj_sat]);
				
				lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("FD_             \0");
				lcd_cmd(0xc3);		/* LCD line-2 */
				print_lcd(tmp);
			}
			else{
				send("Freq. format error ! (ex. FU14590000)\r\n");
			        lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("F_format error \0");
			}
			break;
		
		case 'm':	/* mode set command */
			if((temp[1] | 0x20) == 'u'){ /* upload */
				lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("                \0");
				lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd(temp);

				switch(temp[2] | 0x20){
					case 'l': trx_mode[0] = LSB;break;
					case 'u': trx_mode[0] = USB;break;
					case 'c': trx_mode[0] = CW;break;
					case 'f': trx_mode[0] = FM;break;
					default :
						 trx_mode[0] = NONE;
						 lcd_cmd(0xc0);		/* LCD line-2 */	
						 print_lcd("Mode error     \0");
						 send("Mode error (ex. MUL/MUU/MUF)\r\n");
				}
				rig_mode_set(sat_list[obj_sat]);
			}
			else if((temp[1] | 0x20) == 'd'){ /* download */
				lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("                \0");
				lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd(temp);

				switch(temp[2] | 0x20){
					case 'l': trx_mode[1] = LSB;break;
					case 'u': trx_mode[1] = USB;break;
					case 'c': trx_mode[1] = CW;break;
					case 'f': trx_mode[1] = FM;break;
					default :
						 trx_mode[1] = NONE;
						 lcd_cmd(0xc0);		/* LCD line-2 */	
						 print_lcd("Mode error     \0");
						 send("Mode error (ex. MDL/MDU/MDF)\r\n");
				}
				rig_mode_set(sat_list[obj_sat]);
			}
			else{
				send("Mode Format error (ex. MULSB,MDFM)\r\n");
			        lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("M_format error \0");
			}
			break;
		
		case 'a':	/* Azimuth and Elevation set command */
			if(size < 10){
				send("Az/El format error (ex. Az075 El15)\r\n");
			        lcd_cmd(0xc0);		/* LCD line-2 */
				print_lcd("AZ_format error \0");
				break;
			}
                        for(k=0;k<3;k++) tmp[k]= temp1[2+k];
                        tmp[k]= '\0';
                        AZ_setv = atof(tmp);           /* Azimuth */
			for(k=0;k<2;k++) tmp[k]  = temp2[2+k];
			tmp[k] = '\0';
			EL_setv = atof(tmp);
			set_ant();
			break;

		case 't':      /* tracking */
			for(k=0;k<size;k++) tmp[k] = temp[1+k]; tmp[k] = '\0';
			obj_sat = atoi(tmp) -1;
		        if((obj_sat > 14) || (sat_list[obj_sat] < 0)){
				send("Track # Error !\r\n");
			}
			else {
	                       	SW |= TRACK_ID;
        	                tracking2();
			}
			break;			
		case 's':
			for(k=0;k<size;k++) tmp[k] = temp[1+k];tmp[k] = '\0';
                        sendp(tmp);
			lcd_cmd(0xc0);		/* LCD line-2 */
			print_lcd("                \0");
			lcd_cmd(0xc0);		/* LCD line-2 */
			print_lcd(tmp);
			break;
		case '?':
			if((temp[1] | 0x20) == 'a') disp_ant();
			else send("\r\nCommand:MUxxx,MDxxx,FUxxx,FDxxx,AZxxx_ELxx,Sxxx,Txx,?A,?,Q\r\n");	
			break;		
		default:
			send("Command error \r\n");
			lcd_cmd(0xc0);		/* LCD line-2 */
			print_lcd("Command error   \0");
			break;
	}
   } /* end 'for' loop */
}

void  go_parking(sel)	/* 98-10-28 */
char	sel;
{
	months();
	if((SW2 & PARK_TRX_MODE) && (SW2 & PARK_TRX_FLAG)){
		RFD = (long)(frq_park[0]*1000000l);
		RFU = (long)(frq_park[1]*1000000l);
		trx_mode[0] = trx_mode[2];
		trx_mode[1] = trx_mode[3];
		rig_mode_set(sel);		
		rig_set_u(sel);
		rig_set_d(sel);
		SW2 &= ~PARK_TRX_FLAG;
		sel_sat(sel);
	}
	SW2 &= ~FLIP_ON;	/* Antenna parking */
	EL_setv = EL_park;
	AZ_setv = AZ_park;
	rotor_con2();

	if(((DX - mem_DX_az) > (long)err_timeout) ||
			((DX - mem_DX_el) > (long)err_timeout)) rotor_error();
}

/*********   END   **********/


