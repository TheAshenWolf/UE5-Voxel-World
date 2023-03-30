#include "VoxImport/UVoxParser.h"

#include "VoxImport/UVoxFormatUtilities.h"

UVoxParser::UVoxParser()
{
  // No implementation
}


void UVoxParser::Read4(TArray<uint8> fileData, int32& index, TArray<uint8>& buffer)
{
  buffer = { fileData[index], fileData[index + 1], fileData[index + 2], fileData[index + 3] };
  index += 4;
}

FVoxObject UVoxParser::FromFile(FString path)
{
  TArray<uint8> fileData = TArray<uint8>();
  bool success = FFileHelper::LoadFileToArray(fileData, *path);

  if (!success)
  {
    UE_LOG(LogTemp, Error, TEXT("Wrong file path. Stoopid."));
    return FVoxObject();
  }
  
  uint32 rVersion, rMainSize, rSizeX, rSizeY, rSizeZ, rXyziSize, rVoxelCount;
  FVoxObject result = FVoxObject(0, 0, 0);

  int32 index = 0;
  TArray<uint8> buffer = TArray<uint8>();
  Read4(fileData, index, buffer); // "VOX "
  Read4(fileData, index, buffer);
  rVersion = UVoxFormatUtilities::ToUint32(buffer);
  Read4(fileData, index, buffer); // "MAIN"
  Read4(fileData, index, buffer);
  Read4(fileData, index, buffer);
  rMainSize = UVoxFormatUtilities::ToUint32(buffer);
  Read4(fileData, index, buffer); // "SIZE"
  Read4(fileData, index, buffer);
  Read4(fileData, index, buffer);
  
  Read4(fileData, index, buffer);
  rSizeX = UVoxFormatUtilities::ToUint32(buffer);
  Read4(fileData, index, buffer);
  rSizeY = UVoxFormatUtilities::ToUint32(buffer);
  Read4(fileData, index, buffer);
  rSizeZ = UVoxFormatUtilities::ToUint32(buffer);
  result = FVoxObject(rSizeX, rSizeY, rSizeZ);

  
  Read4(fileData, index, buffer); // "XYZI"
  
  Read4(fileData, index, buffer);
  rXyziSize = UVoxFormatUtilities::ToUint32(buffer);
  Read4(fileData, index, buffer);
  Read4(fileData, index, buffer);
  rVoxelCount = UVoxFormatUtilities::ToUint32(buffer);

  for (uint32 i = 0; i < rVoxelCount; i++)
  {
    Read4(fileData, index, buffer);
    uint8 posX, posY, posZ, color;
    
    posX = buffer[0];
    posY = buffer[1];
    posZ = buffer[2];
    color = buffer[3];
    FVoxel parsed = FVoxel(posX, posY, posZ, color - 1);
    result.Voxels.Set(posX, posY, posZ, parsed);
  }

  return result;
}
