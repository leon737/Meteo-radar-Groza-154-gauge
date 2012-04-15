#define         VERSION_MAJOR           1
#define         VERSION_MINOR           0
#define         VERSION_BUILD           0

// magic to get the preprocessor to do what we want
#define		lita(arg) #arg
#define		xlita(arg) lita(arg)
#define		cat3(w,x,z) w##.##x##.##z##\000
#define		xcat3(w,x,z) cat3(w,x,z)
#define		VERSION_STRING xlita(xcat3(VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD))

#ifndef		VS_VERSION_INFO
#define		VS_VERSION_INFO		0x0001
#endif


#define IDB_GAUGE_BCK					100

#define IDB_LAMP_OFF					123
#define	IDB_LAMP_GREEN_ON				124


#define	IDB_DISPLAY_BCK					140
#define IDB_DISPLAY_IMG					141
#define IDB_DISPLAY_GRID				142
#define IDB_DISPLAY_UPS					143
#define	IDB_DISPLAY2_BCK				144
#define IDB_DISPLAY2_UPS				145

#define IDB_CONTROL_BCK					180
#define IDB_CONTROL_GAL					181
#define IDB_CONTROL_LAMP_0				182
#define IDB_CONTROL_LAMP_1				183
#define IDB_CONTROL_LAMP_2				184
#define IDB_CONTROL_LAMP_3				185
#define IDB_CONTROL_LAMP_4				186
#define IDB_CONTROL_LAMP_5				187
#define IDB_CONTROL_TUMBLER_UP			188
#define IDB_CONTROL_TUMBLER_DOWN		189

#define IDB_LOGIC_BCK					200
#define	IDB_ROTATE_BUTTONS				201

#define IDB_POWER_ON					220
#define IDB_POWER_OFF					221










