void FSAPI displayCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data );

#pragma warning (disable:4305 4244)


extern PELEMENT_HEADER		display_list;
extern MOUSERECT			display_mouse_rect[];


GAUGEHDR GAUGEHDR_VAR_NAME = {
                             GAUGE_HEADER_VERSION_FS800,
						     GAUGE_NAME,
							 &display_list,
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
		                     display_mouse_rect,
		                     displayCallback,
		                     0,
		                     NULL,
		                     0,
		                     0,
		                     NULL,
		                     {0, 0},
		                     {0.0},
	};

FAILURE_RECORD	display_fail[] =
{
	{FAIL_GAUGE_ATTITUDE, FAIL_ACTION_NONE},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


MAKE_STATIC
(
	display_ups,
	IDB_DISPLAY_UPS,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	121,34
)

PELEMENT_HEADER		display_ico_ups[] =
{
	&display_ups.header,
	NULL
};

MAKE_STATIC
(
	display_grid,
	IDB_DISPLAY_GRID,
	&display_ico_ups,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	193,90
)

PELEMENT_HEADER		display_ico_grid[] =
{
	&display_grid.header,
	NULL
};

MAKE_STATIC
(
	display_img,
	IDB_DISPLAY_IMG,
	&display_ico_grid,
	NULL,
	IMAGE_USE_ERASE|IMAGE_USE_BRIGHT|IMAGE_CREATE_DIBSECTION,
	0,
	193,90
)

PELEMENT_HEADER		display_ico_img[] =
{
	&display_img.header,
	NULL
};

MAKE_STATIC
(
	display_background,
	IDB_DISPLAY_BCK,
	&display_ico_img,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		display_list = &display_background.header;


MOUSE_BEGIN( display_mouse_rect, HELP_NONE, 0, 0 )
MOUSE_END

//=========================================================================================================



void FSAPI displayCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data )
{
	HPEN hOldPen, hPen;
	int i;
	float fAngle;
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
							if (iReadyTimer == 0)
								memset (lpdwDisplayRender, 0, DISPLAY_MEMORY_SIZE);
							else
								memset (lpdwDisplayRender, 20, DISPLAY_MEMORY_SIZE);
							StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
							&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
							break;
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

							if (nGridCols > 61)
								fAngle= (iPrevCol - (nGridCols -  1) / 2) * 1.5;
							else
								fAngle= (iPrevCol - (nGridCols -  1) / 2) * 3;
							float x, y, s;
							DWORD dwColor;
							int iIntense;
							for (i = 0; i < nGridRows; i++) {
								s = 160 * i / nGridRows + 30;
								x = s * sin (fAngle * PI / 180);
								y = s * cos (fAngle * PI / 180);
								x += DISPLAY_WIDTH / 2;
								y = DISPLAY_HEIGHT - 196 + y;
								if (y > 0 && y < DISPLAY_HEIGHT && x > 0 && x < DISPLAY_WIDTH) {
									iIntense = *(lpdwGrid + i * (nGridCols - 1) + iPrevCol);
									if (iIntense > 0)
										if (iWorkMode == 4 && iIntense > 12)
											//if (FadeExtremeWeather (i, iPrevCol))
												dwColor = RGB(0, 0, 0);
											//else {
												//dwColor = RGB (200 - (iIntense-2) * 10, 255, 200 - (iIntense-2) * 10);
											//	dwColor = iIntense * 30;
											//	if (dwColor > 255) dwColor = 255;
											//	dwColor = RGB (0, dwColor, 0);
											//}
										else {
											//dwColor = RGB (200 - iIntense * 10, 255, 200 - iIntense * 10);
											dwColor = iIntense * 12;
											if (dwColor > 255) dwColor = 255;
											dwColor = RGB (0, dwColor, 0);
										}
									else
										dwColor = RGB(0, 0, 0);
									DrawFilledCircle (x, y, 3, dwColor);
								}

							}
							if (iReadyTimer < 1)
								StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
							// отрисовываем луч развертки
							hPen = CreatePen (PS_SOLID, 2, RGB(50, 255, 0));
							hOldPen = (HPEN)SelectObject (hdc, hPen);
							MoveToEx (hdc, dim.x/2, dim.y - 5, NULL);
							LineTo (hdc, dim.x/2 + dim.y * 0.9 / (iReadyTimer + 1) * sin (fAngle * PI / 180), dim.y - 5 - dim.y * 0.9 / (iReadyTimer + 1) * cos (fAngle * PI / 180));
							// отрисовываем масштабные кольца дальности
							if (fZoomSelectorPos > 25) {
								MoveToEx (hdc, dim.x/2, dim.y - 5, NULL);
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.9 * (25 / fZoomSelectorPos), 0, 180);
							}
							if (fZoomSelectorPos > 50) {
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.9 * (50 / fZoomSelectorPos), 0, 180);
							}
							if (fZoomSelectorPos > 75) {
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.9 * (75 / fZoomSelectorPos), 0, 180);
							}
							if (fZoomSelectorPos > 100) {
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.9 * (100 / fZoomSelectorPos), 0, 180);
							}
							if (fZoomSelectorPos > 200) {
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.9 * (200 / fZoomSelectorPos), 0, 180);
							}
							if (fZoomSelectorPos > 30) {
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.9 * (300 / fZoomSelectorPos), 0, 180);
							}
							SelectObject (hdc, hOldPen);
							DeleteObject (hPen);
							break; // режим "МЕТЕО", "КОНТУР"
						case 5 : // режим "СНОС"
							if (iReadyTimer > 8) {
								memset (lpdwDisplayRender, 20, DISPLAY_MEMORY_SIZE);
								StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
								break;
							}
							if (iReadyTimer < 9) {
								memset (lpdwDisplayRender, 0, DISPLAY_MEMORY_SIZE);
								StretchDIBits (hdc, 0, 0, dim.x, dim.y, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, lpdwDisplayRender,
								&BitsInfo, DIB_RGB_COLORS, SRCCOPY);
							}
							if (iReadyTimer > 5)
								break;
							// отрисовываем луч развертки
							hPen = CreatePen (PS_SOLID, 2, RGB(50, 255, 0));
							hOldPen = (HPEN)SelectObject (hdc, hPen);
							MoveToEx (hdc, dim.x/2, dim.y - 5, NULL);
							LineTo (hdc, dim.x/2 + dim.y * 0.9 / (iReadyTimer + 1) * sin (fAntennaAzimuth * PI / 180), dim.y - 5 - dim.y * 0.9 / (iReadyTimer + 1) * cos (fAntennaAzimuth * PI / 180));
							if (iReadyTimer < 1) {
								// отрисовываем дуги сноса отраженного сигнала
								float fDriftFeedBack = fDriftAngle - fAntennaAzimuth * 3;
								if (fDriftFeedBack + fAntennaAzimuth > 100) fDriftFeedBack = 100 - fAntennaAzimuth;
								if (fAntennaAzimuth - fDriftFeedBack < -100) fDriftFeedBack = -100 - fAntennaAzimuth;
								MoveToEx (hdc, dim.x/2, dim.y - 5, NULL);
								AngleArc (hdc, dim.x/2, dim.y - 5, dim.y * 0.5, 90-fAntennaAzimuth, -(fDriftAngle - fAntennaAzimuth * 3));
							}
							SelectObject (hdc, hOldPen);
							DeleteObject (hPen);
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

