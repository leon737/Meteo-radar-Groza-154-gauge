void FSAPI rotate_controlCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data );

#pragma warning (disable:4305 4244)


extern PELEMENT_HEADER		rotate_control_list;
extern MOUSERECT			rotate_control_mouse_rect[];


GAUGEHDR GAUGEHDR_VAR_NAME = {
                             GAUGE_HEADER_VERSION_FS800,
						     GAUGE_NAME,
							 &rotate_control_list,
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
		                     rotate_control_mouse_rect,
		                     rotate_controlCallback,
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
	rotate_control_background,
	IDB_ROTATE_BUTTONS,
	NULL,
	NULL,
	IMAGE_USE_TRANSPARENCY,
	0,
	0,0
)

PELEMENT_HEADER		rotate_control_list = &rotate_control_background.header;

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


MOUSE_BEGIN( rotate_control_mouse_rect, HELP_NONE, 0, 0 )
	MOUSE_CHILD_FUNCT(3,3,35,49, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, rotate_control_left_mouse_cb)
	MOUSE_CHILD_FUNCT(3,55,35,49, CURSOR_HAND, MOUSE_LEFTSINGLE|MOUSE_RIGHTSINGLE|MOUSE_DOWN_REPEAT, rotate_control_right_mouse_cb)
MOUSE_END

//=========================================================================================================



void FSAPI rotate_controlCallback( PGAUGEHDR pgauge, SINT32 service_id, UINT32 extra_data )
{
	        
}

////// End of definitions ///////////////////////////////////////////////////////////////////////

#undef GAUGE_NAME
#undef GAUGEHDR_VAR_NAME
#undef GAUGE_W

