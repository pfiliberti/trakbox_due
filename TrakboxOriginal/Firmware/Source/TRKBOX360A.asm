/* Functions written in assembler and derived from original trk350b.rom image */
/* These functions are for the original 8031 version at normal clock speed    */
/* (c) 2022 Peter Filiberti Released under MIT License, located here:         */
/* https://github.com/pfiliberti/trakbox_due/blob/20df6d80123f2a027c8e4ec960ef88d0ae74d38b/TrakboxOriginal/Firmware/Source/LICENSE */


/* TrakBox Functions in assembly */
/* spout = rom addr c961 */
spout	mov a,r7
	mov r5, #09h
	setb c
	clr tcon.5
wl	jnb tcon.5, wl
	setb c
	clr p1.7
	clr tcon.5
w3	rrc a
w2	jnb tcon.5, w4
	mov p1.7,c
	clr tcon.5
	djnz r5, w3
w4	jnb tcon.5, w4
w5	jnb tcon.5, w5
	ret
	
	
/* spout2 = rom addr c980 */	
spout2	mov a,r7
		mov r5, $09h
		setb c
		clr tcon.5
w1		jnb tcon.5, w1
		setb c
		clr p1.5
		clr tcon.5
w2		rrc a
w3		jnb tcon.5,w3
		mov p1.5,c
		clr tcon.5
		djnz r5,w2
w4		jnb tcon.5,w4
w5		jnb tcon.5,w5
		ret
	
/* icom_wait = rom ca08 */

icom_wait	mov r6, #19h
WL1 mov r5, #fah
WL2	nop
	nop
	djnz r5, WL2
	djnz r6, WL1
	mov r6, #0eh
Wl3	mov r5, #0ah
WL4	clr tcon.5
WL5	jnb tcon.5,WL5
	djnz r5, WL4
	djnz r6, WL3
	ret
	