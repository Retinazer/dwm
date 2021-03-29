#include <X11/XF86keysym.h>

/***************************************************

 See LICENSE file for copyright and license details.

***************************************************/

/* appearance */
static const unsigned int borderpx  = 0;        /* border pixel of windows */
static const unsigned int gappx     = 20;       /* gaps between windows */
static const unsigned int snap      = 5;        /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 10;  /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 0;        /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Siji:style=Regular:size=12", "Montserrat:style=Regular:size=10", };
static const char dmenufont[]       = "Montserrat:style=Regular:size=9";

static const char norm_fg[]     = "#d0d0d0";
static const char norm_bg[]     = "#1A2026";
static const char norm_border[] = "#262626";
static const char sel_fg[]      = "#d0d0d0";
static const char sel_bg[]      = "#2d3741";
static const char sel_border[]  = "#262626";

static const char *colors[][3]      = {
    /*                   fg           bg         border                         */
    [SchemeNorm]     = { norm_fg,     norm_bg,   norm_border }, // unfocused wins
    [SchemeSel]      = { sel_fg,      sel_bg,    sel_border  }, // the focused win
    [SchemeStatus]   = { norm_fg,     norm_bg,   "#000000"   }, // Statusbar right {text,background,not used but cannot be empty}
    [SchemeTagsSel]  = { sel_fg,      sel_bg,    "#000000"   }, // Tagbar left selected {text,background,not used but cannot be empty}
    [SchemeTagsNorm] = { norm_fg,     norm_bg,   "#000000"   }, // Tagbar left unselected {text,background,not used but cannot be empty}
    [SchemeInfoSel]  = { norm_fg,     norm_bg,   "#000000"   }, // infobar middle  selected {text,background,not used but cannot be empty}
    [SchemeInfoNorm] = { norm_fg,     norm_bg,   "#000000"   }, // infobar middle  unselected {text,background,not used but cannot be empty}
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                  instance    title                  tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Firefox",              NULL,       NULL,                  0,         0,          0,         -1,        -1 },
	{ "Surf",                 NULL,       NULL,                  0,         0,          0,         -1,        -1 },
	{ "st",                   NULL,       NULL,                  0,         0,          1,         -1,        -1 },
	{ NULL,                   NULL,       "Event Tester",        0,         1,          0,          1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "\ue002",      tile },    /* first entry is default */
	{ "\ue006",      NULL },    /* no layout function means floating behavior */
	{ "\ue004",   monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-p", "run:", "-m", dmenumon, "-fn", dmenufont, "-l", "20", "-c", "-bw", "5", "-h", "22", NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_g,      spawn,          SHCMD("optirun $(dmenu_path | dmenu -p \"optirun:\" -l 20 -c -bw 5 -h 22 \"$@\")")},
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ControlMask,           XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_comma,  setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_period, setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_h,      focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_l,      focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_l,      tagmon,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_z,      quit,           {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_z,      quit,           {1} },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 20 } },
	{ MODKEY|ShiftMask,             XK_w,      spawn,          SHCMD("$BROWSER")},
	{ MODKEY|ShiftMask,             XK_f,      spawn,          SHCMD("$TERMINAL nnn")},
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("$TERMINAL cmus")},
	{ MODKEY|ShiftMask,             XK_c,      spawn,          SHCMD("$TERMINAL calcurse")},
	{ MODKEY|ShiftMask,             XK_a,      spawn,          SHCMD("$TERMINAL pulsemixer")},
	{ MODKEY|ShiftMask,             XK_t,      spawn,          SHCMD("$TERMINAL tremc")},
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("$TERMINAL connmanctl")},
	{ MODKEY|ShiftMask,             XK_v,      spawn,          SHCMD("$TERMINAL nvim -c VimwikiIndex")},
	{ MODKEY|ShiftMask,             XK_e,      spawn,          SHCMD("$TERMINAL nvim")},
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("$TERMINAL newsboat")},
	{ MODKEY|ShiftMask,             XK_i,      spawn,          SHCMD("$TERMINAL irssi")},
	{ MODKEY|ControlMask|ShiftMask, XK_l,      spawn,          SHCMD("slock")},
	{ MODKEY|ShiftMask,             XK_s,      spawn,          SHCMD("scrot $HOME/screenshot-%Y-%m-%d-%H_%M.png -q 100 && notify-send 'Screenshot saved to:' '~/'")},
	{ MODKEY,                       XK_p,      spawn,          SHCMD("dmpass")},
	{ MODKEY|ShiftMask,             XK_d,      spawn,          SHCMD("dmrandr")},
	{ 0,XF86XK_AudioRaiseVolume,               spawn,          SHCMD("pactl set-sink-volume 0 +5% && killall sleep") },
	{ 0,XF86XK_AudioLowerVolume,               spawn,          SHCMD("pactl set-sink-volume 0 -5% && killall sleep") },
	{ 0,XF86XK_AudioMute,                      spawn,          SHCMD("pactl set-sink-mute 0 toggle") },
	{ 0,XF86XK_AudioMicMute,                   spawn,          SHCMD("amixer set Capture toggle") },
	{ 0,XF86XK_AudioPlay,                      spawn,          SHCMD("cmus-remote -u") },
	{ 0,XF86XK_AudioPrev,                      spawn,          SHCMD("cmus-remote -r") },
	{ 0,XF86XK_AudioStop,                      spawn,          SHCMD("cmus-remote -s") },
	{ 0,XF86XK_AudioNext,                      spawn,          SHCMD("cmus-remote -n") },
	{ 0,XF86XK_MonBrightnessUp,                spawn,          SHCMD("sudo bl inc 10") },
	{ 0,XF86XK_MonBrightnessDown,              spawn,          SHCMD("sudo bl dec 10") },
	{ MODKEY,												XK_y,      spawn,          SHCMD("sudo bl dmenu") },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

