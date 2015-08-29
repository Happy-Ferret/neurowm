//----------------------------------------------------------------------------------------------------------------------
// Module      :  client
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

// Includes
#include "client.h"
#include "geometry.h"
#include "system.h"
#include "stackset.h"
#include "layout.h"
#include "rule.h"
#include "workspace.h"
#include "event.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static Bool isProtocolDelete(Window w) {
  int i, n;
  Atom *protocols;
  Bool ret = False;
  if (XGetWMProtocols(display, w, &protocols, &n)) {
    for (i = 0; !ret && i < n; i++)
      if (protocols[ i ] == wmatoms[ WM_DELETE_WINDOW ])
        ret = True;
    XFree(protocols);
  }
  return ret;
}

static Bool setTitleAtomC(Client *c, Atom atom) {
  assert(c);
  XTextProperty tp;
  XGetTextProperty(display, c->win, &tp, atom);
  if (!tp.nitems)
    return False;
  if (tp.encoding == XA_STRING) {
    strncpy(c->title, (char *)tp.value, NAME_MAX);
  } else {
    char **list = NULL;
    int n;
    if (XmbTextPropertyToTextList(display, &tp, &list, &n) >= Success && n > 0 && list[ 0 ]) {
      strncpy(c->title, list[ 0 ], NAME_MAX);
      XFreeStringList(list);
    }
  }
  XFree(tp.value);
  return True;
}

static CliPtr queryPointC(int ws, int x, int y) {
  CliPtr c;
  for (c=getHeadClientStackSS(ws); c; c=getNextClientSS(c))
    if (isPointInRectangleG(getRegionClientSS(c), x, y))
      break;
  return c;
}

//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

void updateC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;

  // Free windows
  if (CLI_GET(c).freeSetterFn != notFreeR)
    CLI_GET(c).freeSetterFn(getRegionClientSS(c), &screenRegion);

  // Fullscreen windows
  Rectangle r;
  if (CLI_GET(c).isFullScreen)
    memmove(&r, &screenRegion, sizeof(Rectangle));
  else
    memmove(&r, getRegionClientSS(c), sizeof(Rectangle));

  // Set border width and border gap
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  const int borderWidth = l->borderWidthSetterFn(c);
  const int borderGap = l->borderGapSetterFn(c);
  setRectangleBorderWidthAndGapG(&r, borderWidth, borderGap);
  if (r.w < 1)
    r.w = 1;
  if (r.h < 1)
    r.h = 1;

  // Draw
  XSetWindowBorder(display, CLI_GET(c).win, l->borderColorSetterFn(c));
  XSetWindowBorderWidth(display, CLI_GET(c).win, borderWidth);
  XMoveResizeWindow(display, CLI_GET(c).win, r.x, r.y, r.w, r.h);
}

void updateClassAndNameC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).class[ 0 ] = '\0';
  CLI_GET(c).name[ 0 ] = '\0';
  XClassHint ch = (XClassHint){.res_name = NULL, .res_class = NULL};
  if (!XGetClassHint(display, CLI_GET(c).win, &ch))
    return;
  strncpy(CLI_GET(c).class, ch.res_class, NAME_MAX);
  strncpy(CLI_GET(c).name, ch.res_name, NAME_MAX);
  if (ch.res_class)
    XFree(ch.res_class);
  if (ch.res_name)
    XFree(ch.res_name);
}

void updateTitleC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).title[ 0 ] = '\0';
  if (!setTitleAtomC(CLI(c), netatoms[ NET_WM_NAME ]))
    setTitleAtomC(CLI(c), XA_WM_NAME);
}

void hideC(CliPtr c, const void *doRules) {  // Move off screen
  if (!c)
    return;
  if (CLI_GET(c).isHidden)
    return;
  if (*(Bool *)doRules)
    unapplyRuleR(c);
  Rectangle *regc = getRegionClientSS(c);
  regc->x += xRes;
  regc->y += yRes;
  XMoveWindow(display, CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).isHidden = True;
}

void showC(CliPtr c, const void *doRules) {  // Move back to screen
  if (!c)
    return;
  if (!CLI_GET(c).isHidden)
    return;
  Rectangle *regc = getRegionClientSS(c);
  regc->x -= xRes;
  regc->y -= yRes;
  if (*(Bool *)doRules)
    applyRuleR(c);
  XMoveWindow(display, CLI_GET(c).win, regc->x, regc->y);
  CLI_GET(c).isHidden = False;
}

void setUrgentC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).isUrgent = True;
}

void unsetUrgentC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  CLI_GET(c).isUrgent = False;
}

void killC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (isProtocolDelete(CLI_GET(c).win)) {
    XEvent ke;
    ke.type = ClientMessage;
    ke.xclient.window = CLI_GET(c).win;
    ke.xclient.message_type = wmatoms[ WM_PROTOCOLS ];
    ke.xclient.format = 32;
    ke.xclient.data.l[ 0 ] = wmatoms[ WM_DELETE_WINDOW ];
    ke.xclient.data.l[ 1 ] = CurrentTime;
    XSendEvent(display, CLI_GET(c).win, False, NoEventMask, &ke);
  } else {
    XKillClient(display, CLI_GET(c).win);
    unmanageCliE(c);
  }
}

void minimizeC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  setCurrClientSS(getNextClientSS(c));
  unapplyRuleR(c);
  Client *cli = rmvClientSS(c);
  if (!cli)
    return;
  if (!pushMinimizedClientSS(cli))
    exitErrorS("minimizeC - could not minimize client");
  XMoveWindow(display, cli->win, xRes + 1, yRes + 1);  // Move client off screen
  runCurrLayoutL(cli->ws);
  updateFocusW(cli->ws);
}

void tileC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).freeSetterFn == notFreeR)
    return;
  CLI_GET(c).freeSetterFn = notFreeR;
  applyRuleR(c);
  runCurrLayoutL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void freeC(CliPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  const GenericArgFn *gaf = (const GenericArgFn *)freeSetterFn;
  if (CLI_GET(c).freeSetterFn == gaf->FreeSetterFn_)
    return;
  CLI_GET(c).freeSetterFn = gaf->FreeSetterFn_;
  unapplyRuleR(c);
  runCurrLayoutL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void toggleFreeC(CliPtr c, const void *freeSetterFn) {
  if (!c)
    return;
  if (CLI_GET(c).freeSetterFn != notFreeR)
    tileC(c, NULL);
  else
    freeC(c, freeSetterFn);
}

void normalC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (!CLI_GET(c).isFullScreen)
    return;
  CLI_GET(c).isFullScreen = False;
  applyRuleR(c);
  runCurrLayoutL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void fullScreenC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).isFullScreen)
    return;
  CLI_GET(c).isFullScreen = True;
  unapplyRuleR(c);
  runCurrLayoutL(CLI_GET(c).ws);
  updateFocusW(CLI_GET(c).ws);
}

void toggleFullScreenC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  if (CLI_GET(c).isFullScreen)
    normalC(c, NULL);
  else
    fullScreenC(c, NULL);
}

void moveC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  int rx, ry;
  getPtrClientW(&rx, &ry);
  moveFocusClientW(c, selfC, NULL);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime) != GrabSuccess)
    return;

  Rectangle *r = &(CLI_GET(c).floatRegion);
  int cx = r->x, cy = r->y;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->x = cx + (ev.xmotion.x - rx);
      r->y = cy + (ev.xmotion.y - ry);
      runCurrLayoutL(CLI_GET(c).ws);
      updateW(CLI_GET(c).ws);
    }
    XRaiseWindow(display, CLI_GET(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

void resizeC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  int rx, ry;
  getPtrClientW(&rx, &ry);
  moveFocusClientW(c, selfC, NULL);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime) != GrabSuccess)
    return;

  Rectangle *r = &(CLI_GET(c).floatRegion);
  XWarpPointer(display, None, CLI_GET(c).win, 0, 0, 0, 0, r->w, r->h);
  int cw = r->w, ch = r->h;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->w = cw + (ev.xmotion.x - (cw + r->x));
      r->h = ch + (ev.xmotion.y - (ch + r->y));
      runCurrLayoutL(CLI_GET(c).ws);
      updateW(CLI_GET(c).ws);
    }
    XRaiseWindow(display, CLI_GET(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

void freeMoveC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  int rx, ry;
  getPtrClientW(&rx, &ry);
  if (CLI_GET(c).freeSetterFn == notFreeR)
    unapplyRuleR(c);
  CLI_GET(c).freeSetterFn = defFreeR;
  runCurrLayoutL(CLI_GET(c).ws);
  moveFocusClientW(c, selfC, NULL);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurMove ], CurrentTime) != GrabSuccess)
    return;

  Rectangle *r = getRegionClientSS(c);
  int cx = r->x, cy = r->y;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->x = cx + (ev.xmotion.x - rx);
      r->y = cy + (ev.xmotion.y - ry);
      updateW(CLI_GET(c).ws);
    }
    XRaiseWindow(display, CLI_GET(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

void freeResizeC(CliPtr c, const void *data) {
  (void)data;
  if (!c)
    return;
  int rx, ry;
  getPtrClientW(&rx, &ry);
  if (CLI_GET(c).freeSetterFn == notFreeR)
    unapplyRuleR(c);
  CLI_GET(c).freeSetterFn = defFreeR;
  runCurrLayoutL(CLI_GET(c).ws);
  moveFocusClientW(c, selfC, NULL);
  if (XGrabPointer(display, root, False, ButtonPressMask|ButtonReleaseMask|PointerMotionMask,
      GrabModeAsync, GrabModeAsync, None, cursors[ CurResize ], CurrentTime) != GrabSuccess)
    return;

  Rectangle *r = getRegionClientSS(c);
  XWarpPointer(display, None, CLI_GET(c).win, 0, 0, 0, 0, r->w, r->h);
  int cw = r->w, ch = r->h;
  XEvent ev;
  do {
    XMaskEvent(display, ButtonPressMask|ButtonReleaseMask|PointerMotionMask, &ev);
    if (ev.type == MotionNotify) {
      r->w = cw + (ev.xmotion.x - (cw + r->x));
      r->h = ch + (ev.xmotion.y - (ch + r->y));
      updateW(CLI_GET(c).ws);
    }
    XRaiseWindow(display, CLI_GET(c).win);
  } while (ev.type != ButtonRelease);
  XUngrabPointer(display, CurrentTime);
}

// Select Functions
CliPtr selfC(const CliPtr c) {
  assert(c);
  return c;
}

CliPtr nextC(const CliPtr c) {
  assert(c);
  CliPtr n = getNextClientSS(c);
  if (!n)
    n = getHeadClientStackSS(CLI_GET(c).ws);
  return n;
}

CliPtr prevC(const CliPtr c) {
  assert(c);
  CliPtr p = getPrevClientSS(c);
  if (!p)
    p = getLastClientStackSS(CLI_GET(c).ws);
  return p;
}

CliPtr oldC(const CliPtr c) {
  assert(c);
  return getPrevClientStackSS(CLI_GET(c).ws);
}

CliPtr headC(const CliPtr c) {
  assert(c);
  return getHeadClientStackSS(CLI_GET(c).ws);
}

CliPtr lastC(const CliPtr c) {
  assert(c);
  return getLastClientStackSS(CLI_GET(c).ws);
}

CliPtr upC(const CliPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPointC(CLI_GET(c).ws, r->x+1, r->y-1);
}

CliPtr downC(const CliPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPointC(CLI_GET(c).ws, r->x+1, r->y + r->h + 1);
}

CliPtr leftC(const CliPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPointC(CLI_GET(c).ws, r->x-1, r->y+1);
}

CliPtr rightC(const CliPtr c) {
  assert(c);
  Rectangle *r = getRegionClientSS(c);
  return queryPointC(CLI_GET(c).ws, r->x + r->w + 1, r->y+1);
}

// Test functions
Bool testWindowC(const CliPtr c, const void *w) {
  assert(c);
  assert(w);
  return CLI_GET(c).win == *((Window *)w);
}

Bool testIsUrgentC(const CliPtr c, const void *p) {
  assert(c);
  (void)p;
  return CLI_GET(c).isUrgent;
}

Bool testIsFixedC(const CliPtr c, const void *p) {
  assert(c);
  (void)p;
  return CLI_GET(c).fixPos != notFixedR;
}

// Border Color
Color onlyCurrBorderColorC(const CliPtr c) {
  assert(c);
  if (isCurrClientSS(c))
    return currBorderColorS;
  return normBorderColorS;
}

Color allBorderColorC(const CliPtr c) {
  assert(c);
  if (isCurrClientSS(c))
    return currBorderColorS;
  else if (CLI_GET(c).isUrgent)
    return urgtBorderColorS;
  else if (CLI_GET(c).freeSetterFn != notFreeR)
    return freeBorderColorS;
  else if (isPrevClientSS(c))
    return prevBorderColorS;
  return normBorderColorS;
}

Color noBorderColorC(const CliPtr c) {
  (void)c;
  return normBorderColorS;
}

// Border Width
int alwaysBorderWidthC(const CliPtr c) {
  (void)c;
  return borderWidthS;
}

int neverBorderWidthC(const CliPtr c) {
  (void)c;
  return 0;
}

int smartBorderWidthC(const CliPtr c) {
  assert(c);
  if (CLI_GET(c).isFullScreen)
    return 0;
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return borderWidthS;
  Rectangle *a = getRegionClientSS(c);
  Rectangle *as = getRegionStackSS(CLI_GET(c).ws);
  if ((a->w == as->w && a->h == as->h) || (a->w == screenRegion.w && a->h == screenRegion.h))
    return 0;
  return borderWidthS;
}

int onlyCurrBorderWidthC(const CliPtr c) {
  assert(c);
  if (isCurrClientSS(c))
    return borderWidthS;
  return 0;
}


// Border Gap
int alwaysBorderGapC(const CliPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == floatArrangerL)
    return 0;
  return borderGapS;
}

int neverBorderGapC(const CliPtr c) {
  (void)c;
  return 0;
}

int smartBorderGapC(const CliPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == floatArrangerL)
    return 0;
  Rectangle *a = getRegionClientSS(c);
  Rectangle *as = getRegionStackSS(CLI_GET(c).ws);
  if ((a->w == as->w && a->h == as->h) || (a->w == screenRegion.w && a->h == screenRegion.h))
    return 0;
  return borderGapS;
}

int onlyCurrBorderGapC(const CliPtr c) {
  assert(c);
  if (CLI_GET(c).freeSetterFn != notFreeR)
    return 0;
  Layout *l = getCurrLayoutStackSS(CLI_GET(c).ws);
  if (CLI_GET(c).isFullScreen || CLI_GET(c).freeSetterFn != notFreeR || l->arrangerFn == floatArrangerL)
    return 0;
  if (!isCurrClientSS(c))
    return 0;
  return borderGapS;
}

