#pragma once

class UVoxFormatUtilities
{
public:
  static std::string ToString(FString inString);
  static uint32 ToUint32(TArray<uint8> bytes);

  // Honestly... believe it or not, but this was much faster than trying to figure out how to read a single-width color strip from a .bmp file
  static const TArray<FColor> PALETTE;


  // // I used this JavaScript code to generate the palette array:
  // ------------------------------------------------------------
  // getAverageRGB(document.getElementById('i'));
  //   
  //
  // function getAverageRGB(imgEl) {
  //   
  //   var blockSize = 1,
  //       defaultRGB = {r:0,g:0,b:0}, // for non-supporting envs
  //       canvas = document.createElement('canvas'),
  //       context = canvas.getContext && canvas.getContext('2d'),
  //       data, width, height,
  //       i = -4,
  //       length,
  //       rgb = {r:0,g:0,b:0},
  //       count = 0;
  //       
  //   if (!context) {
  //     return defaultRGB;
  //   }
  //   
  //   height = canvas.height = imgEl.naturalHeight || imgEl.offsetHeight || imgEl.height;
  //   width = canvas.width = imgEl.naturalWidth || imgEl.offsetWidth || imgEl.width;
  //   
  //   context.drawImage(imgEl, 0, 0);
  //   data = context.getImageData(0, 0, width, height);
  //   
  //   length = data.data.length;
  //   
  //   while ( (i += blockSize * 4) < length ) {
  //     ++count;
  //     document.getElementById("output").innerHTML += "FColor(" + data.data[i] + ", " + data.data[i+1] + ", " + data.data[i+2] + "), ";
  //   }
  // }

};
