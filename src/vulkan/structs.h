#pragma once
#define MKTGLMDOUBLE float

typedef struct MKTRGBA {
    MKTGLMDOUBLE r;
    MKTGLMDOUBLE g;
    MKTGLMDOUBLE b;
    MKTGLMDOUBLE a;
} MKTrgba;

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


typedef struct MKTAGVertex { // i = 0
    MKTxy pos;
    MKTrgba colour;
} AGVertex;

