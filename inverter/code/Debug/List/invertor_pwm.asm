
;CodeVisionAVR C Compiler V3.12 Advanced
;(C) Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
;http://www.hpinfotech.com

;Build configuration    : Debug
;Chip type              : ATmega328P
;Program type           : Application
;Clock frequency        : 7,680000 MHz
;Memory model           : Small
;Optimize for           : Size
;(s)printf features     : int, width
;(s)scanf features      : int, width
;External RAM size      : 0
;Data Stack size        : 512 byte(s)
;Heap size              : 0 byte(s)
;Promote 'char' to 'int': Yes
;'char' is unsigned     : Yes
;8 bit enums            : Yes
;Global 'const' stored in FLASH: Yes
;Enhanced function parameter passing: Yes
;Enhanced core instructions: On
;Automatic register allocation for global variables: On
;Smart register allocation: On

	#define _MODEL_SMALL_

	#pragma AVRPART ADMIN PART_NAME ATmega328P
	#pragma AVRPART MEMORY PROG_FLASH 32768
	#pragma AVRPART MEMORY EEPROM 1024
	#pragma AVRPART MEMORY INT_SRAM SIZE 2048
	#pragma AVRPART MEMORY INT_SRAM START_ADDR 0x100

	#define CALL_SUPPORTED 1

	.LISTMAC
	.EQU EERE=0x0
	.EQU EEWE=0x1
	.EQU EEMWE=0x2
	.EQU UDRE=0x5
	.EQU RXC=0x7
	.EQU EECR=0x1F
	.EQU EEDR=0x20
	.EQU EEARL=0x21
	.EQU EEARH=0x22
	.EQU SPSR=0x2D
	.EQU SPDR=0x2E
	.EQU SMCR=0x33
	.EQU MCUSR=0x34
	.EQU MCUCR=0x35
	.EQU WDTCSR=0x60
	.EQU UCSR0A=0xC0
	.EQU UDR0=0xC6
	.EQU SPL=0x3D
	.EQU SPH=0x3E
	.EQU SREG=0x3F
	.EQU GPIOR0=0x1E

	.DEF R0X0=R0
	.DEF R0X1=R1
	.DEF R0X2=R2
	.DEF R0X3=R3
	.DEF R0X4=R4
	.DEF R0X5=R5
	.DEF R0X6=R6
	.DEF R0X7=R7
	.DEF R0X8=R8
	.DEF R0X9=R9
	.DEF R0XA=R10
	.DEF R0XB=R11
	.DEF R0XC=R12
	.DEF R0XD=R13
	.DEF R0XE=R14
	.DEF R0XF=R15
	.DEF R0X10=R16
	.DEF R0X11=R17
	.DEF R0X12=R18
	.DEF R0X13=R19
	.DEF R0X14=R20
	.DEF R0X15=R21
	.DEF R0X16=R22
	.DEF R0X17=R23
	.DEF R0X18=R24
	.DEF R0X19=R25
	.DEF R0X1A=R26
	.DEF R0X1B=R27
	.DEF R0X1C=R28
	.DEF R0X1D=R29
	.DEF R0X1E=R30
	.DEF R0X1F=R31

	.EQU __SRAM_START=0x0100
	.EQU __SRAM_END=0x08FF
	.EQU __DSTACK_SIZE=0x0200
	.EQU __HEAP_SIZE=0x0000
	.EQU __CLEAR_SRAM_SIZE=__SRAM_END-__SRAM_START+1

	.MACRO __CPD1N
	CPI  R30,LOW(@0)
	LDI  R26,HIGH(@0)
	CPC  R31,R26
	LDI  R26,BYTE3(@0)
	CPC  R22,R26
	LDI  R26,BYTE4(@0)
	CPC  R23,R26
	.ENDM

	.MACRO __CPD2N
	CPI  R26,LOW(@0)
	LDI  R30,HIGH(@0)
	CPC  R27,R30
	LDI  R30,BYTE3(@0)
	CPC  R24,R30
	LDI  R30,BYTE4(@0)
	CPC  R25,R30
	.ENDM

	.MACRO __CPWRR
	CP   R@0,R@2
	CPC  R@1,R@3
	.ENDM

	.MACRO __CPWRN
	CPI  R@0,LOW(@2)
	LDI  R30,HIGH(@2)
	CPC  R@1,R30
	.ENDM

	.MACRO __ADDB1MN
	SUBI R30,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDB2MN
	SUBI R26,LOW(-@0-(@1))
	.ENDM

	.MACRO __ADDW1MN
	SUBI R30,LOW(-@0-(@1))
	SBCI R31,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW2MN
	SUBI R26,LOW(-@0-(@1))
	SBCI R27,HIGH(-@0-(@1))
	.ENDM

	.MACRO __ADDW1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1FN
	SUBI R30,LOW(-2*@0-(@1))
	SBCI R31,HIGH(-2*@0-(@1))
	SBCI R22,BYTE3(-2*@0-(@1))
	.ENDM

	.MACRO __ADDD1N
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	SBCI R22,BYTE3(-@0)
	SBCI R23,BYTE4(-@0)
	.ENDM

	.MACRO __ADDD2N
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	SBCI R24,BYTE3(-@0)
	SBCI R25,BYTE4(-@0)
	.ENDM

	.MACRO __SUBD1N
	SUBI R30,LOW(@0)
	SBCI R31,HIGH(@0)
	SBCI R22,BYTE3(@0)
	SBCI R23,BYTE4(@0)
	.ENDM

	.MACRO __SUBD2N
	SUBI R26,LOW(@0)
	SBCI R27,HIGH(@0)
	SBCI R24,BYTE3(@0)
	SBCI R25,BYTE4(@0)
	.ENDM

	.MACRO __ANDBMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ANDWMNN
	LDS  R30,@0+(@1)
	ANDI R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ANDI R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ANDD1N
	ANDI R30,LOW(@0)
	ANDI R31,HIGH(@0)
	ANDI R22,BYTE3(@0)
	ANDI R23,BYTE4(@0)
	.ENDM

	.MACRO __ANDD2N
	ANDI R26,LOW(@0)
	ANDI R27,HIGH(@0)
	ANDI R24,BYTE3(@0)
	ANDI R25,BYTE4(@0)
	.ENDM

	.MACRO __ORBMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	.ENDM

	.MACRO __ORWMNN
	LDS  R30,@0+(@1)
	ORI  R30,LOW(@2)
	STS  @0+(@1),R30
	LDS  R30,@0+(@1)+1
	ORI  R30,HIGH(@2)
	STS  @0+(@1)+1,R30
	.ENDM

	.MACRO __ORD1N
	ORI  R30,LOW(@0)
	ORI  R31,HIGH(@0)
	ORI  R22,BYTE3(@0)
	ORI  R23,BYTE4(@0)
	.ENDM

	.MACRO __ORD2N
	ORI  R26,LOW(@0)
	ORI  R27,HIGH(@0)
	ORI  R24,BYTE3(@0)
	ORI  R25,BYTE4(@0)
	.ENDM

	.MACRO __DELAY_USB
	LDI  R24,LOW(@0)
__DELAY_USB_LOOP:
	DEC  R24
	BRNE __DELAY_USB_LOOP
	.ENDM

	.MACRO __DELAY_USW
	LDI  R24,LOW(@0)
	LDI  R25,HIGH(@0)
__DELAY_USW_LOOP:
	SBIW R24,1
	BRNE __DELAY_USW_LOOP
	.ENDM

	.MACRO __GETD1S
	LDD  R30,Y+@0
	LDD  R31,Y+@0+1
	LDD  R22,Y+@0+2
	LDD  R23,Y+@0+3
	.ENDM

	.MACRO __GETD2S
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	LDD  R24,Y+@0+2
	LDD  R25,Y+@0+3
	.ENDM

	.MACRO __PUTD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R31
	STD  Y+@0+2,R22
	STD  Y+@0+3,R23
	.ENDM

	.MACRO __PUTD2S
	STD  Y+@0,R26
	STD  Y+@0+1,R27
	STD  Y+@0+2,R24
	STD  Y+@0+3,R25
	.ENDM

	.MACRO __PUTDZ2
	STD  Z+@0,R26
	STD  Z+@0+1,R27
	STD  Z+@0+2,R24
	STD  Z+@0+3,R25
	.ENDM

	.MACRO __CLRD1S
	STD  Y+@0,R30
	STD  Y+@0+1,R30
	STD  Y+@0+2,R30
	STD  Y+@0+3,R30
	.ENDM

	.MACRO __POINTB1MN
	LDI  R30,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW1MN
	LDI  R30,LOW(@0+(@1))
	LDI  R31,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTD1M
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __POINTW1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	LDI  R22,BYTE3(2*@0+(@1))
	LDI  R23,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTB2MN
	LDI  R26,LOW(@0+(@1))
	.ENDM

	.MACRO __POINTW2MN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	.ENDM

	.MACRO __POINTW2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	.ENDM

	.MACRO __POINTD2FN
	LDI  R26,LOW(2*@0+(@1))
	LDI  R27,HIGH(2*@0+(@1))
	LDI  R24,BYTE3(2*@0+(@1))
	LDI  R25,BYTE4(2*@0+(@1))
	.ENDM

	.MACRO __POINTBRM
	LDI  R@0,LOW(@1)
	.ENDM

	.MACRO __POINTWRM
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __POINTBRMN
	LDI  R@0,LOW(@1+(@2))
	.ENDM

	.MACRO __POINTWRMN
	LDI  R@0,LOW(@2+(@3))
	LDI  R@1,HIGH(@2+(@3))
	.ENDM

	.MACRO __POINTWRFN
	LDI  R@0,LOW(@2*2+(@3))
	LDI  R@1,HIGH(@2*2+(@3))
	.ENDM

	.MACRO __GETD1N
	LDI  R30,LOW(@0)
	LDI  R31,HIGH(@0)
	LDI  R22,BYTE3(@0)
	LDI  R23,BYTE4(@0)
	.ENDM

	.MACRO __GETD2N
	LDI  R26,LOW(@0)
	LDI  R27,HIGH(@0)
	LDI  R24,BYTE3(@0)
	LDI  R25,BYTE4(@0)
	.ENDM

	.MACRO __GETB1MN
	LDS  R30,@0+(@1)
	.ENDM

	.MACRO __GETB1HMN
	LDS  R31,@0+(@1)
	.ENDM

	.MACRO __GETW1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	.ENDM

	.MACRO __GETD1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	LDS  R22,@0+(@1)+2
	LDS  R23,@0+(@1)+3
	.ENDM

	.MACRO __GETBRMN
	LDS  R@0,@1+(@2)
	.ENDM

	.MACRO __GETWRMN
	LDS  R@0,@2+(@3)
	LDS  R@1,@2+(@3)+1
	.ENDM

	.MACRO __GETWRZ
	LDD  R@0,Z+@2
	LDD  R@1,Z+@2+1
	.ENDM

	.MACRO __GETD2Z
	LDD  R26,Z+@0
	LDD  R27,Z+@0+1
	LDD  R24,Z+@0+2
	LDD  R25,Z+@0+3
	.ENDM

	.MACRO __GETB2MN
	LDS  R26,@0+(@1)
	.ENDM

	.MACRO __GETW2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	.ENDM

	.MACRO __GETD2MN
	LDS  R26,@0+(@1)
	LDS  R27,@0+(@1)+1
	LDS  R24,@0+(@1)+2
	LDS  R25,@0+(@1)+3
	.ENDM

	.MACRO __PUTB1MN
	STS  @0+(@1),R30
	.ENDM

	.MACRO __PUTW1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	.ENDM

	.MACRO __PUTD1MN
	STS  @0+(@1),R30
	STS  @0+(@1)+1,R31
	STS  @0+(@1)+2,R22
	STS  @0+(@1)+3,R23
	.ENDM

	.MACRO __PUTB1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRB
	.ENDM

	.MACRO __PUTW1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRW
	.ENDM

	.MACRO __PUTD1EN
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMWRD
	.ENDM

	.MACRO __PUTBR0MN
	STS  @0+(@1),R0
	.ENDM

	.MACRO __PUTBMRN
	STS  @0+(@1),R@2
	.ENDM

	.MACRO __PUTWMRN
	STS  @0+(@1),R@2
	STS  @0+(@1)+1,R@3
	.ENDM

	.MACRO __PUTBZR
	STD  Z+@1,R@0
	.ENDM

	.MACRO __PUTWZR
	STD  Z+@2,R@0
	STD  Z+@2+1,R@1
	.ENDM

	.MACRO __GETW1R
	MOV  R30,R@0
	MOV  R31,R@1
	.ENDM

	.MACRO __GETW2R
	MOV  R26,R@0
	MOV  R27,R@1
	.ENDM

	.MACRO __GETWRN
	LDI  R@0,LOW(@2)
	LDI  R@1,HIGH(@2)
	.ENDM

	.MACRO __PUTW1R
	MOV  R@0,R30
	MOV  R@1,R31
	.ENDM

	.MACRO __PUTW2R
	MOV  R@0,R26
	MOV  R@1,R27
	.ENDM

	.MACRO __ADDWRN
	SUBI R@0,LOW(-@2)
	SBCI R@1,HIGH(-@2)
	.ENDM

	.MACRO __ADDWRR
	ADD  R@0,R@2
	ADC  R@1,R@3
	.ENDM

	.MACRO __SUBWRN
	SUBI R@0,LOW(@2)
	SBCI R@1,HIGH(@2)
	.ENDM

	.MACRO __SUBWRR
	SUB  R@0,R@2
	SBC  R@1,R@3
	.ENDM

	.MACRO __ANDWRN
	ANDI R@0,LOW(@2)
	ANDI R@1,HIGH(@2)
	.ENDM

	.MACRO __ANDWRR
	AND  R@0,R@2
	AND  R@1,R@3
	.ENDM

	.MACRO __ORWRN
	ORI  R@0,LOW(@2)
	ORI  R@1,HIGH(@2)
	.ENDM

	.MACRO __ORWRR
	OR   R@0,R@2
	OR   R@1,R@3
	.ENDM

	.MACRO __EORWRR
	EOR  R@0,R@2
	EOR  R@1,R@3
	.ENDM

	.MACRO __GETWRS
	LDD  R@0,Y+@2
	LDD  R@1,Y+@2+1
	.ENDM

	.MACRO __PUTBSR
	STD  Y+@1,R@0
	.ENDM

	.MACRO __PUTWSR
	STD  Y+@2,R@0
	STD  Y+@2+1,R@1
	.ENDM

	.MACRO __MOVEWRR
	MOV  R@0,R@2
	MOV  R@1,R@3
	.ENDM

	.MACRO __INWR
	IN   R@0,@2
	IN   R@1,@2+1
	.ENDM

	.MACRO __OUTWR
	OUT  @2+1,R@1
	OUT  @2,R@0
	.ENDM

	.MACRO __CALL1MN
	LDS  R30,@0+(@1)
	LDS  R31,@0+(@1)+1
	ICALL
	.ENDM

	.MACRO __CALL1FN
	LDI  R30,LOW(2*@0+(@1))
	LDI  R31,HIGH(2*@0+(@1))
	CALL __GETW1PF
	ICALL
	.ENDM

	.MACRO __CALL2EN
	PUSH R26
	PUSH R27
	LDI  R26,LOW(@0+(@1))
	LDI  R27,HIGH(@0+(@1))
	CALL __EEPROMRDW
	POP  R27
	POP  R26
	ICALL
	.ENDM

	.MACRO __CALL2EX
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	CALL __EEPROMRDD
	ICALL
	.ENDM

	.MACRO __GETW1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1STACK
	IN   R30,SPL
	IN   R31,SPH
	ADIW R30,@0+1
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z
	MOVW R30,R0
	.ENDM

	.MACRO __NBST
	BST  R@0,@1
	IN   R30,SREG
	LDI  R31,0x40
	EOR  R30,R31
	OUT  SREG,R30
	.ENDM


	.MACRO __PUTB1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SN
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNS
	LDD  R26,Y+@0
	LDD  R27,Y+@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMN
	LDS  R26,@0
	LDS  R27,@0+1
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1PMNS
	LDS  R26,@0
	LDS  R27,@0+1
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RN
	MOVW R26,R@0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RNS
	MOVW R26,R@0
	ADIW R26,@1
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RNS
	MOVW R26,R@0
	ADIW R26,@1
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RON
	MOV  R26,R@0
	MOV  R27,R@1
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	CALL __PUTDP1
	.ENDM

	.MACRO __PUTB1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X,R30
	.ENDM

	.MACRO __PUTW1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1RONS
	MOV  R26,R@0
	MOV  R27,R@1
	ADIW R26,@2
	CALL __PUTDP1
	.ENDM


	.MACRO __GETB1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R30,Z
	.ENDM

	.MACRO __GETB1HSX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	.ENDM

	.MACRO __GETW1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R31,Z
	MOV  R30,R0
	.ENDM

	.MACRO __GETD1SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R0,Z+
	LD   R1,Z+
	LD   R22,Z+
	LD   R23,Z
	MOVW R30,R0
	.ENDM

	.MACRO __GETB2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R26,X
	.ENDM

	.MACRO __GETW2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	.ENDM

	.MACRO __GETD2SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R1,X+
	LD   R24,X+
	LD   R25,X
	MOVW R26,R0
	.ENDM

	.MACRO __GETBRSX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	LD   R@0,Z
	.ENDM

	.MACRO __GETWRSX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	LD   R@0,Z+
	LD   R@1,Z
	.ENDM

	.MACRO __GETBRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	LD   R@0,X
	.ENDM

	.MACRO __GETWRSX2
	MOVW R26,R28
	SUBI R26,LOW(-@2)
	SBCI R27,HIGH(-@2)
	LD   R@0,X+
	LD   R@1,X
	.ENDM

	.MACRO __LSLW8SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	LD   R31,Z
	CLR  R30
	.ENDM

	.MACRO __PUTB1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __CLRW1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __CLRD1SX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	ST   X+,R30
	ST   X+,R30
	ST   X+,R30
	ST   X,R30
	.ENDM

	.MACRO __PUTB2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z,R26
	.ENDM

	.MACRO __PUTW2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z,R27
	.ENDM

	.MACRO __PUTD2SX
	MOVW R30,R28
	SUBI R30,LOW(-@0)
	SBCI R31,HIGH(-@0)
	ST   Z+,R26
	ST   Z+,R27
	ST   Z+,R24
	ST   Z,R25
	.ENDM

	.MACRO __PUTBSRX
	MOVW R30,R28
	SUBI R30,LOW(-@1)
	SBCI R31,HIGH(-@1)
	ST   Z,R@0
	.ENDM

	.MACRO __PUTWSRX
	MOVW R30,R28
	SUBI R30,LOW(-@2)
	SBCI R31,HIGH(-@2)
	ST   Z+,R@0
	ST   Z,R@1
	.ENDM

	.MACRO __PUTB1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X,R30
	.ENDM

	.MACRO __PUTW1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X,R31
	.ENDM

	.MACRO __PUTD1SNX
	MOVW R26,R28
	SUBI R26,LOW(-@0)
	SBCI R27,HIGH(-@0)
	LD   R0,X+
	LD   R27,X
	MOV  R26,R0
	SUBI R26,LOW(-@1)
	SBCI R27,HIGH(-@1)
	ST   X+,R30
	ST   X+,R31
	ST   X+,R22
	ST   X,R23
	.ENDM

	.MACRO __MULBRR
	MULS R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRRU
	MUL  R@0,R@1
	MOVW R30,R0
	.ENDM

	.MACRO __MULBRR0
	MULS R@0,R@1
	.ENDM

	.MACRO __MULBRRU0
	MUL  R@0,R@1
	.ENDM

	.MACRO __MULBNWRU
	LDI  R26,@2
	MUL  R26,R@0
	MOVW R30,R0
	MUL  R26,R@1
	ADD  R31,R0
	.ENDM

;NAME DEFINITIONS FOR GLOBAL VARIABLES ALLOCATED TO REGISTERS
	.DEF _phase_b_step=R4
	.DEF _phase_c_step=R3
	.DEF _amplitude=R6
	.DEF _frequency=R7
	.DEF _frequency_msb=R8

;GPIOR0 INITIALIZATION VALUE
	.EQU __GPIOR0_INIT=0x00

	.CSEG
	.ORG 0x00

;START OF CODE MARKER
__START_OF_CODE:

;INTERRUPT VECTORS
	JMP  __RESET
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  _timer1_compa_isr
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00
	JMP  0x00

_sinus_table:
	.DB  0x0,0x0,0x0,0x0,0x29,0x5C,0x8F,0x3C
	.DB  0x29,0x5C,0xF,0x3D,0xF4,0xFD,0x54,0x3D
	.DB  0x4D,0xF3,0x8E,0x3D,0xE,0x2D,0xB2,0x3D
	.DB  0x3D,0xA,0xD7,0x3D,0x23,0xDB,0xF9,0x3D
	.DB  0x4,0x56,0xE,0x3E,0x77,0xBE,0x1F,0x3E
	.DB  0xE,0x2D,0x32,0x3E,0x81,0x95,0x43,0x3E
	.DB  0xF4,0xFD,0x54,0x3E,0x66,0x66,0x66,0x3E
	.DB  0xD9,0xCE,0x77,0x3E,0xA6,0x9B,0x84,0x3E
	.DB  0xDF,0x4F,0x8D,0x3E,0x6,0x81,0x95,0x3E
	.DB  0x3F,0x35,0x9E,0x3E,0x79,0xE9,0xA6,0x3E
	.DB  0xA0,0x1A,0xAF,0x3E,0xC7,0x4B,0xB7,0x3E
	.DB  0x0,0x0,0xC0,0x3E,0x27,0x31,0xC8,0x3E
	.DB  0x4E,0x62,0xD0,0x3E,0x75,0x93,0xD8,0x3E
	.DB  0x89,0x41,0xE0,0x3E,0xB0,0x72,0xE8,0x3E
	.DB  0xC5,0x20,0xF0,0x3E,0xEC,0x51,0xF8,0x3E
	.DB  0x0,0x0,0x0,0x3F,0xA,0xD7,0x3,0x3F
	.DB  0x14,0xAE,0x7,0x3F,0x1F,0x85,0xB,0x3F
	.DB  0xA0,0x1A,0xF,0x3F,0xAA,0xF1,0x12,0x3F
	.DB  0x2B,0x87,0x16,0x3F,0xAC,0x1C,0x1A,0x3F
	.DB  0x2D,0xB2,0x1D,0x3F,0x25,0x6,0x21,0x3F
	.DB  0xA6,0x9B,0x24,0x3F,0x9E,0xEF,0x27,0x3F
	.DB  0x96,0x43,0x2B,0x3F,0x8D,0x97,0x2E,0x3F
	.DB  0x85,0xEB,0x31,0x3F,0xF4,0xFD,0x34,0x3F
	.DB  0x62,0x10,0x38,0x3F,0xD1,0x22,0x3B,0x3F
	.DB  0x3F,0x35,0x3E,0x3F,0xAE,0x47,0x41,0x3F
	.DB  0x93,0x18,0x44,0x3F,0x79,0xE9,0x46,0x3F
	.DB  0x5E,0xBA,0x49,0x3F,0x44,0x8B,0x4C,0x3F
	.DB  0xA0,0x1A,0x4F,0x3F,0xFC,0xA9,0x51,0x3F
	.DB  0x58,0x39,0x54,0x3F,0xB4,0xC8,0x56,0x3F
	.DB  0x87,0x16,0x59,0x3F,0x5A,0x64,0x5B,0x3F
	.DB  0x2D,0xB2,0x5D,0x3F,0x0,0x0,0x60,0x3F
	.DB  0x4A,0xC,0x62,0x3F,0x93,0x18,0x64,0x3F
	.DB  0xDD,0x24,0x66,0x3F,0x9E,0xEF,0x67,0x3F
	.DB  0xE7,0xFB,0x69,0x3F,0xA8,0xC6,0x6B,0x3F
	.DB  0xDF,0x4F,0x6D,0x3F,0xA0,0x1A,0x6F,0x3F
	.DB  0xD7,0xA3,0x70,0x3F,0xE,0x2D,0x72,0x3F
	.DB  0xBC,0x74,0x73,0x3F,0x6A,0xBC,0x74,0x3F
	.DB  0x19,0x4,0x76,0x3F,0xC7,0x4B,0x77,0x3F
	.DB  0xEC,0x51,0x78,0x3F,0x10,0x58,0x79,0x3F
	.DB  0x35,0x5E,0x7A,0x3F,0x5A,0x64,0x7B,0x3F
	.DB  0xF6,0x28,0x7C,0x3F,0x91,0xED,0x7C,0x3F
	.DB  0xA4,0x70,0x7D,0x3F,0x3F,0x35,0x7E,0x3F
	.DB  0x52,0xB8,0x7E,0x3F,0xDB,0xF9,0x7E,0x3F
	.DB  0xEE,0x7C,0x7F,0x3F,0x77,0xBE,0x7F,0x3F
	.DB  0x77,0xBE,0x7F,0x3F,0x0,0x0,0x80,0x3F
	.DB  0x0,0x0,0x80,0x3F,0x0,0x0,0x80,0x3F
	.DB  0x77,0xBE,0x7F,0x3F,0x77,0xBE,0x7F,0x3F
	.DB  0xEE,0x7C,0x7F,0x3F,0xDB,0xF9,0x7E,0x3F
	.DB  0x52,0xB8,0x7E,0x3F,0x3F,0x35,0x7E,0x3F
	.DB  0xA4,0x70,0x7D,0x3F,0x91,0xED,0x7C,0x3F
	.DB  0xF6,0x28,0x7C,0x3F,0x5A,0x64,0x7B,0x3F
	.DB  0x35,0x5E,0x7A,0x3F,0x10,0x58,0x79,0x3F
	.DB  0xEC,0x51,0x78,0x3F,0xC7,0x4B,0x77,0x3F
	.DB  0x19,0x4,0x76,0x3F,0x6A,0xBC,0x74,0x3F
	.DB  0xBC,0x74,0x73,0x3F,0xE,0x2D,0x72,0x3F
	.DB  0xD7,0xA3,0x70,0x3F,0xA0,0x1A,0x6F,0x3F
	.DB  0xDF,0x4F,0x6D,0x3F,0xA8,0xC6,0x6B,0x3F
	.DB  0xE7,0xFB,0x69,0x3F,0x9E,0xEF,0x67,0x3F
	.DB  0xDD,0x24,0x66,0x3F,0x93,0x18,0x64,0x3F
	.DB  0x4A,0xC,0x62,0x3F,0x0,0x0,0x60,0x3F
	.DB  0x2D,0xB2,0x5D,0x3F,0x5A,0x64,0x5B,0x3F
	.DB  0x87,0x16,0x59,0x3F,0xB4,0xC8,0x56,0x3F
	.DB  0x58,0x39,0x54,0x3F,0xFC,0xA9,0x51,0x3F
	.DB  0xA0,0x1A,0x4F,0x3F,0x44,0x8B,0x4C,0x3F
	.DB  0x5E,0xBA,0x49,0x3F,0x79,0xE9,0x46,0x3F
	.DB  0x93,0x18,0x44,0x3F,0xAE,0x47,0x41,0x3F
	.DB  0x3F,0x35,0x3E,0x3F,0xD1,0x22,0x3B,0x3F
	.DB  0x62,0x10,0x38,0x3F,0xF4,0xFD,0x34,0x3F
	.DB  0x85,0xEB,0x31,0x3F,0x8D,0x97,0x2E,0x3F
	.DB  0x96,0x43,0x2B,0x3F,0x9E,0xEF,0x27,0x3F
	.DB  0xA6,0x9B,0x24,0x3F,0x25,0x6,0x21,0x3F
	.DB  0x2D,0xB2,0x1D,0x3F,0xAC,0x1C,0x1A,0x3F
	.DB  0x2B,0x87,0x16,0x3F,0xAA,0xF1,0x12,0x3F
	.DB  0xA0,0x1A,0xF,0x3F,0x1F,0x85,0xB,0x3F
	.DB  0x14,0xAE,0x7,0x3F,0xA,0xD7,0x3,0x3F
	.DB  0x0,0x0,0x0,0x3F,0xEC,0x51,0xF8,0x3E
	.DB  0xC5,0x20,0xF0,0x3E,0xB0,0x72,0xE8,0x3E
	.DB  0x89,0x41,0xE0,0x3E,0x75,0x93,0xD8,0x3E
	.DB  0x4E,0x62,0xD0,0x3E,0x27,0x31,0xC8,0x3E
	.DB  0x0,0x0,0xC0,0x3E,0xC7,0x4B,0xB7,0x3E
	.DB  0xA0,0x1A,0xAF,0x3E,0x79,0xE9,0xA6,0x3E
	.DB  0x3F,0x35,0x9E,0x3E,0x6,0x81,0x95,0x3E
	.DB  0xDF,0x4F,0x8D,0x3E,0xA6,0x9B,0x84,0x3E
	.DB  0xD9,0xCE,0x77,0x3E,0x66,0x66,0x66,0x3E
	.DB  0xF4,0xFD,0x54,0x3E,0x81,0x95,0x43,0x3E
	.DB  0xE,0x2D,0x32,0x3E,0x77,0xBE,0x1F,0x3E
	.DB  0x4,0x56,0xE,0x3E,0x23,0xDB,0xF9,0x3D
	.DB  0x3D,0xA,0xD7,0x3D,0xE,0x2D,0xB2,0x3D
	.DB  0x4D,0xF3,0x8E,0x3D,0xF4,0xFD,0x54,0x3D
	.DB  0x29,0x5C,0xF,0x3D,0x29,0x5C,0x8F,0x3C
	.DB  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0

;GLOBAL REGISTER VARIABLES INITIALIZATION
__REG_VARS:
	.DB  0x0,0x0,0x0,0x64
	.DB  0x32,0x0


__GLOBAL_INI_TBL:
	.DW  0x06
	.DW  0x03
	.DW  __REG_VARS*2

_0xFFFFFFFF:
	.DW  0

#define __GLOBAL_INI_TBL_PRESENT 1

__RESET:
	CLI
	CLR  R30
	OUT  EECR,R30

;INTERRUPT VECTORS ARE PLACED
;AT THE START OF FLASH
	LDI  R31,1
	OUT  MCUCR,R31
	OUT  MCUCR,R30

;CLEAR R2-R14
	LDI  R24,(14-2)+1
	LDI  R26,2
	CLR  R27
__CLEAR_REG:
	ST   X+,R30
	DEC  R24
	BRNE __CLEAR_REG

;CLEAR SRAM
	LDI  R24,LOW(__CLEAR_SRAM_SIZE)
	LDI  R25,HIGH(__CLEAR_SRAM_SIZE)
	LDI  R26,LOW(__SRAM_START)
	LDI  R27,HIGH(__SRAM_START)
__CLEAR_SRAM:
	ST   X+,R30
	SBIW R24,1
	BRNE __CLEAR_SRAM

;GLOBAL VARIABLES INITIALIZATION
	LDI  R30,LOW(__GLOBAL_INI_TBL*2)
	LDI  R31,HIGH(__GLOBAL_INI_TBL*2)
__GLOBAL_INI_NEXT:
	LPM  R24,Z+
	LPM  R25,Z+
	SBIW R24,0
	BREQ __GLOBAL_INI_END
	LPM  R26,Z+
	LPM  R27,Z+
	LPM  R0,Z+
	LPM  R1,Z+
	MOVW R22,R30
	MOVW R30,R0
__GLOBAL_INI_LOOP:
	LPM  R0,Z+
	ST   X+,R0
	SBIW R24,1
	BRNE __GLOBAL_INI_LOOP
	MOVW R30,R22
	RJMP __GLOBAL_INI_NEXT
__GLOBAL_INI_END:

;GPIOR0 INITIALIZATION
	LDI  R30,__GPIOR0_INIT
	OUT  GPIOR0,R30

;HARDWARE STACK POINTER INITIALIZATION
	LDI  R30,LOW(__SRAM_END-__HEAP_SIZE)
	OUT  SPL,R30
	LDI  R30,HIGH(__SRAM_END-__HEAP_SIZE)
	OUT  SPH,R30

;DATA STACK POINTER INITIALIZATION
	LDI  R28,LOW(__SRAM_START+__DSTACK_SIZE)
	LDI  R29,HIGH(__SRAM_START+__DSTACK_SIZE)

	JMP  _main

	.ESEG
	.ORG 0

	.DSEG
	.ORG 0x300

	.CSEG
;#include <mega328p.h>
	#ifndef __SLEEP_DEFINED__
	#define __SLEEP_DEFINED__
	.EQU __se_bit=0x01
	.EQU __sm_mask=0x0E
	.EQU __sm_adc_noise_red=0x02
	.EQU __sm_powerdown=0x04
	.EQU __sm_powersave=0x06
	.EQU __sm_standby=0x0C
	.EQU __sm_ext_standby=0x0E
	.SET power_ctrl_reg=smcr
	#endif
;typedef unsigned char uint8_t;
;typedef unsigned int uint16_t;
;#define low(x)   ((x) & 0xFF)
;#define high(x)   (((x)>>8) & 0xFF)
;
;//#define FGEN 7680000UL
;#define FGEN 7946000UL
;#define TIMER0_PWM (1<<COM0A1) | (0<<COM0A0) | (1<<COM0B1) | (0<<COM0B0) | (0<<WGM01) | (1<<WGM00)
;#define TIMER2_PWM (1<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (1<<WGM20)
;#define PWM_DIV (0<<WGM02) | (0<<CS02) | (0<<CS01) | (1<<CS00)
;#define TIMER1_DIV (0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (1<<WGM12) | (0<<CS12) | (0<<CS11) | (1<<CS10)
;#define N 181 //points
;#define DEAD_TIME 0
;#define PHASE_B_DELAY N/3
;#define PHASE_C_DELAY 2*N/3
;#define A 0
;#define B 1
;#define C 2
;#define PHASE_A_PIN PORTD6
;#define PHASE_B_PIN PORTD5
;#define PHASE_C_PIN PORTB3
;#define PHASE_NOT_A_PIN PORTC0
;#define PHASE_NOT_B_PIN PORTC1
;#define PHASE_NOT_C_PIN PORTC2
;
;uint8_t polarity[3];
;uint8_t polarity_not[3];
;uint8_t step[3];
;
;//uint8_t polarity_not = 0;
;
;uint8_t sinus[N+1];
;
;uint8_t phase_b_step = 0;
;uint8_t phase_c_step = 0;
;
;uint8_t amplitude = 100;
;uint16_t frequency = 50;
;
;const float sinus_table[N+1] ={
;    0.0, 0.0175, 0.035, 0.052, 0.0698, 0.087, 0.105, 0.122, 0.139, 0.156, 0.174, 0.191, 0.208,
;    0.225, 0.242, 0.259, 0.276, 0.292, 0.309, 0.326, 0.342, 0.358, 0.375, 0.391, 0.407,
;    0.423, 0.438, 0.454, 0.469, 0.485, 0.5, 0.515, 0.53, 0.545, 0.559, 0.574, 0.588,
;    0.602, 0.616, 0.629, 0.643, 0.656, 0.669, 0.682, 0.695, 0.707, 0.719, 0.731, 0.743,
;    0.755, 0.766, 0.777, 0.788, 0.799, 0.809, 0.819, 0.829, 0.839, 0.848, 0.857, 0.866,
;    0.875, 0.883, 0.891, 0.899, 0.906, 0.914, 0.921, 0.927, 0.934, 0.94, 0.946, 0.951,
;    0.956, 0.961, 0.966, 0.97, 0.974, 0.978, 0.982, 0.985, 0.988, 0.99, 0.993, 0.995,
;    0.996, 0.998, 0.999, 0.999, 1.0, 1.0, 1.0, 0.999, 0.999, 0.998, 0.996,
;    0.995, 0.993, 0.99, 0.988, 0.985, 0.982, 0.978, 0.974, 0.97, 0.966, 0.961, 0.956,
;    0.951, 0.946, 0.94, 0.934, 0.927, 0.921, 0.914, 0.906, 0.899, 0.891, 0.883, 0.875,
;    0.866, 0.857, 0.848, 0.839, 0.829, 0.819, 0.809, 0.799, 0.788, 0.777, 0.766, 0.755,
;    0.743, 0.731, 0.719, 0.707, 0.695, 0.682, 0.669, 0.656, 0.643, 0.629, 0.616, 0.602,
;    0.588, 0.574, 0.559, 0.545, 0.53, 0.515, 0.5, 0.485, 0.469, 0.454, 0.438, 0.423,
;    0.407, 0.391, 0.375, 0.358, 0.342, 0.326, 0.309, 0.292, 0.276, 0.259, 0.242, 0.225,
;    0.208, 0.191, 0.174, 0.156, 0.139, 0.122, 0.105, 0.087, 0.0698, 0.052, 0.035, 0.0175, 0.0};
;
;void stop_timers() {
; 0000 003A void stop_timers() {

	.CSEG
_stop_timers:
; .FSTART _stop_timers
; 0000 003B     TCCR0A=0; TCCR0B=0;
	LDI  R30,LOW(0)
	OUT  0x24,R30
	OUT  0x25,R30
; 0000 003C     OCR0A=0; OCR0B=0;
	OUT  0x27,R30
	OUT  0x28,R30
; 0000 003D     TCCR1A=0; TCCR1B=0;
	STS  128,R30
	STS  129,R30
; 0000 003E     OCR1AH=0; OCR1AL=0;
	STS  137,R30
	STS  136,R30
; 0000 003F     TIMSK1=0;
	STS  111,R30
; 0000 0040     TCCR2A=0; TCCR2B=0;
	STS  176,R30
	STS  177,R30
; 0000 0041     OCR2A=0; OCR2B=0;
	STS  179,R30
	STS  180,R30
; 0000 0042     #asm("cli")
	cli
; 0000 0043 }
	RET
; .FEND
;
;void DC_mode() {
; 0000 0045 void DC_mode() {
_DC_mode:
; .FSTART _DC_mode
; 0000 0046     stop_timers();
	RCALL _stop_timers
; 0000 0047     PORTC=(1<<PORTC1);
	LDI  R30,LOW(2)
	OUT  0x8,R30
; 0000 0048     PORTD=(1<<PORTD5);
	LDI  R30,LOW(32)
	OUT  0xB,R30
; 0000 0049 }
	RET
; .FEND
;
;void sinus_period(uint16_t frequency) {
; 0000 004B void sinus_period(uint16_t frequency) {
_sinus_period:
; .FSTART _sinus_period
; 0000 004C     if (frequency != 0) {
	ST   -Y,R27
	ST   -Y,R26
;	frequency -> Y+0
	LD   R30,Y
	LDD  R31,Y+1
	SBIW R30,0
	BREQ _0x3
; 0000 004D         OCR1AH = high(FGEN/2/N/frequency);
	RCALL SUBOPT_0x0
	MOV  R30,R31
	LDI  R31,0
	STS  137,R30
; 0000 004E         OCR1AL = low(FGEN/2/N/frequency);
	RCALL SUBOPT_0x0
	STS  136,R30
; 0000 004F     } else {
	RJMP _0x4
_0x3:
; 0000 0050         DC_mode();
	RCALL _DC_mode
; 0000 0051     }
_0x4:
; 0000 0052 }
	RJMP _0x2000001
; .FEND
;
;void sinus_amplitude(uint8_t amplitude) {
; 0000 0054 void sinus_amplitude(uint8_t amplitude) {
_sinus_amplitude:
; .FSTART _sinus_amplitude
; 0000 0055     uint8_t i = 0;
; 0000 0056     for (i = 0; i < N; i++) {
	ST   -Y,R26
	ST   -Y,R17
;	amplitude -> Y+1
;	i -> R17
	LDI  R17,0
	LDI  R17,LOW(0)
_0x6:
	CPI  R17,181
	BRSH _0x7
; 0000 0057      sinus[i] = 2.55*amplitude*sinus_table[i];
	MOV  R30,R17
	LDI  R31,0
	SUBI R30,LOW(-_sinus)
	SBCI R31,HIGH(-_sinus)
	PUSH R31
	PUSH R30
	LDD  R30,Y+1
	LDI  R31,0
	CALL __CWD1
	CALL __CDF1
	__GETD2N 0x40233333
	CALL __MULF12
	PUSH R23
	PUSH R22
	PUSH R31
	PUSH R30
	MOV  R30,R17
	LDI  R26,LOW(_sinus_table*2)
	LDI  R27,HIGH(_sinus_table*2)
	LDI  R31,0
	CALL __LSLW2
	ADD  R30,R26
	ADC  R31,R27
	CALL __GETD1PF
	POP  R26
	POP  R27
	POP  R24
	POP  R25
	CALL __MULF12
	POP  R26
	POP  R27
	CALL __CFD1U
	ST   X,R30
; 0000 0058     }
	SUBI R17,-1
	RJMP _0x6
_0x7:
; 0000 0059 }
	LDD  R17,Y+0
_0x2000001:
	ADIW R28,2
	RET
; .FEND
;
;void start_PWM(uint16_t frequency, uint8_t amplitude) {
; 0000 005B void start_PWM(uint16_t frequency, uint8_t amplitude) {
_start_PWM:
; .FSTART _start_PWM
; 0000 005C     polarity[A] = 1;
	ST   -Y,R26
;	frequency -> Y+1
;	amplitude -> Y+0
	LDI  R30,LOW(1)
	STS  _polarity,R30
; 0000 005D     polarity[B] = 0;
	LDI  R30,LOW(0)
	__PUTB1MN _polarity,1
; 0000 005E     polarity[C] = 1;
	LDI  R30,LOW(1)
	__PUTB1MN _polarity,2
; 0000 005F     polarity_not[A] = 0;
	LDI  R30,LOW(0)
	STS  _polarity_not,R30
; 0000 0060     polarity_not[B] = 0;
	__PUTB1MN _polarity_not,1
; 0000 0061     polarity_not[C] = 0;
	__PUTB1MN _polarity_not,2
; 0000 0062     step[A] = 0;
	STS  _step,R30
; 0000 0063     step[B] = 0;
	__PUTB1MN _step,1
; 0000 0064     step[B] = 0;
	__PUTB1MN _step,1
; 0000 0065     // Timer/Counter 0 initialization
; 0000 0066     TCCR0A=TIMER0_PWM;
	LDI  R30,LOW(161)
	OUT  0x24,R30
; 0000 0067     TCCR0B=PWM_DIV;
	LDI  R30,LOW(1)
	OUT  0x25,R30
; 0000 0068     TCNT0=0;
	LDI  R30,LOW(0)
	OUT  0x26,R30
; 0000 0069     // Timer/Counter 2 initialization
; 0000 006A     TCCR2A=TIMER2_PWM;
	LDI  R30,LOW(129)
	STS  176,R30
; 0000 006B     TCCR2B=PWM_DIV;
	LDI  R30,LOW(1)
	STS  177,R30
; 0000 006C     TCNT2=0;
	LDI  R30,LOW(0)
	STS  178,R30
; 0000 006D     // Timer/Counter 1 initialization
; 0000 006E     TCCR1A=0;
	STS  128,R30
; 0000 006F     TCCR1B= TIMER1_DIV;
	LDI  R30,LOW(9)
	STS  129,R30
; 0000 0070     TCNT1H=0; TCNT1L=0; ICR1H=0; ICR1L=0;
	LDI  R30,LOW(0)
	STS  133,R30
	STS  132,R30
	STS  135,R30
	STS  134,R30
; 0000 0071     // Timer/Counter 1 Interrupt(s) initialization
; 0000 0072     TIMSK1=(1<<OCIE1A);
	LDI  R30,LOW(2)
	STS  111,R30
; 0000 0073     #asm("sei")
	sei
; 0000 0074     sinus_period(frequency);
	LDD  R26,Y+1
	LDD  R27,Y+1+1
	RCALL _sinus_period
; 0000 0075     sinus_amplitude(amplitude);
	LD   R26,Y
	RCALL _sinus_amplitude
; 0000 0076 }
	ADIW R28,3
	RET
; .FEND
;
;interrupt [TIM1_COMPA] void timer1_compa_isr(void)
; 0000 0079 {
_timer1_compa_isr:
; .FSTART _timer1_compa_isr
	ST   -Y,R0
	ST   -Y,R1
	ST   -Y,R22
	ST   -Y,R26
	ST   -Y,R27
	ST   -Y,R30
	ST   -Y,R31
	IN   R30,SREG
	ST   -Y,R30
; 0000 007A     /* For phase A */
; 0000 007B    if (step[A] > N) {polarity[A] ^= 1; step[A] = 0;};
	LDS  R26,_step
	CPI  R26,LOW(0xB6)
	BRLO _0x8
	LDS  R26,_polarity
	LDI  R30,LOW(1)
	EOR  R30,R26
	STS  _polarity,R30
	LDI  R30,LOW(0)
	STS  _step,R30
_0x8:
; 0000 007C    if (step[A] == DEAD_TIME || step[A] == N-DEAD_TIME) {polarity_not[A] ^= 1;};
	LDS  R26,_step
	CPI  R26,LOW(0x0)
	BREQ _0xA
	CPI  R26,LOW(0xB5)
	BRNE _0x9
_0xA:
	LDS  R26,_polarity_not
	LDI  R30,LOW(1)
	EOR  R30,R26
	STS  _polarity_not,R30
_0x9:
; 0000 007D    step[A]++;
	LDS  R30,_step
	SUBI R30,-LOW(1)
	STS  _step,R30
; 0000 007E    OCR0A = polarity[A]*sinus[step[A]];
	RCALL SUBOPT_0x1
	LDS  R26,_polarity
	MULS R30,R26
	MOVW R30,R0
	OUT  0x27,R30
; 0000 007F    /* For phase B */
; 0000 0080    if (phase_b_step < PHASE_B_DELAY) {phase_b_step++;} else {
	LDI  R30,LOW(60)
	CP   R4,R30
	BRSH _0xC
	INC  R4
	RJMP _0xD
_0xC:
; 0000 0081        if (step[B] > N) {polarity[B] ^= 1; step[B] = 0;};
	__GETB2MN _step,1
	CPI  R26,LOW(0xB6)
	BRLO _0xE
	__POINTW1MN _polarity,1
	RCALL SUBOPT_0x2
	LDI  R30,LOW(0)
	__PUTB1MN _step,1
_0xE:
; 0000 0082        if (step[B] == DEAD_TIME || step[B] == N-DEAD_TIME) {polarity_not[B] ^= 1;};
	__GETB2MN _step,1
	CPI  R26,LOW(0x0)
	BREQ _0x10
	__GETB2MN _step,1
	CPI  R26,LOW(0xB5)
	BRNE _0xF
_0x10:
	__POINTW1MN _polarity_not,1
	RCALL SUBOPT_0x2
_0xF:
; 0000 0083        step[B]++;
	__GETB1MN _step,1
	SUBI R30,-LOW(1)
	__PUTB1MN _step,1
; 0000 0084        OCR2A = polarity[B]*sinus[step[B]];
	__GETB2MN _polarity,1
	__GETB1MN _step,1
	RCALL SUBOPT_0x1
	MULS R30,R26
	MOVW R30,R0
	STS  179,R30
; 0000 0085    }
_0xD:
; 0000 0086    /* For phase C */
; 0000 0087    if (phase_c_step < PHASE_C_DELAY) {phase_c_step++;} else {
	LDI  R30,LOW(120)
	CP   R3,R30
	BRSH _0x12
	INC  R3
	RJMP _0x13
_0x12:
; 0000 0088        if (step[C] > N) {polarity[C] ^= 1; step[C] = 0;};
	__GETB2MN _step,2
	CPI  R26,LOW(0xB6)
	BRLO _0x14
	__POINTW1MN _polarity,2
	RCALL SUBOPT_0x2
	LDI  R30,LOW(0)
	__PUTB1MN _step,2
_0x14:
; 0000 0089        if (step[C] == DEAD_TIME || step[C] == N-DEAD_TIME) {polarity_not[C] ^= 1;};
	__GETB2MN _step,2
	CPI  R26,LOW(0x0)
	BREQ _0x16
	__GETB2MN _step,2
	CPI  R26,LOW(0xB5)
	BRNE _0x15
_0x16:
	__POINTW1MN _polarity_not,2
	RCALL SUBOPT_0x2
_0x15:
; 0000 008A        step[C]++;
	__GETB1MN _step,2
	SUBI R30,-LOW(1)
	__PUTB1MN _step,2
; 0000 008B        OCR0B = polarity[C]*sinus[step[C]];
	__GETB2MN _polarity,2
	__GETB1MN _step,2
	RCALL SUBOPT_0x1
	MULS R30,R26
	MOVW R30,R0
	OUT  0x28,R30
; 0000 008C    }
_0x13:
; 0000 008D    /* Inverted phases */
; 0000 008E    PORTC =((polarity[A]*polarity_not[A]) << PHASE_NOT_A_PIN) | ((polarity[B]*polarity_not[B]) << PHASE_NOT_B_PIN) | ((po ...
	LDS  R30,_polarity_not
	LDS  R26,_polarity
	MULS R30,R26
	MOV  R22,R0
	__GETB2MN _polarity,1
	__GETB1MN _polarity_not,1
	MULS R30,R26
	MOVW R30,R0
	LSL  R30
	OR   R22,R30
	__GETB2MN _polarity,2
	__GETB1MN _polarity_not,2
	MULS R30,R26
	MOVW R30,R0
	LSL  R30
	LSL  R30
	OR   R30,R22
	OUT  0x8,R30
; 0000 008F }
	LD   R30,Y+
	OUT  SREG,R30
	LD   R31,Y+
	LD   R30,Y+
	LD   R27,Y+
	LD   R26,Y+
	LD   R22,Y+
	LD   R1,Y+
	LD   R0,Y+
	RETI
; .FEND
;
;
;void invertor_setup() {
; 0000 0092 void invertor_setup() {
_invertor_setup:
; .FSTART _invertor_setup
; 0000 0093     // Crystal Oscillator division factor: 1
; 0000 0094     #pragma optsize-
; 0000 0095     CLKPR=(1<<CLKPCE);
	LDI  R30,LOW(128)
	STS  97,R30
; 0000 0096     CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
	LDI  R30,LOW(0)
	STS  97,R30
; 0000 0097     #ifdef _OPTIMIZE_SIZE_
; 0000 0098     #pragma optsize+
; 0000 0099     #endif
; 0000 009A     ACSR=(1<<ACD);
	LDI  R30,LOW(128)
	OUT  0x30,R30
; 0000 009B     // Ports init
; 0000 009C     DDRC=(1<<PHASE_NOT_A_PIN) |(1<<PHASE_NOT_B_PIN) | (1<<PHASE_NOT_C_PIN);
	LDI  R30,LOW(7)
	OUT  0x7,R30
; 0000 009D     DDRD=(1<<PHASE_A_PIN) | (1<<PHASE_B_PIN);
	LDI  R30,LOW(96)
	OUT  0xA,R30
; 0000 009E     DDRB=(1<<PHASE_C_PIN);
	LDI  R30,LOW(8)
	OUT  0x4,R30
; 0000 009F     start_PWM(frequency, amplitude);
	ST   -Y,R8
	ST   -Y,R7
	MOV  R26,R6
	RCALL _start_PWM
; 0000 00A0 }
	RET
; .FEND
;
;void main(void)
; 0000 00A3 {
_main:
; .FSTART _main
; 0000 00A4  invertor_setup();
	RCALL _invertor_setup
; 0000 00A5 while (1)
_0x18:
; 0000 00A6       {
; 0000 00A7       }
	RJMP _0x18
; 0000 00A8 }
_0x1B:
	RJMP _0x1B
; .FEND

	.DSEG
_polarity:
	.BYTE 0x3
_polarity_not:
	.BYTE 0x3
_step:
	.BYTE 0x3
_sinus:
	.BYTE 0xB6

	.CSEG
;OPTIMIZER ADDED SUBROUTINE, CALLED 2 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x0:
	LD   R30,Y
	LDD  R31,Y+1
	LDI  R26,LOW(21950)
	LDI  R27,HIGH(21950)
	CALL __DIVW21U
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 3 TIMES, CODE SIZE REDUCTION:1 WORDS
SUBOPT_0x1:
	LDI  R31,0
	SUBI R30,LOW(-_sinus)
	SBCI R31,HIGH(-_sinus)
	LD   R30,Z
	RET

;OPTIMIZER ADDED SUBROUTINE, CALLED 4 TIMES, CODE SIZE REDUCTION:9 WORDS
SUBOPT_0x2:
	MOVW R0,R30
	LD   R26,Z
	LDI  R30,LOW(1)
	EOR  R30,R26
	MOVW R26,R0
	ST   X,R30
	RET


	.CSEG
__ROUND_REPACK:
	TST  R21
	BRPL __REPACK
	CPI  R21,0x80
	BRNE __ROUND_REPACK0
	SBRS R30,0
	RJMP __REPACK
__ROUND_REPACK0:
	ADIW R30,1
	ADC  R22,R25
	ADC  R23,R25
	BRVS __REPACK1

__REPACK:
	LDI  R21,0x80
	EOR  R21,R23
	BRNE __REPACK0
	PUSH R21
	RJMP __ZERORES
__REPACK0:
	CPI  R21,0xFF
	BREQ __REPACK1
	LSL  R22
	LSL  R0
	ROR  R21
	ROR  R22
	MOV  R23,R21
	RET
__REPACK1:
	PUSH R21
	TST  R0
	BRMI __REPACK2
	RJMP __MAXRES
__REPACK2:
	RJMP __MINRES

__UNPACK:
	LDI  R21,0x80
	MOV  R1,R25
	AND  R1,R21
	LSL  R24
	ROL  R25
	EOR  R25,R21
	LSL  R21
	ROR  R24

__UNPACK1:
	LDI  R21,0x80
	MOV  R0,R23
	AND  R0,R21
	LSL  R22
	ROL  R23
	EOR  R23,R21
	LSL  R21
	ROR  R22
	RET

__CFD1U:
	SET
	RJMP __CFD1U0
__CFD1:
	CLT
__CFD1U0:
	PUSH R21
	RCALL __UNPACK1
	CPI  R23,0x80
	BRLO __CFD10
	CPI  R23,0xFF
	BRCC __CFD10
	RJMP __ZERORES
__CFD10:
	LDI  R21,22
	SUB  R21,R23
	BRPL __CFD11
	NEG  R21
	CPI  R21,8
	BRTC __CFD19
	CPI  R21,9
__CFD19:
	BRLO __CFD17
	SER  R30
	SER  R31
	SER  R22
	LDI  R23,0x7F
	BLD  R23,7
	RJMP __CFD15
__CFD17:
	CLR  R23
	TST  R21
	BREQ __CFD15
__CFD18:
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R23
	DEC  R21
	BRNE __CFD18
	RJMP __CFD15
__CFD11:
	CLR  R23
__CFD12:
	CPI  R21,8
	BRLO __CFD13
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R23
	SUBI R21,8
	RJMP __CFD12
__CFD13:
	TST  R21
	BREQ __CFD15
__CFD14:
	LSR  R23
	ROR  R22
	ROR  R31
	ROR  R30
	DEC  R21
	BRNE __CFD14
__CFD15:
	TST  R0
	BRPL __CFD16
	RCALL __ANEGD1
__CFD16:
	POP  R21
	RET

__CDF1U:
	SET
	RJMP __CDF1U0
__CDF1:
	CLT
__CDF1U0:
	SBIW R30,0
	SBCI R22,0
	SBCI R23,0
	BREQ __CDF10
	CLR  R0
	BRTS __CDF11
	TST  R23
	BRPL __CDF11
	COM  R0
	RCALL __ANEGD1
__CDF11:
	MOV  R1,R23
	LDI  R23,30
	TST  R1
__CDF12:
	BRMI __CDF13
	DEC  R23
	LSL  R30
	ROL  R31
	ROL  R22
	ROL  R1
	RJMP __CDF12
__CDF13:
	MOV  R30,R31
	MOV  R31,R22
	MOV  R22,R1
	PUSH R21
	RCALL __REPACK
	POP  R21
__CDF10:
	RET

__ZERORES:
	CLR  R30
	CLR  R31
	CLR  R22
	CLR  R23
	POP  R21
	RET

__MINRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	SER  R23
	POP  R21
	RET

__MAXRES:
	SER  R30
	SER  R31
	LDI  R22,0x7F
	LDI  R23,0x7F
	POP  R21
	RET

__MULF12:
	PUSH R21
	RCALL __UNPACK
	CPI  R23,0x80
	BREQ __ZERORES
	CPI  R25,0x80
	BREQ __ZERORES
	EOR  R0,R1
	SEC
	ADC  R23,R25
	BRVC __MULF124
	BRLT __ZERORES
__MULF125:
	TST  R0
	BRMI __MINRES
	RJMP __MAXRES
__MULF124:
	PUSH R0
	PUSH R17
	PUSH R18
	PUSH R19
	PUSH R20
	CLR  R17
	CLR  R18
	CLR  R25
	MUL  R22,R24
	MOVW R20,R0
	MUL  R24,R31
	MOV  R19,R0
	ADD  R20,R1
	ADC  R21,R25
	MUL  R22,R27
	ADD  R19,R0
	ADC  R20,R1
	ADC  R21,R25
	MUL  R24,R30
	RCALL __MULF126
	MUL  R27,R31
	RCALL __MULF126
	MUL  R22,R26
	RCALL __MULF126
	MUL  R27,R30
	RCALL __MULF127
	MUL  R26,R31
	RCALL __MULF127
	MUL  R26,R30
	ADD  R17,R1
	ADC  R18,R25
	ADC  R19,R25
	ADC  R20,R25
	ADC  R21,R25
	MOV  R30,R19
	MOV  R31,R20
	MOV  R22,R21
	MOV  R21,R18
	POP  R20
	POP  R19
	POP  R18
	POP  R17
	POP  R0
	TST  R22
	BRMI __MULF122
	LSL  R21
	ROL  R30
	ROL  R31
	ROL  R22
	RJMP __MULF123
__MULF122:
	INC  R23
	BRVS __MULF125
__MULF123:
	RCALL __ROUND_REPACK
	POP  R21
	RET

__MULF127:
	ADD  R17,R0
	ADC  R18,R1
	ADC  R19,R25
	RJMP __MULF128
__MULF126:
	ADD  R18,R0
	ADC  R19,R1
__MULF128:
	ADC  R20,R25
	ADC  R21,R25
	RET

__ANEGD1:
	COM  R31
	COM  R22
	COM  R23
	NEG  R30
	SBCI R31,-1
	SBCI R22,-1
	SBCI R23,-1
	RET

__LSLW2:
	LSL  R30
	ROL  R31
	LSL  R30
	ROL  R31
	RET

__CWD1:
	MOV  R22,R31
	ADD  R22,R22
	SBC  R22,R22
	MOV  R23,R22
	RET

__DIVW21U:
	CLR  R0
	CLR  R1
	LDI  R25,16
__DIVW21U1:
	LSL  R26
	ROL  R27
	ROL  R0
	ROL  R1
	SUB  R0,R30
	SBC  R1,R31
	BRCC __DIVW21U2
	ADD  R0,R30
	ADC  R1,R31
	RJMP __DIVW21U3
__DIVW21U2:
	SBR  R26,1
__DIVW21U3:
	DEC  R25
	BRNE __DIVW21U1
	MOVW R30,R26
	MOVW R26,R0
	RET

__GETD1PF:
	LPM  R0,Z+
	LPM  R1,Z+
	LPM  R22,Z+
	LPM  R23,Z
	MOVW R30,R0
	RET

;END OF CODE MARKER
__END_OF_CODE:
