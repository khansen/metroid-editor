//---------------------------------------------------------------------------
#ifndef metmapH
#define metmapH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TMapForm : public TForm
{
__published:	// IDE-managed Components
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormPaint(TObject *Sender);
    
    
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TMapForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMapForm *MapForm;
//---------------------------------------------------------------------------
#endif
