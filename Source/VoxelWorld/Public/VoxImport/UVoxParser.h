#pragma once
#include "FVoxObject.h"
#include <iostream>
#include <fstream>

class UVoxParser
{

private:
  /// <summary>
  /// These are constants that are used to identify the VOX file format.
  /// </summary>
  static inline const char* HEADER_ID = "VOX ";
  static inline const int32 VERSION = 150;
  static inline const std::string MAIN_ID = "MAIN";
  static inline const std::string SIZE_ID = "SIZE";
  static inline const std::string XYZI_ID = "XYZI";
  static inline const std::string PALETTE_ID = "RGBA";
  
public:
  /// <summary>
  /// Default constructor
  /// </summary>
  UVoxParser();
  /// <summary>
  /// Reads .vox file
  /// </summary>
  /// <param name="path">Path to the file</param>
  /// <returns>Object with the vox data</returns>
  static FVoxObject FromFile(FString path);
  /// <summary>
  /// 
  /// </summary>
  /// <param name="fileData">Array of bytes</param>
  /// <param name="index"></param>
  /// <param name="buffer"></param>
  static void Read4(TArray<uint8> fileData, int32& index, TArray<uint8>& buffer);
  
};
