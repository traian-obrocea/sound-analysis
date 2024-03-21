#include <userint.h>
#include "proiect.h"

//==============================================================================
//
// Title:		func.c
// Purpose:		A short description of the implementation.
//
// Created on:	18/10/2023 at 7:16:21 PM by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include <ansi_c.h>
#include "func.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

unsigned int zero_crossing(double data[], int count) {
	unsigned int crossings = 0;
	if (count < 2) {
        return 0;
    }
	for (int i = 1; i<count; i++) {
	    if ((data[i-1] < 0 && data[i] >= 0) || (data[i-1] >= 0 && data[i] < 0)) {
            crossings++;
        }
	}
    return crossings;
}

void filtrare_ordin_I(double data[], int count, double alpha, double *filt) {
    filt[0] = data[0];
    for (int i = 1; i<count; i++) {
        filt[i] = (1-alpha)*filt[i-1] + alpha * data[i];
    }
}

void filtrare_prin_mediere(double data[], int count, int marime_fereastra, double *filt) {
    for (int i = 0; i<count; i++) {
		double sum = 0;
		for (int j = marime_fereastra; j > 0 && i-j >= 0; j--) {
			sum += data[i-j];
		}
		filt[i] = sum / marime_fereastra;
	}
}