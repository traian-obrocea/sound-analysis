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

#define  FREQ_PANEL                       1       /* callback function: onMainPanel */
#define  FREQ_PANEL_PANEL_SWITCH          2       /* control type: binary, callback function: onPanelSwitch */
#define  FREQ_PANEL_SEMNAL_FILT           3       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SEMNAL                4       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SPECTRU               5       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_SPECTRU_FILT          6       /* control type: graph, callback function: (none) */
#define  FREQ_PANEL_FFT_NPOINTS_NUMERIC   7       /* control type: numeric, callback function: onFFTNPoints */
#define  FREQ_PANEL_FEREASTRA_SWITCH      8       /* control type: binary, callback function: onFreqFereastraSwitch */
#define  FREQ_PANEL_POWER_PEAK_NUMERIC_FI 9       /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FREQ_PEAK_NUMERIC_FIL 10      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_POWER_PEAK_NUMERIC    11      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_FREQ_PEAK_NUMERIC     12      /* control type: numeric, callback function: (none) */
#define  FREQ_PANEL_TIP_FILTRU_SWITCH     13      /* control type: binary, callback function: onFiltruSwitch */
#define  FREQ_PANEL_TEXTMSG               14      /* control type: textMsg, callback function: (none) */
#define  FREQ_PANEL_SPECTRU_TIMER         15      /* control type: timer, callback function: onSpectruTimer */

#define  MAIN_PANEL                       2       /* callback function: onMainPanel */
#define  MAIN_PANEL_LOAD_WAVE_BUTTON      2       /* control type: command, callback function: onLoadWaveButton */
#define  MAIN_PANEL_FILTERED_WAVE_GRAPH   3       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_RAW_WAVE_GRAPH        4       /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_MAX_IDX_DISP          5       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MAX_DISP              6       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_ZERO_CROSSING_DISP    7       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIANA_DISP          8       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MEDIE_DISP            9       /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_DISPERSIE_DISP        10      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MIN_IDX_DISP          11      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_MIN_DISP              12      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_HISTOGRAM_GRAPH       13      /* control type: graph, callback function: (none) */
#define  MAIN_PANEL_FEREASTRA_NUMERIC     14      /* control type: numeric, callback function: onAlphaNumericCB */
#define  MAIN_PANEL_ALPHA_NUMERIC         15      /* control type: numeric, callback function: onAlphaNumericCB */
#define  MAIN_PANEL_AFISARE_SEC_BUTTON    16      /* control type: textButton, callback function: onAfisarePeSecundaButton */
#define  MAIN_PANEL_SEC_CURENTA_NUMERIC   17      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_SEC_URM_BUTTON        18      /* control type: command, callback function: onSecUrmatoareButton */
#define  MAIN_PANEL_SEC_PREC_BUTTON       19      /* control type: command, callback function: onSecPrecedentaButton */
#define  MAIN_PANEL_KURTOSIS_NUMERIC      20      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_SKEWNESS_NUMERIC      21      /* control type: numeric, callback function: (none) */
#define  MAIN_PANEL_FILTRARE_SWITCH       22      /* control type: slide, callback function: onFiltrareSwitch */
#define  MAIN_PANEL_PANEL_SWITCH          23      /* control type: binary, callback function: onPanelSwitch */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onAfisarePeSecundaButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onAlphaNumericCB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onFFTNPoints(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onFiltrareSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onFiltruSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onFreqFereastraSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onLoadWaveButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onMainPanel(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onPanelSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onSecPrecedentaButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onSecUrmatoareButton(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onSpectruTimer(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif