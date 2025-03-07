#pragma once
#include "Curve.h"
class CatmullRom :
    public Curve
{
public:
    CatmullRom();
    void generateCurve(int pointsPerSegment);
};

