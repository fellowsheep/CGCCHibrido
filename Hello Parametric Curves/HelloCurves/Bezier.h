#pragma once
#include "Curve.h"
class Bezier :
    public Curve
{
public:
    Bezier();
    void generateCurve(int pointsPerSegment);
};

