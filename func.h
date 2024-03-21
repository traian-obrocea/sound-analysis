//==============================================================================
//
// Title:		func.h
// Purpose:		A short description of the interface.
//
// Created on:	18/10/2023 at 7:16:21 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __func_H__
#define __func_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions

unsigned int zero_crossing(double data[], int count);

void filtrare_ordin_I(double data[], int count, double alpha, double *filt);
void filtrare_prin_mediere(double data[], int count, int marime_fereastra, double *filt);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __func_H__ */
