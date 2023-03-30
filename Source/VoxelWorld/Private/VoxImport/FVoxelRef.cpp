#include "VoxImport/FVoxelRef.h"

#include "UConfig.h"
#include "Enums/EBasicBlock.h"

bool FVoxelRef::IsEmptyOrWater(bool amIWater)
{
  if (amIWater)
  {
    return IsEmpty;
  }
  return IsEmpty || Color == (int32)EBasicBlock::Water;
}
