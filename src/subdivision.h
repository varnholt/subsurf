#pragma once

#include "array.h"
#include "vector3.h"

// perform loop subdivision sheme on incoming mesh (srcVertices, srcIndices)
// and fill destination arrays (dstvertices, dstIndices)
void loopSubdivision(
   Array<Vector3>& dstVertices,
   Array<int>& dstIndices,
   const Array<Vector3>& srcVertices,
   const Array<int>& srcIndices
);

