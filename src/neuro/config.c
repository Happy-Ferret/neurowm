//----------------------------------------------------------------------------------------------------------------------
// Module      :  config
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "neurowm.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

// Commands
const char* defTerminalCommand[] = { "/usr/bin/xterm", NULL };
const char* defLauncherCommand[] = { "/usr/bin/gmrun", NULL };
const char *defNSPCommand[] = { "/usr/bin/xterm", "-name", WM_SCRATCHPAD_NAME, NULL };

// Startup
const ActionChain defStartUpHook[] = { CHAIN_NULL(NeuroActionChainNothing) };

// Endup
const ActionChain defEndUpHook[] = { CHAIN_NULL(NeuroActionChainNothing) };

// Layouts
static const LayoutConf tile[] = { {
  "Tile", tallArrangerL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
} };
static const LayoutConf mirr[] = { {
  "Mirr", tallArrangerL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  mirrModL, True, {ARG_INT(1), ARG_FLOAT(0.5f), ARG_FLOAT(0.03f), ARG_NULL}
} };
static const LayoutConf grid[] = { {
  "Grid", gridArrangerL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
} };
static const LayoutConf full[] = { {
  "Full", fullArrangerL, allBorderColorC, smartBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, True, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
} };
static const LayoutConf floa[] = { {
  "Float", floatArrangerL, allBorderColorC, alwaysBorderWidthC, alwaysBorderGapC, {0.0f, 0.0f, 1.0f, 1.0f},
  notModL, False, {ARG_NULL, ARG_NULL, ARG_NULL, ARG_NULL}
} };

// Layouts
const LayoutConf *defLayoutSet[] = { tile, mirr, grid, NULL };

// TogLayoutSet
const LayoutConf *defTogLayoutSet[] = { full, floa, NULL };

// Workspaces
static const Workspace ws0[] = { { "ws1", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws1[] = { { "ws2", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws2[] = { { "ws3", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws3[] = { { "ws4", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws4[] = { { "ws5", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws5[] = { { "ws6", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws6[] = { { "ws7", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws7[] = { { "ws8", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws8[] = { { "ws9", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };
static const Workspace ws9[] = { { "ws0", {0, 0, 0, 0},  defLayoutSet, defTogLayoutSet } };

// WorkspaceSet
const Workspace *defWorkspaceSet[] = { ws0, ws1, ws2, ws3, ws4, ws5, ws6, ws7, ws8, ws9, NULL };

// RuleSet
const Rule *defRuleSet[] = { NULL };

// DzenPanelSet
const DzenPanel *defDzenPanelSet[] = { NULL };

// Key
static const Key key00[] = { { Mod1Mask|ShiftMask,   XK_Return, CHAIN(NeuroActionChainSpawn, ARG_CMD(defTerminalCommand))      } };
static const Key key01[] = { { Mod1Mask,             XK_c,      CHAIN_NULL(NeuroActionChainKillClient)                         } };
static const Key key02[] = { { Mod1Mask,             XK_j,      CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(nextC))         } };
static const Key key03[] = { { Mod1Mask,             XK_k,      CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(prevC))         } };
static const Key key04[] = { { Mod1Mask,             XK_Tab,    CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(oldC))          } };
static const Key key05[] = { { Mod1Mask,             XK_m,      CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(headC))         } };
static const Key key06[] = { { Mod1Mask|ShiftMask,   XK_j,      CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(nextC))          } };
static const Key key07[] = { { Mod1Mask|ShiftMask,   XK_k,      CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(prevC))          } };
static const Key key08[] = { { Mod1Mask|ShiftMask,   XK_p,      CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(oldC))           } };
static const Key key09[] = { { Mod1Mask|ShiftMask,   XK_m,      CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(headC))          } };
static const Key key10[] = { { Mod1Mask,             XK_Up,     CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(upC))           } };
static const Key key11[] = { { Mod1Mask,             XK_Down,   CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(downC))         } };
static const Key key12[] = { { Mod1Mask,             XK_Left,   CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(leftC))         } };
static const Key key13[] = { { Mod1Mask,             XK_Right,  CHAIN(NeuroActionChainFocusCurrClient, ARG_CSF(rightC))        } };
static const Key key14[] = { { Mod1Mask|ShiftMask,   XK_Up,     CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(upC))            } };
static const Key key15[] = { { Mod1Mask|ShiftMask,   XK_Down,   CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(downC))          } };
static const Key key16[] = { { Mod1Mask|ShiftMask,   XK_Left,   CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(leftC))          } };
static const Key key17[] = { { Mod1Mask|ShiftMask,   XK_Right,  CHAIN(NeuroActionChainSwapCurrClient, ARG_CSF(rightC))         } };
static const Key key18[] = { { Mod1Mask,             XK_space,  CHAIN_NULL(NeuroActionChainChangeLayout)                       } };
static const Key key19[] = { { Mod1Mask,             XK_t,      CHAIN_NULL(NeuroActionChainToggleFreeCurrClient)               } };
static const Key key20[] = { { Mod1Mask,             XK_z,      CHAIN(NeuroActionChainToggleModLayout, ARG_UINT(mirrModL))     } };
static const Key key21[] = { { Mod1Mask,             XK_x,      CHAIN(NeuroActionChainToggleModLayout, ARG_UINT(reflXModL))    } };
static const Key key22[] = { { Mod1Mask,             XK_y,      CHAIN(NeuroActionChainToggleModLayout, ARG_UINT(reflYModL))    } };
static const Key key23[] = { { Mod1Mask,             XK_f,      CHAIN(NeuroActionChainToggleLayout, ARG_INT(0))                } };
static const Key key24[] = { { Mod1Mask,             XK_o,      CHAIN(NeuroActionChainToggleLayout, ARG_INT(1))                } };
static const Key key25[] = { { Mod1Mask|ShiftMask,   XK_f,      CHAIN_NULL(NeuroActionChainToggleFullscreenCurrClient)         } };
static const Key key26[] = { { Mod1Mask,             XK_comma,  CHAIN_NULL(NeuroActionChainIncreaseMasterLayout)               } };
static const Key key27[] = { { Mod1Mask,             XK_period, CHAIN(NeuroActionChainIncreaseMasterLayout, ARG_INT(-1))       } };
static const Key key28[] = { { Mod1Mask,             XK_l,      CHAIN_NULL(NeuroActionChainResizeMasterLayout)                 } };
static const Key key29[] = { { Mod1Mask,             XK_h,      CHAIN(NeuroActionChainResizeMasterLayout, ARG_FLOAT(-1.0f))    } };
static const Key key30[] = { { Mod1Mask,             XK_1,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(0))             } };
static const Key key31[] = { { Mod1Mask,             XK_2,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(1))             } };
static const Key key32[] = { { Mod1Mask,             XK_3,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(2))             } };
static const Key key33[] = { { Mod1Mask,             XK_4,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(3))             } };
static const Key key34[] = { { Mod1Mask,             XK_5,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(4))             } };
static const Key key35[] = { { Mod1Mask,             XK_6,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(5))             } };
static const Key key36[] = { { Mod1Mask,             XK_7,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(6))             } };
static const Key key37[] = { { Mod1Mask,             XK_8,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(7))             } };
static const Key key38[] = { { Mod1Mask,             XK_9,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(8))             } };
static const Key key39[] = { { Mod1Mask,             XK_0,      CHAIN(NeuroActionChainChangeWorkspace, ARG_INT(9))             } };
static const Key key40[] = { { ControlMask|Mod1Mask, XK_Left,   CHAIN(NeuroActionChainRelativeChangeWorkspace, ARG_WSF(prevW)) } };
static const Key key41[] = { { ControlMask|Mod1Mask, XK_Right,  CHAIN(NeuroActionChainRelativeChangeWorkspace, ARG_WSF(nextW)) } };
static const Key key42[] = { { Mod1Mask|ShiftMask,   XK_Tab,    CHAIN(NeuroActionChainRelativeChangeWorkspace, ARG_WSF(oldW))  } };
static const Key key43[] = { { Mod1Mask|ShiftMask,   XK_1,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(0))              } };
static const Key key44[] = { { Mod1Mask|ShiftMask,   XK_2,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(1))              } };
static const Key key45[] = { { Mod1Mask|ShiftMask,   XK_3,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(2))              } };
static const Key key46[] = { { Mod1Mask|ShiftMask,   XK_4,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(3))              } };
static const Key key47[] = { { Mod1Mask|ShiftMask,   XK_5,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(4))              } };
static const Key key48[] = { { Mod1Mask|ShiftMask,   XK_6,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(5))              } };
static const Key key49[] = { { Mod1Mask|ShiftMask,   XK_7,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(6))              } };
static const Key key50[] = { { Mod1Mask|ShiftMask,   XK_8,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(7))              } };
static const Key key51[] = { { Mod1Mask|ShiftMask,   XK_9,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(8))              } };
static const Key key52[] = { { Mod1Mask|ShiftMask,   XK_0,      CHAIN(NeuroActionChainSendCurrClient, ARG_INT(9))              } };
static const Key key53[] = { { Mod1Mask|ControlMask, XK_1,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(0))        } };
static const Key key54[] = { { Mod1Mask|ControlMask, XK_2,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(1))        } };
static const Key key55[] = { { Mod1Mask|ControlMask, XK_3,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(2))        } };
static const Key key56[] = { { Mod1Mask|ControlMask, XK_4,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(3))        } };
static const Key key57[] = { { Mod1Mask|ControlMask, XK_5,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(4))        } };
static const Key key58[] = { { Mod1Mask|ControlMask, XK_6,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(5))        } };
static const Key key59[] = { { Mod1Mask|ControlMask, XK_7,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(6))        } };
static const Key key60[] = { { Mod1Mask|ControlMask, XK_8,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(7))        } };
static const Key key61[] = { { Mod1Mask|ControlMask, XK_9,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(8))        } };
static const Key key62[] = { { Mod1Mask|ControlMask, XK_0,      CHAIN(NeuroActionChainSendFollowCurrClient, ARG_INT(9))        } };
static const Key key63[] = { { Mod1Mask|ShiftMask,   XK_q,      CHAIN_NULL(NeuroActionChainQuit)                               } };
static const Key key64[] = { { Mod1Mask,             XK_q,      CHAIN_NULL(NeuroActionChainReload)                             } };
static const Key key65[] = { { Mod1Mask,             XK_F2,     CHAIN(NeuroActionChainSpawn, ARG_CMD(defLauncherCommand))      } };
static const Key key66[] = { { Mod1Mask|ShiftMask,   XK_space,  CHAIN_NULL(NeuroActionChainResetLayout)                        } };
static const Key key67[] = { { Mod1Mask,             XK_n,      CHAIN_NULL(NeuroActionChainMinimizeCurrClient)                 } };
static const Key key68[] = { { Mod1Mask|ShiftMask,   XK_n,      CHAIN_NULL(NeuroActionChainRestoreLastMinimized)               } };

// Keys
const Key *defKeys[] = {
  key00, key01, key02, key03, key04, key05, key06, key07, key08, key09,
  key10, key11, key12, key13, key14, key15, key16, key17, key18, key19,
  key20, key21, key22, key23, key24, key25, key26, key27, key28, key29,
  key30, key31, key32, key33, key34, key35, key36, key37, key38, key39,
  key40, key41, key42, key43, key44, key45, key46, key47, key48, key49,
  key50, key51, key52, key53, key54, key55, key56, key57, key58, key59,
  key60, key61, key62, key63, key64, key65, key66, key67, key68, NULL
};

// Button
static const Button button00[] = { { noModMask,          Button1, CHAIN_NULL(NeuroActionChainFocusPtrClient),            True  } };
static const Button button01[] = { { Mod1Mask,           Button1, CHAIN_NULL(NeuroActionChainFreeMovePtrClient),         False } };
static const Button button02[] = { { Mod1Mask,           Button2, CHAIN_NULL(NeuroActionChainToggleFreePtrClient),       False } };
static const Button button03[] = { { Mod1Mask,           Button3, CHAIN_NULL(NeuroActionChainFreeResizePtrClient),       False } };
static const Button button04[] = { { Mod1Mask|ShiftMask, Button1, CHAIN_NULL(NeuroActionChainFloatMovePtrClient),        False } };
static const Button button05[] = { { Mod1Mask|ShiftMask, Button2, CHAIN_NULL(NeuroActionChainToggleFullscreenPtrClient), False } };
static const Button button06[] = { { Mod1Mask|ShiftMask, Button3, CHAIN_NULL(NeuroActionChainFloatResizePtrClient),      False } };

// Buttons
const Button *defButtons[] = { button00, button01, button02, button03, button04, button05, button05, NULL };


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC VARIABLE DEFINITION
//----------------------------------------------------------------------------------------------------------------------

const Configuration defConfiguration = {
  defNormBorderColor,
  defCurrBorderColor,
  defPrevBorderColor,
  defFreeBorderColor,
  defUrgtBorderColor,
  defBorderWidth,
  defBorderGap,
  defWorkspaceSet,
  defRuleSet,
  defDzenPanelSet,
  defKeys,
  defButtons,
  defStartUpHook,
  defEndUpHook
};

