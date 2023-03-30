#pragma once

UENUM(BlueprintType)
enum ECubeSide
{
  Up = 0, // 1 3 5 7
  Down = 1, // 2 0 6 4
  Left = 2, // 4 0 5 1
  Right = 3, // 2 6 3 7
  Front = 4, // 0 2 1 3
  Back = 5 // 6 4 7 5
};
