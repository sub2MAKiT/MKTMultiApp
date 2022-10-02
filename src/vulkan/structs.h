#pragma once
#include <stdalign.h> // damn
#include "../outerDefine.h"
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

typedef struct MKTXYZ {
    MKTGLMDOUBLE x;
    MKTGLMDOUBLE y;
    MKTGLMDOUBLE z;
} MKTxyz;

typedef struct MKTXYZW {
    MKTGLMDOUBLE x;
    MKTGLMDOUBLE y;
    MKTGLMDOUBLE z;
    MKTGLMDOUBLE w;
} MKTxyzw;

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

typedef struct MKTAGDATA {
    IntDex sizeOfVertices;
    AGVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTAGdata;

typedef struct MKTPiCVertex { // i = 0
    MKTxy pos;
    MKTrgba colour;
    MKTxy tex;
} PiCVertex;

typedef struct MKTTDVertex { // i = 0
    MKTxyzw pos;
    MKTrgba colour;
    MKTxy tex;
} TDVertex;

typedef struct MKTRGBAP {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} MKTrgbaP;

typedef struct MKTPICDATA {
    IntDex w;
    IntDex h;
    MKTrgbaP * pix;
    IntDex sizeOfVertices;
    PiCVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTPiCdata;

typedef struct MKTTDDATA {
    IntDex w;
    IntDex h;
    MKTrgbaP * pix;
    IntDex sizeOfVertices;
    PiCVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTTDdata;