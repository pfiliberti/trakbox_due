1 REM  Originally developed in North Star BASIC  by W3IWI
2 REM  Translated into Microsoft BASIC  by JR1SWB
3 REM  Some enhancement for N88/N-BASIC by JR1FIG                                      Disk version for N88/N-BASIC & F-BASIC by JR1HAL 84 May 20
4 REM--FNT$ Returns two character printable string corresponding to D
5  DEF FNT$(D) =  CHR$(48 + INT(D/10)) + CHR$(48 + D - 10 * INT(D/10))
6 REM ----------------------------
7 REM--FNI Rounds to nearest integer, accounting for sign
8  REM Using Fix(D) Function  DEF FNI(D)=SGN(D) * INT (ABS(D) + .5)
9 REM ----------------------------
10  DIM T$(20),S$(40),I$(40),C(3,2) : P=0 : REM--P=Page # counter
11 WIDTH 80,25
15  PRINT "ERROR MEANS YOU FORGOT TO APPEND ORBITAL ELEMENTS !!"
16  RESTORE 20000 : PRINT" THAT'S GOOD!"
20  REM--C8$=Page-separator $tring (could be form-feed) for your printer
30  C8$=CHR$(10)+CHR$(10)+CHR$(10)+CHR$(10) : REM--4 line feeds
40  REM--C9$=Clear CRT screen and beep for prompt
50  C9$=CHR$(12)+CHR$(7) : PRINT C9$ : REM--PC8001/8801 compatible
60  PRINT" AMSAT ORBITAL PREDICTION PROGRAM de W3IWI -- May,1980"
65  PRINT" This version Prepared for Distribution 15 Nov. l981" : PRINT
70  PRINT" COPYRIGHT 1980 by Dr. Thomas A. Clark, W3IWI"
80  PRINT"                   6388 Guilford Road"
90  PRINT"                   Clarksville, MD 21029"
100 PRINT" Permission granted for non-commercial use providing"
110 PRINT" credit is given to the author, AMSAT and ORBIT Magazine."
111 REM--*****   REVISIONS FROM THE VERSION IN ORBIT #6   *****
112 REM  (1) ORBITAL ELEMENTS MOVED FROM THE LINES RANGING FROM
113 REM      LINES 1100 THRU 9999 TO LINES BEGINNING 20000.
114 REM      CHANGES APPEAR IN LINES 5,6,360,400 AND 410.
115 REM      THIS ALLOWS THE ORBITAL ELEMENTS TO BE SEPARATE
116 REM      FILES WHICH CAN BE MERGED WITH "APPEND".  W3IWI  15 July 81
117 REM
118 REM  (2) A PROCEEDURE HAS BEEN ADDED TO ALLOW THE PROGRAM
119 REM      TO STEP MORE RAPIDLY WHILE THE SATELLITE IS BELOW
120 REM      THE ELEVATION LIMIT (E8).  THE PROCEEDURE INVOLVES
121 REM      FORCIBLY MANIPULATING THE TIME VARIABLE (T)
122 REM      WHILE THE SATELLITE IS BELOW THE LIMIT BY THE AD HOC
123 REM      ALGORITHM IN LINE 728.  ONCE THE SATELLITE RISES, THE
124 REM      TIME IS RE-SYNCHRONIZED IN LINE 726. THE 3-POSITION
125 REM      SWITCH (D0) IS USED TO SELECT THE ACTION NEXT TIME
126 REM      THRU THE LOOP.  CHANGES OR ADDITIONS APPEAR IN LINES
127 REM      660,680,685, AND 720 THRU 729.   W3IWI  15 July 81
128 REM
129 REM  (3) INCLUSION OF A PERIOD DERIVATIVE TERM IN MEAN MOTION
130 REM      TO BETTER MODEL LOW-ALTITUDE SATELLITES LIKE UoSAT.
131 REM      THIS INVOLVES CHANGES IN LINES 390, 430, 500-660,
132 REM      10140-10150, 10190, 10345-10350, 10450, PLUS THE NEW
133 REM      VARIABLES N & A (MEAN MOTION & SMA AT T) & N1 (DRAG TERM).
134 REM      NOTE THAT YOU MUST ADD THE N1 DRAG TERM (ORBITS/DAY^2)
135 REM      TO THE ORBITAL ELEMENTS DATA STATEMENTS. THE 1st PAGE
136 REM      SUMMARY OF THE ELEMENTS HAS BEEN MADE MORE INFORMATIVE &
137 REM      THE S.S.P. LONG & LAT NOW HAVE ONE MORE DIGIT THAN
138 REM      IN THE PRINTOUTS IN ORBIT#6 ARTICLE BY SLIGHTLY
139 REM      REVISING LINE 820.
140 REM                                W3IWI   15 NOV 81
141 REM
142 REM (4)  THE INCLUSION OF THIS REVISION LIST IN LINES 111-145.
143 REM 
144 REM                           73 es gud luck de W3IWI
145 REM ---------------------------------------------------------
146 REM ***** Set starting Day/Time, duration & step size & initialize *****
148  INPUT "Start: Year = ";Y : Y=Y/100 : Y=INT(100* (Y-INT(Y)) + .1)
150   IF Y/4=INT(Y/4) THEN F9=1  ELSE F9=0 : REM--F9=Leap-year flag
160  INPUT "       Month (1-12) = ";M : INPUT "       Day = ";D
170  T$=FNT$(Y) + "/" + FNT$(M) + "/" + FNT$(D) + " at " : REM--For printing
175  Y6=Y
180  REM--Calculate D8=Day# (Note that month #13 = January of following year)
190   RESTORE 930 : FOR I=1 TO M : READ D9 : NEXT : D8=D+D9
200   IF M>2  THEN   D8=D8+F9 : PRINT "     Day #";D8 : PRINT
210  INPUT"Start: UTC  Hours = ";H
220  INPUT "            Min. = ";M : T7=D8 + H/24 + M/1440
230  T$=T$ + FNT$(H) + FNT$(M) + ":00" : REM--For printing
240  INPUT"Duration:   Hours = ";H1
250  INPUT "            Min. = ";M1: T8=T7 + H1/24 + M1/1440
260  INPUT"Time Step:   Min. = ";M2 : T9=M2/1440
270  PRINT "     From ";T7;" to ";T8 : PRINT
280  REM
290  RESTORE 950 : READ P1,C,R0,F,G0,G1 : REM--Get physical constants
300   P2=2*P1 : P0=P1/180 : F=1/F
310   READ Y1,G2 : IF Y1=Y THEN 330  ELSE IF Y1>0 THEN 310
320   PRINT "Unable to find Year ";Y;" in Sidereal Time Table" : STOP
330  RESTORE 1090 : READ L9,W9,H9,C$,E8 : REM--Get station data
340   GOSUB 10520 : GOSUB 860 : REM--Call to FNO initializes observer's vector
350 REM ****** Select parameters for satellite of interest ******
360  PRINT "SATELLITE SELECTION MENU" : I=0 : RESTORE 20000 : PRINT
370   READ S$ : IF S$="END" THEN 400 ELSE READ I$ : I=I+1
380   PRINT "Entry #";I;" for ";S$ : PRINT "   ID = ";I$
390   READ D,D,D,D,D,D,D,D,D,D,D,D,D,D,D : GOTO 370 : REM--D's are dummy
400  J=1 : IF I>1 THEN INPUT "Select Entry # ";J  : REM--Get the satellite data
405  RESTORE 20000
410   IF J<1 OR J>I THEN 400 : REM--Hiccup if invalid entry requested
420  FOR I=1 TO J : READ S$,I$,Y3,D3,H3,M3,S3
430   READ K0,M0,N0,A0,N1,I0,E0,W0,O0,F1 : NEXT
440  PRINT : PRINT "Doppler calculated for freq = ";F1;" MHz"
450  INPUT " Change frequency to (0 for default) ";D
460   IF D<>0 THEN F1=D
470  IF Y3=Y THEN 500 ELSE PRINT "ELEMENTS NOT FROM CURRENT YEAR" : STOP
480  REM--IF D3 is an Integer then epoch is assumed to be in HH,MM,SS Format,
490   REM    or IF D3 is not Integer, then D3 is epoch in days + fractions
500   IF D3=INT(D3) THEN T0=D3 + H3/24 + M3/1440 + S3/86400!  ELSE T0=D3
502 PRINT : PRINT : REM--Print Out Program Revision ID Info
504  PRINT "North-Star BASIC Version of W3IWI Orbit Program"
506  PRINT "Revisions as of 15 November 1981 Included by W3IWI"
508  PRINT "(Translated into Microsoft BASIC by JR1SWB)"
510 PRINT : PRINT "Orbital Elements for ";S$ : REM--Print out summary--
520  PRINT "       Reference ID= ";I$ : PRINT
530  PRINT "Reference Epoch = ";Y3;" +";T0
540  PRINT "Starting  Epoch = ";Y;" +";T7;" = ";T$
542  PRINT "Elements are";T7-T0;" Days Old at Starting Epoch."
544  PRINT
546  PRINT "Duration  =";(T8-T7)*1440;" Minutes = ";(T8-T7)*24;" Hours"
548  PRINT "Step Size =";T9*1440;" Minutes."
550  PRINT
560  PRINT "Parameter";TAB(20);"Reference Epoch";TAB(40);"Starting Epoch"
562  PRINT "---------";TAB(20);"---------------";TAB(40);"--------------"
570   REM--Dummy calls to FNC & FNM to initialize
575   T=T7 : GOSUB 10110 : GOSUB 10310
580  PRINT "Orbit Number ";TAB(20);K0;TAB(40);K
590  PRINT "Mean Anomaly ";TAB(20);M0;TAB(40);M/P0
600  PRINT "Inclination  ";TAB(20);I0;TAB(40);"  [ No Change ]"
610  PRINT "Eccentricity ";TAB(20);E0;TAB(40);"  [ No Change ]"
620  PRINT "Mean Motion  ";TAB(20);N0;TAB(40);N
625  PRINT "Drag Correction";TAB(20);N1;"  <+ For normal drag speed-up>"
630  PRINT "S.M.A.,km    ";TAB(20);A0;TAB(40);A
632  PRINT "Perigee Ht,km";TAB(20);A0*(1-E0)-R0;TAB(40);A*(1-E0)-R0
634  PRINT "Apogee  Ht,km";TAB(20);A0*(1+E0)-R0;TAB(40);A*(1+E0)-R0
640  PRINT "Arg. Perigee ";TAB(20);W0;TAB(40);W
650  PRINT "R. A. A. N.  ";TAB(20);O0;TAB(40);O
660  PRINT "Freq.,MHz    ";TAB(20);F1 : K9=9E+09 : K8=9E+09 : D0=2
670 REM ****** Here follows the actual computation loop ******
675  A00=A0:K00=K0:M00=M0:N00=N0:O00=O0:Q00=Q0:T00=T0:W00=W0 : 
676  A0=A : K0=K : M0=M/P0 : N0=N : O0=O : Q0=Q : T0=0.0 : W0=W : 
680  FOR T=0 TO T8-T7 STEP T9
685  K7=INT(T+T7) : GOSUB 10310 : IF D0 THEN 720
690  REM--New orbit? If so, update the a priori values for W & O for new orbit
700   IF K=K9 THEN 720 ELSE GOSUB 10110 : K8=9E+09 : K9=9E+09
710  REM--Above Elevation limit?  New date?
720   GOSUB 10370 : GOSUB 10630 : D=E9-E8 : IF D<0 THEN 728
721 REM--IF IT'S BEEN BELOW HORIZON,THEN RESET TIME IN #726
722   IF D0=0 THEN 726 ELSE D0=2
723 REM--ABOVE HORIZON--TEST ORBIT ## AND DATE
724   IF K7=K8 AND K9=K THEN 770 ELSE 732
725 REM--RESET TIME TO BE IN STEP
726   T= T9*INT((T/T9)-1) : D0=1 : GOTO 685
727 REM--THIS AD-HOC ALGORITHM SPEEDS UP THE STEPPING
728   IF D0=1 THEN 830 ELSE D= R5*D*D *1E-09 : D0=0
729 IF D > .2/N0 THEN T=T+ .2/N0 ELSE T=T+D 
730 GOTO 830
732    IF K=K9 THEN 760  ELSE GOSUB 860 : K9=K
740  PRINT " U.T.C.  [JST]   AZ    EL  DOPPLER  RANGE  HEIGHT   LAT  LONG  PHASE"
750  PRINT "HHMM:SS         deg   deg    Hz       km      km    deg   deg  <256>"
760 RESTORE 930 : M6 = 0
761 READ D5 : IF Y6/4 <> INT(Y6/4) THEN 763
762   IF M6 > 1 THEN D5 = D5+1
763 IF K7 - D5 > 0 THEN M6 = M6+1 : D6 = D5 : GOTO 761
764 D5 = K7 - D6
765  PRINT TAB(10);"- - - DAY  ";FNT$(Y6) + "/" + FNT$(M6) + "/" + FNT$(D5);" - - - ORBIT #";K;" - - -"
770  K8=K7 : T4=T+(T7-K7) : S4=INT(T4*86400! + .5) : H4=INT(S4/3600 + .000001)
780  M4=INT((S4 - H4*3600) / 60 + .000001) : S4=S4 - 3600*H4 - 60*M4
790  T$=FNT$(H4) + FNT$(M4) + ":" + FNT$(S4) : REM--Printable time string
792  JST = H4 + 9 : IF JST > 23 THEN JST = JST - 24
794  TJ$=FNT$(JST)+":"+FNT$(M4)
800  F9=-F1*1E+06 * R8/C : REM--F9=Doppler(Hz)=Frequency * Velocity/C
810  PRINT T$;" [";TJ$; : PRINT USING "] ###   ###  #######";FIX(A9);FIX(E9);FIX(F9);
820   PRINT USING  " ######  ######  ####";FIX(R5);FIX(R-R0);FIX(L5);
822   PRINT USING  "  ####  ####";FIX(W5);M9
830  NEXT : PRINT : INPUT "PRESS RETURN TO CONTINUE ";D$ : GOTO 50
840 REM ----------------------------
850 REM ****** Page header subroutine ******
860  PRINT C9$;C$;"  Lat.=";L9;"  W.Long.=";W9;"  Ht.=";H9;
870  P=P+1 : PRINT TAB(54);"Pg. #";P
880  PRINT TAB(10);"- - - Minimum Elevation =";E8;" Deg. - - -"
890  PRINT  : RETURN
900 REM ----------------------------
910 REM ****** Numerical and Physical Constants ******
920 REM--Day# at start of each month (Thirty days hath Septober ...)
930     DATA 0,31,59,90,120,151,181,212,243,273,304,334,365
940 REM--Pi, Velocity of Light, Earth Radius, 1/Earth flattening coefficient
950     DATA 3.1415926535 , 2.997925E5 , 6378.160 , 298.25
960 REM--GM of Earth in (Orbits/day)^2/km^3 and Sidereal/Solar Time rate r#io
970     DATA 7.5369793E13 , 1.0027379093
980 REM--Greenwich Sidereal Times on Jan 0.0 in days for 1979 thru 1985
990     DATA 79 , 0.2751843198
1000    DATA 80 , 0.2745212008
1010    DATA 81 , 0.2765959911
1020    DATA 82 , 0.2759328721
1030    DATA 83 , 0.2752697531
1040    DATA 84 , 0.2746066342
1051    DATA 85,  0.27668226
1052    DATA 86,  0.27601916
1053    DATA 87,  0.27535606
1054    DATA 88,  0.27469296
1055    DATA 89,  0.27676777
1056    DATA 90,  0.27610467
1057    DATA 91,  0.27544157
1058    DATA 92,  0.27477847
1059    DATA 93,  0.27685328
1060    DATA 94,  0.27619018
1061    DATA 95,  0.27552708
1062    DATA 96,  0.27486399
1063    DATA 97,  0.27693880
1064    DATA 98,  0.27627570
1065    DATA 99,  0.27561260
1068    DATA 0,0
1069    REM ****** Terminates Sidereal Time table ******
1070 REM ----------------------------
1080 REM--Station data--Lat.&W.Long.(Deg),Height(Meters),Call & Min.Elevation
1090    DATA 35.867 , 220.025 , 13 , "JR1HAL" , -3
10000 REM ****** ORBIT DETERMINATION AND UTILITY ROUTINES ******
10010 REM  Here originally begins  FNA(X,Y)
10020  REM--Calculates INVERSE TANGENT in proper quadrant, just like
10030    REM      the FORTRAN ATAN2 function.
10040  REM--Cases lying in Quadrants 2&3
10050   IF X7<0 THEN  10054   ELSE IF X7>0 THEN 10090
10052   GOTO 10060
10054   F8= P1 + ATN(Y7/X7)  :  RETURN
10060  REM--The two cases for X7=0
10070   IF Y7>=0 THEN 10072  ELSE 10074
10072   F8= P1/2  :  RETURN
10074   F8= 3*P1/2  :  RETURN
10080  REM--Cases lying in Quadrants 1&4
10090   IF Y7>=0 THEN F8= ATN(Y7/X7)  ELSE F8= P2 + ATN(Y7/X7)
10092   RETURN
10100 REM --------------------------
10110 REM  Here originally begins  FNC(T)
10120  REM--Routine to initialize the C(J,K) coordinate rotation matrix
10130    REM    and other parameters associated with the orbital elements.
10139 REM--Given N0=Mean Motion, execute Lines 1014x
10140 REM--Input Elements give either A0= Semi-Major Axis in km OR
10142 REM   N0=Mean Motion in Orbits/Day. Decide which was given and
10144 REM   calculate the other, including drag corrections --
10146  IF N0 > .1 THEN 10150
10148  N0=SQR(G0 / (A0^3)) : GOTO 10154 : REM--Calculate Mean Motion
10150  A0=((G0 / (N0*N0)) ^ (1/3)) : REM--Calculate SMA
10152 REM--The following correct for drag in low-altitude satellites--
10154  N=N0 + 2*(T-T0)*N1 : REM--Mean Motion at T
10156  A =((G0 / (N *N )) ^ (1/3)) : REM--SMA at Epoch T
10160  E2=1-E0^2 : E1=SQR(E2) : Q0=M0/360 + K0 : REM--Q0=Initial orbit phase
10161  V0 = A*N*P1/43200! : REM  mean velocity ( km/sec )
10170  REM--Account for nodal effects due to lumpy gravity field due to the
10180    REM    flattened, oblate spheroidal, figure of the earth
10190   K2=9.95 * ((R0/A)^3.5) / (E2^2)
10200  REM--Update elements to current epoch & evaluate their SIN/COS's
10210   S1=SIN(I0*P0) : C1=COS(I0*P0) : REM--I0=Inclination (deg)
10220   O=O0 - (T-T0) * K2 * C1
10230    S0=SIN(O*P0) : C0=COS(O*P0) : REM--O=R.A.A.N. (degrees)
10240   W=W0 + (T-T0) * K2 * (2.5 * (C1^2) - .5)
10250    S2=SIN(W*P0) : C2=COS(W*P0) : REM--W=Argument of Perigee (degrees)
10260  REM--Set up coordinate rotation matrix for the current orbit
10270   C(1,1)= + (C2*C0) - (S2*S0*C1) : C(1,2)= - (S2*C0) - (C2*S0*C1)
10280   C(2,1)= + (C2*S0) + (S2*C0*C1) : C(2,2)= - (S2*S0) + (C2*C0*C1)
10290   C(3,1)= + (S2*S1)              : C(3,2)= + (C2*S1)  :  RETURN
10300 REM --------------------------
10310 REM  Here originally begins  FNM(T)
10320  REM--Function to evaluate M=MEAN ANOMALY in (0-2*PI) range,
10330    REM      K=Perigee Passage Kounter (=Orbit ##) & M9=modulo 256
10340    REM      orbital phase compatible with General Beacon telemetry.
10345   Q=M0/360.0 + N0*(T-T0) + N1*((T-T0)^2) : REM--REVISED TO INCLUDE DRAG
10346   K=INT(Q+.000001) : M9=INT((Q-K+.000001)*256) : M = (Q-K)*P2 
10350   K=K + K0 : Q = Q + K0 : RETURN
10360 REM --------------------------
10370 REM  Here originally begins  FNK(M)
10380  REM--Routine to solve KEPLER's equation given M & return
10390    REM      the Satellite's GEOCENTRIC coordinates.
10400   E=M + E0*SIN(M) + .5 * (E0^2) * SIN(2*M) : REM--Initial trial value
10410  REM--Iteration loop to solve trancendental Kepler's equation
10420   S3=SIN(E) : C3=COS(E) : R3=1 - E0*C3 : M1=E - E0*S3
10430   M5=M1-M : IF ABS(M5) < .000001 THEN 10450  ELSE E=E - M5/R3 : GOTO 10420
10440  REM--Now get satellite's XYZ coordinates
10450   X0=A*(C3-E0) : Y0=A*E1*S3 : R=A*R3 : REM--In the plane of the orbit
10451  VX0 =-V0*S3/R3 : VY0 =+V0*E1*C3/R3 :REM Velocity at XY plane ( km/sec)
10460  REM--Rotate from Orbit plane to INERTIAL CELESTIAL Coordinates.
10470  X1=X0*C(1,1)+Y0*C(1,2) : Y1=X0*C(2,1)+Y0*C(2,2) : Z1=X0*C(3,1)+Y0*C(3,2)
10471  VX1=VX0*C(1,1)+VY0*C(1,2):VY1=VX0*C(2,1)+VY0*C(2,2):VZ1=VX0*C(3,1)+VY0*C(3,2)
10480  REM--Rotate thru current GHA of ARIES, convert to GEOCENTRIC coordinates
10490   G7=(T+T7)*G1 + G2 : G7=(G7-INT(G7)) * P2 : S7=-SIN(G7) : C7=COS(G7)
10499   VX=+(VX1*C7)-(VY1*S7):VY=+(VX1*S7)+(VY1*C7):VZ=VZ1
10500   X= + (X1*C7) - (Y1*S7) : Y= + (X1*S7) + (Y1*C7) : Z=Z1 : RETURN
10510 REM --------------------------
10520 REM  Here originally begins  FNO(D)
10530  REM--Routine to evaluate OBSERVER's GEOCENTRIC coordinates,
10540   REM     where X-axis=GREENWICH, Y-axis=VU-land & Z-axis=North pole.
10550  L8=L9*P0 : S9=SIN(L8) : C9=COS(L8) : REM--Initial GEODETIC coordinates
10560   S8=SIN(-W9*P0) : C8=COS(W9*P0) : REM--W9=West Longitude
10570  REM--Following accounts for flattened oblate spheroidal earth
10580   R9=R0*(1 - (F/2) + (F/2) *COS(2*L8)) + H9/1000 : REM--H9=height(meters)
10590  REM--Following makes L8 be the GEOCENTRIC latitude
10600   L8=ATN( (1-F)^2 * S9/C9 ) : Z9=R9 * SIN(L8)
10609   VX9= -R9*COS(L8)*S8*P1*G1/43200! : VY9=+R9*COS(L8)*C8*P1*G1/43200! : VZ9=0!
10610   X9=R9 * COS(L8) * C8 : Y9=R9 * COS(L8) * S8 : RETURN
10620 REM --------------------------
10630 REM  Here originally begins  FNX(T)
10640  REM--Routine to extract all the parameters you might ever need.
10650  REM     First get vector from observer to satellite
10660   X5= (X-X9) : Y5= (Y-Y9) : Z5= (Z-Z9) : R5=SQR(X5^2 + Y5^2 + Z5^2)
10670  REM--Finite difference the range (R5) to get the velocity (R8)
10680  R8 = ((VX-VX9)*X5+(VY-VY9)*Y5+VZ*Z5)/R5 : REM relative velocity to stn.
10690   R6=R5 : T6=T+T7 : REM--& Save current range & time for next time thru
10700  REM--Now rotate into OBSERVER's LOCAL coordinates
10710    REM    where X8=North, Y8=East & Z8=Up (Left-Handed system!)
10720   Z8= + (X5*C8*C9) + (Y5*S8*C9) + (Z5*S9)
10730   X8= - (X5*C8*S9) - (Y5*S8*S9) + (Z5*C9) : Y8= + (Y5*C8) - (X5*S8)
10740   S5=Z8/R5 : C5=SQR(1-S5*S5) : E9=ATN(S5/C5) / P0 : REM--E9=Elevation
10750   REM--FNA resolves correct quadrant for A9=Azimuth
10752   X7=X8 : Y7=Y8 : GOSUB 10010 : A9=F8 / P0
10760   REM--W5=Sub-Satellite Point (SSP) W.Longitude
10762   X7=X : Y7=Y : GOSUB 10010 : W5=360-F8 / P0
10770   B5=Z/R : L5=ATN(B5 / (SQR(1 - B5^2))) / P0 : REM--L5=SSP Latitude
10780  RETURN : REM--Note R-R0=Satellite's altitude above mean spheroid.
20000 REM ----------------------------
20010 DATA "AO-10 #14129"
20020 DATA "Element set #793"
20030 DATA 92,022.23314193 ,0,0,0, 3675
20031      REM-Epoch date/time & Rev#
20040 DATA 10.1329 , 2.05909224 , 0
20041      REM-Mean Anom, Mean Motion/SMA
20050 DATA -7.2e-07 , 26.0458 , 0.6082608
20051      REM-Drag, Incl & Ecr
20060 DATA 312.2601 , 102.6682 ,  145.809
20061      REM-Arg Perg, RAAN & Freq
20100 REM ----------------------------
20210 DATA "UO-11 #14781"
20220 DATA "Element set #164"
20230 DATA 92,026.05108373 ,0,0,0, 42213
20231      REM-Epoch date/time & Rev#
20240 DATA 185.4659 , 14.68023864 , 0
20241      REM-Mean Anom, Mean Motion/SMA
20250 DATA 1.606e-05 , 97.8719 , 0.0011498
20251      REM-Drag, Incl & Ecr
20260 DATA 174.6669 , 67.5116 ,  145.826
20261      REM-Arg Perg, RAAN & Freq
20300 REM ----------------------------
20410 DATA "RS-10/11 #18129"
20420 DATA "Element set #23"
20430 DATA 92,030.70143667 ,0,0,0, 23079
20431      REM-Epoch date/time & Rev#
20440 DATA 83.6218 , 13.72251488 , 0
20441      REM-Mean Anom, Mean Motion/SMA
20450 DATA 1.75e-06 , 82.9240 , 0.0010387
20451      REM-Drag, Incl & Ecr
20460 DATA 276.3755 , 250.7185 ,  29.4
20461      REM-Arg Perg, RAAN & Freq
20500 REM  ---------------------------
20610 DATA "AO-13 #19216"
20620 DATA "Element set #335"
20630 DATA 92,023.80911430 ,0,0,0, 2765
20631      REM-Epoch date/time & Rev#
20640 DATA 12.8323 , 2.09702302 , 0
20641      REM-Mean Anom, Mean Motion/SMA
20650 DATA 1.84e-06 , 56.6439 , 0.7279257
20651      REM-Drag, Incl & Ecr
20660 DATA 276.1153 , 47.0471 ,  145.812
20661      REM-Arg Perg, RAAN & Freq
20700 REM ----------------------------
20810 DATA "FO-20 #20480"
20820 DATA "Element set #290"
20830 DATA 92,014.83021608 ,0,0,0, 9070
20831      REM-Epoch date/time & Rev#
20840 DATA 177.7899 , 12.83199781 , 0
20841      REM-Mean Anom, Mean Motion/SMA
20850 DATA 1.9e-07 , 99.0604 , 0.0540542
20851      REM-Drag, Incl & Ecr
20860 DATA 182.0931 , 322.1087 ,  435.12
20861      REM-Arg Perg, RAAN & Freq
20900 REM ----------------------------
21010 DATA "AO-21 #21087"
21020 DATA "Element set #224"
21030 DATA 92,031.10781901 ,0,0,0, 5033
21031      REM-Epoch date/time & Rev#
21040 DATA 13.7385 , 13.74449364 , 0
21041      REM-Mean Anom, Mean Motion/SMA
21050 DATA 1.21e-06 , 82.9428 , 0.0036208
21051      REM-Drag, Incl & Ecr
21060 DATA 346.2777 , 65.1103 ,  100
21061      REM-Arg Perg, RAAN & Freq
21100 REM ----------------------------
21210 DATA "RS-12/13 #21089"
21220 DATA "Element set #192"
21230 DATA 92,028.23275278 ,0,0,0, 4904
21231      REM-Epoch date/time & Rev#
21240 DATA 344.7696 , 13.73960032 , 0
21241      REM-Mean Anom, Mean Motion/SMA
21250 DATA 8.9e-07 , 82.9268 , 0.0030589
21251      REM-Drag, Incl & Ecr
21260 DATA 15.4387 , 297.2705 ,  100
21261      REM-Arg Perg, RAAN & Freq
21300 REM ----------------------------
21410 DATA "UO-14 #20437"
21420 DATA "Element set #498"
21430 DATA 92,030.73628348 ,0,0,0, 10549
21431      REM-Epoch date/time & Rev#
21440 DATA 139.8845 , 14.29465311 , 0
21441      REM-Mean Anom, Mean Motion/SMA
21450 DATA 7.19e-06 , 98.6465 , 0.0010502
21451      REM-Drag, Incl & Ecr
21460 DATA 220.1554 , 113.6944 ,  100
21461      REM-Arg Perg, RAAN & Freq
21610 DATA "AO-16 #20439"
21620 DATA "Element set #395"
21630 DATA 92,030.47883211 ,0,0,0, 10546
21631      REM-Epoch date/time & Rev#
21640 DATA 138.5253 , 14.29536324 , 0
21641      REM-Mean Anom, Mean Motion/SMA
21650 DATA 5.37e-06 , 98.6563 , 0.0010019
21651      REM-Drag, Incl & Ecr
21660 DATA 221.5163 , 113.9344 ,  100
21661      REM-Arg Perg, RAAN & Freq
21700 REM ----------------------------
21810 DATA "DO-17 #20440"
21820 DATA "Element set #395"
21830 DATA 92,029.53363050 ,0,0,0, 10533
21831      REM-Epoch date/time & Rev#
21840 DATA 135.9152 , 14.29646954 , 0
21841      REM-Mean Anom, Mean Motion/SMA
21850 DATA 5.96e-06 , 98.6563 , 0.0010218
21851      REM-Drag, Incl & Ecr
21860 DATA 224.1207 , 113.0823 ,  100
21861      REM-Arg Perg, RAAN & Freq
21900 REM ----------------------------
22010 DATA "WO-18 #20441"
22020 DATA "Element set #394"
22030 DATA 92,030.75912456 ,0,0,0, 10551
22031      REM-Epoch date/time & Rev#
22040 DATA 138.9564 , 14.29658556 , 0
22041      REM-Mean Anom, Mean Motion/SMA
22050 DATA 5.18e-06 , 98.6562 , 0.0010661
22051      REM-Drag, Incl & Ecr
22060 DATA 221.0816 , 114.3504 ,  100
22061      REM-Arg Perg, RAAN & Freq
22200 REM ----------------------------
22210 DATA "LO-19 #20442"
22220 DATA "Element set #395"
22230 DATA 92,028.69152463 ,0,0,0, 10522
22231      REM-Epoch date/time & Rev#
22240 DATA 132.8748 , 14.29737597 , 0
22241      REM-Mean Anom, Mean Motion/SMA
22250 DATA 5.48e-06 , 98.6561 , 0.0010994
22251      REM-Drag, Incl & Ecr
22260 DATA 227.1510 , 112.3854 ,  100
22261      REM-Arg Perg, RAAN & Freq
22400 REM ----------------------------
22410 DATA "UO-22 #21575"
22420 DATA "Element set #101"
22430 DATA 92,028.21255279 ,0,0,0, 2801
22431      REM-Epoch date/time & Rev#
22440 DATA 348.7676 , 14.36431733 , 0
22441      REM-Mean Anom, Mean Motion/SMA
22450 DATA 5.86e-06 , 98.5197 , 0.0008184
22451      REM-Drag, Incl & Ecr
22460 DATA 11.3704 , 105.1186 ,  435.12
22461      REM-Arg Perg, RAAN & Freq
22500 REM ----------------------------
22600 DATA "END" : END
