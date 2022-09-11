#pragma once
#include <stdalign.h> // damn
#define MKTGLMDOUBLE float

typedef struct MKTRGBA {
    MKTGLMDOUBLE r;
    MKTGLMDOUBLE g;
    MKTGLMDOUBLE b;
    MKTGLMDOUBLE a;
} MKTrgba;

typedef struct MKTRGB {
    MKTGLMDOUBLE r;
    MKTGLMDOUBLE g;
    MKTGLMDOUBLE b;
} MKTrgb;

typedef struct MKTRG {
    MKTGLMDOUBLE r;
    MKTGLMDOUBLE g;
} MKTrg;

typedef struct MKTXY {
    MKTGLMDOUBLE x;
    MKTGLMDOUBLE y;
} MKTxy;

typedef struct MKTMATF2 {
    MKTrgba a;
    MKTrgba b;
} MKTmatf2;

typedef struct MKTMATF4 {
    MKTrgba a;
    MKTrgba b;
    MKTrgba c;
    MKTrgba d;
} MKTmatf4;

typedef struct defaultDescriptorAG {
    float r;
	alignas(16) MKTrgb movement;
    alignas(16) MKTrgba colourModification;
    alignas(16) MKTrgba transformation;
} AGDescriptor;


typedef struct MKTAGVertex { // i = 0
    MKTxy pos;
    MKTrgba colour;
} AGVertex;

typedef struct MKTPiCVertex { // i = 0
    MKTxy pos;
    MKTrgba colour;
    MKTxy tex;
} PiCVertex;