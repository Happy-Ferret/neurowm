//----------------------------------------------------------------------------------------------------------------------
// Module      :  neurowm
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
#include "system.h"
#include "core.h"
#include "event.h"
#include "dzen.h"


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE VARIABLE DECLARATION
//----------------------------------------------------------------------------------------------------------------------

static Bool stopWhile = False;


//----------------------------------------------------------------------------------------------------------------------
// PRIVATE FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

static int recompile_wm(pid_t *pid) {
  char out[ NAME_MAX ] = "", src[ NAME_MAX ] = "";
  snprintf(out, NAME_MAX, "%s/." WM_NAME "/" WM_MYNAME, getenv("HOME"));
  snprintf(src, NAME_MAX, "%s/." WM_NAME "/" WM_NAME ".c", getenv("HOME"));
  char lib[ NAME_MAX ] = "/usr/lib/neuro/lib" WM_NAME ".a";
  const char *const cmd[] = { "/usr/bin/cc", "-O3", "-o", out, src, lib, "-lX11", "-pthread", NULL };
  return NeuroSystemSpawn(cmd, pid);
}

static void stop_wm() {
  NeuroActionUtilRunActionChain(NeuroSystemGetConfiguration()->endUpHook);
  NeuroDzenStop();
  NeuroCoreStop();
  NeuroSystemStop();
}

static void wm_signal_handler(int signo) {
  if (signo == SIGUSR1) {
    stop_wm();
    pid_t pid;
    if (recompile_wm(&pid) == -1)
      perror("wm_signal_handler - Could not recompile neurowm");
    waitpid(pid, NULL, WUNTRACED);
    exit(EXIT_RELOAD);
  }
}

static void init_wm(const Configuration *c) {
  if (!c)
    NeuroSystemError("init_wm - Could not set configuration");

  // Set configuration and init base, stackset and panels
  NeuroSystemSetConfiguration(c);
  if (!NeuroSystemInit())
    NeuroSystemError("init_wm - Could not init Base");
  if (!NeuroCoreInit())
    NeuroSystemError("init_wm - Could not init StackSet");
  if (!NeuroDzenInit())
    NeuroSystemError("init_wm - Could not init Panels");

  NeuroActionUtilRunActionChain(NeuroSystemGetConfiguration()->startUpHook);

  // Catch asynchronously SIGUSR1
  // if (SIG_ERR == signal(SIGUSR1, signalHandler))
  //   NeuroSystemError("init_wm - Could not set SIGHUP handler");

  // Load existing windows if Xsesion was not closed
  NeuroEventLoadWindows();
}


//----------------------------------------------------------------------------------------------------------------------
// PUBLIC FUNCTION DEFINITION
//----------------------------------------------------------------------------------------------------------------------

int NeuroNeurowmRun(const Configuration *c) {
  // Init window manager
  init_wm(c);

  // Main loop
  XEvent ev;
  while (!stopWhile && !XNextEvent(NeuroSystemGetDisplay(), &ev))
    if (eventArray[ ev.type ])
      eventArray[ ev.type ](&ev);

  // Stop window manager
  stop_wm();

  return EXIT_SUCCESS;
}

void NeuroNeurowmQuit() {
  stopWhile = True;
}

void NeuroNeurowmReload() {
  wm_signal_handler(SIGUSR1);
}

