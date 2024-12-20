/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 12;       /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int user_bh            = 22;       /* user bar height; set to 0 if you want to use dwm's... */
static const char *fonts[]          = { "JetBrainsMono Nerd Font:style=Regular:size=10" };
static const char dmenufont[]       = "JetBrainsMono Nerd Font:style=Regular:size=10";
static const char col_gray1[]       = "#1e1e2e";
static const char col_gray2[]       = "#6c7086";
static const char col_gray3[]       = "#cdd6f4";
static const char col_gray4[]       = "#cdd6f4";
static const char col_cyan[]        = "#1e1e2e";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray3  },
};

/* tagging */
static const char *tags[] = { "/", "www", "", "", };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "mpv",      NULL,       NULL,       0,            1,           -1 },
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
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_gray2, "-sf", col_gray4, NULL };

static Key keys[] = {
    /* modifier                     key        function        argument */
    { MODKEY,                       XK_space,  spawn,          {.v = dmenucmd } },
    { MODKEY|ShiftMask,             XK_space,  spawn,          SHCMD("networkmanager_dmenu") },
    { MODKEY,                       XK_a,      spawn,          SHCMD("st") },
    { MODKEY,                       XK_f,      spawn,          SHCMD("firefox") },

    { MODKEY,                       XK_k,      spawn,          SHCMD("pactl set-sink-volume 0 +10%") },
    { MODKEY,                       XK_j,      spawn,          SHCMD("pactl set-sink-volume 0 -10%") },
    { MODKEY,                       XK_l,      spawn,          SHCMD("pactl set-sink-volume 0 100%") },
    { MODKEY|ShiftMask,             XK_l,      spawn,          SHCMD("pactl set-sink-volume 0 0%") },

    { MODKEY,                       XK_p,      spawn,          SHCMD("mpc play") },
    { MODKEY|ShiftMask,             XK_p,      spawn,          SHCMD("mpc pause") },
    { MODKEY,                       XK_n,      spawn,          SHCMD("mpc next") },
    { MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("st ncmpcpp") },

    { MODKEY,                       XK_Print,  spawn,          SHCMD("maim -s ~/Pictures/$(date +%s).png") },

    { MODKEY,                       XK_b,      togglebar,      {0} },
    { MODKEY|ShiftMask,             XK_m,      togglefullscr,  {0} },
    { MODKEY|ShiftMask,             XK_f,      togglefloating, {0} },
    { MODKEY,                       XK_comma,  tagmon,         {.i = -1 } },
    { MODKEY,                       XK_period, tagmon,         {.i = +1 } },

    { MODKEY,                       XK_minus,  setgaps,        {.i = -1 } },
    { MODKEY|ShiftMask,             XK_minus,  setgaps,        {.i = 0  } },
    { MODKEY,                       XK_equal,  setgaps,        {.i = +1 } },

    TAGKEYS(                        XK_1,                      0)
    TAGKEYS(                        XK_2,                      1)
    TAGKEYS(                        XK_3,                      2)
    TAGKEYS(                        XK_4,                      3)
    TAGKEYS(                        XK_5,                      4)
    TAGKEYS(                        XK_6,                      5)
    TAGKEYS(                        XK_7,                      6)
    TAGKEYS(                        XK_8,                      7)
    TAGKEYS(                        XK_9,                      8)

    { MODKEY,                       XK_w,      killclient,     {0} },
    { MODKEY,                       XK_q,      spawn,          SHCMD("powermenu") },
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },
    /* laptop stuff */
    { 0,                            XF86XK_AudioMute,  spawn,  SHCMD("pactl set-sink-volume 0 0%") },
    { MODKEY,                       XF86XK_AudioMute,  spawn,  SHCMD("pactl set-sink-volume 0 100%") },
    { 0,                            XF86XK_AudioRaiseVolume,spawn,SHCMD("pactl set-sink-volume 0 +1%") },
    { 0,                            XF86XK_AudioLowerVolume,  spawn,  SHCMD("pactl set-sink-volume 0 -1%") },


    { 0,                            XF86XK_MonBrightnessUp,  spawn,  SHCMD("brightnessctl set 10%+") },
    { 0,                            XF86XK_MonBrightnessDown,  spawn,  SHCMD("brightnessctl set 10%-") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
    /* click                event mask      button          function        argument */
    { ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
    { ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
    { ClkWinTitle,          0,              Button2,        zoom,           {0} },
    { ClkStatusText,        0,              Button2,        spawn,          SHCMD("st") },
    { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
    { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
    { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
    { ClkTagBar,            0,              Button1,        view,           {0} },
    { ClkTagBar,            0,              Button3,        toggleview,     {0} },
    { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
    { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
