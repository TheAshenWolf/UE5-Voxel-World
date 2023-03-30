#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "UNoise.generated.h"

	USTRUCT()
	struct FPoint {
		GENERATED_BODY()
		UPROPERTY()
			float x;
		UPROPERTY()
			float y;
		UPROPERTY()
			float z;
	};


	USTRUCT()
	struct FTriangle {
		GENERATED_BODY()
		UPROPERTY()
			FPoint point[3];
	};

	USTRUCT()
	struct FCell {
		GENERATED_BODY()
		UPROPERTY()
			FPoint point[8];
		UPROPERTY()
			float val[8];
	};

UCLASS()
class UNoise : public UObject
{
	GENERATED_UCLASS_BODY()
private:
	static unsigned char perm[];
	static float  _grad(int hash, float x);
	static float  _grad(int hash, float x, float y);
	static float  _grad(int hash, float x, float y, float z);
	static float  _grad(int hash, float x, float y, float z, float t);

	static float _simplexNoise1D(float x);
	static float _simplexNoise2D(float x, float y);
	static float _simplexNoise3D(float x, float y, float z);
	static float _simplexNoise4D(float x, float y, float z, float w);
	static int	  _polygonise(FCell cell, float isolevel, FTriangle* triangles);

public:

		static inline int32 Seed = -1;

	UFUNCTION()
	  static int32 GetSeed();
	
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static void setNoiseSeed(const int32 newSeed);
    
    UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static void setNoiseFromStream(FRandomStream& RandStream);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise1D(float x, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise2D(float x, float y, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise3D(float x, float y, float z, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoise4D(float x, float y, float z, float w, float inFactor = 1.f);

	// Scaled by float value
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled1D(float x , float scaleOut = 1.f, float inFactor = 1.f);
	
	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled2D(float x, float y, float scaleOut = 1.f, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled3D(float x, float y, float z, float scaleOut = 1.f, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseScaled4D(float x, float y, float z, float w, float scaleOut = 1.f, float inFactor = 1.f);

	// Return value in Range between two float numbers
	// Return Value is scaled by difference between rangeMin & rangeMax value

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange1D(float x, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange2D(float x, float y, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange3D(float x, float y, float z, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float SimplexNoiseInRange4D(float x, float y, float z, float w, float rangeMin, float rangeMax, float inFactor = 1.f);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float GetSimplexNoise1D_EX(float x, float lacunarity = 2.3f, float persistance = 0.6f, int octaves = 4, float inFactor = 1.0f, bool ZeroToOne = false);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float GetSimplexNoise2D_EX(float x, float y, float lacunarity = 2.3f, float persistance = 0.6f, int octaves = 4, float inFactor = 1.0f, bool ZeroToOne = false);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float GetSimplexNoise3D_EX(float x, float y, float z, float lacunarity = 2.3f, float persistance = 0.6f, int octaves = 4, float inFactor = 1.0f, bool ZeroToOne = false);

	UFUNCTION(BlueprintCallable, Category = "SimplexNoise")
		static float GetSimplexNoise4D_EX(float x, float y, float z, float w, float lacunarity = 2.3f, float persistance = 0.6f, int octaves = 4, float inFactor = 1.0f, bool ZeroToOne = false);

};