/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  FREQUENCY                        1       /* callback function: OnFrequencyPanel */
#define  FREQUENCY_GRAPH_FREQ_RAW_DATA    2       /* control type: graph, callback function: (none) */
#define  FREQUENCY_Panel_Switch           3       /* control type: binary, callback function: OnPanelSwitch */
#define  FREQUENCY_GRAPH_FREQ             4       /* control type: graph, callback function: (none) */
#define  FREQUENCY_FREQAPPLYBUTTON        5       /* control type: command, callback function: OnFreqAplicaButon */
#define  FREQUENCY_NUMERIC_FOURIER        6       /* control type: numeric, callback function: (none) */
#define  FREQUENCY_NUMERIC_POWER_PEAK     7       /* control type: numeric, callback function: (none) */
#define  FREQUENCY_NUMERIC_FREQ_PEAK      8       /* control type: numeric, callback function: (none) */
#define  FREQUENCY_STOP2                  9       /* control type: numeric, callback function: (none) */
#define  FREQUENCY_START2                 10      /* control type: numeric, callback function: (none) */
#define  FREQUENCY_COMMANDBUTTON_PREV2    11      /* control type: command, callback function: OnPrev */
#define  FREQUENCY_COMMANDBUTTON_NEXT2    12      /* control type: command, callback function: OnNext */

#define  PANEL                            2       /* callback function: MainPanelCallback */
#define  PANEL_GRAPH_RAW_DATA             2       /* control type: graph, callback function: (none) */
#define  PANEL_COMMANDBUTTON              3       /* control type: command, callback function: OnLoadButtonCB */
#define  PANEL_GRAPH_FILTERED             4       /* control type: graph, callback function: (none) */
#define  PANEL_IDC_NUM_MEAN               5       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_NUM_DISPERSIE          6       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_NUM_MEDIAN             7       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_NUM_MIN                8       /* control type: numeric, callback function: (none) */
#define  PANEL_IDC_NUM_MAX                9       /* control type: numeric, callback function: (none) */
#define  PANEL_NUMERIC_ALPHA              10      /* control type: numeric, callback function: (none) */
#define  PANEL_BINARYSWITCH_FILTER        11      /* control type: binary, callback function: (none) */
#define  PANEL_COMMANDBUTTON_APPLY        12      /* control type: command, callback function: OnApply */
#define  PANEL_BINARYSWITCH_MEDIERE       13      /* control type: binary, callback function: (none) */
#define  PANEL_STOP                       14      /* control type: numeric, callback function: (none) */
#define  PANEL_START                      15      /* control type: numeric, callback function: (none) */
#define  PANEL_COMMANDBUTTON_PREV         16      /* control type: command, callback function: OnPrev */
#define  PANEL_COMMANDBUTTON_NEXT         17      /* control type: command, callback function: OnNext */
#define  PANEL_COMMANDBUTTON_PICTURE      18      /* control type: command, callback function: OnPicture */
#define  PANEL_Panel_Switch               19      /* control type: binary, callback function: OnPanelSwitch */
#define  PANEL_NUMERIC_ZEROCROSS          20      /* control type: numeric, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK MainPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnApply(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFreqAplicaButon(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnFrequencyPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnLoadButtonCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnNext(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPanelSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPicture(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnPrev(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif