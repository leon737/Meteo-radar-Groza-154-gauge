void FSAPI logicCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data );

#pragma warning (disable:4305 4244)


extern PELEMENT_HEADER		logic_list;
extern MOUSERECT			logic_mouse_rect[];


GAUGEHDR GAUGEHDR_VAR_NAME = {
                             GAUGE_HEADER_VERSION_FS800,
						     GAUGE_NAME,
							 &logic_list,
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
		                     logic_mouse_rect,
		                     logicCallback,
		                     0,
		                     NULL,
		                     0,
		                     0,
		                     NULL,
		                     {0, 0},
		                     {0.0},
	};

MAKE_STATIC
(
	logic_background,
	IDB_LOGIC_BCK,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		logic_list = &logic_background.header;


MOUSE_BEGIN( logic_mouse_rect, HELP_NONE, 0, 0 )
MOUSE_END



void inline GetAircraftPos (void);


//=========================================================================================================

void FSAPI logicCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data )
{
	POINT_LATLONALT point;
	___LATLONALT lla_point;
	int i;
	
	if (bPowerSwitchPos)
		iWorkMode = iModeSelectorPos;
	else
		iWorkMode = 0;

	FLOAT64 radio_height;
	float fHorizontalDistance;
	float fPitch, fBank;
	int iIntense;

	
	switch (service_id)
	{
	case PANEL_SERVICE_PRE_UPDATE:

		if (iWorkMode > 0 && iReadyTimer > 0)
			iReadyTimer--;
		switch (iWorkMode) {
		case 3: // режим "МЕТЕО"
		case 4: // режим "КОНТУР"
			//lookup_var (&radio_alt);
			//radio_height = radio_alt.var_value.n; // радиовысота в метрах
			GetAircraftPos();
			fAntennaTrueAzimuth = hed_var.var_value.n + fAntennaAzimuth;
			if (fAntennaTrueAzimuth < 0)
				fAntennaTrueAzimuth += 360;
			if (fAntennaTrueAzimuth > 360)
				fAntennaTrueAzimuth -= 360;
			fPitch = -pitch_var.var_value.n;
			if (!bStabTumblerPos || fPitch > 20 || fPitch < -20) 
				fPitch +=fAngleSelectorPos;
			else
				fPitch = fAngleSelectorPos;
			fBank = bank_var.var_value.n;
			if (fBank > 180) fBank = fBank - 360;
			if (!bStabTumblerPos || fBank > 20 || fBank < -20)
				fPitch += sin (fAntennaAzimuth * PI / 180) * fBank;
			for (i = 0; i < nGridRows / 2; i++) {
		//	for (i = 0; i < nGridRows; i++) {
				if (iInterleaveZone) {
					fHorizontalDistance = cos(fPitch * PI /180) * ((float)i*(fZoomSelectorPos+0.5)/(float)nGridRows+fZoomSelectorPos/2);
					point.alt = aircraft_point.alt + ((float)i*(fZoomSelectorPos+0.5)*1000/0.3048/(float)nGridRows+fZoomSelectorPos/2*1000/0.3048) * sin(fPitch * PI / 180);
				}
				else {
					fHorizontalDistance = cos(fPitch * PI /180) * ((float)i*(fZoomSelectorPos+0.5)/(float)nGridRows);
					point.alt = aircraft_point.alt + ((float)i*(fZoomSelectorPos+0.5)*1000/0.3048/(float)nGridRows) * sin(fPitch * PI / 180);
				}


				get_LL (aircraft_point.lat, aircraft_point.lon, fHorizontalDistance, fAntennaTrueAzimuth, &(point.lat), &(point.lon));
				lla_point.lat = (__int64)(point.lat*Lat_Coeff);
				lla_point.lon = (__int64)(point.lon*Lon_Coeff);
				lla_point.alt = (__int64)(point.alt*Alt_Coeff);
				POBSV wx_point = get_weather_lla (lla_point);
				if (iWorkMode == 3)
					iIntense = GetCloudIntense (wx_point, point.alt, fHorizontalDistance, fZoomSelectorPos, FALSE);
				else
					iIntense = GetCloudIntense (wx_point, point.alt, fHorizontalDistance, fZoomSelectorPos, TRUE);
				
				if (iInterleaveZone)
					*(lpdwGrid + (i + nGridRows / 2) * (nGridCols - 1) + iCurrentCol) = iIntense;
				else
					*(lpdwGrid + i * (nGridCols - 1) + iCurrentCol) = iIntense;
			}
			iPrev2Col = iPrevCol;
			iPrevCol = iCurrentCol;
			
			if (iInterleaveZone)
				iInterleaveZone = 0;
			else
				iInterleaveZone = 1;


			if (iInterleaveZone) {
				if (!iAntennaMoveDirection) {
					iCurrentCol--;
					if (iCurrentCol < 0) {
						iCurrentCol = 1;
						iAntennaMoveDirection = 1;
					}
				}
				else {
					iCurrentCol++;
					if (iCurrentCol >= nGridCols - 1) {
						iCurrentCol = nGridCols - 2;
						iAntennaMoveDirection = 0;
					}
				}
			}
			if (nGridCols > 61)
				fAntennaAzimuth = (iCurrentCol - (nGridCols -  1) / 2) * 1.5;
			else
				fAntennaAzimuth = (iCurrentCol - (nGridCols -  1) / 2) * 3;

			break;
			case 2: // режим "ЗЕМЛЯ"
			lookup_var (&radio_alt);
			radio_height = radio_alt.var_value.n; // радиовысота в метрах
			GetAircraftPos();
			fAntennaTrueAzimuth = hed_var.var_value.n + fAntennaAzimuth;
			if (fAntennaTrueAzimuth < 0)
				fAntennaTrueAzimuth += 360;
			if (fAntennaTrueAzimuth > 360)
				fAntennaTrueAzimuth -= 360;
			fPitch = -pitch_var.var_value.n;
			if (!bStabTumblerPos || fPitch > 20 || fPitch < -20) 
				fPitch +=fAngleSelectorPos;
			else
				fPitch = fAngleSelectorPos;
			fBank = bank_var.var_value.n;
			if (fBank > 180) fBank = fBank - 360;
			if (!bStabTumblerPos || fBank > 20 || fBank < -20)
				fPitch += sin (fAntennaAzimuth * PI / 180) * fBank;
			for (i = 0; i < nGridRows / 2; i++) {
			//for (i = 0; i < nGridRows; i++) {
				if (iInterleaveZone) {
					fHorizontalDistance = cos(fPitch * PI /180) * ((float)i*(fZoomSelectorPos+0.5)/(float)nGridRows+fZoomSelectorPos/2);
					point.alt = aircraft_point.alt + ((float)i*(fZoomSelectorPos+0.5)*1000/0.3048/(float)nGridRows+fZoomSelectorPos/2*1000/0.3048) * sin(fPitch * PI / 180);
				}
				else {
					fHorizontalDistance = cos(fPitch * PI /180) * ((float)i*(fZoomSelectorPos+0.5)/(float)nGridRows);
					point.alt = aircraft_point.alt + ((float)i*(fZoomSelectorPos+0.5)*1000/0.3048/(float)nGridRows) * sin(fPitch * PI / 180);
				}
			
				
			
				get_LL (aircraft_point.lat, aircraft_point.lon, fHorizontalDistance, fAntennaTrueAzimuth, &(point.lat), &(point.lon));
				lla_point.lat = (__int64)(point.lat*Lat_Coeff);
				lla_point.lon = (__int64)(point.lon*Lon_Coeff);
				lla_point.alt = (__int64)(point.alt*Alt_Coeff);
				SURFACE_INFO si;
				FSGetFurface3D ((LATLONALT *)&lla_point, &si, 0x001f0009);
				switch (si.eSurfaceType) {
				case 0: //concrete  - бетон
				case 4: //asphalt - асфальт
				case 16: //metal - металл
				case 13: //coral - коралл
					iIntense = 16;
					break;
				case 18: //brick - кирпичи
				case 19: //macadam - щебень
				case 23: //tarmac - гудронированное шоссе
					iIntense= 14;
					break;
				case 7: //hard stuff - жесткая поверхность
				case 12: //dirt - почва, грунт
					iIntense = 10;
					break;
				case 1: // grass - трава
				case 6: //long grass - высокая трава
				case 11: // forest - лес
					iIntense = 9;
					break;
				case 3: //mud - грязь, слякость, шлам
					iIntense = 5;
				case 14: //gravel - гравий
				case 20: //planks - обшивная доска
					iIntense = 10;
					break;
				case 15: //oil	- масло
					iIntense = 3;
					break;
				case 17: //bituminous - битум
					iIntense = 12;
					break;
				case 21: //sand - песок
				case 22: //shale - сланец, сланцевая глина
					iIntense = 10;
					break;
				case 2: // water
					iIntense = 0;
					break;
				case 8: //snow
				case 9:	//ice
					iIntense = 7;
					break;
				default: //else
					iIntense = 3;
					break;
				}

				if (fHorizontalDistance > (radio_height + 300) * 375 / 10000) {
				//	if (fHorizontalDistance > radio_height * 375 / 6000) {
						// недостаточный сигнал
						iIntense = 20 - fHorizontalDistance / 20;
						if (iIntense < 0) iIntense = 0;
				//	}
				//	else {
						// зашумленный завышенный фон от поверхности
				//		iIntense += fHorizontalDistance / 10;
				//		if (iIntense > 18) iIntense = 18;
				//	}		
				}





				if (iInterleaveZone)
					*(lpdwGrid + (i + nGridRows / 2) * (nGridCols - 1) + iCurrentCol) = iIntense;
				else
					*(lpdwGrid + i * (nGridCols - 1) + iCurrentCol) = iIntense;
			
			
			}
			iPrev2Col = iPrevCol;
			iPrevCol = iCurrentCol;


			if (iInterleaveZone)
				iInterleaveZone = 0;
			else
				iInterleaveZone = 1;
			if (iInterleaveZone) {
				if (!iAntennaMoveDirection) {
					iCurrentCol--;
					if (iCurrentCol < 0) {
						iCurrentCol = 1;
						iAntennaMoveDirection = 1;
					}
				}
				else {
					iCurrentCol++;
					if (iCurrentCol >= nGridCols - 1) {
						iCurrentCol = nGridCols - 2;
						iAntennaMoveDirection = 0;
					}
				}
			}
			if (nGridCols > 61)
				fAntennaAzimuth = (iCurrentCol - (nGridCols -  1) / 2) * 1.5;
			else
				fAntennaAzimuth = (iCurrentCol - (nGridCols -  1) / 2) * 3;


			break;
			case 5: // режим "СНОС"
			if (!bManualRotate) {
				// автодоводка антенны к "нулю"
				if (fAntennaAzimuth > 0)
					fAntennaAzimuth-=1;
				else
					fAntennaAzimuth+=1;
				if (fAntennaAzimuth > -0.9 && fAntennaAzimuth < 0.9)
					fAntennaAzimuth=0;
			}
			lookup_var (&forward_var);
			lookup_var (&side_var);
			if ((double)forward_var.var_value.n != 0)
				fDriftAngle = (float)atan((double)side_var.var_value.n / (double)forward_var.var_value.n) * 180 / PI * 3;
			else
				fDriftAngle = 0;

			break;
		}
		break;
	}	
        
}

void inline GetAircraftPos (void) {
	lookup_var(&lat_var);
	lookup_var(&lon_var);
	lookup_var(&alt_var);
	lookup_var(&hed_var);
	lookup_var(&pitch_var);
	lookup_var(&bank_var);
	aircraft_point.lat = lat_var.var_value.n/40007000.0f*360.0f;
	aircraft_point.lon = lon_var.var_value.n/281474976710656.0f*360.0f;
	aircraft_point.alt = alt_var.var_value.n;
}

////// End of definitions ///////////////////////////////////////////////////////////////////////

#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

