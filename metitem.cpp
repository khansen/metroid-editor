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
TItemForm *ItemForm;

extern void DrawMap(unsigned char MapX, unsigned char MapY);
extern void DrawRoom(unsigned char MapX, unsigned char MapY);
extern Image123 MapImage;
extern unsigned char *fileptr;
extern unsigned char MapIndex[];
extern unsigned char MapPosX;
extern unsigned char MapPosY;
extern MetroidArea Areas[5];

unsigned char *itemdata = NULL;

//---------------------------------------------------------------------------

unsigned char *GetItemPointer(unsigned int itemnum, unsigned int areanum);
unsigned char *GetItemPointer(unsigned int itemnum, unsigned int areanum)
{
unsigned int itemofs;
void *p;
unsigned char *itemdata;

itemofs = Areas[areanum].itemofs - 0x08000 + Areas[areanum].ROMofs;
itemdata = &fileptr[itemofs];
for (unsigned int i=0; i<itemnum; i++)
    {
    p = &fileptr[itemofs + 1];
    if (*(unsigned short *)p == 0xFFFF)
        {
        itemdata = NULL;
        break;
        }
    itemofs = *(unsigned short *)p - 0x08000 + Areas[areanum].ROMofs;
    itemdata = &fileptr[itemofs];
    }
return itemdata;
};

//---------------------------------------------------------------------------
__fastcall TItemForm::TItemForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TItemForm::FormPaint(TObject *Sender)
{
HDC WinHDC = GetDC(ItemForm->Handle);
StretchDIBits(WinHDC,
                164,
                8,
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
ReleaseDC(ItemForm->Handle, WinHDC);

}
//---------------------------------------------------------------------------

void __fastcall TItemForm::ListBox1Click(TObject *Sender)
{
ListBox2->Clear();
switch(ListBox1->ItemIndex)
    {
    case 0x00:  // Brinstar
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Long beam");
    ListBox2->Items->Add("Palette switch");
    ListBox2->Items->Add("Ice beam");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Maru Mari");
    ListBox2->Items->Add("Elevator");
    break;

    case 0x01:  // Norfair
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("Ice beam");
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Screw attack");
    ListBox2->Items->Add("Palette switch");
    ListBox2->Items->Add("Energy tank");
    ListBox2->Items->Add("???");
    ListBox2->Items->Add("Wave beam");
    ListBox2->Items->Add("Missile container");
    break;

    case 0x02:  // Tourian
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("???");
    ListBox2->Items->Add("Rinka");
    ListBox2->Items->Add("Rinka");
    ListBox2->Items->Add("Rinka");
    ListBox2->Items->Add("???");
    break;

    case 0x03:  // Kraid
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("Elevator");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Energy tank");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Memu");
    ListBox2->Items->Add("Energy tank");
    break;

    case 0x04:  // Ridley
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Energy tank");
    ListBox2->Items->Add("Missile container");
    ListBox2->Items->Add("Energy tank");
    ListBox2->Items->Add("Missile container");
    break;
    }
}
//---------------------------------------------------------------------------

void __fastcall TItemForm::ListBox2Click(TObject *Sender)
{
unsigned int areanum;
unsigned int itemnum;

areanum = ListBox1->ItemIndex;
itemnum = ListBox2->ItemIndex;
itemdata = GetItemPointer(itemnum, areanum);
MapPosX = itemdata[3];
MapPosY = itemdata[0];
DrawMap(MapPosX, MapPosY);
FormPaint(Sender);
DrawRoom(MapPosX, MapPosY);
SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
}
//---------------------------------------------------------------------------


void __fastcall TItemForm::FormMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if (itemdata == NULL)
    return;
if ((X < 164) || (X >= 452) || (Y < 8) || (Y >= 264))
    return;

X -= 164;
Y -= 8;

if (MapIndex[((Y >> 3) << 5) + (X >> 3)] != ListBox1->ItemIndex)
    {
    Application->MessageBox("Item can't be moved to a different area!", "Error message", MB_OK);
    return;
    }

if (fileptr[0x254E + ((Y >> 3) << 5) + (X >> 3)] == 0xFF)
    return;

itemdata[0] = (unsigned char)(Y >> 3);
itemdata[3] = (unsigned char)(X >> 3);
MapPosX = itemdata[3];
MapPosY = itemdata[0];
DrawMap(MapPosX, MapPosY);
FormPaint(Sender);
DrawRoom(MapPosX, MapPosY);
SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TItemForm::FormShow(TObject *Sender)
{
ListBox1->ItemIndex = 0;
ListBox1Click(Sender);
ListBox2->ItemIndex = 0;
ListBox2Click(Sender);
}
//---------------------------------------------------------------------------

