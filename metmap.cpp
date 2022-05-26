//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "met.h"
#include "metmap.h"
#include "img123.h"
#include "metitem.h"
#include "metpal.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMapForm *MapForm;

extern MetroidArea Areas[5];
extern unsigned char AreaStrings[5][32];
extern unsigned char *fileptr;
extern unsigned char MapIndex[];
extern unsigned char MapPosX;
extern unsigned char MapPosY;
extern unsigned int CurrentArea;
extern void DrawRoom(unsigned char MapX, unsigned char MapY);
void DrawMap(unsigned char MapX, unsigned char MapY);
Image123 MapImage;

//---------------------------------------------------------------------------

void DrawMap(unsigned char MapX, unsigned char MapY)
{
for (int y=0; y<32; y++)
    {
    for (int x=0; x<32; x++)
        {
        if (fileptr[0x254E + (y << 5) + x] != 0xFF)
            {
            for (int i=0; i<7; i++)
                {
                ((int *)MapImage.bmptr)[(y << 9) + (x << 1) + (i << 6)] = (MapIndex[(y << 5) + x]+1) + ((MapIndex[(y << 5) + x]+1) << 8) + ((MapIndex[(y << 5) + x]+1) << 16) + ((MapIndex[(y << 5) + x]+1) << 24);
                ((int *)MapImage.bmptr)[(y << 9) + (x << 1) + (i << 6)+1] = (MapIndex[(y << 5) + x]+1) + ((MapIndex[(y << 5) + x]+1) << 8) + ((MapIndex[(y << 5) + x]+1) << 16);
                }
            }
        else
            {
            for (int i=0; i<8; i++)
                {
                ((int *)MapImage.bmptr)[(y << 9) + (x << 1) + (i << 6)] = 0;
                ((int *)MapImage.bmptr)[(y << 9) + (x << 1) + (i << 6)+1] = 0;
                }
            }
        }
    }

((int *)MapImage.bmptr)[(MapY << 9) + (MapX << 1)] = 0x06060606;
((int *)MapImage.bmptr)[(MapY << 9) + (MapX << 1)+1] = 0x06060606;
for (int i=1; i<6; i++)
    {
    ((char *)MapImage.bmptr)[((MapY << 9) + (MapX << 1) + (i << 6)) << 2] = 6;
    ((char *)MapImage.bmptr)[(((MapY << 9) + (MapX << 1) + (i << 6)+1) << 2) + 3] = 6;
    }
((int *)MapImage.bmptr)[(MapY << 9) + (MapX << 1)+384] = 0x06060606;
((int *)MapImage.bmptr)[(MapY << 9) + (MapX << 1)+385] = 0x06060606;
};

//---------------------------------------------------------------------------
__fastcall TMapForm::TMapForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::FormCreate(TObject *Sender)
{

// Set up bitmap header

MapImage.bminfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
MapImage.bminfo.bmiHeader.biWidth = 256;
MapImage.bminfo.bmiHeader.biHeight = -256;
MapImage.bminfo.bmiHeader.biPlanes = 1;
MapImage.bminfo.bmiHeader.biBitCount = 8;
MapImage.bminfo.bmiHeader.biCompression = BI_RGB;
MapImage.bminfo.bmiHeader.biSizeImage = 0;
MapImage.bminfo.bmiHeader.biClrUsed = 0;
MapImage.bminfo.bmiHeader.biClrImportant = 0;

// Create bitmap

MapImage.bmhnd = CreateDIBSection(GetDC(MapForm->Handle),
                                (BITMAPINFO *)&MapImage.bminfo,
                                DIB_RGB_COLORS,
                                &MapImage.bmptr,
                                NULL,
                                NULL);

// Set up palette

MapImage.bminfo.bmiColors[1].rgbRed = 0;
MapImage.bminfo.bmiColors[1].rgbGreen = 0;
MapImage.bminfo.bmiColors[1].rgbBlue = 255;

MapImage.bminfo.bmiColors[2].rgbRed = 0;
MapImage.bminfo.bmiColors[2].rgbGreen = 196;
MapImage.bminfo.bmiColors[2].rgbBlue = 0;

MapImage.bminfo.bmiColors[3].rgbRed = 255;
MapImage.bminfo.bmiColors[3].rgbGreen = 0;
MapImage.bminfo.bmiColors[3].rgbBlue = 0;

MapImage.bminfo.bmiColors[4].rgbRed = 196;
MapImage.bminfo.bmiColors[4].rgbGreen = 0;
MapImage.bminfo.bmiColors[4].rgbBlue = 196;

MapImage.bminfo.bmiColors[5].rgbRed = 0;
MapImage.bminfo.bmiColors[5].rgbGreen = 196;
MapImage.bminfo.bmiColors[5].rgbBlue = 196;

MapImage.bminfo.bmiColors[6].rgbRed = 255;
MapImage.bminfo.bmiColors[6].rgbGreen = 255;
MapImage.bminfo.bmiColors[6].rgbBlue = 255;
}
//---------------------------------------------------------------------------
void __fastcall TMapForm::FormPaint(TObject *Sender)
{
HDC WinHDC = GetDC(MapForm->Handle);
StretchDIBits(WinHDC,
                0,
                0,
                256,
                256,
                0,
                0,
                256,
                256,
                MapImage.bmptr,
                (BITMAPINFO *) &MapImage.bminfo,
                DIB_RGB_COLORS,
                SRCCOPY);
ReleaseDC(MapForm->Handle, WinHDC);

}
//---------------------------------------------------------------------------
void __fastcall TMapForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
unsigned int mappos = ((Y >> 3) << 5) + (X >> 3);
if (Button == mbLeft)
    {
    if (fileptr[0x254E + mappos] == 0xFF)
        {
        int result = Application->MessageBox("This map position is not in use. Activate?", "System message", MB_YESNO);
        if (result == ID_YES)
            {
            fileptr[0x254E + mappos] = 0;
            MapPosX = (unsigned char)(X >> 3);
            MapPosY = (unsigned char)(Y >> 3);
            DrawMap(MapPosX, MapPosY);
            FormPaint(Sender);
            DrawRoom(MapPosX, MapPosY);
            SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
            }
        }
    else
        {
        MapPosX = (unsigned char)(X >> 3);
        MapPosY = (unsigned char)(Y >> 3);
        DrawMap(MapPosX, MapPosY);
        FormPaint(Sender);
        DrawRoom(MapPosX, MapPosY);
        SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
        }
    if (Shift.Contains(ssShift))
        {
        unsigned int mapposofs;
        char buffer[256];
        mapposofs =  0x095D7 - 0x08000 + Areas[CurrentArea].ROMofs;
        fileptr[mapposofs] = MapPosX;
        fileptr[mapposofs+1] = MapPosY;
        wsprintf(buffer, "Start position on map in %s set to (%.2X, %.2X)", AreaStrings[CurrentArea], MapPosX, MapPosY);
        Application->MessageBox(buffer, "Message", MB_OK);
        }
    }   // End of LMB handler
else
    {   // RMB pressed
    if (fileptr[0x254E + mappos] == 0xFF)
        return;
    if (MapIndex[mappos] == 4)
        MapIndex[mappos] = 0;
    else
        MapIndex[mappos]++;
    fileptr[0x254E + mappos] = 0;
    DrawMap(MapPosX, MapPosY);
    FormPaint(Sender);
    }   // End of RMB handler
}
//---------------------------------------------------------------------------

void __fastcall TMapForm::FormShow(TObject *Sender)
{
DrawMap(MapPosX, MapPosY);
}
//---------------------------------------------------------------------------

void __fastcall TMapForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
unsigned int roomnum;
switch(Key)
    {
    case 0x2E:  // Del
    fileptr[0x254E + (MapPosY << 5) + MapPosX] = 0xFF;
    DrawMap(MapPosX, MapPosY);
    FormPaint(Sender);
    break;

    case 37:    // Left
    if (MapPosX == 0) break;
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX - 1];
    if (roomnum == 0xFF) break;
    MapPosX--;
    DrawMap(MapPosX, MapPosY);
    FormPaint(Sender);
    DrawRoom(MapPosX, MapPosY);
    SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
    break;

    case 38:    // Up
    if (MapPosY == 0) break;
    roomnum = fileptr[0x254E + ((MapPosY - 1) << 5) + MapPosX];
    if (roomnum == 0xFF) break;
    MapPosY--;
    DrawMap(MapPosX, MapPosY);
    FormPaint(Sender);
    DrawRoom(MapPosX, MapPosY);
    SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
    break;

    case 39:    // Right
    if (MapPosX == 31) break;
    roomnum = fileptr[0x254E + (MapPosY << 5) + MapPosX + 1];
    if (roomnum == 0xFF) break;
    MapPosX++;
    DrawMap(MapPosX, MapPosY);
    FormPaint(Sender);
    DrawRoom(MapPosX, MapPosY);
    SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
    break;

    case 40:    // Down
    if (MapPosY == 31) break;
    roomnum = fileptr[0x254E + ((MapPosY + 1) << 5) + MapPosX];
    if (roomnum == 0xFF) break;
    MapPosY++;
    DrawMap(MapPosX, MapPosY);
    FormPaint(Sender);
    DrawRoom(MapPosX, MapPosY);
    SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
    break;
    }
}
//---------------------------------------------------------------------------

