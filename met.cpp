//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdlib.h>
#include "met.h"
#include "metmap.h"
#include "img123.h"
#include "metitem.h"
#include "metpal.h"
#include "metpass.h"
#include "metabout.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditForm *EditForm;

//---------------------------------------------------------------------------

unsigned char frametable0[16] = { 0, 0, 0x03, 0x1F, 0x25, 0x3E, 0x63, 0x66, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned char frametable1[16] = { 0x18, 0, 0x1D, 0, 0, 0, 0x45, 0x63, 0, 0, 0, 0x15, 0x24, 0x30, 0x42, 0 };
unsigned char frametable2[16] = { 0x03, 0x04, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned char frametable3[16] = { 0x03, 0x03, 0, 0x1F, 0x25, 0x3E, 0, 0x66, 0x91, 0, 0, 0, 0, 0, 0, 0 };
unsigned char frametable4[16] = { 0x18, 0, 0x31, 0x31, 0, 0, 0x45, 0x63, 0, 0x03, 0, 0, 0x23, 0, 0, 0 };

char AreaStrings[5][32] = { "Brinstar",
                        "Norfair",
                        "Tourian",
                        "Kraid's Hideout",
                        "Ridley's Hideout",
                        };

//---------------------------------------------------------------------------

extern void DrawMap(unsigned char MapX, unsigned char MapY);

unsigned char hardtype;
unsigned int ScreenWidth = 256;
unsigned int ScreenHeight = 240;
unsigned int OBJPOS = 0;
unsigned int CurrentArea = 0;
unsigned int filesize;
void *filehandle;
unsigned char *fileptr = NULL;
char filename[256];
char romdir[256];
char mapfilename[256];
unsigned long NumBytes;
unsigned char MapPosX;
unsigned char MapPosY;
unsigned char NameTable[32*30];
unsigned char AttribTable[8*8];
void *TilePatPointer;
Image123 MetroidImage;
MetroidArea Areas[5];
unsigned char Palette[32*4];
unsigned char *CurrentObject = NULL;
unsigned int DeltaX = 0;
unsigned int DeltaY = 0;
unsigned char defpalnum;

//---------------------------------------------------------------------------

// Following is a NES R,G,B palette by Loopy.

unsigned char NESPalette[] = {
    117, 117, 117, 0,
     39,  27, 143, 0,
      0,   0, 171, 0,
     71,   0, 159, 0,
    143,   0, 119, 0,
    171,   0,  19, 0,
    167,   0,   0, 0,
    127,  11,   0, 0,
     67,  47,   0, 0,
      0,  71,   0, 0,
      0,  81,   0, 0,
      0,  63,  23, 0,
     27,  63,  95, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
    188, 188, 188, 0,
      0, 115, 239, 0,
     35,  59, 239, 0,
    131,   0, 243, 0,
    191,   0, 191, 0,
    231,   0,  91, 0,
    219,  43,   0, 0,
    203,  79,  15, 0,
    139, 115,   0, 0,
      0, 151,   0, 0,
      0, 171,   0, 0,
      0, 147,  59, 0,
      0, 131, 139, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
    255, 255, 255, 0,
     63, 191, 255, 0,
     95, 151, 255, 0,
    167, 139, 253, 0,
    247, 123, 255, 0,
    255, 119, 183, 0,
    255, 119,  99, 0,
    255, 155,  59, 0,
    243, 191,  63, 0,
    131, 211,  19, 0,
     79, 223,  75, 0,
     88, 248, 152, 0,
      0, 235, 219, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
    255, 255, 255, 0,
    171, 231, 255, 0,
    199, 215, 255, 0,
    215, 203, 255, 0,
    255, 199, 255, 0,
    255, 199, 219, 0,
    255, 191, 179, 0,
    255, 219, 171, 0,
    255, 231, 163, 0,
    227, 255, 163, 0,
    171, 243, 191, 0,
    179, 255, 207, 0,
    159, 255, 243, 0,
      0,   0,   0, 0,
      0,   0,   0, 0,
      0,   0,   0, 0
    };

//---------------------------------------------------------------------------

// Following are file offsets for each 8x8 tile of
// each area. The 256 first entries are the background
// tile offsets, the next 256 are sprite offsets.
// This sure was fun figuring out.....

unsigned int BrinGFX[512] = {
        0x19DB0,0x19DC0,0x19DD0,0x19DE0,0x19DF0,0x19E00,0x19E10,0x19E20,
        0x19E30,0x19E40,0x19E50,0x19E60,0x19E70,0x19E80,0x19E90,0x19EA0,
        0x19EB0,0x19EC0,0x19ED0,0x19EE0,0x19EF0,0x1B620,0x1B630,0x1B640,
        0x1B650,0x1B660,0x1B670,0x1B680,0x1B690,0x1B6A0,0x1B6B0,0x1B6C0,
        0x14D70,0x14D80,0x14D90,0x14DA0,0x14DB0,0x14DC0,0x14DD0,0x14DE0,
        0x14DF0,0x14E00,0x14E10,0x14E20,0x14E30,0x14E40,0x14E50,0x14E60,
        0x14E70,0x14E80,0x14E90,0x14EA0,0x14EB0,0x14EC0,0x14ED0,0x14EE0,
        0x14EF0,0x14F00,0x14F10,0x14F20,0x14F30,0x14F40,0x14F50,0x14F60,
        0x14F70,0x14F80,0x14F90,0x14FA0,0x14FB0,0x14FC0,0x14FD0,0x14FE0,
        0x14FF0,0x15000,0x15010,0x15020,0x15030,0x15040,0x15050,0x15060,
        0x15070,0x15080,0x15090,0x150A0,0x150B0,0x150C0,0x150D0,0x150E0,
        0x150F0,0x15100,0x15110,0x15120,0x15130,0x15140,0x15150,0x15160,
        0x15170,0x15180,0x15190,0x151A0,0x151B0,0x18E40,0x18E50,0x18E60,
        0x18E70,0x18E80,0x18E90,0x18EA0,0x18EB0,0x18EC0,0x18ED0,0x18EE0,
        0x18EF0,0x18F00,0x18F10,0x18F20,0x18F30,0x18F40,0x18F50,0x18F60,
        0x18F70,0x18F80,0x18F90,0x18FA0,0x18FB0,0x18FC0,0x18FD0,0x18FE0,

        0x19F00,0x19F10,0x19F20,0x19F30,0x19F40,0x19F50,0x19F60,0x19F70,
        0x19F80,0x19F90,0x19FA0,0x19FB0,0x19FC0,0x19FD0,0x19FE0,0x19FF0,
        0x1A000,0x1A010,0x1A020,0x1A030,0x1A040,0x1A050,0x1A060,0x1A070,
        0x1A080,0x1A090,0x1A0A0,0x1A0B0,0x1A0C0,0x1A0D0,0x1A0E0,0x1A0F0,
        0x1A100,0x1A110,0x1A120,0x1A130,0x1A140,0x1A150,0x1A160,0x1A170,
        0x1A180,0x1A190,0x1A1A0,0x1A1B0,0x1A1C0,0x1A1D0,0x1A1E0,0x1A1F0,
        0x1A200,0x1A210,0x1A220,0x1A230,0x1A240,0x1A250,0x1A260,0x1A270,
        0x1A280,0x1A290,0x1A2A0,0x1A2B0,0x1A2C0,0x1A2D0,0x1A2E0,0x1A2F0,
        0x1A300,0x1A310,0x1A320,0x1A330,0x1A340,0x1A350,0x1A360,0x1A370,
        0x1A380,0x1A390,0x1A3A0,0x1A3B0,0x1A3C0,0x1A3D0,0x1A3E0,0x1A3F0,
        0x1A400,0x1A410,0x1A420,0x1A430,0x1A440,0x1A450,0x1A460,0x1A470,
        0x1A480,0x1A490,0x1A4A0,0x1A4B0,0x1A4C0,0x1A4D0,0x1A4E0,0x1A4F0,
        0x1A500,0x1A510,0x1A520,0x1A530,0x1A540,0x1A550,0x1A560,0x1A570,
        0x1A580,0x1A590,0x1A5A0,0x1A5B0,0x1A5C0,0x1A5D0,0x1A5E0,0x1A5F0,
        0x1A600,0x1A610,0x1A620,0x1A630,0x1A640,0x1A650,0x1A660,0x1A670,
        0x1A680,0x1A690,0x1A6A0,0x1A6B0,0x19990,0x199A0,0x199B0,0x199C0,

        0x18010,0x18020,0x18030,0x18040,0x18050,0x18060,0x18070,0x18080,
        0x18090,0x180A0,0x180B0,0x180C0,0x180D0,0x180E0,0x180F0,0x18100,
        0x18110,0x18120,0x18130,0x18140,0x18150,0x18160,0x18170,0x18180,
        0x18190,0x181A0,0x181B0,0x181C0,0x181D0,0x181E0,0x181F0,0x18200,
        0x18210,0x18220,0x18230,0x18240,0x18250,0x18260,0x18270,0x18280,
        0x18290,0x182A0,0x182B0,0x182C0,0x182D0,0x182E0,0x182F0,0x18300,
        0x18310,0x18320,0x18330,0x18340,0x18350,0x18360,0x18370,0x18380,
        0x18390,0x183A0,0x183B0,0x183C0,0x183D0,0x183E0,0x183F0,0x18400,
        0x18410,0x18420,0x18430,0x18440,0x18450,0x18460,0x18470,0x18480,
        0x18490,0x184A0,0x184B0,0x184C0,0x184D0,0x184E0,0x184F0,0x18500,
        0x18510,0x18520,0x18530,0x18540,0x18550,0x18560,0x18570,0x18580,
        0x18590,0x185A0,0x185B0,0x185C0,0x185D0,0x185E0,0x185F0,0x18600,
        0x18610,0x18620,0x18630,0x18640,0x18650,0x18660,0x18670,0x18680,
        0x18690,0x186A0,0x186B0,0x186C0,0x186D0,0x186E0,0x186F0,0x18700,
        0x18710,0x18720,0x18730,0x18740,0x18750,0x18760,0x18770,0x18780,
        0x18790,0x187A0,0x187B0,0x187C0,0x187D0,0x187E0,0x187F0,0x18800,

        0x18810,0x18820,0x18830,0x18840,0x18850,0x18860,0x18870,0x18880,
        0x18890,0x188A0,0x188B0,0x188C0,0x188D0,0x188E0,0x188F0,0x18900,
        0x18910,0x18920,0x18930,0x18940,0x18950,0x18960,0x18970,0x18980,
        0x18990,0x189A0,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x1B4D0,0x1B4E0,0x1B4F0,0x1B500,0x1B510,0x1B520,0x1B530,0x1B540,
        0x1B550,0x1B560,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x05170,0x05180,0x05190,0x051A0,0x051B0,0x051C0,0x051D0,0x051E0,
        0x051F0,0x05200,0x05210,0x05220,0x05230,0x05240,0x05250,0x05260,
        0x05270,0x05280,0x05290,0x052A0,0x052B0,0x052C0,0x052D0,0x052E0,
        0x052F0,0x05300,0x05310,0x05320,0x05330,0x05340,0x05350,0x05360,
        0x05370,0x05380,0x05390,0x053A0,0x053B0,0x053C0,0x053D0,0x053E0,
        0x053F0,0x05400,0x05410,0x05420,0x05430,0x05440,0x05450,0x05460,
        0x05470,0x05480,0x05490,0x054A0,0x054B0,0x054C0,0x054D0,0x054E0,
        0x054F0,0x05500,0x05510,0x05520,0x19990,0x199A0,0x199B0,0x199C0
        };

unsigned int KraidGFX[512] = {
        0x1A9D0,0x1A9E0,0x1A9F0,0x1AA00,0x1AA10,0x1AA20,0x1AA30,0x1AA40,
        0x1AA50,0x1AA60,0x1AA70,0x1AA80,0x1AA90,0x1AAA0,0x1AAB0,0x1AAC0,
        0x1AAD0,0x1AAE0,0x1AAF0,0x1AB00,0x1AB10,0x1AB20,0x1AB30,0x1AB40,
        0x1AB50,0x1AB60,0x1AB70,0x1AB80,0x1AB90,0x1ABA0,0x1ABB0,0x1ABC0,
        0x1ABD0,0x1ABE0,0x1ABF0,0x1AC00,0x1AC10,0x1AC20,0x1AC30,0x1AC40,
        0x1AC50,0x1AC60,0x1AC70,0x1AC80,0x1AC90,0x1ACA0,0x14E50,0x14E60,
        0x14E70,0x14E80,0x14E90,0x14EA0,0x14EB0,0x14EC0,0x14ED0,0x14EE0,
        0x14EF0,0x14F00,0x14F10,0x14F20,0x14F30,0x14F40,0x14F50,0x14F60,
        0x14F70,0x14F80,0x14F90,0x14FA0,0x14FB0,0x14FC0,0x14FD0,0x14FE0,
        0x14FF0,0x15000,0x15010,0x15020,0x15030,0x15040,0x15050,0x15060,
        0x15070,0x15080,0x15090,0x150A0,0x150B0,0x150C0,0x150D0,0x150E0,
        0x150F0,0x15100,0x15110,0x15120,0x15130,0x15140,0x15150,0x15160,
        0x15170,0x15180,0x15190,0x151A0,0x151B0,0x18E40,0x18E50,0x18E60,
        0x18E70,0x18E80,0x18E90,0x18EA0,0x18EB0,0x18EC0,0x18ED0,0x18EE0,
        0x1B340,0x1B350,0x1B360,0x1B370,0x1B380,0x1B390,0x1B3A0,0x1B3B0,
        0x1B3C0,0x1B3D0,0x1B3E0,0x1B3F0,0x18FB0,0x18FC0,0x18FD0,0x18FE0,

        0x19F00,0x19F10,0x19F20,0x19F30,0x19F40,0x19F50,0x19F60,0x19F70,
        0x19F80,0x19F90,0x19FA0,0x19FB0,0x19FC0,0x19FD0,0x19FE0,0x19FF0,
        0x1A000,0x1A010,0x1A020,0x1A030,0x1A040,0x1A050,0x1A060,0x1A070,
        0x1A080,0x1A090,0x1A0A0,0x1A0B0,0x1A0C0,0x1A0D0,0x1A0E0,0x1A0F0,
        0x1A100,0x1A110,0x1A120,0x1A130,0x1A140,0x1A150,0x1A160,0x1A170,
        0x1A180,0x1A190,0x1A1A0,0x1A1B0,0x1A1C0,0x1A1D0,0x1A1E0,0x1A1F0,
        0x1A200,0x1A210,0x1A220,0x1A230,0x1A240,0x1A250,0x1A260,0x1A270,
        0x1A280,0x1A290,0x1A2A0,0x1A2B0,0x1A2C0,0x1A2D0,0x1A2E0,0x1A2F0,
        0x1A300,0x1A310,0x1A320,0x1A330,0x1A340,0x1A350,0x1A360,0x1A370,
        0x1A380,0x1A390,0x1A3A0,0x1A3B0,0x1A3C0,0x1A3D0,0x1A3E0,0x1A3F0,
        0x1A400,0x1A410,0x1A420,0x1A430,0x1A440,0x1A450,0x1A460,0x1A470,
        0x1A480,0x1A490,0x1A4A0,0x1A4B0,0x1A4C0,0x1A4D0,0x1A4E0,0x1A4F0,
        0x1A500,0x1A510,0x1A520,0x1A530,0x1A540,0x1A550,0x1A560,0x1A570,
        0x113F0,0x11400,0x11410,0x11420,0x11430,0x11440,0x11450,0x11460,
        0x11470,0x11480,0x11490,0x114A0,0x114B0,0x114C0,0x114D0,0x114E0,
        0x114F0,0x11500,0x11510,0x11520,0x19990,0x199A0,0x199B0,0x199C0,

        0x18010,0x18020,0x18030,0x18040,0x18050,0x18060,0x18070,0x18080,
        0x18090,0x180A0,0x180B0,0x180C0,0x180D0,0x180E0,0x180F0,0x18100,
        0x18110,0x18120,0x18130,0x18140,0x18150,0x18160,0x18170,0x18180,
        0x18190,0x181A0,0x181B0,0x181C0,0x181D0,0x181E0,0x181F0,0x18200,
        0x18210,0x18220,0x18230,0x18240,0x18250,0x18260,0x18270,0x18280,
        0x18290,0x182A0,0x182B0,0x182C0,0x182D0,0x182E0,0x182F0,0x18300,
        0x18310,0x18320,0x18330,0x18340,0x18350,0x18360,0x18370,0x18380,
        0x18390,0x183A0,0x183B0,0x183C0,0x183D0,0x183E0,0x183F0,0x18400,
        0x18410,0x18420,0x18430,0x18440,0x18450,0x18460,0x18470,0x18480,
        0x18490,0x184A0,0x184B0,0x184C0,0x184D0,0x184E0,0x184F0,0x18500,
        0x18510,0x18520,0x18530,0x18540,0x18550,0x18560,0x18570,0x18580,
        0x18590,0x185A0,0x185B0,0x185C0,0x185D0,0x185E0,0x185F0,0x18600,
        0x18610,0x18620,0x18630,0x18640,0x18650,0x18660,0x18670,0x18680,
        0x18690,0x186A0,0x186B0,0x186C0,0x186D0,0x186E0,0x186F0,0x18700,
        0x18710,0x18720,0x18730,0x18740,0x18750,0x18760,0x18770,0x18780,
        0x18790,0x187A0,0x187B0,0x187C0,0x187D0,0x187E0,0x187F0,0x18800,

        0x18810,0x18820,0x18830,0x18840,0x18850,0x18860,0x18870,0x18880,
        0x18890,0x188A0,0x188B0,0x188C0,0x188D0,0x188E0,0x188F0,0x18900,
        0x18910,0x18920,0x18930,0x18940,0x18950,0x18960,0x18970,0x18980,
        0x18990,0x189A0,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x1B4D0,0x1B4E0,0x1B4F0,0x1B500,0x1B510,0x1B520,0x1B530,0x1B540,
        0x1B550,0x1B560,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x0CD70,0x0CD80,0x0CD90,0x0CDA0,0x0CDB0,0x0CDC0,0x0CDD0,0x0CDE0,
        0x0CDF0,0x0CE00,0x0CE10,0x0CE20,0x0CE30,0x0CE40,0x0CE50,0x0CE60,
        0x0CE70,0x0CE80,0x0CE90,0x0CEA0,0x0CEB0,0x0CEC0,0x0CED0,0x0CEE0,
        0x0CEF0,0x0CF00,0x0CF10,0x0CF20,0x0CF30,0x0CF40,0x0CF50,0x0CF60,
        0x0CF70,0x0CF80,0x0CF90,0x0CFA0,0x0CFB0,0x0CFC0,0x0CFD0,0x0CFE0,
        0x0CFF0,0x0D000,0x0D010,0x0D020,0x0D030,0x0D040,0x0D050,0x0D060,
        0x0D070,0x0D080,0x0D090,0x0D0A0,0x0D0B0,0x0D0C0,0x0D0D0,0x0D0E0,
        0x0D0F0,0x0D100,0x0D110,0x0D120,0x19990,0x199A0,0x199B0,0x199C0
        };

unsigned int RidGFX[512] = {
        0x1A9D0,0x1A9E0,0x1A9F0,0x1AA00,0x1AA10,0x1AA20,0x1AA30,0x1AA40,
        0x1AA50,0x1AA60,0x1AA70,0x1AA80,0x1AA90,0x1AAA0,0x1AAB0,0x1AAC0,
        0x1AAD0,0x1AAE0,0x1AAF0,0x1AB00,0x1AB10,0x1AB20,0x1AB30,0x1AB40,
        0x1AB50,0x1AB60,0x1AB70,0x1AB80,0x1AB90,0x1ABA0,0x1ABB0,0x1ABC0,
        0x1ABD0,0x1ABE0,0x1ABF0,0x1AC00,0x1AC10,0x1AC20,0x1AC30,0x1AC40,
        0x1AC50,0x1AC60,0x1AC70,0x1AC80,0x1AC90,0x1ACA0,0x14E50,0x14E60,
        0x14E70,0x14E80,0x14E90,0x14EA0,0x14EB0,0x14EC0,0x14ED0,0x14EE0,
        0x14EF0,0x14F00,0x14F10,0x14F20,0x14F30,0x14F40,0x14F50,0x14F60,
        0x14F70,0x14F80,0x14F90,0x14FA0,0x14FB0,0x14FC0,0x14FD0,0x14FE0,
        0x14FF0,0x15000,0x15010,0x15020,0x15030,0x15040,0x15050,0x15060,
        0x15070,0x15080,0x15090,0x150A0,0x150B0,0x150C0,0x150D0,0x150E0,
        0x150F0,0x15100,0x15110,0x15120,0x15130,0x15140,0x15150,0x15160,
        0x15170,0x15180,0x15190,0x151A0,0x151B0,0x18E40,0x18E50,0x18E60,
        0x18E70,0x18E80,0x18E90,0x18EA0,0x18EB0,0x18EC0,0x18ED0,0x18EE0,
        0x1B340,0x1B350,0x1B360,0x1B370,0x1B380,0x1B390,0x1B3A0,0x1B3B0,
        0x1B3C0,0x1B3D0,0x1B3E0,0x1B3F0,0x18FB0,0x18FC0,0x18FD0,0x18FE0,

        0x19F00,0x19F10,0x19F20,0x19F30,0x19F40,0x19F50,0x19F60,0x19F70,
        0x19F80,0x19F90,0x19FA0,0x19FB0,0x19FC0,0x19FD0,0x19FE0,0x19FF0,
        0x1A000,0x1A010,0x1A020,0x1A030,0x1A040,0x1A050,0x1A060,0x1A070,
        0x1A080,0x1A090,0x1A0A0,0x1A0B0,0x1A0C0,0x1A0D0,0x1A0E0,0x1A0F0,
        0x1A100,0x1A110,0x1A120,0x1A130,0x1A140,0x1A150,0x1A160,0x1A170,
        0x1A180,0x1A190,0x1A1A0,0x1A1B0,0x1A1C0,0x1A1D0,0x1A1E0,0x1A1F0,
        0x1A200,0x1A210,0x1A220,0x1A230,0x1A240,0x1A250,0x1A260,0x1A270,
        0x1A280,0x1A290,0x1A2A0,0x1A2B0,0x1A2C0,0x1A2D0,0x1A2E0,0x1A2F0,
        0x1A300,0x1A310,0x1A320,0x1A330,0x1A340,0x1A350,0x1A360,0x1A370,
        0x1A380,0x1A390,0x1A3A0,0x1A3B0,0x1A3C0,0x1A3D0,0x1A3E0,0x1A3F0,
        0x1A400,0x1A410,0x1A420,0x1A430,0x1A440,0x1A450,0x1A460,0x1A470,
        0x1A480,0x1A490,0x1A4A0,0x1A4B0,0x1A4C0,0x1A4D0,0x1A4E0,0x1A4F0,
        0x1A500,0x1A510,0x1A520,0x1A530,0x1A540,0x1A550,0x1A560,0x1A570,
        0x113F0,0x11400,0x11410,0x11420,0x11430,0x11440,0x11450,0x11460,
        0x11470,0x11480,0x11490,0x114A0,0x114B0,0x114C0,0x114D0,0x114E0,
        0x114F0,0x11500,0x11510,0x11520,0x19990,0x199A0,0x199B0,0x199C0,

        0x18010,0x18020,0x18030,0x18040,0x18050,0x18060,0x18070,0x18080,
        0x18090,0x180A0,0x180B0,0x180C0,0x180D0,0x180E0,0x180F0,0x18100,
        0x18110,0x18120,0x18130,0x18140,0x18150,0x18160,0x18170,0x18180,
        0x18190,0x181A0,0x181B0,0x181C0,0x181D0,0x181E0,0x181F0,0x18200,
        0x18210,0x18220,0x18230,0x18240,0x18250,0x18260,0x18270,0x18280,
        0x18290,0x182A0,0x182B0,0x182C0,0x182D0,0x182E0,0x182F0,0x18300,
        0x18310,0x18320,0x18330,0x18340,0x18350,0x18360,0x18370,0x18380,
        0x18390,0x183A0,0x183B0,0x183C0,0x183D0,0x183E0,0x183F0,0x18400,
        0x18410,0x18420,0x18430,0x18440,0x18450,0x18460,0x18470,0x18480,
        0x18490,0x184A0,0x184B0,0x184C0,0x184D0,0x184E0,0x184F0,0x18500,
        0x18510,0x18520,0x18530,0x18540,0x18550,0x18560,0x18570,0x18580,
        0x18590,0x185A0,0x185B0,0x185C0,0x185D0,0x185E0,0x185F0,0x18600,
        0x18610,0x18620,0x18630,0x18640,0x18650,0x18660,0x18670,0x18680,
        0x18690,0x186A0,0x186B0,0x186C0,0x186D0,0x186E0,0x186F0,0x18700,
        0x18710,0x18720,0x18730,0x18740,0x18750,0x18760,0x18770,0x18780,
        0x18790,0x187A0,0x187B0,0x187C0,0x187D0,0x187E0,0x187F0,0x18800,

        0x18810,0x18820,0x18830,0x18840,0x18850,0x18860,0x18870,0x18880,
        0x18890,0x188A0,0x188B0,0x188C0,0x188D0,0x188E0,0x188F0,0x18900,
        0x18910,0x18920,0x18930,0x18940,0x18950,0x18960,0x18970,0x18980,
        0x18990,0x189A0,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x1B4D0,0x1B4E0,0x1B4F0,0x1B500,0x1B510,0x1B520,0x1B530,0x1B540,
        0x1B550,0x1B560,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x0D170,0x0D180,0x0D190,0x0D1A0,0x0D1B0,0x0D1C0,0x0D1D0,0x0D1E0,
        0x0D1F0,0x0D200,0x0D210,0x0D220,0x0D230,0x0D240,0x0D250,0x0D260,
        0x0D270,0x0D280,0x0D290,0x0D2A0,0x0D2B0,0x0D2C0,0x0D2D0,0x0D2E0,
        0x0D2F0,0x0D300,0x0D310,0x0D320,0x0D330,0x0D340,0x0D350,0x0D360,
        0x0D370,0x0D380,0x0D390,0x0D3A0,0x0D3B0,0x0D3C0,0x0D3D0,0x0D3E0,
        0x0D3F0,0x0D400,0x0D410,0x0D420,0x0D430,0x0D440,0x0D450,0x0D460,
        0x0D470,0x0D480,0x0D490,0x0D4A0,0x0D4B0,0x0D4C0,0x0D4D0,0x0D4E0,
        0x0D4F0,0x0D500,0x0D510,0x0D520,0x19990,0x199A0,0x199B0,0x199C0
        };

unsigned int NorGFX[512] = {
        0x1A700,0x1A710,0x1A720,0x1A730,0x1A740,0x1A750,0x1A760,0x1A770,
        0x1A780,0x1A790,0x1A7A0,0x1A7B0,0x1A7C0,0x1A7D0,0x1A7E0,0x1A7F0,
        0x1A800,0x1A810,0x1A820,0x1A830,0x1A840,0x1A850,0x1A860,0x1A870,
        0x1A880,0x1A890,0x1A8A0,0x1A8B0,0x1A8C0,0x1A8D0,0x1A8E0,0x1A8F0,
        0x1A900,0x1A910,0x1A920,0x1A930,0x1A940,0x1A950,0x14DD0,0x14DE0,
        0x14DF0,0x14E00,0x14E10,0x14E20,0x14E30,0x14E40,0x14E50,0x14E60,
        0x14E70,0x14E80,0x14E90,0x14EA0,0x14EB0,0x14EC0,0x14ED0,0x14EE0,
        0x14EF0,0x14F00,0x14F10,0x14F20,0x14F30,0x14F40,0x14F50,0x14F60,
        0x14F70,0x14F80,0x14F90,0x14FA0,0x14FB0,0x14FC0,0x14FD0,0x14FE0,
        0x14FF0,0x15000,0x15010,0x15020,0x15030,0x15040,0x15050,0x15060,
        0x15070,0x15080,0x15090,0x150A0,0x150B0,0x150C0,0x150D0,0x150E0,
        0x150F0,0x15100,0x15110,0x15120,0x15130,0x15140,0x15150,0x15160,
        0x15170,0x15180,0x15190,0x151A0,0x151B0,0x18E40,0x18E50,0x18E60,
        0x18E70,0x18E80,0x18E90,0x18EA0,0x18EB0,0x18EC0,0x18ED0,0x18EE0,
        0x1A960,0x1A970,0x1A980,0x1A990,0x1A9A0,0x1A9B0,0x1A9C0,0x18F60,
        0x18F70,0x18F80,0x18F90,0x18FA0,0x18FB0,0x18FC0,0x18FD0,0x18FE0,

        0x19F00,0x19F10,0x19F20,0x19F30,0x19F40,0x19F50,0x19F60,0x19F70,
        0x19F80,0x19F90,0x19FA0,0x19FB0,0x19FC0,0x19FD0,0x19FE0,0x19FF0,
        0x1A000,0x1A010,0x1A020,0x1A030,0x1A040,0x1A050,0x1A060,0x1A070,
        0x1A080,0x1A090,0x1A0A0,0x1A0B0,0x1A0C0,0x1A0D0,0x1A0E0,0x1A0F0,
        0x1A100,0x1A110,0x1A120,0x1A130,0x1A140,0x1A150,0x1A160,0x1A170,
        0x1A180,0x1A190,0x1A1A0,0x1A1B0,0x1A1C0,0x1A1D0,0x1A1E0,0x1A1F0,
        0x1A200,0x1A210,0x1A220,0x1A230,0x1A240,0x1A250,0x1A260,0x1A270,
        0x1A280,0x1A290,0x1A2A0,0x1A2B0,0x1A2C0,0x1A2D0,0x1A2E0,0x1A2F0,
        0x1A300,0x1A310,0x1A320,0x1A330,0x1A340,0x1A350,0x1A360,0x1A370,
        0x1A380,0x1A390,0x1A3A0,0x1A3B0,0x1A3C0,0x1A3D0,0x1A3E0,0x1A3F0,
        0x1A400,0x1A410,0x1A420,0x1A430,0x1A440,0x1A450,0x1A460,0x1A470,
        0x1A480,0x1A490,0x1A4A0,0x1A4B0,0x1A4C0,0x1A4D0,0x1A4E0,0x1A4F0,
        0x1A500,0x1A510,0x1A520,0x1A530,0x1A540,0x1A550,0x1A560,0x1A570,
        0x1A580,0x1A590,0x1A5A0,0x1A5B0,0x1A5C0,0x1A5D0,0x1A5E0,0x1A5F0,
        0x1A600,0x1A610,0x1A620,0x1A630,0x1A640,0x1A650,0x1A660,0x1A670,
        0x1A680,0x1A690,0x1A6A0,0x1A6B0,0x19990,0x199A0,0x199B0,0x199C0,

        0x18010,0x18020,0x18030,0x18040,0x18050,0x18060,0x18070,0x18080,
        0x18090,0x180A0,0x180B0,0x180C0,0x180D0,0x180E0,0x180F0,0x18100,
        0x18110,0x18120,0x18130,0x18140,0x18150,0x18160,0x18170,0x18180,
        0x18190,0x181A0,0x181B0,0x181C0,0x181D0,0x181E0,0x181F0,0x18200,
        0x18210,0x18220,0x18230,0x18240,0x18250,0x18260,0x18270,0x18280,
        0x18290,0x182A0,0x182B0,0x182C0,0x182D0,0x182E0,0x182F0,0x18300,
        0x18310,0x18320,0x18330,0x18340,0x18350,0x18360,0x18370,0x18380,
        0x18390,0x183A0,0x183B0,0x183C0,0x183D0,0x183E0,0x183F0,0x18400,
        0x18410,0x18420,0x18430,0x18440,0x18450,0x18460,0x18470,0x18480,
        0x18490,0x184A0,0x184B0,0x184C0,0x184D0,0x184E0,0x184F0,0x18500,
        0x18510,0x18520,0x18530,0x18540,0x18550,0x18560,0x18570,0x18580,
        0x18590,0x185A0,0x185B0,0x185C0,0x185D0,0x185E0,0x185F0,0x18600,
        0x18610,0x18620,0x18630,0x18640,0x18650,0x18660,0x18670,0x18680,
        0x18690,0x186A0,0x186B0,0x186C0,0x186D0,0x186E0,0x186F0,0x18700,
        0x18710,0x18720,0x18730,0x18740,0x18750,0x18760,0x18770,0x18780,
        0x18790,0x187A0,0x187B0,0x187C0,0x187D0,0x187E0,0x187F0,0x18800,

        0x18810,0x18820,0x18830,0x18840,0x18850,0x18860,0x18870,0x18880,
        0x18890,0x188A0,0x188B0,0x188C0,0x188D0,0x188E0,0x188F0,0x18900,
        0x18910,0x18920,0x18930,0x18940,0x18950,0x18960,0x18970,0x18980,
        0x18990,0x189A0,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x1B4D0,0x1B4E0,0x1B4F0,0x1B500,0x1B510,0x1B520,0x1B530,0x1B540,
        0x1B550,0x1B560,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x08D70,0x08D80,0x08D90,0x08DA0,0x08DB0,0x08DC0,0x08DD0,0x08DE0,
        0x08DF0,0x08E00,0x08E10,0x08E20,0x08E30,0x08E40,0x08E50,0x08E60,
        0x08E70,0x08E80,0x08E90,0x08EA0,0x08EB0,0x08EC0,0x08ED0,0x08EE0,
        0x08EF0,0x08F00,0x08F10,0x08F20,0x08F30,0x08F40,0x08F50,0x08F60,
        0x08F70,0x08F80,0x08F90,0x08FA0,0x08FB0,0x08FC0,0x08FD0,0x08FE0,
        0x08FF0,0x09000,0x09010,0x09020,0x09030,0x09040,0x09050,0x09060,
        0x09070,0x09080,0x09090,0x090A0,0x090B0,0x090C0,0x090D0,0x090E0,
        0x090F0,0x09100,0x09110,0x09120,0x19990,0x199A0,0x199B0,0x199C0
        };

unsigned int TourGFX[512] = {
        0x1A9D0,0x1A9E0,0x1A9F0,0x1AA00,0x1AA10,0x1AA20,0x1AA30,0x1AA40,
        0x1AA50,0x1AA60,0x1AA70,0x1AA80,0x1AA90,0x1AAA0,0x1AAB0,0x1AAC0,
        0x1AAD0,0x1AAE0,0x1AAF0,0x1AB00,0x1AB10,0x1AB20,0x1AB30,0x1AB40,
        0x1AB50,0x1AB60,0x1AB70,0x1AB80,0x1AB90,0x1ABA0,0x1ABB0,0x1ABC0,
        0x1ACB0,0x1ACC0,0x1ACD0,0x1ACE0,0x1ACF0,0x1AD00,0x1AD10,0x1AD20,
        0x1AD30,0x1AD40,0x1AD50,0x1AD60,0x1AD70,0x1AD80,0x1AD90,0x1ADA0,
        0x1ADB0,0x1ADC0,0x1ADD0,0x1ADE0,0x1ADF0,0x1AE00,0x1AE10,0x1AE20,
        0x1AE30,0x1AE40,0x1AE50,0x1AE60,0x1AE70,0x1AE80,0x1AE90,0x1AEA0,
        0x1AEB0,0x1AEC0,0x1AED0,0x1AEE0,0x1AEF0,0x1AF00,0x1AF10,0x1AF20,
        0x1AF30,0x1AF40,0x1AF50,0x1AF60,0x1AF70,0x1AF80,0x1AF90,0x1AFA0,
        0x1AFB0,0x1AFC0,0x1AFD0,0x1AFE0,0x1AFF0,0x1B000,0x1B010,0x1B020,
        0x1B030,0x1B040,0x1B050,0x1B060,0x1B070,0x1B080,0x1B090,0x1B0A0,
        0x1B0B0,0x1B0C0,0x1B0D0,0x1B0E0,0x1B0F0,0x1B100,0x1B110,0x1B120,
        0x1B130,0x1B140,0x1B150,0x1B160,0x1B170,0x1B180,0x1B190,0x1B1A0,
        0x1B1B0,0x1B1C0,0x1B1D0,0x1B1E0,0x1B1F0,0x1B200,0x1B210,0x1B220,
        0x1B230,0x1B240,0x1B250,0x1B260,0x1B270,0x1B280,0x1B290,0x1B2A0,

        0x19F00,0x19F10,0x19F20,0x19F30,0x19F40,0x19F50,0x19F60,0x19F70,
        0x19F80,0x19F90,0x19FA0,0x19FB0,0x19FC0,0x19FD0,0x19FE0,0x19FF0,
        0x1B2B0,0x1B2C0,0x1B2D0,0x1B2E0,0x1B2F0,0x1B300,0x1B310,0x1B320,
        0x1B330,0x1A090,0x1A0A0,0x1A0B0,0x1A0C0,0x1A0D0,0x1A0E0,0x1A0F0,
        0x1A100,0x1A110,0x1A120,0x1A130,0x1A140,0x1A150,0x1A160,0x1A170,
        0x1A180,0x1A190,0x1A1A0,0x1A1B0,0x1A1C0,0x1A1D0,0x1A1E0,0x1A1F0,
        0x1A200,0x1A210,0x1A220,0x1A230,0x1A240,0x1A250,0x1A260,0x1A270,
        0x1A280,0x1A290,0x1A2A0,0x1A2B0,0x1A2C0,0x1A2D0,0x1A2E0,0x1A2F0,
        0x1A300,0x1A310,0x1A320,0x1A330,0x1A340,0x1A350,0x1A360,0x1A370,
        0x1A380,0x1A390,0x1A3A0,0x1A3B0,0x1A3C0,0x1A3D0,0x1A3E0,0x1A3F0,
        0x1A400,0x1A410,0x1A420,0x1A430,0x1A440,0x1A450,0x1A460,0x1A470,
        0x1A480,0x1A490,0x1A4A0,0x1A4B0,0x1A4C0,0x1A4D0,0x1A4E0,0x1A4F0,
        0x1A500,0x1A510,0x1A520,0x1A530,0x1A540,0x1A550,0x1A560,0x1A570,
        0x1A580,0x1A590,0x1A5A0,0x1A5B0,0x1A5C0,0x1A5D0,0x1A5E0,0x1A5F0,
        0x1A600,0x1A610,0x1A620,0x1A630,0x1A640,0x1A650,0x1A660,0x1A670,
        0x1A680,0x1A690,0x1A6A0,0x1A6B0,0x19990,0x199A0,0x199B0,0x199C0,

        0x18010,0x18020,0x18030,0x18040,0x18050,0x18060,0x18070,0x18080,
        0x18090,0x180A0,0x180B0,0x180C0,0x180D0,0x180E0,0x180F0,0x18100,
        0x18110,0x18120,0x18130,0x18140,0x18150,0x18160,0x18170,0x18180,
        0x18190,0x181A0,0x181B0,0x181C0,0x181D0,0x181E0,0x181F0,0x18200,
        0x18210,0x18220,0x18230,0x18240,0x18250,0x18260,0x18270,0x18280,
        0x18290,0x182A0,0x182B0,0x182C0,0x182D0,0x182E0,0x182F0,0x18300,
        0x18310,0x18320,0x18330,0x18340,0x18350,0x18360,0x18370,0x18380,
        0x18390,0x183A0,0x183B0,0x183C0,0x183D0,0x183E0,0x183F0,0x18400,
        0x18410,0x18420,0x18430,0x18440,0x18450,0x18460,0x18470,0x18480,
        0x18490,0x184A0,0x184B0,0x184C0,0x184D0,0x184E0,0x184F0,0x18500,
        0x18510,0x18520,0x18530,0x18540,0x18550,0x18560,0x18570,0x18580,
        0x18590,0x185A0,0x185B0,0x185C0,0x185D0,0x185E0,0x185F0,0x18600,
        0x18610,0x18620,0x18630,0x18640,0x18650,0x18660,0x18670,0x18680,
        0x18690,0x186A0,0x186B0,0x186C0,0x186D0,0x186E0,0x186F0,0x18700,
        0x18710,0x18720,0x18730,0x18740,0x18750,0x18760,0x18770,0x18780,
        0x18790,0x187A0,0x187B0,0x187C0,0x187D0,0x187E0,0x187F0,0x18800,

        0x18810,0x18820,0x18830,0x18840,0x18850,0x18860,0x18870,0x18880,
        0x18890,0x188A0,0x188B0,0x188C0,0x188D0,0x188E0,0x188F0,0x18900,
        0x18910,0x18920,0x18930,0x18940,0x18950,0x18960,0x18970,0x18980,
        0x18990,0x189A0,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x1B4D0,0x1B4E0,0x1B4F0,0x1B500,0x1B510,0x1B520,0x1B530,0x1B540,
        0x1B550,0x1B560,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,0x00000,
        0x09170,0x09180,0x09190,0x091A0,0x091B0,0x091C0,0x091D0,0x091E0,
        0x091F0,0x09200,0x09210,0x09220,0x09230,0x09240,0x09250,0x09260,
        0x09270,0x09280,0x09290,0x092A0,0x092B0,0x092C0,0x092D0,0x092E0,
        0x092F0,0x09300,0x09310,0x09320,0x09330,0x09340,0x09350,0x09360,
        0x09370,0x09380,0x09390,0x093A0,0x093B0,0x093C0,0x093D0,0x093E0,
        0x093F0,0x09400,0x09410,0x09420,0x09430,0x09440,0x09450,0x09460,
        0x09470,0x09480,0x09490,0x094A0,0x094B0,0x094C0,0x094D0,0x094E0,
        0x094F0,0x09500,0x09510,0x09520,0x19990,0x199A0,0x199B0,0x199C0
        };

//---------------------------------------------------------------------------

// Following are the X,Y dimensions of each object
// type of each area. These are used when handling mouse
// presses, to check if an object in the room has
// been selected.

unsigned char BrinCoords[] = {
128,32,
128,80,
32,128,
16,48,
16,48,
32,16,
16,80,
16,96,
48,48,
64,16,
48,16,
32,80,
32,16,
128,16,
128,64,
128,16,
128,32,
128,48,
96,32,
128,16,
128,16,
16,80,
112,32,
128,64,
16,16,
64,16,
32,64,
128,32,
128,16,
16,32,
64,64,
64,64,
64,16,
16,16,
48,80,
48,64,
64,64,
16,64,
32,16,
128,32,
16,80,
16,16,
64,32,
128,16,
16,16,
128,32,
16,16,
16,16,
16,64,
16,16
};

unsigned char NorCoords[] = {
128,32,
128,80,
32,128,
16,48,
16,48,
32,16,
64,16,
32,80,
16,32,
64,16,
64,32,
64,64,
16,64,
128,16,
64,32,
48,48,
16,16,
128,16,
16,16,
64,80,
64,64,
64,16,
64,16,
16,64,
16,64,
128,32,
64,64,
64,64,
16,128,
64,16,
16,16,
16,64,
64,32,
64,16,
64,64,
32,16,
64,32,
16,64,
64,64,
16,16,
16,64,
16,16,
16,16,
64,16,
32,64,
128,16,
64,64,
128,32,
128,32
};

unsigned char KraidCoords[] = {
128,32,
128,80,
32,128,
16,48,
16,48,
32,16,
32,80,
32,80,
16,32,
16,32,
16,16,
16,16,
16,80,
64,16,
48,16,
16,64,
64,64,
16,16,
128,32,
64,64,
32,16,
128,32,
32,64,
128,32,
128,32,
64,64,
128,16,
16,16,
16,48,
128,48,
16,64,
64,64,
16,16,
16,64,
48,16,
48,16,
128,16,
16,64,
64,32
};

unsigned char RidCoords[] = {
128,32,
16,48,
16,48,
32,128,
32,16,
16,32,
16,32,
32,80,
64,64,
64,16,
16,80,
64,80,
32,64,
16,64,
64,32,
64,32,
16,16,
64,16,
128,16,
64,64,
128,48,
64,32,
16,48,
64,64,
128,48,
16,48,
16,64,
16,16,
64,16
};

unsigned char TourCoords[] = {
128,32,
128,80,
32,128,
16,48,
16,48,
16,16,
48,64,
80,64,
48,64,
16,128,
128,16,
112,80,
128,96,
32,32,
16,112,
16,16,
112,16,
80,80,
48,16,
16,32,
128,80,
128,32,
128,48,
64,64,
112,64,
16,112,
16,80,
32,16,
16,16,
64,32,
128,16,
128,32
};

//---------------------------------------------------------------------------

// Following are the area values for each position on
// the 32x32 map grid. These are used to determine what
// game area each map position is part of.

unsigned char MapIndex[] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,
    0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,
    0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,3,0,0,3,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,1,0,
    0,0,0,0,0,0,0,0,3,3,3,3,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,3,3,3,3,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,0,0,0,0,0,0,0,3,3,3,3,0,1,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,0,
    0,3,0,0,0,0,0,3,3,0,0,3,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,
    0,3,3,3,3,3,3,3,3,3,3,3,0,1,0,0,0,1,1,1,1,1,0,0,1,1,1,1,1,1,1,0,
    0,3,3,3,3,3,3,3,3,3,3,3,0,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,0,0,1,0,
    0,3,0,0,3,0,0,3,0,0,0,0,3,0,0,0,0,1,0,0,0,0,0,4,1,1,0,0,0,0,1,0,
    0,3,0,0,3,3,3,3,3,3,3,3,3,0,4,0,0,4,4,4,4,4,4,4,4,4,0,0,0,0,4,0,
    0,3,0,0,3,0,0,3,0,3,3,3,3,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,
    0,3,0,0,3,3,3,3,3,3,0,3,3,0,4,0,0,4,0,0,0,0,4,0,0,0,0,0,0,0,4,0,
    0,3,3,3,3,3,3,3,3,3,3,3,3,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,
    0,3,0,3,3,3,3,3,3,3,3,3,3,0,4,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,4,0,
    0,0,0,0,0,0,0,0,3,3,3,3,3,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,
    0,0,0,0,0,0,0,0,0,0,0,0,3,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

//---------------------------------------------------------------------------

unsigned char *GetRoomPointer(unsigned int roomnum, unsigned int areanum);
unsigned char *GetStructPointer(unsigned int structnum, unsigned int areanum);
unsigned char *GetSpritePosPointer(unsigned int posnum, unsigned int areanum);
unsigned char *GetFramePointer(unsigned int framenum, unsigned int areanum);
unsigned int GetRoomSize(unsigned char *roomdata);
void NESPalToRGB(unsigned char *p1, unsigned char *p2);
void SetArea(unsigned int areanum);
void DrawRoom(unsigned char MapX, unsigned char MapY);
void DrawStruct(unsigned char *objptr, unsigned int areanum);
void DrawEnemy(unsigned char *objptr);
void DrawSprite(unsigned int tilenum, unsigned int tileflip, unsigned int palnum, unsigned int areanum, unsigned int xpos, unsigned int ypos);
void DrawFrame(unsigned int framenum, unsigned int areanum, unsigned int xpos, unsigned int ypos);

//---------------------------------------------------------------------------

void DrawFrame(unsigned int framenum, unsigned int areanum, unsigned int xpos, unsigned int ypos)
{
unsigned char *frame2data;
unsigned char *sprpos2data;
unsigned int framepos = 0;
unsigned int sprpos = 0;
unsigned int xpostemp;
unsigned int ypostemp;
unsigned int palnum;
unsigned int tileflip;
bool done = false;

frame2data = GetFramePointer(framenum, areanum);
sprpos2data = GetSpritePosPointer(frame2data[0] & 0x0F, areanum);

palnum = ((frame2data[0] >> 4) | hardtype) & 0x03;
tileflip = frame2data[0] >> 6;
ypos += frame2data[1];
xpos += frame2data[2];
framepos = 3;

while (done == false)
    {
    switch (frame2data[framepos])
        {
        case 0xFC:
        ypos = (ypos + frame2data[framepos+1]) & 0xFF;
        xpos = (xpos + frame2data[framepos+2]) & 0xFF;
        framepos += 3;
        break;

        case 0xFD:
        palnum = (frame2data[framepos+1] | hardtype) & 0x03;
        tileflip = frame2data[framepos+1] >> 6;
        framepos += 2;
        break;

        case 0xFE:
        sprpos += 2;
        framepos++;
        break;

        case 0xFF:
        done = true;
        break;

        default:
        ypostemp = (ypos + sprpos2data[sprpos]) & 0xFF;
        xpostemp = (xpos + sprpos2data[sprpos+1]) & 0xFF;
        sprpos += 2;
        DrawSprite(frame2data[framepos++], tileflip, palnum, CurrentArea, xpostemp, ypostemp);
        break;
        }
    }
};

//---------------------------------------------------------------------------

void DrawEnemy(unsigned char *objptr)
{
unsigned char enemynum;

enemynum = (unsigned char)(objptr[1] & 0x0F);
hardtype = (unsigned char)(objptr[1] >> 7);
DrawFrame(Areas[CurrentArea].frametable[enemynum], CurrentArea, (objptr[2] & 0x0F) << 4, objptr[2] & 0xF0);
};

//---------------------------------------------------------------------------

unsigned char *GetStructPointer(unsigned int structnum, unsigned int areanum)
{
unsigned int structptrofs;
void *p;
unsigned char *structdata;

structptrofs = Areas[areanum].structptrofs - 0x08000 + Areas[areanum].ROMofs;
p = &fileptr[structptrofs + (structnum << 1)];
structdata = *(unsigned short *)p - 0x08000 + Areas[areanum].ROMofs + fileptr;
return structdata;
};

unsigned char *GetRoomPointer(unsigned int roomnum, unsigned int areanum)
{
unsigned int roomptrofs;
void *p;
unsigned char *roomdata;

roomptrofs = Areas[areanum].roomptrofs - 0x08000 + Areas[areanum].ROMofs;
p = &fileptr[roomptrofs + (roomnum << 1)];
roomdata = *(unsigned short *)p - 0x08000 + Areas[areanum].ROMofs + fileptr;
return roomdata;
};

unsigned char *GetSpritePosPointer(unsigned int posnum, unsigned int areanum)
{
unsigned int sprpos2ptrofs;
void *p;
unsigned char *sprpos2data;

sprpos2ptrofs = Areas[areanum].sprpos2ptrofs - 0x08000 + Areas[areanum].ROMofs;
p = &fileptr[sprpos2ptrofs + (posnum << 1)];
sprpos2data = *(unsigned short *)p - 0x08000 + Areas[areanum].ROMofs + fileptr;
return sprpos2data;
};

unsigned char *GetFramePointer(unsigned int framenum, unsigned int areanum)
{
unsigned int frame2ptrofs;
void *p;
unsigned char *frame2data;

frame2ptrofs = Areas[areanum].frame2ptrofs - 0x08000 + Areas[areanum].ROMofs;
p = &fileptr[frame2ptrofs + (framenum << 1)];
frame2data = *(unsigned short *)p - 0x08000 + Areas[areanum].ROMofs + fileptr;
return frame2data;
};

unsigned int GetRoomSize(unsigned char *roomdata)
{
unsigned int roomptr = 0;
while (roomdata[roomptr++] != 0xFF);
return roomptr;
};

//---------------------------------------------------------------------------

void NESPalToRGB(unsigned char *p1, unsigned char *p2)
{
int color;
unsigned char data;

for (int i=0; i<32; i++)
    {
    color = p2[i];
    data = NESPalette[(color << 2) + 0];
    p1[(i << 2) + 2] = data;
    data = NESPalette[(color << 2) + 1];
    p1[(i << 2) + 1] = data;
    data = NESPalette[(color << 2) + 2];
    p1[(i << 2) + 0] = data;
    p1[(i << 2) + 3] = 0;
    }
};

//---------------------------------------------------------------------------

void SetArea(unsigned int areanum)
{
char buffer[256];
unsigned int palofs;

palofs = Areas[areanum].palofs - 0x08000 + Areas[areanum].ROMofs;
NESPalToRGB(Palette, &fileptr[palofs]);
memcpy(&MetroidImage.bminfo.bmiColors, Palette, 32*4);
wsprintf(buffer, "Metedit - %s",AreaStrings[areanum]);
EditForm->Caption = buffer;
};

//---------------------------------------------------------------------------

// Draw Room -  draws the room at the specified map position
//              to a bitmap

void DrawRoom(unsigned char MapX, unsigned char MapY)
{
unsigned int areanum;       // Area number (0..4)
unsigned int roomnum;       // Room number
unsigned int palnum;
unsigned char *roomdata;    // Pointer to the room data
unsigned char attribdata;
unsigned int roompos = 1;   // Start at 2nd byte of room data

roomnum = fileptr[0x254E + (MapY << 5) + MapX];  // Get room number at map position
areanum = MapIndex[(MapY << 5) + MapX]; // Get area number at map position
SetArea(areanum);   // Update palette
CurrentArea = areanum;
roomdata = GetRoomPointer(roomnum, areanum);    // Get pointer to room data

// Set name table entries to 0xFF (blank tile)

memset(NameTable, 0xFF, 30*32);

// Set attribute table entries to default palette selector

defpalnum = roomdata[0];
attribdata = fileptr[0x1F028 + defpalnum];
memset(AttribTable, attribdata, 8*8);

// Draw room objects on name table

while ((roomdata[roompos] != 0xFD)
        && (roomdata[roompos] != 0xFF))   // Start object loop
    {
    DrawStruct(&roomdata[roompos], areanum);
    roompos += 3;   // Advance to next object
    }

// Render name table

unsigned int bmpos = 0;
unsigned int bmpostemp;
unsigned char *TilePtr;
unsigned char tilenum;
unsigned int TileOfs;
unsigned int TileAdd;
unsigned int BitIndex;

for (int y=0; y<30; y++)
    {
    for (int x=0; x<32; x++)
        {
        attribdata = AttribTable[((y & 0xFC) << 1) + (x >> 2)];
        palnum = (attribdata >> (((y & 2) << 1) + (x & 2))) & 3;
        tilenum = NameTable[(y << 5) + x];
        TileOfs = Areas[areanum].gfxptr[tilenum];
        TilePtr = &fileptr[TileOfs];
        TileAdd = (palnum << 2) + ((palnum << 2) << 8) + ((palnum << 2) << 16) + ((palnum << 2) << 24);
        bmpostemp = bmpos;
        for (int a=0; a<8; a++) // Do eight lines
            {
            BitIndex = TilePtr[a] + (TilePtr[a + 8] << 8);
            ((int *)MetroidImage.bmptr)[bmpostemp] = ((int *)TilePatPointer)[BitIndex << 1] | TileAdd;
            ((int *)MetroidImage.bmptr)[bmpostemp+1] = ((int *)TilePatPointer)[(BitIndex << 1) + 1] | TileAdd;
            bmpostemp += 256/4;
            }
        bmpos += 2;
        }
    bmpos += 512-64;
    }   // End Y loop


// Draw enemies (if any)

if (roomdata[roompos++] != 0xFD)
    return; // FF reached, no enemy data

while (roomdata[roompos] != 0xFF)   // Start enemy loop
    {
    switch (roomdata[roompos])
        {
        case 0x02:  // Door
        palnum = roomdata[roompos+1] & 3;
        switch (roomdata[roompos+1] & 0xF0)
            {
            case 0x0A0: // Right door
            DrawSprite(0x0F, 1, palnum, 0, 232, 80);
            DrawSprite(0x1F, 1, palnum, 0, 232, 88);
            DrawSprite(0x2F, 1, palnum, 0, 232, 96);
            DrawSprite(0x2F, 3, palnum, 0, 232, 104);
            DrawSprite(0x1F, 3, palnum, 0, 232, 112);
            DrawSprite(0x0F, 3, palnum, 0, 232, 120);
            break;

            case 0x0B0: // Left door
            DrawSprite(0x0F, 0, palnum, 0, 16, 80);
            DrawSprite(0x1F, 0, palnum, 0, 16, 88);
            DrawSprite(0x2F, 0, palnum, 0, 16, 96);
            DrawSprite(0x2F, 2, palnum, 0, 16, 104);
            DrawSprite(0x1F, 2, palnum, 0, 16, 112);
            DrawSprite(0x0F, 2, palnum, 0, 16, 120);
            break;
            }
        roompos += 2;
        break;

        case 0x06:  // Ridley & Kraid statues
        roompos++;
        break;

        default:    // Normal enemy
        DrawEnemy(&roomdata[roompos]);
        roompos += 3;
        break;
        }
    }   // End of while loop

};

//---------------------------------------------------------------------------

// Draw Structure -     draws a structure on the nametable according to
//                      position and palette bits

void DrawStruct(unsigned char *objptr, unsigned int areanum)
{
unsigned int posx;
unsigned int posy;
unsigned char *structdata;
unsigned int structpos = 0;
unsigned int xlength;
unsigned char *macroptr;
unsigned int macroofs;
unsigned int macronum;
unsigned char palnum;
unsigned char palselect;
unsigned char andval;
unsigned char attribdata;

posy = (objptr[0] & 0xF0) >> 3;
structdata = GetStructPointer(objptr[1], areanum);  // Get pointer to structure data
palnum = objptr[2];

while (structdata[structpos] != 0xFF)   // Start drawing macros
    {
    posx = (objptr[0] & 0x0F) << 1; // Reset nametable x pos
    posx += (structdata[structpos] & 0xF0) >> 3;
    xlength = structdata[structpos++] & 0x0F;  // Number of macros to draw horizontally
    for (unsigned int i=0; i<xlength; i++)  // Do'em all
        {

// Draw one macro

        macronum = structdata[structpos++];
        if ((posy < 30) && (posx < 32))
            {   // Draw only if inside nametable
            macroofs = Areas[areanum].macroofs - 0x08000 + Areas[areanum].ROMofs;
            macroptr = &fileptr[macroofs + (macronum << 2)]; // Set up pointer to macro data

// Update nametable

            NameTable[(posy<<5)+posx] = macroptr[0];
            NameTable[(posy<<5)+posx+1] = macroptr[1];
            NameTable[((posy+1)<<5)+posx] = macroptr[2];
            NameTable[((posy+1)<<5)+posx+1] = macroptr[3];

// Update attribute table (if necessary)

            if (palnum != defpalnum)
                {
                attribdata = AttribTable[((posy & 0xFC) << 1) + (posx >> 2)];
                palselect = (unsigned char)((posy & 2) + ((posx & 2) >> 1));
                andval = fileptr[0x1F004 + palselect];
                attribdata &= andval;
                attribdata |= (unsigned char)(palnum << (palselect << 1));
                AttribTable[((posy & 0xFC) << 1) + (posx >> 2)] = attribdata;
                }
            }
        posx += 2;
        }
    posy += 2;
    }

};

//---------------------------------------------------------------------------

void DrawSprite(unsigned int tilenum, unsigned int tileflip, unsigned int palnum, unsigned int areanum, unsigned int xpos, unsigned int ypos)
{
unsigned int BitMapPos;
unsigned char *TilePtr;
unsigned int TileOfs;
char TileData;
char TileAdd;
unsigned int BitIndex;

TileOfs = Areas[areanum].gfxptr[tilenum+256];
TilePtr = &fileptr[TileOfs];
palnum += 4;
TileAdd = (char)(palnum << 2);
BitMapPos = (ypos << 8) + xpos;   // Calculate position on bitmap

switch (tileflip)
    {
    case 0: // Normal
    for (int a=0; a<8; a++) // Do eight lines
        {
        if (BitMapPos >= (256*240))
            break;
        BitIndex = TilePtr[a] + (TilePtr[a + 8] << 8);
        for (int b=0; b<8; b++)
            {
            TileData = ((char *)TilePatPointer)[(BitIndex << 3) + b] | TileAdd;
            if (TileData & 0x03)
                ((char *)MetroidImage.bmptr)[BitMapPos+b] = TileData;
            }
        BitMapPos += 256;
        }
    break;

    case 1: // Horizontal flip
    for (int a=0; a<8; a++) // Do eight lines
        {
        if (BitMapPos >= (256*240))
            break;
        BitIndex = TilePtr[a] + (TilePtr[a + 8] << 8);
        for (int b=0; b<8; b++)
            {
            TileData = ((char *)TilePatPointer)[(BitIndex << 3) + (7-b)] | TileAdd;
            if (TileData & 0x03)
                ((char *)MetroidImage.bmptr)[BitMapPos+b] = TileData;
            }
        BitMapPos += 256;
        }
    break;

    case 2: // Vertical flip
    for (int a=7; a>=0; a--) // Do eight lines
        {
        if (BitMapPos >= (256*240))
            break;
        BitIndex = TilePtr[a] + (TilePtr[a + 8] << 8);
        for (int b=0; b<8; b++)
            {
            TileData = ((char *)TilePatPointer)[(BitIndex << 3) + b] | TileAdd;
            if (TileData & 0x03)
                ((char *)MetroidImage.bmptr)[BitMapPos+b] = TileData;
            }
        BitMapPos += 256;
        }
    break;

    case 3: // Both
    for (int a=7; a>=0; a--) // Do eight lines
        {
        if (BitMapPos >= (256*240))
            break;
        BitIndex = TilePtr[a] + (TilePtr[a + 8] << 8);
        for (int b=0; b<8; b++)
            {
            TileData = ((char *)TilePatPointer)[(BitIndex << 3) + (7-b)] | TileAdd;
            if (TileData & 0x03)
                ((char *)MetroidImage.bmptr)[BitMapPos+b] = TileData;
            }
        BitMapPos += 256;
        }
    break;
    }   // end switch
};

//---------------------------------------------------------------------------
__fastcall TEditForm::TEditForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::Open1Click(TObject *Sender)
{
if (OpenDialog1->Execute() == false)
    {   // "Cancel" was clicked
    if (fileptr != NULL) // A Metroid ROM already loaded?
        return; // Yes, go back editing it
    int result = Application->MessageBox("You must select a Metroid ROM to open.", "Error message", MB_OKCANCEL);
    if (result == ID_CANCEL)
        Application->Terminate();
    else
        Open1Click(Sender);
    return;
    }

if (fileptr != NULL)
    GlobalFree(fileptr);

// Open file

wsprintf(filename, "%s", OpenDialog1->FileName);
filehandle = CreateFile(filename,
                        GENERIC_READ,
                        NULL,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
if (filehandle == INVALID_HANDLE_VALUE)
    {
    Application->MessageBox("Could not open the specified file.", "Error message", MB_OK);
    Open1Click(Sender);
    return;
    }

// Retrieve filesize

filesize = GetFileSize(filehandle, NULL);
if (filesize != 131088)
    {
    Application->MessageBox("Error: The Metroid ROM must be 131,088 bytes in size.", "Size error", MB_OK);
    CloseHandle(filehandle);
    Open1Click(Sender);
    return;
    }

// Attempt to allocate memory for file

fileptr = (unsigned char *)GlobalAlloc(GMEM_FIXED, filesize);
if (fileptr == NULL)
    {
    Application->MessageBox("Fatal error: Could not allocate memory for file.", "Error message", MB_OK);
    Application->Terminate();
    }

// Read contents of file

ReadFile(filehandle,
        fileptr,
        filesize,
        &NumBytes,
        NULL);
CloseHandle(filehandle);

// Check if ROM ID at 1FFF9h is correct

int result = memcmp(&fileptr[0x1FFF9], "METROID", 7);
if (result != 0)
    {
    Application->MessageBox("Fatal error: The ROM ID string is incorrect. This is not a valid Metroid ROM!", "Error message", MB_OK);
    GlobalFree(fileptr);
    fileptr = NULL;
    Open1Click(Sender);
    return;
    }

// Create map filename (<romname>.MAP);

strcpy(mapfilename, filename);
char *sp = strrchr(mapfilename, '.');
if (sp != NULL)
    *sp = NULL;
strcat(mapfilename, ".MAP");

// Attempt to open map file

filehandle = CreateFile(mapfilename,
                        GENERIC_READ,
                        NULL,
                        NULL,
                        OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

if (filehandle != NULL)
    {
    ReadFile(filehandle,
            MapIndex,
            32*32,
            &NumBytes,
            NULL);
    CloseHandle(filehandle);
    }

// Set Samus' initial (X,Y) position on map

MapPosX = fileptr[0x055E7];
MapPosY = fileptr[0x055E8];

// Initialize some area variables

void *p;
for (int areanum=0; areanum<5; areanum++)
    {
    p = Areas[areanum].ROMofs - 0x08000 + 0x09598 + fileptr;
    Areas[areanum].itemofs = ((unsigned short *)p)[0];
    Areas[areanum].roomptrofs = ((unsigned short *)p)[1];
    Areas[areanum].structptrofs = ((unsigned short *)p)[2];
    Areas[areanum].macroofs = ((unsigned short *)p)[3];
    Areas[areanum].frame2ptrofs = ((unsigned short *)p)[4];
    Areas[areanum].sprpos2ptrofs = ((unsigned short *)p)[6];
    Areas[areanum].anim2ofs = ((unsigned short *)p)[7];
    }

DrawRoom(MapPosX, MapPosY);
FormPaint(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::Exit1Click(TObject *Sender)
{
Application->Terminate();    
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormClose(TObject *Sender, TCloseAction &Action)
{

// Free file memory (128K)

if (fileptr != NULL)
    GlobalFree(fileptr);

// Free memory used for tile patterns (512K)

GlobalFree(TilePatPointer);
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormCreate(TObject *Sender)
{
// Allocate memory for bit patterns

TilePatPointer = (unsigned char *)GlobalAlloc(GMEM_FIXED, 65536*8);
if (TilePatPointer == NULL)
    {
    Application->MessageBox("Fatal error: Unable to allocate memory for bit patterns.", "Error message", MB_OK);
    }

// Generate all possible bit patterns

char bp1;
char bp2;
int index = 0;

for (int a=0; a<65536; a++)
    {
    bp1 = (char)(a & 0xFF);
    bp2 = (char)(a >> 8);
    bp2 = _rotl(bp2, 1);
    for (int b=0; b<8; b++)
        {
        bp1 = _rotl(bp1, 1);
        bp2 = _rotl(bp2, 1);
        ((char *)TilePatPointer)[index++] = (char)((bp1 & 1) | (bp2 & 2));
        }
    }

// Set up bitmap header

MetroidImage.bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
MetroidImage.bminfo.bmiHeader.biWidth = 256;
MetroidImage.bminfo.bmiHeader.biHeight = -240;
MetroidImage.bminfo.bmiHeader.biPlanes = 1;
MetroidImage.bminfo.bmiHeader.biBitCount = 8;
MetroidImage.bminfo.bmiHeader.biCompression = BI_RGB;
MetroidImage.bminfo.bmiHeader.biSizeImage = 0;
MetroidImage.bminfo.bmiHeader.biClrUsed = 0;
MetroidImage.bminfo.bmiHeader.biClrImportant = 0;

// Create bitmap

MetroidImage.bmhnd = CreateDIBSection(GetDC(EditForm->Handle),
                                (BITMAPINFO *)&MetroidImage.bminfo,
                                DIB_RGB_COLORS,
                                &MetroidImage.bmptr,
                                NULL,
                                NULL);

// Initialize area variables

Areas[BRINSTAR].ROMofs = 0x04010;
Areas[BRINSTAR].mapposx = 0x03;
Areas[BRINSTAR].mapposy = 0x0E;
Areas[BRINSTAR].numstruct = 50;
Areas[BRINSTAR].palofs = 0x0A274;
Areas[BRINSTAR].numrooms = 47;
Areas[BRINSTAR].gfxptr = BrinGFX;
Areas[BRINSTAR].coordptr = BrinCoords;
Areas[BRINSTAR].frametable = frametable0;

Areas[NORFAIR].ROMofs = 0x08010;
Areas[NORFAIR].mapposx = 0x16;
Areas[NORFAIR].mapposy = 0x0D;
Areas[NORFAIR].numstruct = 49;
Areas[NORFAIR].palofs = 0x0A17B;
Areas[NORFAIR].numrooms = 46;
Areas[NORFAIR].gfxptr = NorGFX;
Areas[NORFAIR].coordptr = NorCoords;
Areas[NORFAIR].frametable = frametable1;

Areas[TOURIAN].ROMofs = 0x0C010;
Areas[TOURIAN].mapposx = 0x03;
Areas[TOURIAN].mapposy = 0x04;
Areas[TOURIAN].numstruct = 32;
Areas[TOURIAN].palofs = 0x0A71B;
Areas[TOURIAN].numrooms = 21;
Areas[TOURIAN].gfxptr = TourGFX;
Areas[TOURIAN].coordptr = TourCoords;
Areas[TOURIAN].frametable = frametable2;

Areas[KRAID].ROMofs = 0x10010;
Areas[KRAID].mapposx = 0x07;
Areas[KRAID].mapposy = 0x14;
Areas[KRAID].numstruct = 39;
Areas[KRAID].palofs = 0x0A158;
Areas[KRAID].numrooms = 37;
Areas[KRAID].gfxptr = KraidGFX;
Areas[KRAID].coordptr = KraidCoords;
Areas[KRAID].frametable = frametable3;

Areas[RIDLEY].ROMofs = 0x14010;
Areas[RIDLEY].mapposx = 0x19;
Areas[RIDLEY].mapposy = 0x18;
Areas[RIDLEY].numstruct = 29;
Areas[RIDLEY].palofs = 0x0A0EE;
Areas[RIDLEY].numrooms = 42;
Areas[RIDLEY].gfxptr = RidGFX;
Areas[RIDLEY].coordptr = RidCoords;
Areas[RIDLEY].frametable = frametable4;

// Set up helpfile property

char buffer[256];
GetCurrentDirectory(256, buffer);
strcat(buffer, "\\metedit.hlp");
Application->HelpFile = buffer;

// Request file

Open1Click(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormPaint(TObject *Sender)
{
HDC WinHDC = GetDC(EditForm->Handle);
StretchDIBits(WinHDC,
                0,
                0,
                ScreenWidth,
                ScreenHeight,
                0,
                0,
                256,
                240,
                MetroidImage.bmptr,
                (BITMAPINFO *) &MetroidImage.bminfo,
                DIB_RGB_COLORS,
                SRCCOPY);
ReleaseDC(EditForm->Handle, WinHDC);

}
//---------------------------------------------------------------------------
void __fastcall TEditForm::Save1Click(TObject *Sender)
{

fileptr[0x1EDBB] = 0x00;

// Write metroid.nes

filehandle = CreateFile(filename,
                        GENERIC_WRITE,
                        NULL,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);
WriteFile(filehandle,
        fileptr,
        filesize,
        &NumBytes,
        NULL);
CloseHandle(filehandle);

// Write metedit.map

filehandle = CreateFile(mapfilename,
                        GENERIC_WRITE,
                        NULL,
                        NULL,
                        CREATE_ALWAYS,
                        FILE_ATTRIBUTE_NORMAL,
                        NULL);

WriteFile(filehandle,
        MapIndex,
        32*32,
        &NumBytes,
        NULL);
CloseHandle(filehandle);

}
//---------------------------------------------------------------------------

void __fastcall TEditForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
unsigned int roomnum;
char buffer[256];

switch (Key)
    {
    case 0xBB:  // +
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX];
    if ((int)roomnum == (int)Areas[CurrentArea].numrooms-1)
        fileptr[0x254E + (MapPosY << 5) + MapPosX] = 0;
    else
        fileptr[0x254E + (MapPosY << 5) + MapPosX]++;
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    break;

    case 0xBD:  // -
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX];
    if (roomnum == 0)
        fileptr[0x254E + (MapPosY << 5) + MapPosX] = (unsigned char)(Areas[CurrentArea].numrooms-1);
    else
        fileptr[0x254E + (MapPosY << 5) + MapPosX]--;
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    break;

    case 37:    // Left
    if (MapPosX == 0) break;
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX - 1];
    if (roomnum == 0xFF) break;
    CurrentObject = NULL;
    MapPosX--;
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    if (MapForm->Visible == true)
        {
        DrawMap(MapPosX, MapPosY);
        SendMessage(MapForm->Handle, WM_PAINT, 0, 0);
        }
    break;

    case 38:    // Up
    if (MapPosY == 0) break;
    roomnum = fileptr[0x254E + ((MapPosY - 1) << 5) + MapPosX];
    if (roomnum == 0xFF) break;
    CurrentObject = NULL;
    MapPosY--;
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    if (MapForm->Visible == true)
        {
        DrawMap(MapPosX, MapPosY);
        SendMessage(MapForm->Handle, WM_PAINT, 0, 0);
        }
    break;

    case 39:    // Right
    if (MapPosX == 31) break;
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX + 1];
    if (roomnum == 0xFF) break;
    CurrentObject = NULL;
    MapPosX++;
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    if (MapForm->Visible == true)
        {
        DrawMap(MapPosX, MapPosY);
        SendMessage(MapForm->Handle, WM_PAINT, 0, 0);
        }
    break;

    case 40:    // Down
    if (MapPosY == 31) break;
    roomnum = fileptr[0x254E + ((MapPosY + 1) << 5) + MapPosX];
    if (roomnum == 0xFF) break;
    CurrentObject = NULL;
    MapPosY++;
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    if (MapForm->Visible == true)
        {
        DrawMap(MapPosX, MapPosY);
        SendMessage(MapForm->Handle, WM_PAINT, 0, 0);
        }
    break;

    case 32:    // " "
    if (CurrentObject == NULL) break;
    if (OBJPOS == 0)
        {
        CurrentObject[2]++;
        CurrentObject[2] &= 3;
//        wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.2X (%.5X) | Palette: %.1X", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1], GetStructPointer(CurrentObject[1], CurrentArea) - fileptr, CurrentObject[2]);
        wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.2X | Palette: %.1X", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1], CurrentObject[2]);
        StatusBar1->SimpleText = buffer;
        }
    else
        CurrentObject[1] ^= 0x80;

    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);
    break;
    }

// Update status bar

if (CurrentObject == NULL)
    {
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX];
//    wsprintf(buffer, "Map: (%.2X, %.2X) | Room: %.2X (%.5X)", MapPosX, MapPosY, roomnum, GetRoomPointer(roomnum, CurrentArea) - fileptr);
    wsprintf(buffer, "Map: (%.2X, %.2X) | Room: %.2X", MapPosX, MapPosY, roomnum);
    StatusBar1->SimpleText = buffer;
    }

}
//---------------------------------------------------------------------------

void __fastcall TEditForm::FormMouseDown(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
char buffer[256];
unsigned char enemynum;
unsigned char framenum;

if (Button == mbRight)
    {   // Right mouse button pressed
    if (CurrentObject == NULL)
        return;  // Exit if no object selected
    if (OBJPOS == 0)
        {   // Room object
        if (Shift.Contains(ssShift))
            {   // If Shift pressed
            if (CurrentObject[1] == 0)  // Is the object type 0?
                CurrentObject[1] = (unsigned char)(Areas[CurrentArea].numstruct-1);    // Set to last type
            else
                CurrentObject[1]--; // Decrease object type #
            }
        else
            {
            if (CurrentObject[1] == Areas[CurrentArea].numstruct-1)   // Reached last object #?
                CurrentObject[1] = 0;   // If so, wrap-around
            else
                CurrentObject[1]++; // Else increase object type #
            }
        wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.2X | Palette: %.1X", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1], CurrentObject[2]);
        StatusBar1->SimpleText = buffer;
        }
    else
        {   // Enemy
        enemynum = (unsigned char)(CurrentObject[1] & 0x0F);
        if (Shift.Contains(ssShift))
            {   // If Shift pressed
            do  {
                enemynum = (unsigned char)((enemynum-1) & 0x0F);
                framenum = Areas[CurrentArea].frametable[enemynum];
                } while (framenum == 0);
            }
        else
            {
            do  {
                enemynum = (unsigned char)((enemynum+1) & 0x0F);
                framenum = Areas[CurrentArea].frametable[enemynum];
                } while (framenum == 0);
            }
        CurrentObject[1] &= 0xF0;
        CurrentObject[1] |= enemynum;
        wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.1X ", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1] & 0x0F);
        StatusBar1->SimpleText = buffer;
        }
    DrawRoom(MapPosX, MapPosY); // Update screen bitmap
    FormPaint(Sender);
    }
else
    {   // Left mouse button pressed
    unsigned int objleft;
    unsigned int objtop;
    unsigned int objright;
    unsigned int objbottom;

    unsigned int structnum;
    unsigned int roomnum;
    unsigned char *roomdata;
    unsigned char *coordptr;
    unsigned int roompos = 1;
    char buffer[256];
    bool done = false;

    X = X >> (ScreenWidth / 512); // Convert to range 0...255
    Y = Y >> (ScreenHeight / 480); // Convert to range 0...255
    if (Y > 239) Y = 239;

    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX];  // Get room number at map position
    roomdata = GetRoomPointer(roomnum, CurrentArea);    // Get pointer to room data
    coordptr = Areas[CurrentArea].coordptr;

// Set roompos to start of enemy/door data

    while (((roomdata[roompos] != 0xFD)
    && (roomdata[roompos] != 0xFF)))
        roompos += 3;

    if (roomdata[roompos++] == 0xFD)
        {   // Enemy/door data present
        while (done == false)   // Start scanning enemies
            {
            switch (roomdata[roompos])
                {
                case 0x02:  // Door
                roompos += 2;
                break;

                case 0x06:  // Kraid & Ridley statues
                roompos++;
                break;

                default:    // Normal enemy
                objleft = (roomdata[roompos+2] & 0x0F) << 4;
                objtop = roomdata[roompos+2] & 0xF0;
                objright = objleft;
                objright += 16;
                objbottom = objtop;
                objbottom += 16;
                if (((unsigned int)X >= objleft)
                    && ((unsigned int)X < objright)
                    && ((unsigned int)Y >= objtop)
                    && ((unsigned int)Y < objbottom))
                    {
                    CurrentObject = &roomdata[roompos];
                    DeltaX = (X - objleft) >> 4;
                    DeltaY = (Y - objtop) >> 4;
                    OBJPOS = 2;
                    wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.1X ", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1] & 0x0F);
                    StatusBar1->SimpleText = buffer;
                    done = true;
                    }
                roompos += 3;   // Advance to next enemy
                break;
                }   // End of switch statement

// Check if end of enemy/door data has been reached

            if (roomdata[roompos] == 0xFF)
                done = true;
            }   // While
        }   // If enemy data present

// Exit if enemy was selected

    if (CurrentObject != NULL) return;

// Set roompos to last object.
// The objects are scanned in reverse order
//  to account for priorities.

    roompos = 1;
    while (((roomdata[roompos+3] != 0xFD)
        && (roomdata[roompos+3] != 0xFF)))
        roompos += 3;

    done = false;
    while (done == false)   // Start scanning objects
        {
        objleft = (roomdata[roompos] & 0x0F) << 4;
        objtop = roomdata[roompos] & 0xF0;
        structnum = roomdata[roompos+1];
        objright = objleft;
        objright += coordptr[structnum << 1];
        objbottom = objtop;
        objbottom += coordptr[(structnum << 1) + 1];

        if (((unsigned int)X >= objleft)
            && ((unsigned int)X < objright)
            && ((unsigned int)Y >= objtop)
            && ((unsigned int)Y < objbottom))
            {
            CurrentObject = &roomdata[roompos];
            DeltaX = (X - objleft) >> 4;
            DeltaY = (Y - objtop) >> 4;
            OBJPOS = 0;
            wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.2X | Palette: %.1X", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1], CurrentObject[2]);
            StatusBar1->SimpleText = buffer;
            done = true;
            }
        roompos -= 3;   // try previous object
        if ((signed int)roompos < 0)
            done = true;    // Exit if tried all objects in room
        }   // End of while loop

    }   // End of left mouse button handler

}
//---------------------------------------------------------------------------

void __fastcall TEditForm::FormMouseMove(TObject *Sender, TShiftState Shift,
      int X, int Y)
{
if (CurrentObject == NULL) return;

X = X >> (ScreenWidth / 512); // Convert to range 0...255
Y = Y >> (ScreenHeight / 480); // Convert to range 0...255
if (Y > 239) Y = 239;
else if (Y < 0) Y = 0;
if (X > 255) X = 255;
else if (X < 0) X = 0;

int xpos = (X >> 4) - DeltaX;
if (xpos < 0) xpos = 0;
int ypos = (Y >> 4) - DeltaY;
if (ypos < 0) ypos = 0;

if (CurrentObject[OBJPOS] != ((ypos << 4) + xpos))
    {
    CurrentObject[OBJPOS] = (unsigned char)((ypos << 4) + xpos);
    DrawRoom(MapPosX, MapPosY);
    FormPaint(Sender);

    // Update status bar

    char buffer[256];
    if (OBJPOS == 0)
        wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.2X | Palette: %.1X", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1], CurrentObject[2]);
    else
        wsprintf(buffer, "Pos: (%.1X, %.1X) | Type: %.1X ", CurrentObject[OBJPOS] & 0x0F, CurrentObject[OBJPOS] >> 4, CurrentObject[1] & 0x0F);
    StatusBar1->SimpleText = buffer;
    }

}
//---------------------------------------------------------------------------


void __fastcall TEditForm::FormMouseUp(TObject *Sender, TMouseButton Button,
      TShiftState Shift, int X, int Y)
{
if (Button == mbLeft)
    {
    CurrentObject = NULL;   // Release object

// Update status bar

    unsigned int roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX];
    char buffer[256];
    wsprintf(buffer, "Map: (%.2X, %.2X) | Room: %.2X", MapPosX, MapPosY, roomnum);
    StatusBar1->SimpleText = buffer;
    }
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::MapEditor1Click(TObject *Sender)
{
MapForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::ItemEditor1Click(TObject *Sender)
{
ItemForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::Contents1Click(TObject *Sender)
{
Application->HelpContext(3000);    
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::PaletteEditor1Click(TObject *Sender)
{
PalForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::PasswordCreator1Click(TObject *Sender)
{
PassForm->Visible = true;    
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::N1x1Click(TObject *Sender)
{
ScreenWidth = 256;
ScreenHeight = 240;
EditForm->ClientWidth = ScreenWidth;
EditForm->ClientHeight = ScreenHeight + 25;
FormPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::N2x1Click(TObject *Sender)
{
ScreenWidth = 512;
ScreenHeight = 480;
EditForm->ClientWidth = ScreenWidth;
EditForm->ClientHeight = ScreenHeight + 25;
FormPaint(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::About1Click(TObject *Sender)
{
AboutForm->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::SaveAs1Click(TObject *Sender)
{
if (SaveDialog1->Execute() == false)
    return;
wsprintf(filename, "%s", SaveDialog1->FileName);

wsprintf(romdir, "%s", ExtractFileDir(SaveDialog1->FileName));
strcpy(mapfilename, romdir);
strcat(mapfilename, "\\metedit.map");

Save1Click(Sender);
}
//---------------------------------------------------------------------------

