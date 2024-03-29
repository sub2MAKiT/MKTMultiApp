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

typedef struct MKTPiCVertex { // i = 1
    MKTxy pos;
    MKTrgba colour;
    MKTxy tex;
} PiCVertex;

typedef struct MKTQDRVertex { // i = 3
    MKTxy pos1;
    MKTxy pos2;
    MKTxy pos3;
    MKTxy pos4;
    char option;
    MKTrgba colour;
} QDRVertex;

typedef struct MKTTDVertex { // i = 0
    MKTxyzw pos;
    MKTrgba colour;
    MKTxy tex;
} TDVertex;

typedef unsigned char MKTaBM;

typedef struct MKTRGBAP {
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} MKTrgbaP;

typedef struct MKTAGDATA {
    IntDex sizeOfVertices;
    AGVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTAGdata;

typedef struct MKTQDRDATA {
    IntDex sizeOfVertices; // totally not a vertex, but oh well, no one will use it anyway
    QDRVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTQDRdata;

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
    TDVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTTDdata;

typedef struct MKTBMDATA {
    IntDex w;
    IntDex h;
    MKTaBM * pix;
    IntDex sizeOfVertices;
    PiCVertex * vertices;
    IntDex sizeOfIndices;
    unsigned int * indices;
} MKTbmdata;