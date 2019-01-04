/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2018. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: Panel */
#define  PANEL_COMMANDBUTTON_6            2       /* control type: command, callback function: buttonClearCount */
#define  PANEL_COMMANDBUTTON_5            3       /* control type: command, callback function: buttonClearRev */
#define  PANEL_COMMANDBUTTON              4       /* control type: command, callback function: buttonSend */
#define  PANEL_COMMANDBUTTON_2            5       /* control type: command, callback function: buttonClearSend */
#define  PANEL_COMMANDBUTTON_3            6       /* control type: command, callback function: buttonHelp */
#define  PANEL_COMMANDBUTTON_4            7       /* control type: command, callback function: buttonQuit */
#define  PANEL_TEXTBOX                    8       /* control type: textBox, callback function: (none) */
#define  PANEL_RING_5                     9       /* control type: ring, callback function: (none) */
#define  PANEL_RING_4                     10      /* control type: ring, callback function: (none) */
#define  PANEL_RING_3                     11      /* control type: ring, callback function: (none) */
#define  PANEL_RING_2                     12      /* control type: ring, callback function: (none) */
#define  PANEL_RING                       13      /* control type: ring, callback function: (none) */
#define  PANEL_TEXTMSG                    14      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  15      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  16      /* control type: textMsg, callback function: (none) */
#define  PANEL_BINARYSWITCH               17      /* control type: binary, callback function: switchOpenCom */
#define  PANEL_LED                        18      /* control type: LED, callback function: (none) */
#define  PANEL_CHECKBOX_2                 19      /* control type: radioButton, callback function: butCheckI */
#define  PANEL_CHECKBOX                   20      /* control type: radioButton, callback function: butCheckX */
#define  PANEL_RING_6                     21      /* control type: ring, callback function: (none) */
#define  PANEL_A6                         22      /* control type: numeric, callback function: (none) */
#define  PANEL_A5                         23      /* control type: numeric, callback function: (none) */
#define  PANEL_A2                         24      /* control type: numeric, callback function: (none) */
#define  PANEL_A3                         25      /* control type: numeric, callback function: (none) */
#define  PANEL_TEXTMSG_4                  26      /* control type: textMsg, callback function: (none) */
#define  PANEL_A4                         27      /* control type: numeric, callback function: (none) */
#define  PANEL_A1                         28      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK butCheckI(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK butCheckX(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonClearCount(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonClearRev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonClearSend(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonHelp(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK buttonSend(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK switchOpenCom(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
