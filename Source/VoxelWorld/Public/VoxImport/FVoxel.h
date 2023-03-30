#pragma once
#include "FVoxelRef.h"

#include "FVoxel.generated.h"

USTRUCT()
struct FVoxel
{
	GENERATED_BODY()

	/// <summary>
	/// Whether the voxel is empty or has a color
	/// </summary>
	bool IsEmpty;
	/// <summary>
	/// Color of the voxel; ignored if IsEmpty is true
	/// </summary>
	int32 Color;
	/// <summary>
	/// Position of the voxel along the X axis
	/// </summary>
	int32 PositionX = -1;
	/// <summary>
	/// Position of the voxel along the Y axis
	/// </summary>
	int32 PositionY = -1;
	/// <summary>
	/// Position of the voxel along the Z axis
	/// </summary>
	int32 PositionZ = -1;

	/// <summary>
	/// Empty constructor used for field initialization - Empty voxel isn't rendered
	/// </summary>
	/// <param name="positionX">X position of the Voxel</param>
	/// <param name="positionY">Y position of the Voxel</param>
	/// <param name="positionZ">Z position of the Voxel</param>
	FVoxel(uint8 positionX, uint8 positionY, uint8 positionZ)
	{
		IsEmpty = true;
		Color = 0;
		PositionX = static_cast<int32>(positionX);
		PositionY = static_cast<int32>(positionY);
		PositionZ = static_cast<int32>(positionZ);
	}

	/// <summary>
	/// Constructor used for filling in the voxel data; Voxel should be initialized using the empty constructor first
	/// </summary>
	/// <param name="positionX">X position of the Voxel</param>
	/// <param name="positionY">Y position of the Voxel</param>
	/// <param name="positionZ">Z position of the Voxel</param>
	/// <param name="color">Color of the Voxel</param>
	FVoxel(uint8 positionX, uint8 positionY, uint8 positionZ, uint8 color)
	{
		PositionX = static_cast<int32>(positionX);
		PositionY = static_cast<int32>(positionY);
		PositionZ = static_cast<int32>(positionZ);
		IsEmpty = false;
		Color = static_cast<int32>(color);
	}

	FVoxel(FIntVector position, uint8 color)
	{
		PositionX = position.X;
		PositionY = position.Y;
		PositionZ = position.Z;
		IsEmpty = false;
		Color = static_cast<int32>(color);
	}

	/// <summary>
	/// Converts the voxel structure into a string
	/// </summary>
	/// <returns>String representation of a voxel</returns>
	FString ToString() const;

	/// <summary>
	/// Default constructor. Do not use.
	/// </summary>
	FVoxel();
	FVoxel(FIntVector position, FVoxelRef voxelRef);

	FIntVector GetIntPosition() const
	{
		return FIntVector(PositionX, PositionY, PositionZ);
	}

	FVector GetPosition() const
	{
		return FVector(PositionX, PositionY, PositionZ);
	}

	void SetPosition(const FIntVector position);
};
