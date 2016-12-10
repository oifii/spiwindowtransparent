/*
 * Copyright (c) 2015-2016 Stephane Poirier
 *
 * stephane.poirier@oifii.org
 *
 * Stephane Poirier
 * 3532 rue Ste-Famille, #3
 * Montreal, QC, H2X 2L1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

//////////////////////////////////////////////////////
//include all headers required to have a minimal 
//transparent windows application with tonic portaudio
//////////////////////////////////////////////////////

#include "stdafx.h"
#include "spiwindowtransparent.h"
#include "FreeImage.h"
#include <shellapi.h> //for CommandLineToArgW()
#include <mmsystem.h> //for timeSetEvent()
#include <stdio.h> //for swprintf()
#include <assert.h>
#include "spiwavsetlib.h"


#include "portaudio.h"

#ifdef WIN32
#if PA_USE_ASIO
#include "pa_asio.h"
#endif
#endif

#include "Tonic.h"
/*
#include "ControlSwitcherTestSynth.h"
#include "ControlSwitcherExpSynth.h"
#include "BasicSynth.h"
#include "SimpleInstrumentSynth.h"
#include "StepSequencerSynth.h"
#include "StepSequencerExpSynth.h"
#include "StepSequencerBufferPlayerExpSynth.h"
#include "StepSequencerBufferPlayerEffectExpSynth.h"
//#include "EventsSynth.h"
#include "EventsExpSynth.h"
#include "BufferPlayerExpSynth.h"
#include "ArbitraryTableLookupSynth.h"
#include "BandlimitedOscillatorTestSynth.h"
#include "CompressorDuckingTestSynth.h"
#include "CompressorTestSynth.h"
#include "CompressorExpSynth.h"
#include "ControlSnapToScaleTestSynth.h"
#include "DelayTestSynth.h"
#include "FilteredNoiseSynth.h"
#include "FilterExpSynth.h"
#include "FMDroneSynth.h"
#include "InputDemoSynth.h"
#include "InputExpSynth.h"
#include "InputProcessSynth.h"
#include "InputProcessFlangerSynth.h"
#include "LFNoiseTestSynth.h"
#include "ReverbTestSynth.h"
#include "SimpleStepSeqSynth.h"
#include "SineSumSynth.h"
#include "StereoDelayTestSynth.h"
#include "SynthsAsGeneratorsDemoSynth.h"
#include "XYSpeedSynth.h"
*/
#include "InputProcessSynth.h"
using namespace Tonic;


#include <string>
#include <map>
using namespace std;

#include "spiutility.h"
#include "defs.h"
#include "resource.h"


//////////////////////////////////////////////////////
//include all headers related to your visual interface
//////////////////////////////////////////////////////

#include "CMainWindow.h"
#include "CFirstWindow.h"
#include "CSecondWindow.h"







/////////////////////
//globals
/////////////////////

RingBufferWriter global_RingBufferWriter;

// Static smart pointer for our Synth
/*
static Synth synth;
*/
//static ControlSwitcherTestSynth synth;
//static ControlSwitcherExpSynth synth;
//static BasicSynth synth;
//static SimpleInstrumentSynth synth;
//static StepSequencerSynth synth;
//static StepSequencerExpSynth synth;
//static StepSequencerBufferPlayerExpSynth synth;
//static StepSequencerBufferPlayerEffectExpSynth synth;
//static EventsSynth synth;
//static EventsExpSynth synth;
//static BufferPlayerExpSynth synth;
//static ArbitraryTableLookupSynth synth;
//static BandlimitedOscillatorTestSynth synth;
//static CompressorDuckingTestSynth synth;
//static CompressorTestSynth synth;
//static CompressorExpSynth synth;
//static ControlSnapToScaleTestSynth synth;
//static DelayTestSynth synth;
//static FilteredNoiseSynth synth;
//static FilterExpSynth synth;
//static FMDroneSynth synth;
//static InputDemoSynth* global_pSynth;
//static InputExpSynth* global_pSynth;
static InputProcessSynth* global_pSynth;
//static InputProcessFlangerSynth* global_pSynth;
//static LFNoiseTestSynth synth;
//static ReverbTestSynth synth;
//static SimpleStepSeqSynth synth;
//static SineSumSynth synth;
//static StereoDelayTestSynth synth;
//static SynthsAsGeneratorsDemoSynth synth;
//static XYSpeedSynth synth;


std::map<string, int> global_inputdevicemap;
std::map<string, int> global_outputdevicemap;


PaStream* global_stream;
PaStreamParameters global_inputParameters;
PaStreamParameters global_outputParameters;
PaError global_err;
string global_audioinputdevicename = "";
string global_audiooutputdevicename = "";
int global_inputAudioChannelSelectors[2];
int global_outputAudioChannelSelectors[2];
PaAsioStreamInfo global_asioInputInfo;
PaAsioStreamInfo global_asioOutputInfo;

FILE* pFILE = NULL;

// Global Variables:

FIBITMAP* global_dib;
HFONT global_hFont;
HFONT global_hFontHuge;
HWND global_hwnd = NULL;
MMRESULT global_timer = 0;
//#define MAX_GLOBALTEXT	4096
//WCHAR global_text[MAX_GLOBALTEXT+1];
//int global_delay_ms=5000; //default to 5 seconds delay
float global_cutfreq_hz = 1000.0f; //default to 1000.0 Hz
float global_duration_sec = 180;
int global_x = 100;
int global_y = 200;
int global_xwidth = 400;
int global_yheight = 400;
BYTE global_alpha = 200;
int global_fontheight = 24;
int global_fontwidth = -1; //will be computed within WM_PAINT handler
int global_staticalignment = 0; //0 for left, 1 for center and 2 for right
int global_staticheight = -1; //will be computed within WM_SIZE handler
int global_staticwidth = -1; //will be computed within WM_SIZE handler 
//COLORREF global_statictextcolor=RGB(0xFF, 0xFF, 0xFF); //white
COLORREF global_statictextcolor = RGB(0xFF, 0x00, 0x00); //red
//spi, begin
int global_imageheight = -1; //will be computed within WM_SIZE handler
int global_imagewidth = -1; //will be computed within WM_SIZE handler 
//spi, end
int global_titlebardisplay = 1; //0 for off, 1 for on
int global_acceleratoractive = 0; //0 for off, 1 for on
int global_menubardisplay = 0; //0 for off, 1 for on

DWORD global_startstamp_ms;
//FILE* global_pFILE=NULL;
string global_line;
std::ifstream global_ifstream;

#define IDC_MAIN_EDIT	100
#define IDC_MAIN_STATIC	101

//HINSTANCE hInst;								// current instance
TCHAR global_szTitle[1024] = { L"spiwindowtransparenttitle" };					// The title bar text
TCHAR global_szWindowClass[1024] = { L"spiwindowtransparentclass" };				// the main window class name

//new parameters
int global_statictextcolor_red = 255;
int global_statictextcolor_green = 0;
int global_statictextcolor_blue = 0;
string global_begin = "begin.ahk";
string global_end = "end.ahk";

bool global_abort = false;



///////////////////////////
//implementation
///////////////////////////

static int renderCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData);

static int gNumNoInputs = 0;
// This routine will be called by the PortAudio engine when audio is needed.
// It may be called at interrupt level on some machines so don't do anything
// that could mess up the system like calling malloc() or free().
//
static int renderCallback(const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData)
{
	SAMPLE *out = (SAMPLE*)outputBuffer;
	const SAMPLE *in = (const SAMPLE*)inputBuffer;
	unsigned int i;
	(void)timeInfo; // Prevent unused variable warnings.
	(void)statusFlags;
	(void)userData;

	if (global_abort == true) return paAbort;


	if (inputBuffer == NULL)
	{
		for (i = 0; i<framesPerBuffer; i++)
		{
			*out++ = 0;  // left - silent
			*out++ = 0;  // right - silent 
		}
		gNumNoInputs += 1;
	}
	else
	{
		/*
		for (i = 0; i<framesPerBuffer; i++)
		{
		*out++ = *in++;  // left - unprocessed
		*out++ = *in++;  // right - unprocessed
		}
		*/
		global_RingBufferWriter.write((float*)inputBuffer, framesPerBuffer, NUM_CHANNELS);
		global_pSynth->fillBufferOfFloats((float*)outputBuffer, framesPerBuffer, NUM_CHANNELS);

		/*
		if (0) pthread_mutex_lock(&g_mutex);
		callbackFilledAudioBuffers->acquireAndSeparateNewBuffer((float*)outputBuffer);
		g_ready = true;
		if (0) pthread_mutex_unlock(&g_mutex);
		*/
	}


	return paContinue;
}

bool SelectAudioInputDevice()
{
	const PaDeviceInfo* deviceInfo;
	int numDevices = Pa_GetDeviceCount();
	for (int i = 0; i<numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		string devicenamestring = deviceInfo->name;
		global_inputdevicemap.insert(pair<string, int>(devicenamestring, i));
		if (pFILE) fprintf(pFILE, "id=%d, name=%s\n", i, devicenamestring.c_str());
	}

	int deviceid = Pa_GetDefaultInputDevice(); // default input device 
	std::map<string, int>::iterator it;
	it = global_inputdevicemap.find(global_audioinputdevicename);
	if (it != global_inputdevicemap.end())
	{
		deviceid = (*it).second;
		//printf("%s maps to %d\n", global_audiodevicename.c_str(), deviceid);
		deviceInfo = Pa_GetDeviceInfo(deviceid);
		//assert(inputAudioChannelSelectors[0]<deviceInfo->maxInputChannels);
		//assert(inputAudioChannelSelectors[1]<deviceInfo->maxInputChannels);
	}
	else
	{
		//Pa_Terminate();
		//return -1;
		//printf("error, audio device not found, will use default\n");
		//MessageBox(win,"error, audio device not found, will use default\n",0,0);
		deviceid = Pa_GetDefaultInputDevice();
	}


	global_inputParameters.device = deviceid;
	if (global_inputParameters.device == paNoDevice)
	{
		//MessageBox(win,"error, no default input device.\n",0,0);
		return false;
	}
	//global_inputParameters.channelCount = 2;
	global_inputParameters.channelCount = NUM_CHANNELS;
	global_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
	global_inputParameters.suggestedLatency = Pa_GetDeviceInfo(global_inputParameters.device)->defaultLowOutputLatency;
	//inputParameters.hostApiSpecificStreamInfo = NULL;

	//Use an ASIO specific structure. WARNING - this is not portable. 
	//PaAsioStreamInfo asioInputInfo;
	global_asioInputInfo.size = sizeof(PaAsioStreamInfo);
	global_asioInputInfo.hostApiType = paASIO;
	global_asioInputInfo.version = 1;
	global_asioInputInfo.flags = paAsioUseChannelSelectors;
	global_asioInputInfo.channelSelectors = global_inputAudioChannelSelectors;
	if (deviceid == Pa_GetDefaultInputDevice())
	{
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO)
	{
		global_inputParameters.hostApiSpecificStreamInfo = &global_asioInputInfo;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS)
	{
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else
	{
		//assert(false);
		global_inputParameters.hostApiSpecificStreamInfo = NULL;
	}
	return true;
}



bool SelectAudioOutputDevice()
{
	const PaDeviceInfo* deviceInfo;
	int numDevices = Pa_GetDeviceCount();
	for (int i = 0; i<numDevices; i++)
	{
		deviceInfo = Pa_GetDeviceInfo(i);
		string devicenamestring = deviceInfo->name;
		global_outputdevicemap.insert(pair<string, int>(devicenamestring, i));
		if (pFILE) fprintf(pFILE, "id=%d, name=%s\n", i, devicenamestring.c_str());
	}

	int deviceid = Pa_GetDefaultOutputDevice(); // default output device 
	std::map<string, int>::iterator it;
	it = global_outputdevicemap.find(global_audiooutputdevicename);
	if (it != global_outputdevicemap.end())
	{
		deviceid = (*it).second;
		//printf("%s maps to %d\n", global_audiodevicename.c_str(), deviceid);
		deviceInfo = Pa_GetDeviceInfo(deviceid);
		//assert(inputAudioChannelSelectors[0]<deviceInfo->maxInputChannels);
		//assert(inputAudioChannelSelectors[1]<deviceInfo->maxInputChannels);
	}
	else
	{
		//Pa_Terminate();
		//return -1;
		//printf("error, audio device not found, will use default\n");
		//MessageBox(win,"error, audio device not found, will use default\n",0,0);
		deviceid = Pa_GetDefaultOutputDevice();
	}


	global_outputParameters.device = deviceid;
	if (global_outputParameters.device == paNoDevice)
	{
		//MessageBox(win,"error, no default output device.\n",0,0);
		return false;
	}
	//global_inputParameters.channelCount = 2;
	global_outputParameters.channelCount = NUM_CHANNELS;
	global_outputParameters.sampleFormat = PA_SAMPLE_TYPE;
	global_outputParameters.suggestedLatency = Pa_GetDeviceInfo(global_outputParameters.device)->defaultLowOutputLatency;
	//outputParameters.hostApiSpecificStreamInfo = NULL;

	//Use an ASIO specific structure. WARNING - this is not portable. 
	//PaAsioStreamInfo asioInputInfo;
	global_asioOutputInfo.size = sizeof(PaAsioStreamInfo);
	global_asioOutputInfo.hostApiType = paASIO;
	global_asioOutputInfo.version = 1;
	global_asioOutputInfo.flags = paAsioUseChannelSelectors;
	global_asioOutputInfo.channelSelectors = global_outputAudioChannelSelectors;
	if (deviceid == Pa_GetDefaultOutputDevice())
	{
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paASIO)
	{
		global_outputParameters.hostApiSpecificStreamInfo = &global_asioOutputInfo;
	}
	else if (Pa_GetHostApiInfo(Pa_GetDeviceInfo(deviceid)->hostApi)->type == paWDMKS)
	{
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	else
	{
		//assert(false);
		global_outputParameters.hostApiSpecificStreamInfo = NULL;
	}
	return true;
}



//for debugging
void myPurecallHandler(void)
{
	//printf("In _purecall_handler.");
	exit(0);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	_set_purecall_handler(myPurecallHandler); //for debugging

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	global_startstamp_ms = GetTickCount();

	//LPWSTR *szArgList;
	LPSTR *szArgList;
	int nArgs;
	int i;

	//szArgList = CommandLineToArgvW(GetCommandLineW(), &nArgs);
	szArgList = CommandLineToArgvA(GetCommandLineA(), &nArgs);
	if (NULL == szArgList)
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return FALSE;
	}
	LPWSTR *szArgListW;
	int nArgsW;
	szArgListW = CommandLineToArgvW(GetCommandLineW(), &nArgsW);
	if (NULL == szArgListW)
	{
		//wprintf(L"CommandLineToArgvW failed\n");
		return FALSE;
	}

	global_audioinputdevicename = "E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	if (nArgs>1)
	{
		//global_filename = szArgList[1];
		global_audioinputdevicename = szArgList[1];
	}
	global_inputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	global_inputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	//global_inputAudioChannelSelectors[0] = 2; // on emu patchmix ASIO device channel 3 (left)
	//global_inputAudioChannelSelectors[1] = 3; // on emu patchmix ASIO device channel 4 (right)
	//global_inputAudioChannelSelectors[0] = 8; // on emu patchmix ASIO device channel 9 (left)
	//global_inputAudioChannelSelectors[1] = 9; // on emu patchmix ASIO device channel 10 (right)
	//global_inputAudioChannelSelectors[0] = 10; // on emu patchmix ASIO device channel 11 (left)
	//global_inputAudioChannelSelectors[1] = 11; // on emu patchmix ASIO device channel 12 (right)
	if (nArgs>2)
	{
		global_inputAudioChannelSelectors[0] = atoi((LPCSTR)(szArgList[2])); //0 for first asio channel (left) or 2, 4, 6, etc.
	}
	if (nArgs>3)
	{
		global_inputAudioChannelSelectors[1] = atoi((LPCSTR)(szArgList[3])); //1 for second asio channel (right) or 3, 5, 7, etc.
	}
	global_audiooutputdevicename = "E-MU ASIO"; //"Wave (2- E-MU E-DSP Audio Proce"
	if (nArgs>4)
	{
		//global_filename = szArgList[1];
		global_audiooutputdevicename = szArgList[4];
	}
	global_outputAudioChannelSelectors[0] = 0; // on emu patchmix ASIO device channel 1 (left)
	global_outputAudioChannelSelectors[1] = 1; // on emu patchmix ASIO device channel 2 (right)
	//global_outputAudioChannelSelectors[0] = 2; // on emu patchmix ASIO device channel 3 (left)
	//global_outputAudioChannelSelectors[1] = 3; // on emu patchmix ASIO device channel 4 (right)
	//global_outputAudioChannelSelectors[0] = 8; // on emu patchmix ASIO device channel 9 (left)
	//global_outputAudioChannelSelectors[1] = 9; // on emu patchmix ASIO device channel 10 (right)
	//global_outputAudioChannelSelectors[0] = 10; // on emu patchmix ASIO device channel 11 (left)
	//global_outputAudioChannelSelectors[1] = 11; // on emu patchmix ASIO device channel 12 (right)
	if (nArgs>5)
	{
		global_outputAudioChannelSelectors[0] = atoi((LPCSTR)(szArgList[5])); //0 for first asio channel (left) or 2, 4, 6, etc.
	}
	if (nArgs>6)
	{
		global_outputAudioChannelSelectors[1] = atoi((LPCSTR)(szArgList[6])); //1 for second asio channel (right) or 3, 5, 7, etc.
	}


	if (nArgs>7)
	{
		global_cutfreq_hz = atof(szArgList[7]);
	}
	if (nArgs>8)
	{
		global_duration_sec = atof(szArgList[8]);
	}


	if (nArgs>9)
	{
		global_x = atoi(szArgList[9]);
	}
	if (nArgs>10)
	{
		global_y = atoi(szArgList[10]);
	}
	if (nArgs>11)
	{
		global_xwidth = atoi(szArgList[11]);
	}
	if (nArgs>12)
	{
		global_yheight = atoi(szArgList[12]);
	}
	if (nArgs>13)
	{
		global_alpha = atoi(szArgList[13]);
	}
	if (nArgs>14)
	{
		global_titlebardisplay = atoi(szArgList[14]);
	}
	if (nArgs>15)
	{
		global_menubardisplay = atoi(szArgList[15]);
	}
	if (nArgs>16)
	{
		global_acceleratoractive = atoi(szArgList[16]);
	}
	if (nArgs>17)
	{
		global_fontheight = atoi(szArgList[17]);
	}

	//new parameters
	if (nArgs>18)
	{
		global_statictextcolor_red = atoi(szArgList[18]);
	}
	if (nArgs>19)
	{
		global_statictextcolor_green = atoi(szArgList[19]);
	}
	if (nArgs>20)
	{
		global_statictextcolor_blue = atoi(szArgList[20]);
	}
	if (nArgs>21)
	{
		wcscpy(global_szWindowClass, szArgListW[21]);
	}
	if (nArgs>22)
	{
		wcscpy(global_szTitle, szArgListW[22]);
	}
	if (nArgs>23)
	{
		global_begin = szArgList[23];
	}
	if (nArgs > 24)
	{
		global_end = szArgList[24];
	}

	global_statictextcolor = RGB(global_statictextcolor_red, global_statictextcolor_green, global_statictextcolor_blue);
	LocalFree(szArgList);
	LocalFree(szArgListW);

	int nShowCmd = false;
	//ShellExecuteA(NULL, "open", "begin.bat", "", NULL, nShowCmd);
	ShellExecuteA(NULL, "open", global_begin.c_str(), "", NULL, nCmdShow);


	//////////////////////////
	//initialize random number
	//////////////////////////
	srand((unsigned)time(0));



	pFILE = fopen("devices.txt", "w");

	///////////////////////
	//initialize port audio
	///////////////////////
	global_err = Pa_Initialize();
	if (global_err != paNoError)
	{
		//MessageBox(0,"portaudio initialization failed",0,MB_ICONERROR);
		if (pFILE)
		{
			fprintf(pFILE, "portaudio initialization failed.\n");
			fclose(pFILE);
		}
		return 1;
	}



	////////////////////////
	//audio device selection
	////////////////////////
	SelectAudioInputDevice();
	SelectAudioOutputDevice();


	// You don't necessarily have to do this - it will default to 44100 if not set.
	Tonic::setSampleRate(SAMPLE_RATE);

	//tonic ringbuffer initialization
	global_RingBufferWriter = RingBufferWriter("input", FRAMES_PER_BUFFER, NUM_CHANNELS);
	//tonic synth creation
	//global_pSynth = new(InputDemoSynth);
	//global_pSynth = new(InputExpSynth);
	global_pSynth = new(InputProcessSynth);
	//global_pSynth = new(InputProcessFlangerSynth);


	//////////////
	//setup stream  
	//////////////
	global_err = Pa_OpenStream(
		&global_stream,
		&global_inputParameters, //NULL, //&global_inputParameters,
		&global_outputParameters,
		SAMPLE_RATE,
		FRAMES_PER_BUFFER,
		0, //paClipOff,      // we won't output out of range samples so don't bother clipping them
		renderCallback,
		NULL); //no callback userData
	if (global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Unable to open stream: %s\n", Pa_GetErrorText(global_err));
		//MessageBox(0,errorbuf,0,MB_ICONERROR);
		if (pFILE)
		{
			fprintf(pFILE, "%s\n", errorbuf);
			fclose(pFILE);
		}
		return 1;
	}



	//////////////
	//start stream  
	//////////////
	global_err = Pa_StartStream(global_stream);
	if (global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Unable to start stream: %s\n", Pa_GetErrorText(global_err));
		//MessageBox(0,errorbuf,0,MB_ICONERROR);
		if (pFILE)
		{
			fprintf(pFILE, "%s\n", errorbuf);
			fclose(pFILE);
		}
		return 1;
	}

	fclose(pFILE);
	pFILE = NULL;


	///////////////////////
	//load background image
	///////////////////////
	global_dib = FreeImage_Load(FIF_JPEG, "background.jpg", JPEG_DEFAULT);
	//global_dib = FreeImage_Load(FIF_BMP, "background.bmp", BMP_DEFAULT);

	//////////////
	//update icons 
	//////////////
	FIBITMAP* local_16x16xrgbdib = FreeImage_Rescale(global_dib, 16, 16, FILTER_BICUBIC);
	FreeImage_Save(FIF_ICO, local_16x16xrgbdib, "background_16x16xrgb-new.ico");
	FreeImage_Unload(local_16x16xrgbdib);
	FIBITMAP* local_32x32xrgbdib = FreeImage_Rescale(global_dib, 32, 32, FILTER_BICUBIC);
	FreeImage_Save(FIF_ICO, local_32x32xrgbdib, "background_32x32xrgb-new.ico");
	FreeImage_Unload(local_32x32xrgbdib);
	FIBITMAP* local_48x48xrgbdib = FreeImage_Rescale(global_dib, 48, 48, FILTER_BICUBIC);
	FreeImage_Save(FIF_ICO, local_48x48xrgbdib, "background_48x48xrgb-new.ico");
	FreeImage_Unload(local_48x48xrgbdib);
	//////////////
	//create fonts 
	//////////////
	//global_hFont=CreateFontW(global_fontheight,0,0,0,FW_NORMAL,0,0,0,0,0,0,2,0,L"SYSTEM_FIXED_FONT");
	global_hFont = CreateFontW(global_fontheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe Script");
	global_hFontHuge = CreateFontW(global_yheight, 0, 0, 0, FW_NORMAL, 0, 0, 0, 0, 0, 0, 2, 0, L"Segoe Script");


	wstring myclassname = global_szWindowClass; 
	myclassname += L"1";
	CMainWindow* pMainWindow = new CMainWindow(L"mymainwindowtitle", hInstance, myclassname.c_str(),
		global_x, global_y, global_xwidth, global_yheight, 
		global_alpha, global_titlebardisplay, global_menubardisplay);
	pMainWindow->Create();
	pMainWindow->Show();

	
	myclassname = global_szWindowClass;
	myclassname += L"2";
	CFirstWindow* pFirstWindow = new CFirstWindow(L"myfirstwindowtitle", hInstance, myclassname.c_str(),
		global_x, global_y, global_xwidth, global_yheight, 
		global_alpha, global_titlebardisplay, 0);
	pFirstWindow->Create();
	pFirstWindow->Show();
	
	myclassname = global_szWindowClass;
	myclassname += L"3";
	CSecondWindow* pSecondWindow = new CSecondWindow(L"mysecondwindowtitle", hInstance, myclassname.c_str(),
		global_x, global_y, global_xwidth, global_yheight, 
		global_alpha, global_titlebardisplay, 0);
	pSecondWindow->Create();
	pSecondWindow->Show();

	//// pump messages:
	MSG  msg;
	HACCEL hAccelTable;
	int status;
	if (global_acceleratoractive)
	{
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPIWAVWIN32));
	}
	else
	{
		hAccelTable = NULL;
	}
	while ((status = ::GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (status == -1) 
		{
			// handle the error, break
			break;
		}
		::TranslateMessage(&msg);
		::DispatchMessage(&msg);
	}

	/////////////////////
	//terminate portaudio
	/////////////////////
	global_err = Pa_StopStream(global_stream);
	if (global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Error stoping stream: %s\n", Pa_GetErrorText(global_err));
		MessageBoxA(0, errorbuf, 0, MB_ICONERROR);
		return 1;
	}
	global_err = Pa_CloseStream(global_stream);
	if (global_err != paNoError)
	{
		char errorbuf[2048];
		sprintf(errorbuf, "Error closing stream: %s\n", Pa_GetErrorText(global_err));
		MessageBoxA(0, errorbuf, 0, MB_ICONERROR);
		return 1;
	}
	Pa_Terminate();

	/////////////////////
	//free tonic
	/////////////////////
	if (global_pSynth) delete global_pSynth;

	//ShellExecute with 0 sends a WM_ACTIVATEAPP so we must put it here 
	//before deleting CWindowTransparent derived classes
	ShellExecuteA(NULL, "open", global_end.c_str(), "", NULL, 0);

	///////////////////////
	//free memory
	///////////////////////
	delete pSecondWindow;
	delete pFirstWindow;
	delete pMainWindow;
	FreeImage_Unload(global_dib);
	DeleteObject(global_hFont);
	DeleteObject(global_hFontHuge);

	return msg.wParam;
}



