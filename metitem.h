//---------------------------------------------------------------------------
#ifndef metitemH
#define metitemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TItemForm : public TForm
{
__published:	// IDE-managed Components
    TListBox *ListBox1;
    TListBox *ListBox2;
    void __fastcall FormPaint(TObject *Sender);
    void __fastcall ListBox1Click(TObject *Sender);
    void __fastcall ListBox2Click(TObject *Sender);
    
    void __fastcall FormMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TItemForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TItemForm *ItemForm;
//---------------------------------------------------------------------------
#endif
