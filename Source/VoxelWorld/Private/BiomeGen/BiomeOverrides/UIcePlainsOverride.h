#pragma once

class UIcePlainsOverride
{
public:
  static const inline TArray<EBasicBlock> REPLACE_BLOCKS = TArray{
    EBasicBlock::Snow,
    EBasicBlock::Dirt,
    EBasicBlock::Dirt
  };
};
