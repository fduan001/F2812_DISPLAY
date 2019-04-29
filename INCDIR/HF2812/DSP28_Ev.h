//
//      TMDX ALPHA RELEASE
//      Intended for product evaluation purposes
//
//###########################################################################
//
// FILE:	DSP28_Ev.h
//
// TITLE:	DSP28 Device Event Manager Register Definitions.
//
//###########################################################################
//
//  Ver | dd mmm yyyy | Who  | Description of changes
// =====|=============|======|===============================================
//  0.55| 06 May 2002 | L.H. | EzDSP Alpha Release
//  0.56| 17 May 2002 | L.H. | Removed extra reserved word in EVA EVB reg file
//  0.57| 27 May 2002 | L.H. | No change
//###########################################################################

#ifndef DSP28_EV_H
#define DSP28_EV_H
#include "F2812_datatype.h"
/* --------------------------------------------------- */
/* F2810/12 Event Manager (EV) GP Timer Registers      */
/*                                                     */
/* ----------------------------------------------------*/

/* Overall Timer Control Register */

struct GPTCONA_BITS
{
    UINT16 T1PIN: 2;        // 1:0   Polarity of GP timer 1 compare
    UINT16 T2PIN: 2;        // 3:2   Polarity of GP timer 2 compare
    UINT16 rsvd1: 2;        // 5:4   reserved
    UINT16 TCOMPOE: 1;      // 6     Compare output enable
    UINT16 T1TOADC: 2;      // 8:7   Start ADC with timer 1 event
    UINT16 T2TOADC: 2;      // 10:9  Start ADC with timer 2 event
    UINT16 rsvd2: 2;        // 12:11 reserved
    UINT16 T1STAT: 1;       // 13    GP Timer 1 status (read only)
    UINT16 T2STAT: 1;       // 14    GP Timer 2 status (read only)
    UINT16 rsvd: 1;         // 15    reserved
};

/* Allow access to the bit fields or entire register */
union GPTCONA_REG
{
    UINT16        all;
    struct  GPTCONA_BITS bit;
};

struct GPTCONB_BITS
{
    UINT16 T3PIN: 2;        // 1:0   Polarity of GP timer 3 compare
    UINT16 T4PIN: 2;        // 3:2   Polarity of GP timer 4 compare
    UINT16 T1CMPOE: 1;      // 4     Timer1 compare output
    UINT16 T2CMPOE: 1;      // 5     Timer2 compare output
    UINT16 TCOMPOE: 1;      // 6     Compare output enable
    UINT16 T3TOADC: 2;      // 8:7   Start ADC with timer 3 event
    UINT16 T4TOADC: 2;      // 10:9  Start ADC with timer 4 event
    UINT16 T1CTRIP: 1;      // 11    Timer1 trip enable
    UINT16 T2CTRIP: 1;      // 12    Timer2 trip enable
    UINT16 T3STAT: 1;       // 13    GP Timer 3 status (read only)
    UINT16 T4STAT: 1;       // 14    GP Timer 4 status (read only)
    UINT16 rsvd: 1;         // 15    reserved
};

/* Allow access to the bit fields or entire register */
union GPTCONB_REG
{
    UINT16        all;
    struct  GPTCONB_BITS bit;
};

/* Timer Control Register bit defintions */
struct TCONA_BITS
{
    UINT16  SET1PR: 1;       // 0     Period register select
    UINT16  TECMPR: 1;       // 1     Timer compare enable
    UINT16  TCLD10: 2;       // 3:2   Timer copare register reload
    UINT16  TCLKS10: 2;      // 5:4   Clock source select
    UINT16  TENABLE: 1;      // 6     Timer enable
    UINT16  T2SWT1: 1;       // 7     Start GP timer 2 with GP timer 1's enable
    UINT16  TPS: 3;          // 10:8  Input clock prescaler
    UINT16  TMODE: 2;        // 12:11 Count mode selection
    UINT16  rsvd: 1;         // 13    reserved
    UINT16  FREE: 1;         // 14    Free emulation control
    UINT16  SOFT: 1;         // 15    Soft emulation control
};

/* Allow access to the bit fields or entire register */
union TCONA_REG
{
    UINT16        all;
    struct  TCONA_BITS bit;
};

struct TCONB_BITS
{
    UINT16  SET3PR: 1;       // 0     Period register select
    UINT16  TECMPR: 1;       // 1     Timer compare enable
    UINT16  TCLD10: 2;       // 3:2   Timer copare register reload
    UINT16  TCLKS10: 2;      // 5:4   Clock source select
    UINT16  TENABLE: 1;      // 6     Timer enable
    UINT16  T4SWT3: 1;       // 7     Start GP timer 2 with GP timer 1's enable
    UINT16  TPS: 3;          // 10:8  Input clock prescaler
    UINT16  TMODE: 2;        // 12:11 Count mode selection
    UINT16  rsvd: 1;         // 13    reserved
    UINT16  FREE: 1;         // 14    Free emulation control
    UINT16  SOFT: 1;         // 15    Soft emulation control
};

/* Allow access to the bit fields or entire register */
union TCONB_REG
{
    UINT16        all;
    struct  TCONB_BITS bit;
};


struct EXTCONA_BITS
{
    UINT16   INDCOE: 1;       // 0      Independant compare output
    UINT16   QEPIQEL: 1;      // 1      QEP/CAP3 Index Qual Mode
    UINT16   QEPIE: 1;        // 2      QEP index enable
    UINT16   EVSOCE: 1;       // 3      Ev start of conversion output enable
    UINT16   rsvd: 12;        // 15:4   reserved
};

/* Allow access to the bit fields or entire register */
union EXTCONA_REG
{
    UINT16        all;
    struct  EXTCONA_BITS bit;
};


struct EXTCONB_BITS
{
    UINT16   INDCOE: 1;       // 0      Independant compare output
    UINT16   QEPIQEL: 1;      // 1      QEP/CAP3 Index Qual Mode
    UINT16   QEPIE: 1;        // 2      QEP index enable
    UINT16   EVSOCE: 1;       // 3      Ev start of conversion output enable
    UINT16   rsvd: 12;        // 15:4   reserved
};

/* Allow access to the bit fields or entire register */
union EXTCONB_REG
{
    UINT16        all;
    struct  EXTCONA_BITS bit;
};



/* Compare Control Register */
struct COMCONA_BITS
{
    UINT16   rsvd: 8;        // 7:0    reserved
    UINT16   PDPINTASTATUS: 1; // 8      Current status of the PDPINTA pin
    UINT16   FCOMPOE: 1;     // 9      Compare output enable
    UINT16   ACTRLD: 2;      // 11:10  Action control register reload
    UINT16   SVENABLE: 1;    // 12     Space vector PWM Mode enable
    UINT16   CLD: 2;         // 14:13  Compare register reload condition
    UINT16   CENABLE: 1;     // 15     Compare enable
};

/* Allow access to the bit fields or entire register */
union COMCONA_REG
{
    UINT16        all;
    struct  COMCONA_BITS bit;
};

struct COMCONB_BITS
{
    UINT16   rsvd: 8;        // 7:0    reserved
    UINT16   PDPINTBSTATUS: 1; // 8      Current status of the PDPINTB pin
    UINT16   FCOMPOE: 1;     // 9      Compare output enable
    UINT16   ACTRLD: 2;      // 11:10  Action control register reload
    UINT16   SVENABLE: 1;    // 12     Space vector PWM Mode enable
    UINT16   CLD: 2;         // 14:13  Compare register reload condition
    UINT16   CENABLE: 1;     // 15     Compare enable
};

/* Allow access to the bit fields or entire register */
union COMCONB_REG
{
    UINT16        all;
    struct  COMCONB_BITS bit;
};

/* Compare Action Control Register bit definitions */

struct ACTRA_BITS
{
    UINT16   CMP1ACT: 2;    // 1:0    Action on compare output pin 1 CMP1
    UINT16   CMP2ACT: 2;    // 3:2    Action on compare output pin 2 CMP2
    UINT16   CMP3ACT: 2;    // 5:4    Action on compare output pin 3 CMP3
    UINT16   CMP4ACT: 2;    // 7:6    Action on compare output pin 4 CMP4
    UINT16   CMP5ACT: 2;    // 9:8    Action on compare output pin 5 CMP5
    UINT16   CMP6ACT: 2;    // 11:10  Action on compare output pin 6 CMP6
    UINT16   D: 3;          // 14:12  Basic vector bits
    UINT16   SVRDIR: 1;     // 15     Space vecor PWM rotation dir
};

/* Allow access to the bit fields or entire register */
union ACTRA_REG
{
    UINT16        all;
    struct  ACTRA_BITS bit;
};

struct ACTRB_BITS
{
    UINT16   CMP7ACT: 2;    // 1:0    Action on compare output pin 7 CMP7
    UINT16   CMP8ACT: 2;    // 3:2    Action on compare output pin 8 CMP8
    UINT16   CMP9ACT: 2;    // 5:4    Action on compare output pin 9 CMP9
    UINT16   CMP10ACT: 2;   // 7:6    Action on compare output pin 10 CMP10
    UINT16   CMP11ACT: 2;   // 9:8    Action on compare output pin 11 CMP11
    UINT16   CMP12ACT: 2;   // 11:10  Action on compare output pin 12 CMP12
    UINT16   D: 3;          // 14:12  Basic vector bits
    UINT16   SVRDIR: 1;     // 15     Space vecor PWM rotation dir
};

/* Allow access to the bit fields or entire register */
union ACTRB_REG
{
    UINT16        all;
    struct  ACTRB_BITS bit;
};

/* Dead-Band Timer Control register bit definitions */
struct DBTCON_BITS
{
    UINT16   rsvd1: 2;      // 1:0    reserved
    UINT16   DBTPS: 3;      // 4:2    Dead-Band timer prescaler
    UINT16   EDBT1: 1;      // 5      Dead-Band timer 1 enable
    UINT16   EDBT2: 1;      // 6      Dead-Band timer 2 enable
    UINT16   EDBT3: 1;      // 7      Dead-Band timer 3 enable
    UINT16   DBT: 4;        // 11:8   Dead-Band timer period
    UINT16   rsvd2: 4;      // 15:12  reserved
};

/* Allow access to the bit fields or entire register */
union DBTCON_REG
{
    UINT16        all;
    struct  DBTCON_BITS bit;
};


/* Capture Control register bit definitions */
struct CAPCONA_BITS
{
    UINT16  rsvd1: 2;        // 1:0   reserved
    UINT16  CAP3EDGE: 2;     // 3:2   Edge Detection for Unit 3
    UINT16  CAP2EDGE: 2;     // 5:4   Edge Detection for Unit 2
    UINT16  CAP1EDGE: 2;     // 7:6   Edge Detection for Unit 1
    UINT16  CAP3TOADC: 1;    // 8     Unit 3 starts the ADC
    UINT16  CAP12TSEL: 1;    // 9     GP Timer selection for Units 1 and 2
    UINT16  CAP3TSEL: 1;     // 10    GP Timer selection for Unit 3
    UINT16  rsvd2: 1;        // 11    reserved
    UINT16  CAP3EN: 1;       // 12    Capture Unit 3 control
    UINT16  CAPQEPN: 2;      // 14:13 Capture Unit 1 and 2 control
    UINT16  CAPRES: 1;       // 15    Capture reset (always reads 0)
};

/* Allow access to the bit fields or entire register */
union CAPCONA_REG
{
    UINT16        all;
    struct  CAPCONA_BITS bit;
};


/* Control register bit definitions */
struct CAPCONB_BITS
{
    UINT16  rsvd1: 2;        // 1:0   reserved
    UINT16  CAP6EDGE: 2;     // 3:2   Edge Detection for Unit 6
    UINT16  CAP5EDGE: 2;     // 5:4   Edge Detection for Unit 5
    UINT16  CAP4EDGE: 2;     // 7:6   Edge Detection for Unit 4
    UINT16  CAP6TOADC: 1;    // 8     Unit 6 starts the ADC
    UINT16  CAP45TSEL: 1;    // 9     GP Timer selection for Units 4 and 5
    UINT16  CAP6TSEL: 1;     // 10    GP Timer selection for Unit 6
    UINT16  rsvd2: 1;        // 11    reserved
    UINT16  CAP6EN: 1;       // 12    Capture Unit 6 control
    UINT16  CAPQEPN: 2;      // 14:13 Capture Unit 4 and 5 control
    UINT16  CAPRES: 1;       // 15    Capture reset (always reads 0)
};

/* Allow access to the bit fields or entire register */
union CAPCONB_REG
{
    UINT16        all;
    struct  CAPCONB_BITS bit;
};

/* Capture FIFO Status Register bit definitions */
struct CAPFIFOA_BITS
{
    UINT16 rsvd1: 8;          // 7:0   reserved
    UINT16 CAP1FIFO: 2;       // 9:8   CAP1 FIFO status
    UINT16 CAP2FIFO: 2;       // 11:10 CAP2 FIFO status
    UINT16 CAP3FIFO: 2;       // 13:12 CAP2 FIFO status
    UINT16 rsvd2: 2;          // 15:14 reserved
};

/* Allow access to the bit fields or entire register */
union CAPFIFOA_REG
{
    UINT16        all;
    struct  CAPFIFOA_BITS bit;
};

/* Capture FIFO Status Register bit definitions */
struct CAPFIFOB_BITS
{
    UINT16 rsvd1: 8;          // 7:0   reserved
    UINT16 CAP4FIFO: 2;       // 9:8   CAP4 FIFO status
    UINT16 CAP5FIFO: 2;       // 11:10 CAP5 FIFO status
    UINT16 CAP6FIFO: 2;       // 13:12 CAP6 FIFO status
    UINT16 rsvd2: 2;          // 15:14 reserved
};

/* Allow access to the bit fields or entire register */
union CAPFIFOB_REG
{
    UINT16        all;
    struct  CAPFIFOB_BITS bit;
};

/* Interrupt Mask Register bit definitions */
struct EVAIMRA_BITS
{
    UINT16  PDPINTA: 1;     // 0       Enable PDPINTA
    UINT16  CMP1INT: 1;     // 1       Enable
    UINT16  CMP2INT: 1;     // 2       Enable
    UINT16  CMP3INT: 1;     // 3       Enable
    UINT16  rsvd1: 3;       // 6:4     reserved
    UINT16  T1PINT: 1;      // 7       Enable
    UINT16  T1CINT: 1;      // 8       Enable
    UINT16  T1UFINT: 1;     // 9       Enable
    UINT16  T1OFINT: 1;     // 10      Enable
    UINT16  rsvd2: 5;       // 15:11   reserved
};

/* Allow access to the bit fields or entire register */
union EVAIMRA_REG
{
    UINT16         all;
    struct  EVAIMRA_BITS bit;
};

/* Interrupt Mask Register bit definitions */
struct EVBIMRA_BITS
{
    UINT16  PDPINTB: 1;     // 0       Enable PDPINTB
    UINT16  CMP4INT: 1;     // 1       Enable
    UINT16  CMP5INT: 1;     // 2       Enable
    UINT16  CMP6INT: 1;     // 3       Enable
    UINT16  rsvd1: 3;       // 6:4     reserved
    UINT16  T3PINT: 1;      // 7       Enable
    UINT16  T3CINT: 1;      // 8       Enable
    UINT16  T3UFINT: 1;     // 9       Enable
    UINT16  T3OFINT: 1;     // 10      Enable
    UINT16  rsvd2: 5;       // 15:11   reserved
};

/* Allow access to the bit fields or entire register */
union EVBIMRA_REG
{
    UINT16         all;
    struct  EVBIMRA_BITS bit;
};

struct EVAIMRB_BITS
{
    UINT16   T2PINT: 1;      // 0       Enable
    UINT16   T2CINT: 1;      // 1       Enable
    UINT16   T2UFINT: 1;     // 2       Enable
    UINT16   T2OFINT: 1;     // 3       Enable
    UINT16   rsvd1: 12;      // 15:4    reserved
};

/* Allow access to the bit fields or entire register */
union EVAIMRB_REG
{
    UINT16         all;
    struct  EVAIMRB_BITS bit;
};

struct EVBIMRB_BITS
{
    UINT16   T4PINT: 1;      // 0       Enable
    UINT16   T4CINT: 1;      // 1       Enable
    UINT16   T4UFINT: 1;     // 2       Enable
    UINT16   T4OFINT: 1;     // 3       Enable
    UINT16   rsvd1: 12;      // 15:4    reserved
};

/* Allow access to the bit fields or entire register */
union EVBIMRB_REG
{
    UINT16         all;
    struct  EVBIMRB_BITS bit;
};

struct EVAIMRC_BITS
{
    UINT16   CAP1INT: 1;    // 0        Enable
    UINT16   CAP2INT: 1;    // 1        Enable
    UINT16   CAP3INT: 1;    // 2        Enable
    UINT16   rsvd1: 13;     // 15:3     reserved
};

/* Allow access to the bit fields or entire register */
union EVAIMRC_REG
{
    UINT16         all;
    struct  EVAIMRC_BITS bit;
};

struct EVBIMRC_BITS
{
    UINT16   CAP4INT: 1;    // 0        Enable
    UINT16   CAP5INT: 1;    // 1        Enable
    UINT16   CAP6INT: 1;    // 2        Enable
    UINT16   rsvd1: 13;     // 15:3     reserved
};

/* Allow access to the bit fields or entire register */
union EVBIMRC_REG
{
    UINT16         all;
    struct  EVBIMRC_BITS bit;
};

/* Interrupt Flag Register bit definitions */
struct EVAIFRA_BITS
{
    UINT16  PDPINTA: 1;     // 0       Flag PDPINTA
    UINT16  CMP1INT: 1;     // 1       Flag
    UINT16  CMP2INT: 1;     // 2       Flag
    UINT16  CMP3INT: 1;     // 3       Flag
    UINT16  rsvd1: 3;       // 6:4     reserved
    UINT16  T1PINT: 1;      // 7       Flag
    UINT16  T1CINT: 1;      // 8       Flag
    UINT16  T1UFINT: 1;     // 9       Flag
    UINT16  T1OFINT: 1;     // 10      Flag
    UINT16  rsvd2: 5;       // 15:11   reserved
};

/* Allow access to the bit fields or entire register */
union EVAIFRA_REG
{
    UINT16         all;
    struct  EVAIMRA_BITS bit;
};

/* Interrupt Mask Register bit definitions */
struct EVBIFRA_BITS
{
    UINT16  PDPINTB: 1;     // 0       Flag PDPINTB
    UINT16  CMP4INT: 1;     // 1       Flag
    UINT16  CMP5INT: 1;     // 2       Flag
    UINT16  CMP6INT: 1;     // 3       Flag
    UINT16  rsvd1: 3;       // 6:4     reserved
    UINT16  T3PINT: 1;      // 7       Flag
    UINT16  T3CINT: 1;      // 8       Flag
    UINT16  T3UFINT: 1;     // 9       Flag
    UINT16  T3OFINT: 1;     // 10      Flag
    UINT16  rsvd2: 5;       // 15:11   reserved
};

/* Allow access to the bit fields or entire register */
union EVBIFRA_REG
{
    UINT16         all;
    struct  EVBIFRA_BITS bit;
};

struct EVAIFRB_BITS
{
    UINT16   T2PINT: 1;      // 0       Flag
    UINT16   T2CINT: 1;      // 1       Flag
    UINT16   T2UFINT: 1;     // 2       Flag
    UINT16   T2OFINT: 1;     // 3       Flag
    UINT16   rsvd1: 12;      // 15:4    reserved
};

/* Allow access to the bit fields or entire register */
union EVAIFRB_REG
{
    UINT16         all;
    struct  EVAIFRB_BITS bit;
};

struct EVBIFRB_BITS
{
    UINT16   T4PINT: 1;      // 0       Flag
    UINT16   T4CINT: 1;      // 1       Flag
    UINT16   T4UFINT: 1;     // 2       Flag
    UINT16   T4OFINT: 1;     // 3       Flag
    UINT16   rsvd1: 12;      // 15:4    reserved
};

/* Allow access to the bit fields or entire register */
union EVBIFRB_REG
{
    UINT16         all;
    struct  EVBIFRB_BITS bit;
};

struct EVAIFRC_BITS
{
    UINT16   CAP1INT: 1;    // 0        Flag
    UINT16   CAP2INT: 1;    // 1        Flag
    UINT16   CAP3INT: 1;    // 2        Flag
    UINT16   rsvd1: 13;     // 15:3     reserved
};

/* Allow access to the bit fields or entire register */
union EVAIFRC_REG
{
    UINT16         all;
    struct  EVAIFRC_BITS bit;
};

struct EVBIFRC_BITS
{
    UINT16   CAP4INT: 1;    // 0        Flag
    UINT16   CAP5INT: 1;    // 1        Flag
    UINT16   CAP6INT: 1;    // 2        Flag
    UINT16   rsvd1: 13;     // 15:3     reserved
};

/* Allow access to the bit fields or entire register */
union EVBIFRC_REG
{
    UINT16         all;
    struct  EVBIFRC_BITS bit;
};

/* EVA Register File */

struct EVA_REGS
{
    union GPTCONA_REG  GPTCONA;	//0x7400
    UINT16       T1CNT;			//0x7401
    UINT16       T1CMPR;			//0x7402
    UINT16       T1PR;			//0x7403
    union TCONA_REG    T1CON;		//0x7404
    UINT16       T2CNT;			//0x7405
    UINT16       T2CMPR;			//0x7406
    UINT16       T2PR;			//0x7407
    union TCONA_REG    T2CON;         //0x7408
    union EXTCONA_REG  EXTCON;	      //0x7409
    UINT16  rsvd1[7];		      //0x740A-10
    union COMCONA_REG  COMCONA;	      //0x7411
    UINT16  rsvd2;				//0x7412
    union ACTRA_REG    ACTR;		//0x7413
    UINT16  rsvd3;				//0x7414
    union DBTCON_REG   DBTCONA;	      //0x7415
    UINT16  rsvd4;				//0x7416
    UINT16       CMPR1;			//0x7417
    UINT16       CMPR2;			//0x7418
    UINT16       CMPR3;			//0x7419
    UINT16   rsvd5[6];			//0x741A-F
    union CAPCONA_REG  CAPCON;	      //0x7420
    UINT16   rsvd6;				//0x7421
    union CAPFIFOA_REG CAPFIFO;	      //0x7422
    UINT16       CAP1FIFO;		//0x7423
    UINT16       CAP2FIFO;		//0x7424
    UINT16       CAP3FIFO;		//0x7425
    UINT16  rsvd7;				//0x7426
    UINT16       CAP1FBOT;		//0x7427
    UINT16       CAP2FBOT;		//0x7428
    UINT16       CAP3FBOT;		//0x7429
    UINT16  rsvd8[2];			//0x742A-B
    union EVAIMRA_REG  EVAIMRA;	//0x742C
    union EVAIMRB_REG  EVAIMRB;	//0x742D
    union EVAIMRC_REG  EVAIMRC;	//0x742E
    union EVAIFRA_REG  EVAIFRA;	//0x742F
    union EVAIFRB_REG  EVAIFRB;	//0x7430
    union EVAIFRC_REG  EVAIFRC;	//0x7431
};


/* EVB Register File */

struct EVB_REGS
{
    union GPTCONB_REG  GPTCONB;	//0x7500
    UINT16       T3CNT;           //0x7501
    UINT16       T3CMPR;          //0x7502
    UINT16       T3PR;            //0x7503
    union TCONB_REG    T3CON;     //0x7504
    UINT16       T4CNT;           //0x7505
    UINT16       T4CMPR;          //0x7506
    UINT16       T4PR;            //0x7507
    union TCONB_REG    T4CON;     //0x7508
    union EXTCONB_REG  EXTCONB;   //0x7509
    UINT16  rsvd1[7];             //0x750A-10
    union COMCONB_REG  COMCONB;   //0x7511
    UINT16  rsvd2;                //0x7512
    union ACTRB_REG    ACTRB;     //0x7513
    UINT16  rsvd3;                //0x7514
    union DBTCON_REG   DBTCONB;   //0x7515
    UINT16  rsvd4;                //0x7516
    UINT16       CMPR4;           //0x7517
    UINT16       CMPR5;           //0x7518
    UINT16       CMPR6;           //0x7519
    UINT16   rsvd5[6];            //0x751A-F
    union CAPCONB_REG  CAPCONB;   //0x7520
    UINT16   rsvd6;               //0x7521
    union CAPFIFOB_REG CAPFIFOB;  //0x7522
    UINT16       CAP4FIFO;        //0x7523
    UINT16       CAP5FIFO;        //0x7524
    UINT16       CAP6FIFO;        //0x7525
    UINT16  rsvd7;                //0x7526
    UINT16       CAP4FBOT;        //0x7527
    UINT16       CAP5FBOT;        //0x7528
    UINT16       CAP6FBOT;        //0x7529
    UINT16  rsvd8[2];             //0x752A-B
    union EVBIMRA_REG  EVBIMRA;   //0x752C
    union EVBIMRB_REG  EVBIMRB;   //0x752D
    union EVBIMRC_REG  EVBIMRC;   //0x752E
    union EVBIFRA_REG  EVBIFRA;   //0x752F
    union EVBIFRB_REG  EVBIFRB;   //0x7530
    union EVBIFRC_REG  EVBIFRC;   //0x7531
};



//---------------------------------------------------------------------------
// Function prototypes and external definitions:
//

extern volatile struct EVA_REGS EvaRegs;
extern volatile struct EVB_REGS EvbRegs;

#endif  // end of DSP28_EV.H definition


//===========================================================================
// No more.
//===========================================================================
