#include "VoxImport/UVoxFormatUtilities.h"

std::string UVoxFormatUtilities::ToString(FString inString)
{
  return std::string(TCHAR_TO_UTF8(*inString));
}

uint32 UVoxFormatUtilities::ToUint32(TArray<uint8> bytes)
{
  return ((static_cast<uint32>(bytes[3]) << 24)
    | (static_cast<uint32>(bytes[2]) << 16) 
    | (static_cast<uint32>(bytes[1]) << 8) 
    | (static_cast<uint32>(bytes[0])));
}

const TArray<FColor> UVoxFormatUtilities::PALETTE  = {
    FColor(244, 67, 54), FColor(233, 30, 99), FColor(156, 39, 176), FColor(103, 58, 183), FColor(63, 81, 181),
    FColor(33, 150, 243), FColor(3, 169, 244), FColor(0, 188, 212), FColor(0, 150, 136), FColor(76, 175, 80),
    FColor(139, 195, 74), FColor(205, 220, 57), FColor(255, 235, 59), FColor(255, 193, 7), FColor(255, 152, 0),
    FColor(255, 87, 34), FColor(121, 85, 72), FColor(158, 158, 158), FColor(96, 125, 139), FColor(255, 235, 238),
    FColor(255, 205, 210), FColor(239, 154, 154), FColor(229, 115, 115), FColor(239, 83, 80), FColor(229, 57, 53),
    FColor(211, 47, 47), FColor(198, 40, 40), FColor(183, 28, 28), FColor(255, 138, 128), FColor(255, 82, 82),
    FColor(255, 23, 68), FColor(213, 0, 0), FColor(252, 228, 236), FColor(248, 187, 208), FColor(244, 143, 177),
    FColor(240, 98, 146), FColor(236, 64, 122), FColor(216, 27, 96), FColor(194, 24, 91), FColor(173, 20, 87),
    FColor(136, 14, 79), FColor(255, 128, 171), FColor(255, 64, 129), FColor(245, 0, 87), FColor(197, 17, 98),
    FColor(243, 229, 245), FColor(225, 190, 231), FColor(206, 147, 216), FColor(186, 104, 200), FColor(171, 71, 188),
    FColor(142, 36, 170), FColor(123, 31, 162), FColor(106, 27, 154), FColor(74, 20, 140), FColor(234, 128, 252),
    FColor(224, 64, 251), FColor(213, 0, 249), FColor(170, 0, 255), FColor(237, 231, 246), FColor(209, 196, 233),
    FColor(179, 157, 219), FColor(149, 117, 205), FColor(126, 87, 194), FColor(94, 53, 177), FColor(81, 45, 168),
    FColor(69, 39, 160), FColor(49, 27, 146), FColor(179, 136, 255), FColor(124, 77, 255), FColor(101, 31, 255),
    FColor(98, 0, 234), FColor(232, 234, 246), FColor(197, 202, 233), FColor(159, 168, 218), FColor(121, 134, 203),
    FColor(92, 107, 192), FColor(57, 73, 171), FColor(48, 63, 159), FColor(40, 53, 147), FColor(26, 35, 126),
    FColor(140, 158, 255), FColor(83, 109, 254), FColor(61, 90, 254), FColor(48, 79, 254), FColor(227, 242, 253),
    FColor(187, 222, 251), FColor(144, 202, 249), FColor(100, 181, 246), FColor(66, 165, 245), FColor(30, 136, 229),
    FColor(25, 118, 210), FColor(21, 101, 192), FColor(13, 71, 161), FColor(130, 177, 255), FColor(68, 138, 255),
    FColor(41, 121, 255), FColor(41, 98, 255), FColor(225, 245, 254), FColor(179, 229, 252), FColor(129, 212, 250),
    FColor(79, 195, 247), FColor(41, 182, 246), FColor(3, 155, 229), FColor(2, 136, 209), FColor(2, 119, 189),
    FColor(1, 87, 155), FColor(128, 216, 255), FColor(64, 196, 255), FColor(0, 176, 255), FColor(0, 145, 234),
    FColor(224, 247, 250), FColor(178, 235, 242), FColor(128, 222, 234), FColor(77, 208, 225), FColor(38, 198, 218),
    FColor(0, 172, 193), FColor(0, 151, 167), FColor(0, 131, 143), FColor(0, 96, 100), FColor(132, 255, 255),
    FColor(24, 255, 255), FColor(0, 229, 255), FColor(0, 184, 212), FColor(224, 242, 241), FColor(178, 223, 219),
    FColor(128, 203, 196), FColor(77, 182, 172), FColor(38, 166, 154), FColor(0, 137, 123), FColor(0, 121, 107),
    FColor(0, 105, 92), FColor(0, 77, 64), FColor(167, 255, 235), FColor(100, 255, 218), FColor(29, 233, 182),
    FColor(0, 191, 165), FColor(232, 245, 233), FColor(200, 230, 201), FColor(165, 214, 167), FColor(129, 199, 132),
    FColor(102, 187, 106), FColor(67, 160, 71), FColor(56, 142, 60), FColor(46, 125, 50), FColor(27, 94, 32),
    FColor(185, 246, 202), FColor(105, 240, 174), FColor(0, 230, 118), FColor(0, 200, 83), FColor(241, 248, 233),
    FColor(220, 237, 200), FColor(197, 225, 165), FColor(174, 213, 129), FColor(156, 204, 101), FColor(124, 179, 66),
    FColor(104, 159, 56), FColor(85, 139, 47), FColor(51, 105, 30), FColor(204, 255, 144), FColor(178, 255, 89),
    FColor(118, 255, 3), FColor(100, 221, 23), FColor(249, 251, 231), FColor(240, 244, 195), FColor(230, 238, 156),
    FColor(220, 231, 117), FColor(212, 225, 87), FColor(192, 202, 51), FColor(175, 180, 43), FColor(158, 157, 36),
    FColor(130, 119, 23), FColor(244, 255, 129), FColor(238, 255, 65), FColor(198, 255, 0), FColor(174, 234, 0),
    FColor(255, 253, 231), FColor(255, 249, 196), FColor(255, 245, 157), FColor(255, 241, 118), FColor(255, 238, 88),
    FColor(253, 216, 53), FColor(251, 192, 45), FColor(249, 168, 37), FColor(245, 127, 23), FColor(255, 255, 141),
    FColor(255, 255, 0), FColor(255, 234, 0), FColor(255, 214, 0), FColor(255, 248, 225), FColor(255, 236, 179),
    FColor(255, 224, 130), FColor(255, 213, 79), FColor(255, 202, 40), FColor(255, 179, 0), FColor(255, 160, 0),
    FColor(255, 143, 0), FColor(255, 111, 0), FColor(255, 229, 127), FColor(255, 215, 64), FColor(255, 196, 0),
    FColor(255, 171, 0), FColor(255, 243, 224), FColor(255, 224, 178), FColor(255, 204, 128), FColor(255, 183, 77),
    FColor(255, 167, 38), FColor(251, 140, 0), FColor(245, 124, 0), FColor(239, 108, 0), FColor(230, 81, 0),
    FColor(255, 209, 128), FColor(255, 171, 64), FColor(255, 145, 0), FColor(255, 109, 0), FColor(251, 233, 231),
    FColor(255, 204, 188), FColor(255, 171, 145), FColor(255, 138, 101), FColor(255, 112, 67), FColor(244, 81, 30),
    FColor(230, 74, 25), FColor(216, 67, 21), FColor(191, 54, 12), FColor(255, 158, 128), FColor(255, 110, 64),
    FColor(255, 61, 0), FColor(221, 44, 0), FColor(239, 235, 233), FColor(215, 204, 200), FColor(188, 170, 164),
    FColor(161, 136, 127), FColor(141, 110, 99), FColor(109, 76, 65), FColor(93, 64, 55), FColor(78, 52, 46),
    FColor(62, 39, 35), FColor(250, 250, 250), FColor(245, 245, 245), FColor(238, 238, 238), FColor(224, 224, 224),
    FColor(189, 189, 189), FColor(117, 117, 117), FColor(97, 97, 97), FColor(66, 66, 66), FColor(33, 33, 33),
    FColor(236, 239, 241), FColor(207, 216, 220), FColor(176, 190, 197), FColor(144, 164, 174), FColor(120, 144, 156),
    FColor(84, 110, 122), FColor(69, 90, 100), FColor(55, 71, 79), FColor(38, 50, 56), FColor(0, 0, 0)
  };