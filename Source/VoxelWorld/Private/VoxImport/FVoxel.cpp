#include "VoxImport/FVoxel.h"

#include <string>

#include "VoxImport/FVoxelRef.h"

using namespace std;

FString FVoxel::ToString() const
{
	return UTF8_TO_TCHAR((IsEmpty
		       ? "Empty Voxel"
		       : "Voxel at " + to_string(PositionX) + ", " + to_string(PositionY) + ", " + to_string(PositionZ)).c_str());
}


FVoxel::FVoxel(): IsEmpty(true), Color(0)
{
	// No implementation
}

FVoxel::FVoxel(FIntVector position, FVoxelRef voxelRef)
{
	PositionX = position.X;
	PositionY = position.Y;
	PositionZ = position.Z;
	IsEmpty = voxelRef.IsEmpty;
	Color = voxelRef.Color;
}

void FVoxel::SetPosition(const FIntVector position)
{
	PositionX = position.X;
	PositionY = position.Y;
	PositionZ = position.Z;
}
