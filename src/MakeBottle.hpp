#pragma once

#include <TopoDS.hxx>

namespace ShapeEditor {

TopoDS_Shape
MakeBottle(const double myWidth, const double myHeight,
           const double myThickness);

}
