#pragma once

UENUM(BlueprintType)
enum EStructureDeviation
{
  Nothing = 0,
  CanBeMissing = 1,
  PivotMarker = 2,
  VariableHeightMarker = 3,
  ForcedPivot = 4,

  SpecialHandlePoint = 8,
  SpecialCase = 9,

  SpecialHandlePoint2 = 16,
  SpecialCase2 = 17,
};
