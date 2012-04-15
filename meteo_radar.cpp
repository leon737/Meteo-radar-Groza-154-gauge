#include "..\fs9sdk_3d\fs9sdk.h"
//#include "sim1_lib.h"
#include "meteo_radar.h"
#include <stdio.h>	
#include <math.h>

void FSAPI meteo_radarCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data );

#pragma warning (disable:4305 4244)




const double Alt_Coeff = 4294967296.0000010;
const double Lat_Coeff = 477302101697422.31;
const double Lon_Coeff = 51240955760304312.000000;


typedef struct ___LATLONALT
{
  __int64 lat;
  __int64 lon;
  __int64 alt;
}__LATLONALT;

typedef struct __POINT_LATLONALT {
	FLOAT64 lat;
	FLOAT64 lon;
	FLOAT64 alt;
} POINT_LATLONALT;


POBSV get_weather_lla(___LATLONALT lla)
{
   POBSV _wx;
   _wx = (POBSV)Weather_conditions_at_lla_fixed_point((LATLONALT *)&lla,0x3F);
   return(_wx);
}

#define SIM1_READ_VALUE -1000000.0f
//#define PI 3.14159265358979f

//=============================================================================================


void get_LL(FLOAT64 Lat_src,FLOAT64 Lon_src,FLOAT64 S,FLOAT64 OD,FLOAT64 *dLat,FLOAT64 *dLon)
{
        double s_from_beg = S;

        if(s_from_beg<0) s_from_beg=0;

        double _cos_S = cos(s_from_beg/(57.29578 * 60 * 1.853)); 
        double _sin_S = sin(s_from_beg/(57.29578 * 60 * 1.853)); 
        
        double Lat = 57.29578 * asin( sin(Lat_src/57.29578) * 
                                          _cos_S + cos(Lat_src/57.29578) * _sin_S * 
                                                                  cos(OD/57.29578)
                                                                 );

        double cos_Lon_S_Lon = (_cos_S - sin(Lat_src/57.29578)*sin(Lat/57.29578))/
                                   (cos(Lat_src/57.29578)*cos(Lat/57.29578));
        double Lon;

        double s_Lon = Lon_src<0?Lon_src+360.0f:Lon_src;

        if(OD<=180) Lon = (acos(cos_Lon_S_Lon) + s_Lon/57.29578)*57.29578;
        else Lon = (-acos(fabs(cos_Lon_S_Lon)>1.0f?cos_Lon_S_Lon>0?1.0f:-1.0f:cos_Lon_S_Lon) + s_Lon/57.29578)*57.29578;
		if(Lon>180) Lon = Lon-360.0;
        *dLat=Lat;
        *dLon=Lon;
}
//=============================================================================================


BOOL inline IsCloud (POBSV wx, float alt) {
	BOOL bOK = FALSE;
	if (!wx)
		return FALSE;
	PCLOUD_LAYER layer = wx->cloud;
	while (layer) {
		if (layer->cloud_base <= alt && layer->cloud_tops >= alt && layer->cloud_coverage > 0)
			bOK = TRUE;
		layer = layer->next_layer;
	}
	return bOK;
}

int inline GetCloudIntense (POBSV wx, float alt, float fHorizontalDistance, float fZoomSelectorPos, BOOL bIsVARUOn) {
	int c = 0, p=0, t=0, i=0, h = 0;
	if (!wx)
		return 0;
	PCLOUD_LAYER layer = wx->cloud;
	while (layer) {
		if (layer->cloud_coverage > 0 && layer->cloud_base <= alt + 0.035 * fHorizontalDistance * 500 / 0.3048 && layer->cloud_tops >= alt  - 0.035 * fHorizontalDistance * 500 / 0.3048) {
			if (c < layer->cloud_coverage)
				c = layer->cloud_coverage;
			if (t < layer->cloud_turbulence)
				t = layer->cloud_turbulence;
			if (layer->cloud_type == CLOUD_TYPE_THUNDERSTORM)
				h = 1;
		}
		if (layer->precip_type > 0 && layer->cloud_tops >= alt  - 0.035 * fHorizontalDistance * 500 / 0.3048)
			if (p < layer->precip_rate + 1)
				p = layer->precip_rate + 1;
		layer = layer->next_layer;
	}


	i = c + p * 2 + t + h * 4;

	if (i > 20) i = 20;

	if (!bIsVARUOn)
		return i / (1 + 0.5 *(fHorizontalDistance / fZoomSelectorPos));
	else
		return i;
}

//==========================================================================================


//////////////////////////////////////////////////////////////////////////////
//Глобальные переменные
//////////////////////////////////////////////////////////////////////////////

LPDWORD lpdwDisplayRender;				// растр ЭЛТ РЛС
BITMAPINFO BitsInfo;					// метаданные растра


LPDWORD lpdwGrid;						// матрица для хранения данных с приемника антенны
int nGridRows = 0;						// кол-во строк матрицы
int nGridCols = 0;						// кол-во столбцов матрицы

int iWorkMode = 0;						// режим работы (0 - выкл, 1 - готов, 2 - земля, 3 - метео, 4 - контур, 5 - снос)
int iPrdSelected = 1;					// выбранный ПРД (1 - основной, 2 - резервный)

float fAntennaAzimuth = 0;				// относительный азимут разворота антенны
int iCurrentCol = 30;					// заполняемый столбец матрицы
int iPrevCol = 30;						// предыдущий заполненный столбец матрицы
int iPrev2Col = 30;						// предыдущий заполненный столбец матрицы
float fAntennaTrueAzimuth = 0;			// истинный азимут антенны (относительный азимут + истинный ПУ самолета)
int iAntennaMoveDirection = 0;			// направление движения антенны (0 - влево, 1 - вправо)
int iInterleaveZone = 0;				// флаг чередования сканирования зон (верхние ряды = 1, нижние ряды = 0)
BOOL bManualRotate = FALSE;				// антенна в режиме ручного управления разворотом


BOOL bPowerSwitchPos = FALSE;			// выключатель питания РЛС
float fAngleSelectorPos = 0;			// положение ручки настройки угла наклона антенны
int iModeSelectorPos = 1;				// положение переключателя выбора режима работы РЛС
float fZoomSelectorPos = 10;			// положение ручки настройки масштаба
BOOL bStabTumblerPos = FALSE;			// тумблер "СТАБИЛИЗАЦИЯ АНТЕННЫ"
BOOL bPrdTumblerPos = TRUE;				// тумблер выбора ПРД (основной - TRUE, резервный - FALSE)


MODULE_VAR lat_var = {PLANE_LATITUDE};	// / 40007000.0f * 360.0f
MODULE_VAR lon_var = {PLANE_LONGITUDE};	//  / 281474976710656.0f * 360.0f
MODULE_VAR alt_var = {PLANE_ALTITUDE};
MODULE_VAR hed_var = {PLANE_HEADING_DEGREES_TRUE};
MODULE_VAR pitch_var =  {PLANE_PITCH_DEGREES};
MODULE_VAR bank_var = {PLANE_BANK_DEGREES};
MODULE_VAR forward_var = {FORWARD_GROUND_VELOCITY};
MODULE_VAR side_var = {SIDE_GROUND_VELOCITY};
MODULE_VAR radio_alt= {RADIO_HEIGHT}; // высота над местностью

POINT_LATLONALT aircraft_point;			// позиция излучателя РЛС

float fDriftAngle;						// угол сноса

#define DISPLAY_WIDTH					328
#define DISPLAY_HEIGHT					206
#define DISPLAY_ELEMENT_SIZE			4

const int DISPLAY_MEMORY_SIZE =			DISPLAY_WIDTH*DISPLAY_HEIGHT*DISPLAY_ELEMENT_SIZE;


void CreateDisplayRender (void);
void CreateGrid (void);

int iReadyTimer = 0;					// таймер готовности прогрева трубки

DWORD inline GetColor (float x, float y) {
	int xc = (int)x;
	int yc = (int)y;
	return *(lpdwDisplayRender + (int)yc * DISPLAY_WIDTH + (int)xc);
}

/*
void inline MixColor (int x, int y, DWORD dwColor) {
	DWORD dwColor2;
	BYTE r, g, b;
	BYTE r2, g2, b2;
	if (x <=0 || y <= 0 || x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
	dwColor2 = *(lpdwDisplayRender + (int)y * DISPLAY_WIDTH + (int)x);
	r = (BYTE)(dwColor & 0xFF);
	g = (BYTE)((dwColor & 0xFF00) >> 8);
	b = (BYTE)((dwColor & 0xFF0000) >> 16);
	r2 = (BYTE)(dwColor2 & 0xFF);
	g2 = (BYTE)((dwColor2 & 0xFF00) >> 8);
	b2 = (BYTE)((dwColor2 & 0xFF0000) >> 16);
	*(lpdwDisplayRender + (int)y * DISPLAY_WIDTH + (int)x) = RGB (r+r2/2, g+g2/2, b+b2/2);
}*/


void inline MixColor (int x, int y, DWORD dwColor) {
	if (x <=0 || y <= 0 || x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
	*(lpdwDisplayRender + (int)y * DISPLAY_WIDTH + (int)x) = dwColor;
}

void inline SetColor (int x, int y, DWORD dwColor) {
	if (x <=0 || y <= 0 || x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
	*(lpdwDisplayRender + (int)y * DISPLAY_WIDTH + (int)x) = dwColor;
}

void inline DrawFilledCircle (float x, float y, int ri, DWORD dwColor) {
	int xc = (int)x;
	int yc = (int)y;
	
	switch (ri) {
	case 2:
		MixColor(xc-1,yc-1,dwColor);
		SetColor(xc,yc-1,dwColor);
		MixColor(xc+1,yc-1,dwColor);
		SetColor(xc-1,yc,dwColor);
		SetColor(xc,yc,dwColor);
		SetColor(xc+1,yc,dwColor);
		MixColor(xc-1,yc+1,dwColor);
		SetColor(xc,yc+1,dwColor);
		MixColor(xc+1,yc+1,dwColor);
		break;
	case 3:
		MixColor(xc-1,yc-2,dwColor);
		SetColor(xc,yc-2,dwColor);
		MixColor(xc+1,yc-2,dwColor);
		MixColor(xc-2,yc-1,dwColor);
		SetColor(xc-1,yc-1,dwColor);
		SetColor(xc,yc-1,dwColor);
		SetColor(xc+1,yc-1,dwColor);
		MixColor(xc+2,yc-1,dwColor);
		SetColor(xc-2,yc,dwColor);
		SetColor(xc-1,yc,dwColor);
		SetColor(xc,yc,dwColor);
		SetColor(xc+1,yc,dwColor);
		SetColor(xc+2,yc,dwColor);
		MixColor(xc-2,yc+1,dwColor);
		SetColor(xc-1,yc+1,dwColor);
		SetColor(xc,yc+1,dwColor);
		SetColor(xc+1,yc+1,dwColor);
		MixColor(xc+2,yc+1,dwColor);
		MixColor(xc-1,yc+2,dwColor);
		SetColor(xc,yc+2,dwColor);
		MixColor(xc+1,yc+2,dwColor);
		break;
	}
	/*
	for (int r = 1; r <= ri; r++) {
		for (float a = 0; a < 360; a+= 30) {
			int xcc = (int)(xc + cos (a * PI / 360) * r);
			int ycc = (int)(yc + sin (a * PI / 360) * r);
			if (xcc > 0 && ycc > 0 && xcc < DISPLAY_WIDTH && ycc < DISPLAY_HEIGHT)
				*(lpdwDisplayRender + (int)ycc * DISPLAY_WIDTH + (int)xcc) = dwColor;
		}
	}
	*/
}

void inline DrawFilledCircle2 (float x, float y, int ri, DWORD dwColor) {
	int xc = (int)x;
	int yc = (int)y;
	
	for (int r = 1; r <= ri; r++) {
		for (float a = 0; a < 360; a+= 30) {
			int xcc = (int)(xc + cos (a * PI / 360) * r);
			int ycc = (int)(yc + sin (a * PI / 360) * r);
			if (xcc > 0 && ycc > 0 && xcc < DISPLAY_WIDTH && ycc < DISPLAY_HEIGHT)
				*(lpdwDisplayRender + (int)ycc * DISPLAY_WIDTH + (int)xcc) = dwColor;
		}
	}
}

void inline DrawFilledCircle3 (float x, float y, int ri, DWORD dwColor) {
	int xc = (int)x;
	int yc = (int)y;
	
	for (int r = 1; r <= ri; r++) {
		for (float a = 0; a < 360; a+= 30) {
			int xcc = (int)(xc + cos (a * PI / 180) * r);
			int ycc = (int)(yc + sin (a * PI / 180) * r);
			if (xcc > 0 && ycc > 0 && xcc < DISPLAY_WIDTH && ycc < DISPLAY_HEIGHT)
				if (r >= ri - 1) {
					DWORD dwColor2;
					int r, g, b;
					int r2, g2, b2;
					if (x <=0 || y <= 0 || x >= DISPLAY_WIDTH || y >= DISPLAY_HEIGHT) return;
					dwColor2 = *(lpdwDisplayRender + (int)ycc * DISPLAY_WIDTH + (int)xcc);
					r = (int)(dwColor & 0xFF);
					g = (int)((dwColor & 0xFF00) >> 8);
					b = (int)((dwColor & 0xFF0000) >> 16);
					r2 = (int)(dwColor2 & 0xFF);
					g2 = (int)((dwColor2 & 0xFF00) >> 8);
					b2 = (int)((dwColor2 & 0xFF0000) >> 16);
					*(lpdwDisplayRender + (int)ycc * DISPLAY_WIDTH + (int)xcc) = RGB ((r+r2)/2, (g+g2)/2, (b+b2)/2);
				}
				else {
					*(lpdwDisplayRender + (int)ycc * DISPLAY_WIDTH + (int)xcc) = dwColor;
				}
		}
	}
}




BOOL inline FadeExtremeWeather (int y, int x) {
	int iOK = 0;
	if (x > 0)
		if (*(lpdwGrid + y * (nGridCols - 1) + (x-1)) > 6) iOK++;
	if (y > 0)
		if (*(lpdwGrid + (y-1) * (nGridCols - 1) + x) > 6) iOK++;
	if (x < nGridCols - 1)
		if (*(lpdwGrid + y * (nGridCols - 1) + (x+1)) > 6) iOK++;
	if (y < nGridRows - 1)
		if (*(lpdwGrid + (y+1) * (nGridCols - 1) + x) > 6) iOK++;
	return (iOK > 3);
}



/////////////////////////////////////////////////////////////////////////////
// Модуль логики
/////////////////////////////////////////////////////////////////////////////
#define		GAUGE_NAME			"Logic"
#define		GAUGEHDR_VAR_NAME	gaugehdr_logic
#define		GAUGE_W				1

#include "logic.cpp"


/////////////////////////////////////////////////////////////////////////////
// Экран РЛС
/////////////////////////////////////////////////////////////////////////////
//#define		GAUGE_NAME			"Display"
//#define		GAUGEHDR_VAR_NAME	gaugehdr_display
//#define		GAUGE_W				722

//#include "display.cpp"


/////////////////////////////////////////////////////////////////////////////
// Экран РЛС (вариант 2)
/////////////////////////////////////////////////////////////////////////////
#define		GAUGE_NAME			"ICU"
#define		GAUGEHDR_VAR_NAME	gaugehdr_display2
#define		GAUGE_W				722

#include "display2.cpp"


/////////////////////////////////////////////////////////////////////////////
// Пульт РЛС
/////////////////////////////////////////////////////////////////////////////
//#define		GAUGE_NAME			"Control"
//#define		GAUGEHDR_VAR_NAME	gaugehdr_control
//#define		GAUGE_W				251

//#include "control.cpp"

/////////////////////////////////////////////////////////////////////////////
// Пульт управления разворотом РЛС
/////////////////////////////////////////////////////////////////////////////
//#define		GAUGE_NAME			"RotateControl"
//#define		GAUGEHDR_VAR_NAME	gaugehdr_rotate_control
//#define		GAUGE_W				43

//#include "rotate_control.cpp"


//==========================================================================================


void CreateDisplayRender (void) {
	// создаем растр ЭЛТ РЛС
	// размерностью 328х206 + 1 строка запаса 32 bit
	lpdwDisplayRender = (LPDWORD)malloc (DISPLAY_MEMORY_SIZE + DISPLAY_WIDTH * 4);
	memset (&BitsInfo, 0, sizeof (BITMAPINFO));
	BitsInfo.bmiHeader.biSize = sizeof (BITMAPINFOHEADER);
	BitsInfo.bmiHeader.biWidth = DISPLAY_WIDTH;
	BitsInfo.bmiHeader.biHeight = DISPLAY_HEIGHT;
	BitsInfo.bmiHeader.biPlanes = 1;
	BitsInfo.bmiHeader.biBitCount = 32;
	BitsInfo.bmiHeader.biCompression = BI_RGB;
	BitsInfo.bmiHeader.biSizeImage = 0;
	BitsInfo.bmiHeader.biXPelsPerMeter = 1;
	BitsInfo.bmiHeader.biYPelsPerMeter = 1;
	BitsInfo.bmiHeader.biClrUsed = 0;
	BitsInfo.bmiHeader.biClrImportant = 0;
}

void CreateGrid (void) {
	// создаем матрицу
	lpdwGrid = (LPDWORD)malloc (nGridRows * nGridCols * 4);
	iCurrentCol = (int)(nGridCols / 2);
}

void FSAPI	module_init(void)
{
	iReadyTimer = 15*18;
	// создаем растр ЭЛТ РЛС
	CreateDisplayRender ();
	// создаем матрицу данных с приемника антенны РЛС
	// включаем основной ПРД
	nGridCols = 61;
	nGridRows = 10;
	CreateGrid ();
	
}		
void FSAPI	module_deinit(void)
{
	// удаляем растр ЭЛТ РЛС
	if (lpdwDisplayRender)
		free (lpdwDisplayRender);
	// удаляем матрицу данных с приемника антенны РЛС
	if (lpdwGrid)
		free (lpdwGrid);
}	

BOOL WINAPI DllMain (HINSTANCE hDLL, DWORD dwReason, LPVOID lpReserved)	
{														
	return TRUE;										
}

GAUGESIMPORT	ImportTable =							
{														
	{ 0x0000000F, (PPANELS)NULL },
	{ 0x00000002, (PGLOBAL)NULL },						
	{ 0x00000001, (PFS6API)NULL },						
	{ 0x00000019, (PSIM1)NULL },
	{ 0x00000027, (PSOUND)NULL },						
	{ 0x00000010, (PCONTROLS)NULL },						
	{ 0x0000001c, (PWEATHER)NULL },						
	{ 0x0000000E, (g3d_linkage *)NULL },
	{ 0x00000000, NULL }								
};														
													
/* This is the module's export table.	*/				
GAUGESLINKAGE	Linkage =								
{														
	0x00000013,											
	module_init,										
	module_deinit,										
	0,													
	0,
	FS9LINK_VERSION, 
	{
		(&gaugehdr_logic), 
	//	(&gaugehdr_display), 
		(&gaugehdr_display2), 
//		(&gaugehdr_control), 
//		(&gaugehdr_rotate_control),
		0 
	}
};
