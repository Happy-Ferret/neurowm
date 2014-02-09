//----------------------------------------------------------------------------------------------------------------------
// Module      :  workspace
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_WORKSPACE_H_
#define NEURO_WORKSPACE_H_

// Includes
#include "general.h"


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

void updateW(int ws);
void updateFocusW(int ws);
void moveFocusW(const CliPtr c, const SelectCliF sf);
void swapClientsW(const CliPtr c, const SelectCliF sf);
void hideW(int ws, Bool doRules);
void showW(int ws, Bool doRules);
void tileW(int ws);
void freeW(int ws, const FreeLocF ff);
void changeToWorkspaceW(int ws);
void changeToPrevWorkspaceW();
void changeToNextWorkspaceW();
void changeToLastWorkspaceW();
void moveCliToWorkspaceW(CliPtr c, int ws);
void moveCliToPrevWorkspaceW(CliPtr c);
void moveCliToNextWorkspaceW(CliPtr c);
void moveCliToLastWorkspaceW(CliPtr c);
void moveNSPCliToWorkspaceW(int ws);
void moveCliToNSPWorkspaceW(CliPtr c);
void moveCliToWorkspaceAndFollowW(CliPtr c, int ws);
void moveCliToPrevWorkspaceAndFollowW(CliPtr c);
void moveCliToNextWorkspaceAndFollowW(CliPtr c);
void moveCliToLastWorkspaceAndFollowW(CliPtr c);

// Find functions
CliPtr findWindowClientAllW(Window w);
CliPtr findWindowClientW(int ws, Window w);
CliPtr findUrgentClientW(int ws);
CliPtr findFixedClientW(int ws);

#endif  // NEURO_WORKSPACE_H_

