T$="B.GHAA(0)": REM     GMST of 0000 utc Calculator.  Based on
REM                   1984 Astronomical  Almanac Pages B6 & S13.
REM
REM Version 1.1   Last modified 1985 May 08 by JRM
REM
REM                 (C)1991 J R Miller G3RUH
REM
REM GMST =24110.54841 + TU*(8640184.812866+TU*(0.093104+TU*(-6.2E-6)))
REM (seconds).
REM
REM  where Tu is the interval of time, (measured in Julian centuries
REM  of 36525 days of universal time), elapsed since the epoch
REM  2000 Jan 1.5   i.e TU = (JD - 2451545.0)/36525
REM
FI%=&80F: FF%=&2060F: REM  Print formats;  #########   #.######
:
PRINT"Program ";T$;"    GHAA for Year yyyy Jan 0.0     (C)1991 G3RUH"
PRINT
PRINT"           YEAR      GHAA deg        GMST rev"
PRINT"         -------------------------------------"
FOR YR = 1978 TO 2010
DD = FND0(YR)-(FND0(2000)+1.5): REM Days since 2000 Jan 1.5
TU = DD/36525
G1 = TU*(184.812866+TU*(0.093104+TU*(-6.2E-6)))/86400 :REM Small part
G2 = TU*100                                    :REM Big part
G3 = 0.2790572733                              :REM 24110.54841/86400
GMST = G1+G2+G3: GMST=GMST-INT(GMST): GHAA=GMST*360
@%=FI%: PRINT YR;: @%=FF%: PRINT GHAA,GMST
NEXT
STOP
DEF FND0(YR)=INT((YR-1)*365.25)
