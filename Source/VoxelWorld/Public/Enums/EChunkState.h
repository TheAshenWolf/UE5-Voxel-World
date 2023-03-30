#pragma once

UENUM(BlueprintType)
enum class EChunkState : uint8
{
  Empty,
  Generated,
  Rendered,
  Reload
};