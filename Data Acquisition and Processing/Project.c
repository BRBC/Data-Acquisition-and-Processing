#include <advanlys.h>
#include <utility.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "Project.h"
#include "toolbox.h"

#define SAMPLE_RATE		0
#define NPOINTS			1


//==============================================================================
// Global variables
int waveInfo[2]; //waveInfo[0] = sampleRate
				 //waveInfo[1] = number of elements

sampleRate = 0.0;
int npoints = 0;
double *waveData = 0;
double *filtered=0;
double alpha=0.0;
int filtru_semnal;
const int SoundDim=6;
int start,stop;
int n_mediere;
int filtru_semnal;
char Unit[1000]="V";

int panelHandle = 0;
int freqHandle = 0;
WindowConst windowConst;
double *spectrum=0;


int main (int argc, char *argv[])
{
	int error = 0;
	
	/* initialize and load resources */
	nullChk (InitCVIRTE (0, argv, 0));
	errChk (panelHandle = LoadPanel (0, "Project.uir",PANEL));
	errChk (freqHandle = LoadPanel (0, "Project.uir", FREQUENCY));
	
	/* display the panel and run the user interface */
	errChk (DisplayPanel (panelHandle));
	errChk (RunUserInterface ());

Error:
	/* clean up */
	if (panelHandle > 0)
		DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK OnLoadButtonCB (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			//executa script python pentru conversia unui fisierului .wav in .txt
			LaunchExecutable("python main.py");
			
			//astept sa fie generate cele doua fisiere (modificati timpul daca este necesar
			Delay(4);
			
			//incarc informatiile privind rata de esantionare si numarul de valori
			FileToArray("wafeInfo.txt", waveInfo, VAL_INTEGER, 2, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			sampleRate = waveInfo[SAMPLE_RATE];
			npoints = waveInfo[NPOINTS];
			
			//alocare memorie pentru numarul de puncte
			waveData = (double *) calloc(npoints, sizeof(double));
			filtered=(double *)calloc(npoints,sizeof(double));
			
			
			
			
			//incarcare din fisierul .txt in memorie (vector)
			FileToArray("waveData.txt", waveData, VAL_DOUBLE, npoints, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
			//afisare pe grapf
		
			PlotY(panel,PANEL_GRAPH_RAW_DATA,waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			
			
			double maxVal = 0.0;
			double minVal = 0.0;
			int maxIndex = 0;
			int minIndex = 0;
			double mean = 0.0;
			double median=0.0;
			double dispersie=0.0;
			
	
			MaxMin1D(waveData,npoints,&maxVal,&maxIndex,&minVal,&minIndex);
			SetCtrlVal(panel,PANEL_IDC_NUM_MIN,minVal);
			SetCtrlVal(panel,PANEL_IDC_NUM_MAX,maxVal);
			
			Mean(waveData,npoints,&mean);
			SetCtrlVal(panel,PANEL_IDC_NUM_MEAN,mean);
			
			Median(waveData,npoints,&median);
			SetCtrlVal(panel,PANEL_IDC_NUM_MEDIAN,median);
			Variance(waveData,npoints,&median,&dispersie);
			SetCtrlVal(panel,PANEL_IDC_NUM_DISPERSIE,dispersie);
			
			
			break;
		
			
	}
	return 0;
}

int CVICALLBACK MainPanelCallback (int panel, int event, void *callbackData,
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

int CVICALLBACK OnApply (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int zeroCross=0;
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlVal(panel,PANEL_START,0);
			SetCtrlVal(panel,PANEL_STOP,1);
			//int filtru_semnal;
			for (int i=0;i<npoints-1;i++){
				if(((waveData[i]<0)&&(waveData[i+1]>0))||((waveData[i]>0)&&(waveData[i+1]<0)))
					zeroCross++;
			}
			SetCtrlVal(panel,PANEL_NUMERIC_ZEROCROSS,zeroCross);
			
			GetCtrlVal(panel,PANEL_BINARYSWITCH_FILTER,&filtru_semnal);
			DeleteGraphPlot(panelHandle,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
			if(filtru_semnal){
				//int n_mediere;
				double sum=0.0;
				GetCtrlVal(panelHandle,PANEL_BINARYSWITCH_MEDIERE,&n_mediere);
				for(int i=0;i<n_mediere;i++)
					sum+=waveData[i];
				for(int i=0;i<n_mediere;i++)
					filtered[i]=sum/n_mediere;
				for(int i=n_mediere;i<waveInfo[1];i++)
				{
					sum-=waveData[i-n_mediere];
					sum+=waveData[i];
					filtered[i]=sum/n_mediere;
				}
				
			}
			else if (!filtru_semnal)
			{
				GetCtrlVal(panel,PANEL_NUMERIC_ALPHA,&alpha);
			
				filtered[0]=waveData[0];
				for(int i=1;i<npoints;i++)
					filtered[i]=(1-alpha)*filtered[i-1]+alpha*waveData[i];
			}
				
			PlotY(panel,PANEL_GRAPH_FILTERED,filtered, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS,VAL_RED);

			break;
			
	}
	return 0;
}



int CVICALLBACK OnPrev (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	double *tempData=(double *)calloc(npoints/SoundDim,sizeof(double));;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_START,&start);
			GetCtrlVal(panelHandle,PANEL_STOP,&stop);
			DeleteGraphPlot(panelHandle,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
			if((start>0)&&(stop>1)){
				
				start--;
				stop--;
				SetCtrlVal(panelHandle,PANEL_START,start);
				SetCtrlVal(panelHandle,PANEL_STOP,stop);
			}
			for(int i=0;i<npoints/6;i++)
					tempData[i]=filtered[(int)(start*npoints/SoundDim+i)];
			PlotY(panel,PANEL_GRAPH_FILTERED,tempData, npoints/SoundDim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED );
	}
	free(tempData);
	return 0;
}

int CVICALLBACK OnNext (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	double *tempData=(double *)calloc(npoints/SoundDim,sizeof(double));
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_START,&start);
			GetCtrlVal(panelHandle,PANEL_STOP,&stop);
			DeleteGraphPlot(panelHandle,PANEL_GRAPH_FILTERED,-1,VAL_IMMEDIATE_DRAW);
				
			if(stop<SoundDim){

				start++;
				stop++;
				SetCtrlVal(panelHandle,PANEL_START,start);
				SetCtrlVal(panelHandle,PANEL_STOP,stop);
				
			}
			for(int i=0;i<npoints/6;i++)
					tempData[i]=filtered[(int)(start*npoints/SoundDim+i)];
				PlotY(panel,PANEL_GRAPH_FILTERED,tempData, npoints/SoundDim, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED );
			
			break;
	}
	free(tempData);
	return 0;
}

int CVICALLBACK OnPicture (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	char *nume_fisier=(char*)malloc(80);
	
	int imghandle;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlDisplayBitmap(panel,PANEL_GRAPH_FILTERED,1,&imghandle);
			
			if(filtru_semnal)
				sprintf(nume_fisier,"%s%d%s%d%s%d%s","C:\\Disc D\\Anul 3\\APD\\Proiect\\Lab2\\filtruMediere",n_mediere,"secunda",(int)start,"-",(int)stop,".jpg");
			else if(!filtru_semnal)
				sprintf(nume_fisier,"%s%.2f%s%d%s%d%s","C:\\Disc D\\Anul 3\\APD\\Proiect\\Lab2\\filtruAlpha",alpha,"secunda",(int)start,"-",(int)stop,".jpg");
			SaveBitmapToJPEGFile(imghandle,nume_fisier,JPEG_DCTFAST,100);

			DiscardBitmap(imghandle);
			break;
	}
	free(nume_fisier);
	return 0;
}


int CVICALLBACK OnPanelSwitch (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(panel == panelHandle)
			{
				SetCtrlVal(freqHandle, FREQUENCY_Panel_Switch, 1);
				DisplayPanel(freqHandle);
				HidePanel(panel);
			}
			else
			{
				SetCtrlVal(panelHandle,PANEL_Panel_Switch, 0);
				DisplayPanel(panelHandle);
				HidePanel(panel);
			}
			break;

	
	}
	return 0;
}

int CVICALLBACK OnFrequencyPanel (int panel, int event, void *callbackData,
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

int CVICALLBACK OnFreqAplicaButon (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int nr_puncte;
	GetCtrlVal(freqHandle, FREQUENCY_NUMERIC_FOURIER, &nr_puncte);
	double freq_peak, power_peak;
	
	double *autospectrum=(double *) calloc(nr_puncte/2, sizeof(double));
	double *freqData=(double *) calloc(nr_puncte, sizeof(double));
	double df;

	//int fs=44100;
	
	spectrum=(double *) calloc(nr_puncte/2, sizeof(double));
	
	if(waveData!=NULL)
	{
		switch(event)
		{
			case EVENT_COMMIT:
				int aux=0;
				while(aux<npoints)
				{
					if(aux+nr_puncte<=npoints)
					{
						for(int i=0;i<nr_puncte;i++)
							freqData[i]=waveData[i+aux];
					}
					else
					{
						int aux2=npoints-aux;
						for(int i=0;i<aux2;i++)
							freqData[i]=waveData[i+aux];
						for(int i=aux2;i<nr_puncte;i++)
							freqData[i]=0;
					}
					DeleteGraphPlot(freqHandle, FREQUENCY_GRAPH_FREQ, -1, VAL_DELAYED_DRAW);
					ScaledWindowEx(freqData, nr_puncte, RECTANGLE_, 0, &windowConst);
					AutoPowerSpectrum(freqData,nr_puncte, 1.0/sampleRate, autospectrum, &df);
					
					PowerFrequencyEstimate(autospectrum, nr_puncte/2, -1, windowConst, df, 7, &freq_peak,&power_peak);
					SpectrumUnitConversion(autospectrum,nr_puncte/2,0,SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS,df,windowConst,spectrum,Unit);
					PlotWaveform(freqHandle,FREQUENCY_GRAPH_FREQ,spectrum,nr_puncte/2,VAL_DOUBLE,1.0,0.0,0.0,df,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,1,VAL_MAGENTA);
					
					aux+=nr_puncte;
					SetCtrlVal(panel,FREQUENCY_NUMERIC_FREQ_PEAK,freq_peak);
					SetCtrlVal(panel,FREQUENCY_NUMERIC_POWER_PEAK,power_peak);
				

					Delay(2);
				}
				break;
		}
	}
	else
	{
		switch(event)
		{
			case EVENT_COMMIT:
				MessagePopup("Danger", "trebuie achizitionat semnal mai intai");
				break;
		}
	}
	return 0;
}


/*
int CVICALLBACK OnFreqAplicaButon (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	double Freq_Peak=0.0,Power_Peak=0.0;
	double df=0.0;
	char unit[32]="V";
	int npoints2=2048;
	int fs=44100;
	double frequencyWave[npoints2];
	double autospectrum[npoints2/2];

	double spectrum[npoints2/2];
	int nr_puncte;
	for (int i=0;i<npoints2;i++)
		frequencyWave[i]=waveData[i];
	
	switch (event)
	{
		case EVENT_COMMIT:
			PlotY(panel,FREQUENCY_GRAPH_FREQ_RAW_DATA,waveData, npoints, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, VAL_CONNECTED_POINTS, VAL_RED);
			
			DeleteGraphPlot(panel,FREQUENCY_GRAPH_FREQ,-1,VAL_IMMEDIATE_DRAW);
			
			GetCtrlVal(panel,FREQUENCY_NUMERIC_FOURIER,&nr_puncte);
			
			ScaledWindowEx(frequencyWave,npoints2,RECTANGLE,0,&windowConst);
			
			AutoPowerSpectrum(frequencyWave,npoints2,1.0/fs,autospectrum,&df);
			
			PowerFrequencyEstimate(autospectrum,npoints2/2,-1.0,windowConst,df,7,&Freq_Peak,&Power_Peak);
			
			SetCtrlVal(panel,FREQUENCY_NUMERIC_FREQ_PEAK,Freq_Peak);
			SetCtrlVal(panel,FREQUENCY_NUMERIC_POWER_PEAK,Power_Peak);
			
			SpectrumUnitConversion(autospectrum,npoints2/2,0,SCALING_MODE_LINEAR,DISPLAY_UNIT_VRMS,df,windowConst,spectrum,unit);
			
			PlotWaveform(freqHandle,FREQUENCY_GRAPH_FREQ,spectrum,npoints2/2,VAL_DOUBLE,1.0,0.0,0.0,df,VAL_THIN_LINE,VAL_EMPTY_SQUARE,VAL_SOLID,1,VAL_MAGENTA);

			break;
	}
	return 0;
}
*/