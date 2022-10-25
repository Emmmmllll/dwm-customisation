/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char blank[]	        = "#000000";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	/*[SchemeNorm]*/ { col_gray3, col_gray1, col_gray2 },
	/*[SchemeSel]*/  { col_gray4, col_cyan,  col_cyan  },
    /*[SchemeBlank]*/{ blank	, blank		, blank    },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define SUPERKEY Mod4Mask
#define XF86_AudioLowerVolume 0x1008ff11
#define XF86_AudioMute 0x1008ff12
#define XF86_AudioRaiseVolume 0x1008ff13
#define XF86_AudioPlay 0x1008ff14
#define XF86_AudioStop 0x1008ff15
#define XF86_AudioPrev 0x1008ff16
#define XF86_AudioNext 0x1008ff17

#define Grabbed 1
#define Repeat 1 << 1
#define Release 1 << 2

#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       Grabbed,		KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           Grabbed,		KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             Grabbed,		KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, Grabbed,		KEY,      toggletag,      {.ui = 1 << TAG} }

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
// static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
//static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *roficmd[] = { "rofi", "-show", "drun", NULL }; /* rofi */
static const char *termcmd[]  = { "alacritty", NULL }; /* terminal */
static const char *audioPlayPause[] = { "playerctl", "play-pause", NULL }; /* playPause */
static const char *audioNext[] = { "playerctl", "next", NULL };
static const char *audioPrev[] = { "playerctl", "previous", NULL };
static const char *audioMuteToggle[] = {"pactl", "set-sink-mute", "@DEFAULT_SINK", "toggle", NULL};
static const char *lowerVolume[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK", "-5%", NULL};
static const char *raiseVolume[] = {"pactl", "set-sink-volume", "@DEFAULT_SINK", "+5%", NULL};


using config::Key;
static Key keys[] = {
	/* modifier     	   			grabRepRel_Mask				key        function        argument */
	{ 0,	          				Grabbed|Repeat,				XF86_AudioNext, spawn,     { .v = audioNext }},
	{ 0,	          				Grabbed|Repeat,				XF86_AudioPrev, spawn,     { .v = audioPrev }},
	{ 0,	          				Grabbed|Repeat,				XF86_AudioLowerVolume, spawn, {.v = lowerVolume }},
	{ 0,	          				Grabbed|Repeat,				XF86_AudioRaiseVolume, spawn, {.v = raiseVolume }},
	{ 0,	          				Grabbed|Repeat,				XF86_AudioMute, spawn,     {.v = audioMuteToggle }},
	{ MODKEY,               		Grabbed|Repeat,				XK_b,      togglebar,      {0} },
	{ MODKEY,               		Grabbed|Repeat,				XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,               		Grabbed|Repeat,				XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,               		Grabbed|Repeat,				XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,               		Grabbed|Repeat,				XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,               		Grabbed|Repeat,				XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,               		Grabbed|Repeat,				XK_l,      setmfact,       {.f = +0.05} },
	//{ MODKEY,             		Grabbed|Repeat,		  		XK_Return, zoom,           {0} },
	{ MODKEY,               		Grabbed|Repeat,				XK_Tab,    view,           {0} },
	{ MODKEY,               		Grabbed|Repeat,				XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,               		Grabbed|Repeat,				XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,               		Grabbed|Repeat,				XK_m,      setlayout,      {.v = &layouts[2]} },
	//{ MODKEY,             		Grabbed|Repeat,		  		XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,     		Grabbed|Repeat,				XK_space,  togglefloating, {0} },
	{ MODKEY,               		Grabbed|Repeat,				XK_0,      view,           {.ui = ~0U } },
	{ MODKEY|ShiftMask,     		Grabbed|Repeat,				XK_0,      tag,            {.ui = ~0U } },
	{ MODKEY,               		Grabbed|Repeat,				XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,               		Grabbed|Repeat,				XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,     		Grabbed|Repeat,				XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,     		Grabbed|Repeat,				XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                      								XK_1,                      0),
	TAGKEYS(                      								XK_2,                      1),
	TAGKEYS(                      								XK_3,                      2),
	TAGKEYS(                      								XK_4,                      3),
	TAGKEYS(                      								XK_5,                      4),
	TAGKEYS(                      								XK_6,                      5),
	TAGKEYS(                      								XK_7,                      6),
	TAGKEYS(                      								XK_8,                      7),
	TAGKEYS(                      								XK_9,                      8),
	// on released
	{ MODKEY|ShiftMask,				Grabbed|Release,            XK_e,      quit,           {0} },
	// no repeate
	{ 0,	          		        Grabbed,					XF86_AudioPlay, spawn,     {.v = audioPlayPause }},
	{ MODKEY|ShiftMask,             Grabbed,					XK_q,      killclient,     {0} },
	{ SUPERKEY,                     Grabbed,					XK_space,  spawn,          {.v = roficmd } },
	{ SUPERKEY,	                	Grabbed,					XK_Return, spawn,          {.v = termcmd } },
	{ ControlMask|MODKEY,			Grabbed,					XK_r, refreshconfig,       {0} }
};
/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
using config::Button;
using config::ClkLtSymbol;
using config::ClkWinTitle;
using config::ClkStatusText;
using config::ClkTagBar;
using config::ClkClientWin;
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

