#pragma once
#include "Curve.h"
class Hermite :
    public Curve
{
public:
    Hermite();
    void generateCurve(int pointsPerSegment);
};

