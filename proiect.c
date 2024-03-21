#include <advanlys.h>
#include "toolbox.h"
#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include "proiect.h"
#include "func.h"
static int tip_filtrare;
static double median;
static double variance;
static double mean;

#define SAMPLE_RATE_IDX 0
#define NPOINTS_IDX 1

#define FER_HAMMING 0
#define FER_FLATTOP 1
#define FIL_BESSEL 0
#define FIL_BUTTERWORTH 1

static int mainPanel;
static int freqPanel;

int waveInfo[2];
double sampleRate = 0;
int npoints = 0;
double *waveData = 0;

const int nr_intervale = 20;

static ssize_t minIdx;
static double minVal;
static ssize_t maxIdx;
static double maxVal;
unsigned int zero_crossing_count;
ssize_t histogramArray[nr_intervale];
double axisArray[nr_intervale];
static int filteredPlotHandle = -1;
static int afisarePeSecunda = 0;
static int secundaSelectata = 0;

static int freqSpectruPos = 0;
static int freqSpectruNPoints = 1024;
static int freqTipFereastra = FER_HAMMING;
static int freqTipFiltru = FIL_BESSEL;


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((mainPanel = LoadPanel (0, "proiect.uir", MAIN_PANEL)) < 0)
		return -1;
    if ((freqPanel = LoadPanel(0, "proiect.uir", FREQ_PANEL)) < 0)
        return -1;
	DisplayPanel (mainPanel);
	RunUserInterface ();
	DiscardPanel (mainPanel);
	return 0;
}

int CVICALLBACK onMainPanel (int panel, int event, void *callbackData,
							 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

void filterAndPlot() {
	if (waveData != 0) {
		double* data;
		int nr_puncte;
		
		if (afisarePeSecunda == 1) {
			int offset = secundaSelectata * sampleRate;
// TODO allocate one chunk of memory at the start, don't allocate every time
			data = malloc((int)sampleRate*sizeof(double));
			Subset1D (waveData, npoints, offset, sampleRate, data);
			nr_puncte = sampleRate;
		} else {
			data = waveData;
			nr_puncte = npoints;
		}
		
	    double* filtrat = malloc(sizeof *data * nr_puncte);
	    if (tip_filtrare == 0) {
			double alpha;
			GetCtrlVal(mainPanel, MAIN_PANEL_ALPHA_NUMERIC, &alpha);
	        filtrare_ordin_I(data, nr_puncte, alpha, filtrat);
	    } else if (tip_filtrare == 1) {
			int fereastra;
			GetCtrlVal(mainPanel, MAIN_PANEL_FEREASTRA_NUMERIC, &fereastra);
	        filtrare_prin_mediere(data, nr_puncte, fereastra, filtrat);
	    } else {
            double initial_cond[] = {0};
            DifferenceEx(data, nr_puncte, 1, initial_cond, 1, NULL, 0, 3, filtrat);
        }
	    
		if (filteredPlotHandle != -1) {
			DeleteGraphPlot (mainPanel, MAIN_PANEL_FILTERED_WAVE_GRAPH, filteredPlotHandle, VAL_IMMEDIATE_DRAW);
		}
	    filteredPlotHandle = PlotY(mainPanel, MAIN_PANEL_FILTERED_WAVE_GRAPH, filtrat, nr_puncte, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
	    
		if (afisarePeSecunda==1) {
			free(data);
		}
	    free(filtrat);

	}
}

int CVICALLBACK onLoadWaveButton (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	double *peak_2nd_derivs;
	double *peak_amplitudes;
	double *peak_locations;
	ssize_t peaks_count;
	
	switch (event)
	{
		case EVENT_COMMIT:
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE_IDX];
			npoints = waveInfo[NPOINTS_IDX];
			
			if (sampleRate * 10 < npoints) {
				npoints = sampleRate*6;
			}
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			
			//incarcare din fisierul .txt in memorie (vector)
			//FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			FileToArray ("waveData.txt", waveData, VAL_DOUBLE, npoints, 2, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);

			
			//afisare pe graf
			PlotY(mainPanel, MAIN_PANEL_RAW_WAVE_GRAPH, waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			MaxMin1D (waveData, npoints, &maxVal, &maxIdx, &minVal, &minIdx);
			SetCtrlVal (mainPanel, MAIN_PANEL_MIN_DISP, minVal);
			SetCtrlVal (mainPanel, MAIN_PANEL_MIN_IDX_DISP, minIdx);
			SetCtrlVal (mainPanel, MAIN_PANEL_MAX_DISP, maxVal);
			SetCtrlVal (mainPanel, MAIN_PANEL_MAX_IDX_DISP, maxIdx);
			
			Variance (waveData, npoints, &mean, &variance);
			SetCtrlVal(mainPanel, MAIN_PANEL_MEDIE_DISP, mean);
			SetCtrlVal(mainPanel, MAIN_PANEL_DISPERSIE_DISP, sqrt(variance));
			
			Median (waveData, npoints, &median);
			SetCtrlVal(mainPanel, MAIN_PANEL_MEDIANA_DISP, median);
			
            zero_crossing_count = zero_crossing(waveData, npoints);
            SetCtrlVal(mainPanel, MAIN_PANEL_ZERO_CROSSING_DISP, zero_crossing_count);
            
            double skewness;
            Moment (waveData, 256, 3, &skewness);
            
            double kurtosis;
            Moment(waveData, 256, 4, &kurtosis);
            
            SetCtrlVal(mainPanel, MAIN_PANEL_SKEWNESS_NUMERIC, skewness);
            SetCtrlVal(mainPanel, MAIN_PANEL_KURTOSIS_NUMERIC, kurtosis);
            
            Histogram (waveData, npoints, minVal, maxVal, histogramArray, axisArray, nr_intervale);
            PlotXY (mainPanel, MAIN_PANEL_HISTOGRAM_GRAPH, axisArray, histogramArray, nr_intervale, VAL_DOUBLE, VAL_SSIZE_T, VAL_VERTICAL_BAR, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
            
            PeakDetector (waveData, npoints, 0, 3, DETECT_PEAKS, ANALYSIS_TRUE, ANALYSIS_TRUE, &peaks_count, &peak_locations, &peak_amplitudes, &peak_2nd_derivs);
            PlotXY (mainPanel, MAIN_PANEL_RAW_WAVE_GRAPH, peak_locations, peak_amplitudes, peaks_count, VAL_DOUBLE, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_BLUE);
            FreeAnalysisMem(peak_locations);
            FreeAnalysisMem(peak_2nd_derivs);
            FreeAnalysisMem(peak_amplitudes);
            
			filterAndPlot();
            
			break;
	}
	return 0;
}

int CVICALLBACK onFiltrareSwitch (int panel, int control, int event,
                                  void *callbackData, int eventData1, int eventData2) {
    switch (event) {
        case EVENT_COMMIT:
            GetCtrlVal (mainPanel, MAIN_PANEL_FILTRARE_SWITCH, &tip_filtrare);
			
			SetCtrlAttribute(mainPanel, MAIN_PANEL_ALPHA_NUMERIC, ATTR_DIMMED, !(tip_filtrare==0));
			SetCtrlAttribute(mainPanel, MAIN_PANEL_FEREASTRA_NUMERIC, ATTR_DIMMED, !(tip_filtrare==1));
			
			filterAndPlot();
            break;
    }
    return 0;
}

int CVICALLBACK onAlphaNumericCB (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			filterAndPlot();
			break;
	}
	return 0;
}

void saveImage() {
    char fileName[30];
    int bitmapId;
    sprintf(fileName, "./grafic_%d.jpg", secundaSelectata);
    GetCtrlDisplayBitmap(mainPanel, MAIN_PANEL_FILTERED_WAVE_GRAPH, 1, &bitmapId);
    SaveBitmapToJPEGFile(bitmapId, fileName, JPEG_PROGRESSIVE, 100);
}

int CVICALLBACK onAfisarePeSecundaButton (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(mainPanel, MAIN_PANEL_AFISARE_SEC_BUTTON, &afisarePeSecunda);
			SetCtrlAttribute(mainPanel, MAIN_PANEL_SEC_PREC_BUTTON, ATTR_DIMMED, !afisarePeSecunda);
			SetCtrlAttribute(mainPanel, MAIN_PANEL_SEC_URM_BUTTON, ATTR_DIMMED, !afisarePeSecunda);
            filterAndPlot();
			break;
	}
	return 0;
}

int CVICALLBACK onSecPrecedentaButton (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(mainPanel, MAIN_PANEL_SEC_CURENTA_NUMERIC, &secundaSelectata);
			if (secundaSelectata > 0) {
				SetCtrlVal(mainPanel, MAIN_PANEL_SEC_CURENTA_NUMERIC, --secundaSelectata);
			}
			filterAndPlot();
            saveImage();
			break;
	}
	return 0;
}

int CVICALLBACK onSecUrmatoareButton (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(mainPanel, MAIN_PANEL_SEC_CURENTA_NUMERIC, &secundaSelectata);
            if (secundaSelectata < 5) {
			    SetCtrlVal(mainPanel, MAIN_PANEL_SEC_CURENTA_NUMERIC, ++secundaSelectata);
            }
			filterAndPlot();
            saveImage();
			break;
	}
	return 0;
}

int CVICALLBACK onPanelSwitch(int panel, int control, int event, void *callbackData, int eventData1, int eventData2) {
    switch (event) {
        case EVENT_COMMIT:
            if (panel == mainPanel){
                SetCtrlVal(mainPanel, MAIN_PANEL_PANEL_SWITCH, 0);
                DisplayPanel(freqPanel);
                HidePanel(mainPanel);
                if (waveData != 0) {
                    SetCtrlAttribute (freqPanel, FREQ_PANEL_SPECTRU_TIMER, ATTR_ENABLED, 1);
                }
            } else {
                SetCtrlVal(freqPanel, FREQ_PANEL_PANEL_SWITCH, 1);
                DisplayPanel(mainPanel);
                HidePanel(freqPanel);
                if (waveData != 0) {
                    SetCtrlAttribute (freqPanel, FREQ_PANEL_SPECTRU_TIMER, ATTR_ENABLED, 0);
                }
            }
    }

    return 0;
}

int CVICALLBACK onSpectruTimer (int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2) {
    switch (event) {
        case EVENT_TIMER_TICK:
            WindowConst windowConst;
            int np = freqSpectruNPoints;
            if (freqSpectruPos + np > npoints) {
                freqSpectruPos = 0;
            }
            double samples[np];
            Copy1D(waveData+freqSpectruPos, np, samples);
            
            DeleteGraphPlot(freqPanel, FREQ_PANEL_SEMNAL, -1, VAL_IMMEDIATE_DRAW);
            PlotY(freqPanel, FREQ_PANEL_SEMNAL, samples, np, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
            
            int windowType;
            if (freqTipFereastra == FER_HAMMING) {
                windowType = HAMMING;
            } else {
                windowType = FLATTOP;
            }
            double df;
            double autoSpectrum[np/2], convertedSpectrum[np/2];
            char unit[32] = "V";
            double freqPeak, powerPeak;
            
            ScaledWindowEx(samples, np, windowType, 0, &windowConst);
            
            AutoPowerSpectrum(samples, np, 1/sampleRate, autoSpectrum, &df);
            
            PowerFrequencyEstimate(autoSpectrum, np/2, -1.0, windowConst, df, 7, &freqPeak, &powerPeak);
            
            SetCtrlVal(freqPanel, FREQ_PANEL_FREQ_PEAK_NUMERIC, freqPeak);
            SetCtrlVal(freqPanel, FREQ_PANEL_POWER_PEAK_NUMERIC, powerPeak);
            
            SpectrumUnitConversion (autoSpectrum, np/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, windowConst, convertedSpectrum, unit);
            
            DeleteGraphPlot(freqPanel, FREQ_PANEL_SPECTRU, -1, VAL_IMMEDIATE_DRAW);
            PlotWaveform(freqPanel, FREQ_PANEL_SPECTRU, convertedSpectrum, np/2, VAL_DOUBLE, 1.0, 0.0, 0.0, 2*df, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);

            
            Copy1D(waveData+freqSpectruPos, np, samples);
            double fl = 0.25 * 0.5 * sampleRate;
            double fh = 0.75 * 0.5 * sampleRate;
            if (freqTipFiltru == FIL_BESSEL) {
                Bssl_BPF(samples, np, sampleRate, fl, fh, 5, samples); 
            } else {
                Bw_BPF(samples, np, sampleRate, fl, fh, 5, samples);
            }
            
            DeleteGraphPlot(freqPanel, FREQ_PANEL_SEMNAL_FILT, -1, VAL_IMMEDIATE_DRAW);
            PlotY(freqPanel, FREQ_PANEL_SEMNAL_FILT, samples, np, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
            
            ScaledWindowEx(samples, np, windowType, 0, &windowConst);
            
            AutoPowerSpectrum(samples, np, 1/sampleRate, autoSpectrum, &df);
            
            PowerFrequencyEstimate(autoSpectrum, np/2, -1.0, windowConst, df, 7, &freqPeak, &powerPeak);
            
            SetCtrlVal(freqPanel, FREQ_PANEL_FREQ_PEAK_NUMERIC_FIL, freqPeak);
            SetCtrlVal(freqPanel, FREQ_PANEL_POWER_PEAK_NUMERIC_FI, powerPeak);
            
            SpectrumUnitConversion (autoSpectrum, np/2, SPECTRUM_POWER, SCALING_MODE_LINEAR, DISPLAY_UNIT_VRMS, df, windowConst, convertedSpectrum, unit);
            
            DeleteGraphPlot(freqPanel, FREQ_PANEL_SPECTRU_FILT, -1, VAL_IMMEDIATE_DRAW);
            PlotWaveform(freqPanel, FREQ_PANEL_SPECTRU_FILT, convertedSpectrum, np/2, VAL_DOUBLE, 1.0, 0.0, 0.0, 2*df, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
            
            freqSpectruPos += np;
            
            char fileName[70];
            int bitmapId;
            char * time = TimeStr();
            time[2] = '.';
            time[5] = '.';
            char * date = DateStr();

            sprintf(fileName, "./grafic_frecv_%s_%s_semnal.jpg", time, date);
            GetCtrlDisplayBitmap(freqPanel, FREQ_PANEL_SEMNAL, 1, &bitmapId);
            SaveBitmapToJPEGFile(bitmapId, fileName, JPEG_PROGRESSIVE, 100);
            DiscardBitmap(bitmapId);
            
            sprintf(fileName, "./grafic_frecv_%s_%s_semnal_filt.jpg", time, date);
            GetCtrlDisplayBitmap(freqPanel, FREQ_PANEL_SEMNAL_FILT, 1, &bitmapId);
            SaveBitmapToJPEGFile(bitmapId, fileName, JPEG_PROGRESSIVE, 100);
            DiscardBitmap(bitmapId);
            
            sprintf(fileName, "./grafic_frecv_%s_%s_spectru.jpg", time, date);
            GetCtrlDisplayBitmap(freqPanel, FREQ_PANEL_SPECTRU, 1, &bitmapId);
            SaveBitmapToJPEGFile(bitmapId, fileName, JPEG_PROGRESSIVE, 100);
            DiscardBitmap(bitmapId);
            
            sprintf(fileName, "./grafic_frecv_%s_%s_spectru_filt.jpg", time, date);
            GetCtrlDisplayBitmap(freqPanel, FREQ_PANEL_SEMNAL_FILT, 1, &bitmapId);
            SaveBitmapToJPEGFile(bitmapId, fileName, JPEG_PROGRESSIVE, 100);
            DiscardBitmap(bitmapId);
            
            break;
    }
    return 0;
}

int CVICALLBACK onFFTNPoints (int panel, int control, int event,
                              void *callbackData, int eventData1, int eventData2) {
    switch (event) {
        case EVENT_COMMIT:
                GetCtrlVal(freqPanel, FREQ_PANEL_FFT_NPOINTS_NUMERIC, &freqSpectruNPoints);
            break;
    }
    return 0;
}

int CVICALLBACK onFreqFereastraSwitch (int panel, int control, int event,
                                       void *callbackData, int eventData1, int eventData2) {
    switch (event) {
        case EVENT_COMMIT:
                GetCtrlVal(freqPanel, FREQ_PANEL_FEREASTRA_SWITCH, &freqTipFereastra);
            break;
    }
    return 0;
}

int CVICALLBACK onFiltruSwitch (int panel, int control, int event,
                                void *callbackData, int eventData1, int eventData2) {
    switch (event) {
        case EVENT_COMMIT:
                GetCtrlVal(freqPanel, FREQ_PANEL_TIP_FILTRU_SWITCH, &freqTipFiltru);
            break;
    }
    return 0;
}
