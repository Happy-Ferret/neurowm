//----------------------------------------------------------------------------------------------------------------------
// Module      :  rule
// Copyright   :  (c) Julian Bouzas 2014
// License     :  BSD3-style (see LICENSE)
// Maintainer  :  Julian Bouzas - nnoell3[at]gmail.com
// Stability   :  stable
//----------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------
// PREPROCESSOR
//----------------------------------------------------------------------------------------------------------------------

#ifndef NEURO_RULE_H_
#define NEURO_RULE_H_

// Includes
#include "general.h"

// Defines
#define notFreeR     NULL
#define notFixedR    0
#define upFixedR     1
#define downFixedR   2
#define leftFixedR   3
#define rightFixedR  4
#define currWSR      (-1)


//----------------------------------------------------------------------------------------------------------------------
// FUNCTION DECLARATION
//----------------------------------------------------------------------------------------------------------------------

// Main Functions
Client *allocCliAndSetRulesR(Window w, const XWindowAttributes *wa);
void applyRuleR(const CliPtr c);
void unapplyRuleR(const CliPtr c);

// Free Locations
void defFreeR(Area *a, const Area *r);
void centerFreeR(Area *a, const Area *r);
void bigCenterFreeR(Area *a, const Area *r);
void scratchpadFreeR(Area *a, const Area *r);


#endif  // NEURO_RULE_H_

