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
TPalForm *PalForm;

extern unsigned char *fileptr;
extern unsigned char NESPalette[];
extern unsigned char Palette[32*4];
extern unsigned int CurrentArea;
extern MetroidArea Areas[5];
extern Image123 MetroidImage;
extern void NESPalToRGB(unsigned char *p1, unsigned char *p2);
unsigned int CurrentColor = 0;

//---------------------------------------------------------------------------
__fastcall TPalForm::TPalForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TPalForm::FormShow(TObject *Sender)
{
TShape *Shapes[96] = { Shape1, Shape2, Shape3, Shape4,
                       Shape5, Shape6, Shape7, Shape8,
                       Shape9, Shape10, Shape11, Shape12,
                       Shape13, Shape14, Shape15, Shape16,
                       Shape17, Shape18, Shape19, Shape20,
                       Shape21, Shape22, Shape23, Shape24,
                       Shape25, Shape26, Shape27, Shape28,
                       Shape29, Shape30, Shape31, Shape32,
                       Shape33, Shape34, Shape35, Shape36,
                       Shape37, Shape38, Shape39, Shape40,
                       Shape41, Shape42, Shape43, Shape44,
                       Shape45, Shape46, Shape47, Shape48,
                       Shape49, Shape50, Shape51, Shape52,
                       Shape53, Shape54, Shape55, Shape56,
                       Shape57, Shape58, Shape59, Shape60,
                       Shape61, Shape62, Shape63, Shape64,
                       Shape65, Shape66, Shape67, Shape68,
                       Shape69, Shape70, Shape71, Shape72,
                       Shape73, Shape74, Shape75, Shape76,
                       Shape77, Shape78, Shape79, Shape80,
                       Shape81, Shape82, Shape83, Shape84,
                       Shape85, Shape86, Shape87, Shape88,
                       Shape89, Shape90, Shape91, Shape92,
                       Shape93, Shape94, Shape95, Shape96
                       };

Shape97->Left = Shapes[CurrentColor]->Left;
Shape97->Top = Shapes[CurrentColor]->Top - 4;

ListBox1->ItemIndex = CurrentArea;

for (int i=0; i<32; i++)
    Shapes[i]->Brush->Color = (TColor)(MetroidImage.bminfo.bmiColors[i].rgbRed + (MetroidImage.bminfo.bmiColors[i].rgbGreen << 8) + (MetroidImage.bminfo.bmiColors[i].rgbBlue << 16));

for (int i=0; i<64; i++)
    Shapes[i+32]->Brush->Color = (TColor)(NESPalette[i << 2] + (NESPalette[(i << 2) + 1] << 8) + (NESPalette[(i << 2) + 2] << 16));
}
//---------------------------------------------------------------------------

void __fastcall TPalForm::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
TShape *Shapes[96] = { Shape1, Shape2, Shape3, Shape4,
                       Shape5, Shape6, Shape7, Shape8,
                       Shape9, Shape10, Shape11, Shape12,
                       Shape13, Shape14, Shape15, Shape16,
                       Shape17, Shape18, Shape19, Shape20,
                       Shape21, Shape22, Shape23, Shape24,
                       Shape25, Shape26, Shape27, Shape28,
                       Shape29, Shape30, Shape31, Shape32,
                       Shape33, Shape34, Shape35, Shape36,
                       Shape37, Shape38, Shape39, Shape40,
                       Shape41, Shape42, Shape43, Shape44,
                       Shape45, Shape46, Shape47, Shape48,
                       Shape49, Shape50, Shape51, Shape52,
                       Shape53, Shape54, Shape55, Shape56,
                       Shape57, Shape58, Shape59, Shape60,
                       Shape61, Shape62, Shape63, Shape64,
                       Shape65, Shape66, Shape67, Shape68,
                       Shape69, Shape70, Shape71, Shape72,
                       Shape73, Shape74, Shape75, Shape76,
                       Shape77, Shape78, Shape79, Shape80,
                       Shape81, Shape82, Shape83, Shape84,
                       Shape85, Shape86, Shape87, Shape88,
                       Shape89, Shape90, Shape91, Shape92,
                       Shape93, Shape94, Shape95, Shape96
                       };

for (int i=0; i<96; i++)
    {
    if (Shapes[i] == (TShape *)Sender)
        {
        if (i<32)
            {
            CurrentColor = i;
            Shape97->Left = Shapes[CurrentColor]->Left;
            Shape97->Top = Shapes[CurrentColor]->Top - 4;
            break;
            }
        else
            {
            unsigned int palofs;
            i = i-32;
            palofs = Areas[ListBox1->ItemIndex].palofs - 0x08000 + Areas[ListBox1->ItemIndex].ROMofs;
            fileptr[palofs + CurrentColor] = (unsigned char)i;

            Shapes[CurrentColor]->Brush->Color = (TColor)(NESPalette[i << 2] + (NESPalette[(i << 2) + 1] << 8) + (NESPalette[(i << 2) + 2] << 16));

            if ((unsigned int)ListBox1->ItemIndex == CurrentArea)
                {
                NESPalToRGB(Palette, &fileptr[palofs]);
                memcpy(&MetroidImage.bminfo.bmiColors, Palette, 32*4);
                SendMessage(EditForm->Handle, WM_PAINT, 0, 0);
                }
            break;
            }
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TPalForm::ListBox1Click(TObject *Sender)
{
TShape *Shapes[96] = { Shape1, Shape2, Shape3, Shape4,
                       Shape5, Shape6, Shape7, Shape8,
                       Shape9, Shape10, Shape11, Shape12,
                       Shape13, Shape14, Shape15, Shape16,
                       Shape17, Shape18, Shape19, Shape20,
                       Shape21, Shape22, Shape23, Shape24,
                       Shape25, Shape26, Shape27, Shape28,
                       Shape29, Shape30, Shape31, Shape32,
                       Shape33, Shape34, Shape35, Shape36,
                       Shape37, Shape38, Shape39, Shape40,
                       Shape41, Shape42, Shape43, Shape44,
                       Shape45, Shape46, Shape47, Shape48,
                       Shape49, Shape50, Shape51, Shape52,
                       Shape53, Shape54, Shape55, Shape56,
                       Shape57, Shape58, Shape59, Shape60,
                       Shape61, Shape62, Shape63, Shape64,
                       Shape65, Shape66, Shape67, Shape68,
                       Shape69, Shape70, Shape71, Shape72,
                       Shape73, Shape74, Shape75, Shape76,
                       Shape77, Shape78, Shape79, Shape80,
                       Shape81, Shape82, Shape83, Shape84,
                       Shape85, Shape86, Shape87, Shape88,
                       Shape89, Shape90, Shape91, Shape92,
                       Shape93, Shape94, Shape95, Shape96
                       };

unsigned int palofs;
palofs = Areas[ListBox1->ItemIndex].palofs - 0x08000 + Areas[ListBox1->ItemIndex].ROMofs;
NESPalToRGB(Palette, &fileptr[palofs]);

for (int i=0; i<32; i++)
    Shapes[i]->Brush->Color = (TColor)(Palette[(i << 2) + 2] + (Palette[(i << 2) + 1] << 8) + (Palette[(i << 2)] << 16));
}
//---------------------------------------------------------------------------

