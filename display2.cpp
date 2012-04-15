void FSAPI display2Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data );

#pragma warning (disable:4305 4244)


extern PELEMENT_HEADER		display2_list;
extern MOUSERECT			control_mouse_rect[];


GAUGEHDR GAUGEHDR_VAR_NAME = {
                             GAUGE_HEADER_VERSION_FS800,
						     GAUGE_NAME,
							 &display2_list,
		                     query_routine_610,
		                     install_routine_610,
		                     initialize_routine_610,
		                     update_routine_610,
		                     generate_routine_610,
		                     draw_routine_610,
		                     kill_routine_610,
		                     NULL,
		                     GAUGE_W, 
		                     0,
		                     0.0,
		                     0.0,
		                     NULL,
							 NULL,
		                     {0, 0},
		                     NULL,
		                     control_mouse_rect,
		                     display2Callback,
		                     0,
		                     NULL,
		                     0,
		                     0,
		                     NULL,
		                     {0, 0},
		                     {0.0},
	};



NONLINEARITY  angle_selector_nonlinearity[] =
{
	{{138+106,	143+192},	 -15, 0},
	{{136+106,	147+192},	 -14, 0},
	{{130+106,	149+192},	 -12, 0},
	{{123+106,	150+192},	 -10, 0},
	{{116+106,	149+192},	  -8, 0},
	{{110+106,	146+192},	  -6, 0},
	{{105+106,	142+192},	  -4, 0},
	{{102+106,	136+192},	  -2, 0},
	{{101+106,	129+192},	   0, 0},
	{{102+106,	122+192},	   2, 0},
	{{105+106,	116+192},	   4, 0},
	{{110+106,	111+192},	   6, 0},
	{{116+106,	108+192},	   8, 0},
	{{123+106,	106+192},	  10, 0}
};

NONLINEARITY  mode_selector_nonlinearity[] =
{
	{{116,	374},	  1, 0},
	{{121,	382},	  2, 0},
	{{122,	390},	  3, 0},
	{{120,	400},	  4, 0},
	{{116,	407},	  5, 0}
};

NONLINEARITY  zoom_selector_nonlinearity[] =
{
	{{288,	405},	  10, 0},
	//{{172,	216},	  60, 0},
	//{{194,	193},	 192, 0},
	//{{217,	216},	 314, 0},
	{{302,	369},	 365/2, 0},
	{{317,	405},	 375, 0}
};


FLOAT64 FSAPI control_power_on_buttons_cb ( PELEMENT_ICON pelement ) {
	return bPowerSwitchPos?0:1;
}

FLOAT64 FSAPI control_stab_tumbler_cb ( PELEMENT_ICON pelement ) {
	return bStabTumblerPos?0:1;
}

FLOAT64 FSAPI control_prd_tumbler_cb ( PELEMENT_ICON pelement ) {
	return bPrdTumblerPos?0:1;
}

FLOAT64 FSAPI control_angle_selector_cb ( PELEMENT_NEEDLE pelement ) {
	return (FLOAT64)fAngleSelectorPos;
}

FLOAT64 FSAPI control_mode_selector_cb ( PELEMENT_NEEDLE pelement ) {
	return (FLOAT64)iModeSelectorPos;
}

FLOAT64 FSAPI control_zoom_selector_cb ( PELEMENT_NEEDLE pelement ) {
	return (FLOAT64)fZoomSelectorPos;
}

FLOAT64 FSAPI control_mode_indicator_cb ( PELEMENT_ICON pelement ) {
	if (bPowerSwitchPos)
		return (FLOAT64)iWorkMode;
	else
		return 0;
}

FAILURE_RECORD	display2_fail[] =
{
	{FAIL_GAUGE_ATTITUDE, FAIL_ACTION_NONE},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


MAKE_ICON
(	
	control_power_on_buttons,
	IDB_POWER_ON,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_LUMINOUS,
	0,
	359, 276,
	MODULE_VAR_NONE,
	control_power_on_buttons_cb,
	ICON_SWITCH_TYPE_SET_CUR_ICON,
	2,
	0,
	0
)

MAKE_ICON
(	
	control_stab_tumbler,
	IDB_CONTROL_TUMBLER_UP,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_LUMINOUS,
	0,
	146, 292,
	MODULE_VAR_NONE,
	control_stab_tumbler_cb,
	ICON_SWITCH_TYPE_SET_CUR_ICON,
	2,
	0,
	0
)

MAKE_ICON
(	
	control_prd_tumbler,
	IDB_CONTROL_TUMBLER_UP,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_LUMINOUS ,
	0,
	297, 292,
	MODULE_VAR_NONE,
	control_prd_tumbler_cb,
	ICON_SWITCH_TYPE_SET_CUR_ICON,
	2,
	0,
	0
)


MAKE_NEEDLE
(
	control_angle_selector,
	IDB_CONTROL_GAL,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_LUMINOUS,
	0,
	206+19, 301+19,
	19, 19,
	MODULE_VAR_NONE,
	control_angle_selector_cb,
	angle_selector_nonlinearity,
	6
)


MAKE_NEEDLE
(
	control_mode_selector,
	IDB_CONTROL_GAL,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_LUMINOUS,
	0,
	81+19, 371+19,
	19, 19,
	MODULE_VAR_NONE,
	control_mode_selector_cb,
	mode_selector_nonlinearity,
	6
)


MAKE_NEEDLE
(
	control_zoom_selector,
	IDB_CONTROL_GAL,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_LUMINOUS,
	0,
	282+19, 371+19,
	19, 19,
	MODULE_VAR_NONE,
	control_zoom_selector_cb,
	zoom_selector_nonlinearity,
	6
)


MAKE_ICON
(	
	control_mode_indicator,
	IDB_CONTROL_LAMP_0,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE | IMAGE_USE_BRIGHT ,
	0,
	170, 358,
	MODULE_VAR_NONE,
	control_mode_indicator_cb,
	ICON_SWITCH_TYPE_SET_CUR_ICON,
	6,
	0,
	0
)



PELEMENT_HEADER		control_ico[] =
{
	&control_stab_tumbler.header,
	&control_prd_tumbler.header,
	&control_angle_selector.header,
	&control_mode_selector.header,
	&control_zoom_selector.header,
	&control_mode_indicator.header,
	&control_power_on_buttons.header,
	NULL
};

MAKE_STATIC
(
	control_back,
	IDB_CONTROL_BCK,
	&control_ico,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_LUMINOUS,
	0,
	0,0
)


PELEMENT_HEADER		control_ico_ups[] =
{
	&control_back.header,
	NULL
};

MAKE_STATIC
(
	display2_ups,
	IDB_DISPLAY2_UPS,
	&control_ico_ups,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		display2_ico_ups[] =
{
	&display2_ups.header,
	NULL
};

MAKE_STATIC
(
	display2_grid,
	IDB_DISPLAY_GRID,
	&display2_ico_ups,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_LUMINOUS,
	0,
	49,40
)

PELEMENT_HEADER		display2_ico_grid[] =
{
	&display2_grid.header,
	NULL
};

MAKE_STATIC
(
	display2_img,
	IDB_DISPLAY_IMG,
	&display2_ico_grid,
	NULL,
	IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION,
	0,
	49,40
)

PELEMENT_HEADER		display2_ico_img[] =
{
	&display2_img.header,
	NULL
};

MAKE_STATIC
(
	display2_background,
	IDB_DISPLAY2_BCK,
	&display2_ico_img,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		display2_list = &display2_background.header;


BOOL FSAPI control_power_switch_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	bPowerSwitchPos=!bPowerSwitchPos;
	if (bPowerSwitchPos) {
		iWorkMode = 0;
		if (lpdwGrid)
			free (lpdwGrid);
		if (bPrdTumblerPos) {
			nGridCols = 121;
			nGridRows = 60;
		}
		else {
			nGridCols = 121;
			nGridRows = 60;
		}
		CreateGrid();
		iWorkMode = iModeSelectorPos;
	}
	else
		iReadyTimer = 5*18;
		iWorkMode = 0;
	return FALSE;
}

BOOL FSAPI control_stab_tumbler_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	bStabTumblerPos=!bStabTumblerPos;
	return FALSE;
}

BOOL FSAPI control_prd_tumbler_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	bPrdTumblerPos=!bPrdTumblerPos;
	return FALSE;
}

BOOL FSAPI control_angle_selector_down_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	if (mf & MOUSE_LEFTSINGLE)
		fAngleSelectorPos-=0.5;
	if (mf & MOUSE_RIGHTSINGLE)
		fAngleSelectorPos-=2;
	if (fAngleSelectorPos < -15)
		fAngleSelectorPos = -15;
	return FALSE;
}

BOOL FSAPI control_angle_selector_up_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	if (mf & MOUSE_LEFTSINGLE)
		fAngleSelectorPos+=0.5;
	if (mf & MOUSE_RIGHTSINGLE)
		fAngleSelectorPos+=2;
	if (fAngleSelectorPos > 10)
		fAngleSelectorPos = 10;
	return FALSE;
}

BOOL FSAPI control_mode_selector_down_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	iModeSelectorPos--;
	if (iModeSelectorPos < 1)
		iModeSelectorPos = 1;
	bManualRotate = FALSE;
	if (nGridCols > 61)
		iCurrentCol = (int)(fAntennaAzimuth / 1.5 + (nGridCols -  1) / 2);
	else
		iCurrentCol = (int)(fAntennaAzimuth / 3 + (nGridCols -  1) / 2);
	if (iCurrentCol < 0) iCurrentCol = 0;
	if (iCurrentCol > nGridCols - 1) iCurrentCol = nGridCols - 1;
	return FALSE;
}

BOOL FSAPI control_mode_selector_up_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	iModeSelectorPos++;
	if (iModeSelectorPos > 5)
		iModeSelectorPos = 5;
	bManualRotate = FALSE;
	if (nGridCols > 61)
		iCurrentCol = (int)(fAntennaAzimuth / 1.5 + (nGridCols -  1) / 2);
	else
		iCurrentCol = (int)(fAntennaAzimuth / 3 + (nGridCols -  1) / 2);
	if (iCurrentCol < 0) iCurrentCol = 0;
	if (iCurrentCol > nGridCols - 1) iCurrentCol = nGridCols - 1;
	return FALSE;
}

BOOL FSAPI control_zoom_selector_down_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	if (mf & MOUSE_LEFTSINGLE)
		fZoomSelectorPos--;
	if (mf & MOUSE_RIGHTSINGLE)
		fZoomSelectorPos-=10;
	if (fZoomSelectorPos < 10)
		fZoomSelectorPos = 10;
	return FALSE;
}

BOOL FSAPI control_zoom_selector_up_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	if (mf & MOUSE_LEFTSINGLE)
		fZoomSelectorPos++;
	if (mf & MOUSE_RIGHTSINGLE)
		fZoomSelectorPos+=10;
	if (fZoomSelectorPos > 375)
		fZoomSelectorPos = 375;
	return FALSE;
}

BOOL FSAPI rotate_control_left_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	if (iWorkMode == 5) {
		fAntennaAzimuth-=1;
		if (fAntennaAzimuth < -85) fAntennaAzimuth = -85;
		bManualRotate = TRUE;
	}
	return FALSE;
}

BOOL FSAPI rotate_control_right_mouse_cb (PPIXPOINT rp, FLAGS32 mf) {
	if (iWorkMode == 5) {
		fAntennaAzimuth+=1;
		if (fAntennaAzimuth > 85) fAntennaAzimuth = 85;
		bManualRotate = TRUE;
	}
	return FALSE;
}


MOUSE_BEGIN( control_mouse_rect, HELP_NONE, 0, 0 )
	MOUSE_CHILD_FUNCT(358,275,45, 107, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE, control_power_switch_mouse_cb)
	MOUSE_CHILD_FUNCT(146,292,15,32, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE, control_stab_tumbler_mouse_cb)
	MOUSE_CHILD_FUNCT(297,292,15,32, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE, control_prd_tumbler_mouse_cb)
	MOUSE_CHILD_FUNCT(195,295,20,60, CURSOR_DOWNARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_angle_selector_down_mouse_cb)
	MOUSE_CHILD_FUNCT(225,295,20,60, CURSOR_UPARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_angle_selector_up_mouse_cb)
	MOUSE_CHILD_FUNCT(75,368,20,60, CURSOR_DOWNARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_mode_selector_down_mouse_cb)
	MOUSE_CHILD_FUNCT(95,368,20,60, CURSOR_UPARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_mode_selector_up_mouse_cb)
	MOUSE_CHILD_FUNCT(265,368,35,60, CURSOR_DOWNARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_zoom_selector_down_mouse_cb)
	MOUSE_CHILD_FUNCT(300,368,35,60, CURSOR_UPARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_zoom_selector_up_mouse_cb)
	MOUSE_CHILD_FUNCT(17,278,35,45, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, rotate_control_left_mouse_cb)
	MOUSE_CHILD_FUNCT(17,329,35,45, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, rotate_control_right_mouse_cb)
MOUSE_END

//=========================================================================================================


/*
void inline DrawFilledCircle (float x, float y, int ri, DWORD dwColor) {
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

*/

void FSAPI display2Callback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data )
{
	HPEN hOldPen, hPen;
	BYTE r, g, b;
	int i, j;
	float fAngle;
	float x, y, s;
	DWORD dwColor;
	switch (service_id)
	{
	case PANEL_SERVICE_PRE_DRAW:
		if (iWorkMode > -1) {
			PELEMENT_STATIC_IMAGE pelement = (PELEMENT_STATIC_IMAGE)(pgauge->elements_list[0]->next_element[0]);		
			if (pelement) {
				HDC hdc = pelement->hdc;
				PIXPOINT dim = pelement->image_data.final->dim;
				if (hdc) {
					if (lpdwDisplayRender) {
						switch (iWorkMode) {
						case 0: // выключен
							memset (lpdwDisplayRender, 20, DISPLAY_MEMORY_SIZE);
							StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
							&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
							break;
						case 1: // режим "ГОТОВ"
							// гасим старые засветки
							for (i = 0; i < DISPLAY_WIDTH; i++)
								for (j = 0; j < DISPLAY_HEIGHT; j++) {
									dwColor = GetColor (i, j);
									r = (BYTE)(dwColor & 0xFF);
									g = (BYTE)((dwColor & 0xFF00) >> 8);
									b = (BYTE)((dwColor & 0xFF0000) >> 16);
									r=r>0?r-1:0;
									g=g>0?g-1:0;
									b=b>0?b-1:0;
									if (dwColor)
										*(lpdwDisplayRender + (int)j * DISPLAY_WIDTH + (int)i) = RGB(r,g,b);
								}
							StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
							&BitsInfo, DIB_RGB_COLORS, SRCCOPY);

							break;
						case 2: // режим "ЗЕМЛЯ"
						case 3: // режим "МЕТЕО"
						case 4: // режим "КОНТУР"
							if (iReadyTimer > 8) {
								memset (lpdwDisplayRender, 20, DISPLAY_MEMORY_SIZE);
								StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
								break;
							}
							if (iReadyTimer == 8) {
								memset (lpdwDisplayRender, 0, DISPLAY_MEMORY_SIZE);
								StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
								break;
							}

							//if (nGridCols > 61)
							//	fAngle= (iPrevCol - (nGridCols -  1) / 2) * 1.5;
							//else
							//	fAngle= (iPrevCol - (nGridCols -  1) / 2) * 3;
							
							//DWORD dwColor;
							int iIntense;
							// гасим старые засветки
							for (i = 0; i < DISPLAY_WIDTH; i++)
								for (j = 0; j < DISPLAY_HEIGHT; j++) {
									dwColor = GetColor (i, j);
									r = (BYTE)(dwColor & 0xFF);
									g = (BYTE)((dwColor & 0xFF00) >> 8);
									b = (BYTE)((dwColor & 0xFF0000) >> 16);
									if (g > 210) g-= 40;
									r=r>0?r-1:0;
									g=g>0?g-1:0;
									b=b>0?b-1:0;
									if (dwColor)
										*(lpdwDisplayRender + (int)j * DISPLAY_WIDTH + (int)i) = RGB(r,g,b);
								}
							
							if (nGridCols > 61)
								fAngle= (iCurrentCol - (nGridCols -  1) / 2) * 1.5;
							else
								fAngle= (iCurrentCol - (nGridCols -  1) / 2) * 3;

						
								
							// отрисовываем луч развертки
							for (i = 0; i < nGridRows / (iReadyTimer + 1); i++) {
								s = 160 * i / nGridRows + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									dwColor = RGB(25, 200, 0);
									DrawFilledCircle2 (x, y, 2, dwColor);
								}
							}
						
							for (i = 0; i < nGridRows / (iReadyTimer + 1); i++) {
								s = 160 * i / nGridRows + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									iIntense = *(lpdwGrid + i * (nGridCols - 1) + iPrev2Col);
									if (iIntense > 0)
										if (iWorkMode == 4 && iIntense > 12)
											dwColor = RGB(25, 50, 0);
										else {
											if (iWorkMode == 4)
												dwColor = (iIntense + 8) * 12;
											else {
												if (iWorkMode == 3)
													dwColor = (iIntense + 5) * 12;
												else
													dwColor = (iIntense + 2) * 12;
											}

											if (dwColor > 255) dwColor = 255;
											dwColor = RGB (int(dwColor / 3), dwColor, 0);
										}
									else
										dwColor = RGB(25, 50, 0);

									//if (abs(iCurrentCol - iPrevCol) > 1)
									//	dwColor = RGB (255, 0, 0);

									DrawFilledCircle (x, y, 3, dwColor);
								}
							}


							// отрисовываем отметки масштабных колец дальности на луче
							if (fZoomSelectorPos > 25) {
								s = 160 * (25 / fZoomSelectorPos) + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									DrawFilledCircle (x, y, 2, RGB(50, 255, 0));
								}
							}
							if (fZoomSelectorPos > 50) {
								s = 160 * (50 / fZoomSelectorPos) + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									DrawFilledCircle (x, y, 2, RGB(50, 255, 0));
								}
							}
							if (fZoomSelectorPos > 75) {
								s = 160 * (75 / fZoomSelectorPos) + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									DrawFilledCircle (x, y, 2, RGB(50, 255, 0));
								}
							}
							if (fZoomSelectorPos > 100) {
								s = 160 * (100 / fZoomSelectorPos) + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									DrawFilledCircle (x, y, 2, RGB(50, 255, 0));
								}
							}
							if (fZoomSelectorPos > 200) {
								s = 160 * (200 / fZoomSelectorPos) + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									DrawFilledCircle (x, y, 2, RGB(50, 255, 0));
								}
							}
							if (fZoomSelectorPos > 300) {
								s = 160 * (300 / fZoomSelectorPos) + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									DrawFilledCircle (x, y, 2, RGB(50, 255, 0));
								}
							}			


							StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);


							break; // режим "МЕТЕО", "КОНТУР"
						case 5 : // режим "СНОС"
							// гасим старые засветки
							for (i = 0; i < DISPLAY_WIDTH; i++)
								for (j = 0; j < DISPLAY_HEIGHT; j++) {
									dwColor = GetColor (i, j);
									r = (BYTE)(dwColor & 0xFF);
									g = (BYTE)((dwColor & 0xFF00) >> 8);
									b = (BYTE)((dwColor & 0xFF0000) >> 16);
									r=r>0?r-1:0;
									g=g>0?g-1:0;
									b=b>0?b-1:0;
									if (dwColor)
										*(lpdwDisplayRender + (int)j * DISPLAY_WIDTH + (int)i) = RGB(r,g,b);
								}
								if (iReadyTimer > 5) {
									StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
									&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
									break;
								}

							// отрисовываем луч развертки
							for (i = 0; i < 160 / (iReadyTimer + 1); i++) {
								s = 160 * i / 160 + 30;
								x = s * sin (fAntennaAzimuth * PI / 180);
								y = s * cos (fAntennaAzimuth * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									dwColor = RGB(25, 200, 0);
									DrawFilledCircle2 (x, y, 2, dwColor);
								}
							}

							// отрисовываем дуги сноса отраженного сигнала
							float fDriftFeedBack = fDriftAngle - fAntennaAzimuth * 3;
							if (fDriftFeedBack + fAntennaAzimuth > 100) fDriftFeedBack = 100 - fAntennaAzimuth;
							if (fAntennaAzimuth + fDriftFeedBack < -100) fDriftFeedBack = -100 - fAntennaAzimuth;

							int k = 150;

							if (fDriftFeedBack < 0) {
								for (j = fAntennaAzimuth - 1; j > fAntennaAzimuth + fDriftFeedBack; j--) {
									for (i = 0; i < 60 / (iReadyTimer + 1); i++) {
										s = 160 * i / 60 + 30;
										x = s * sin ((float)j * PI / 180);
										y = s * cos ((float)j * PI / 180);
										x += DISPLAY_WIDTH / 2;
										y = DISPLAY_HEIGHT - 196 + y;
										if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
											dwColor = RGB(0, k, 0);
											DrawFilledCircle2 (x, y, 2, dwColor);
										}
									}
									k--;
									if (k < 0) k = 0;
								}
							}
							else {
								for (j = fAntennaAzimuth + 1; j < fAntennaAzimuth + fDriftFeedBack; j++) {
									for (i = 0; i < 60 / (iReadyTimer + 1); i++) {
										s = 160 * i / 60 + 30;
										x = s * sin ((float)j * PI / 180);
										y = s * cos ((float)j * PI / 180);
										x += DISPLAY_WIDTH / 2;
										y = DISPLAY_HEIGHT - 196 + y;
										if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
											dwColor = RGB(0, k, 0);
											DrawFilledCircle2 (x, y, 2, dwColor);
										}
									}
									k--;
									if (k < 0) k = 0;
								}
							}

							/*
								for (i = 0; i < 160 / (iReadyTimer + 1); i++) {
								
							}

							*/
							// отрисовываем луч развертки
							/*
							hPen = CreatePen (PS_SOLID, 2, RGB(50, 255, 0));
							hOldPen = (HPEN)SelectObject (hdc, hPen);
							MoveToEx (hdc, dim.x/2, dim.y - 5, NULL);
							LineTo (hdc, dim.x/2 + dim.y * 0.9 / (iReadyTimer + 1) * sin (fAntennaAzimuth * PI / 180), dim.y - 5 - dim.y * 0.9 / (iReadyTimer + 1) * cos (fAntennaAzimuth * PI / 180));
							if (iReadyTimer < 1) {
							*/
								// отрисовываем дуги сноса отраженного сигнала
							/*
								float fDriftFeedBack = fDriftAngle - fAntennaAzimuth * 3;
								if (fDriftFeedBack + fAntennaAzimuth > 100) fDriftFeedBack = 100 - fAntennaAzimuth;
								if (fAntennaAzimuth - fDriftFeedBack < -100) fDriftFeedBack = -100 - fAntennaAzimuth;
								MoveToEx (hdc, dim.x/2, dim.y - 5, NULL);
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.5, 90-fAntennaAzimuth, -(fDriftAngle - fAntennaAzimuth * 3));
							}
							SelectObject (hdc, hOldPen);
							DeleteObject (hPen);
							*/
							StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
							break;
						}
					}
				}
				SET_OFF_SCREEN(pelement); 
			}
			else {
				SET_ON_SCREEN(pelement); 
				SHOW_IMAGE (pelement);
			}
		}
		break;
	}	
        
}

////// End of definitions ///////////////////////////////////////////////////////////////////////

#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

