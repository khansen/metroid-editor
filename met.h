//---------------------------------------------------------------------------

typedef struct {
    unsigned char mapposx;
    unsigned char mapposy;
    unsigned int roomptrofs;
    unsigned int structptrofs;
    unsigned char numstruct;
    unsigned int macroofs;
    unsigned int itemofs;
    unsigned int palofs;
    unsigned int ROMofs;
    unsigned int frameptrofs;
    unsigned int sprposptrofs;
    unsigned int frame2ptrofs;
    unsigned int sprpos2ptrofs;
    unsigned int anim2ofs;
    unsigned char numenemies;
    unsigned char numrooms;
    unsigned char *frametable;
    unsigned int *gfxptr;
    unsigned char *coordptr;
} MetroidArea;

#define BRINSTAR 0
#define NORFAIR 1
#define TOURIAN 2
#define KRAID 3
#define RIDLEY 4

//---------------------------------------------------------------------------
#ifndef metH
#define metH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TEditForm : public TForm
{
__published:	// IDE-managed Components
    TMainMenu *MainMenu1;
    TMenuItem *File1;
    TMenuItem *Open1;
    TMenuItem *Exit1;
    TOpenDialog *OpenDialog1;
    TMenuItem *Save1;
    TStatusBar *StatusBar1;
    TMenuItem *N1;
    TMenuItem *Options1;
    TMenuItem *MapEditor1;
    TMenuItem *ItemEditor1;
    TMenuItem *Help1;
    TMenuItem *Contents1;
    TMenuItem *PaletteEditor1;
    TMenuItem *Edit1;
    TMenuItem *Insert1;
    TMenuItem *Object1;
    TMenuItem *Enemy1;
    TMenuItem *PasswordCreator1;
    TMenuItem *Window1;
    TMenuItem *Size1;
    TMenuItem *N1x1;
    TMenuItem *N2x1;
    TMenuItem *About1;
    TSaveDialog *SaveDialog1;
    TMenuItem *SaveAs1;
    void __fastcall Open1Click(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall Save1Click(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
    void __fastcall FormMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall MapEditor1Click(TObject *Sender);
    void __fastcall ItemEditor1Click(TObject *Sender);
    void __fastcall Contents1Click(TObject *Sender);
    void __fastcall PaletteEditor1Click(TObject *Sender);
    void __fastcall PasswordCreator1Click(TObject *Sender);
    void __fastcall N1x1Click(TObject *Sender);
    void __fastcall N2x1Click(TObject *Sender);
    void __fastcall About1Click(TObject *Sender);
    void __fastcall SaveAs1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TEditForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditForm *EditForm;
//---------------------------------------------------------------------------
#endif
