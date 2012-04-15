void FSAPI controlCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data );

#pragma warning (disable:4305 4244)


extern PELEMENT_HEADER		control_list;
extern MOUSERECT			control_mouse_rect[];


GAUGEHDR GAUGEHDR_VAR_NAME = {
                             GAUGE_HEADER_VERSION_FS800,
						     GAUGE_NAME,
							 &control_list,
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
		                     controlCallback,
		                     0,
		                     NULL,
		                     0,
		                     0,
		                     NULL,
		                     {0, 0},
		                     {0.0},
	};

FAILURE_RECORD	control_fail[] =
{
	{FAIL_GAUGE_ATTITUDE, FAIL_ACTION_NONE},
	{FAIL_NONE, FAIL_ACTION_NONE}
};


NONLINEARITY  angle_selector_nonlinearity[] =
{
	{{138,	143},	 -15, 0},
	{{136,	147},	 -14, 0},
	{{130,	149},	 -12, 0},
	{{123,	150},	 -10, 0},
	{{116,	149},	  -8, 0},
	{{110,	146},	  -6, 0},
	{{105,	142},	  -4, 0},
	{{102,	136},	  -2, 0},
	{{101,	129},	   0, 0},
	{{102,	122},	   2, 0},
	{{105,	116},	   4, 0},
	{{110,	111},	   6, 0},
	{{116,	108},	   8, 0},
	{{123,	106},	  10, 0}
};

NONLINEARITY  mode_selector_nonlinearity[] =
{
	{{72,	194},	  1, 0},
	{{77,	201},	  2, 0},
	{{78,	211},	  3, 0},
	{{76,	221},	  4, 0},
	{{72,	228},	  5, 0}
};

NONLINEARITY  zoom_selector_nonlinearity[] =
{
	{{175,	229},	  10, 0},
	{{172,	216},	  60, 0},
	{{194,	193},	 192, 0},
	{{217,	216},	 314, 0},
	{{214,	229},	 375, 0}
};


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
	if (bPowerSwitchPos && iModeSelectorPos != 2)
		return (FLOAT64)iWorkMode;
	else
		return 0;
}


MAKE_ICON
(	
	control_stab_tumbler,
	IDB_CONTROL_TUMBLER_UP,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE ,
	0,
	146, 57,
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
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE ,
	0,
	297, 57,
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
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE,
	0,
	206, 64,
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
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE,
	0,
	81, 134,
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
	IMAGE_USE_TRANSPARENCY | IMAGE_USE_ERASE,
	0,
	282, 134,
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
	179, 121,
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
	NULL
};

MAKE_STATIC
(
	control_background,
	IDB_CONTROL_BCK,
	&control_ico,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		control_list = &control_background.header;



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


MOUSE_BEGIN( control_mouse_rect, HELP_NONE, 0, 0 )
	MOUSE_CHILD_FUNCT(358,39,401-358, 148-39, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE, control_power_switch_mouse_cb)
	MOUSE_CHILD_FUNCT(148,58,15,32, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE, control_stab_tumbler_mouse_cb)
	MOUSE_CHILD_FUNCT(298,58,15,32, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE, control_prd_tumbler_mouse_cb)
	MOUSE_CHILD_FUNCT(195,81,250-195,120-81, CURSOR_DOWNARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_angle_selector_down_mouse_cb)
	MOUSE_CHILD_FUNCT(195,54,250-195,80-54, CURSOR_UPARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_angle_selector_up_mouse_cb)
	MOUSE_CHILD_FUNCT(89,132,103-89,174-132, CURSOR_DOWNARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_mode_selector_down_mouse_cb)
	MOUSE_CHILD_FUNCT(104,132,103-89,174-132, CURSOR_UPARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_mode_selector_up_mouse_cb)
	MOUSE_CHILD_FUNCT(268,130,302-268,171-130, CURSOR_DOWNARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_zoom_selector_down_mouse_cb)
	MOUSE_CHILD_FUNCT(303,130,302-268,171-130, CURSOR_UPARROW, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, control_zoom_selector_up_mouse_cb)
MOUSE_END


//=========================================================================================================

void FSAPI controlCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data )
{

	switch (service_id)
	{
	case PANEL_SERVICE_PRE_UPDATE:
		break;
	}	
        
}

////// End of definitions ///////////////////////////////////////////////////////////////////////

#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

