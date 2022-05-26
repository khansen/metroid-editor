//---------------------------------------------------------------------------
#ifndef metpassH
#define metpassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <checklst.hpp>
//---------------------------------------------------------------------------
class TPassForm : public TForm
{
__published:	// IDE-managed Components
    TMaskEdit *MaskEdit1;
    TBitBtn *BitBtn1;
    TBevel *Bevel1;
    TCheckListBox *CheckListBox1;
    void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    
    
    void __fastcall CheckListBox1ClickCheck(TObject *Sender);
    void __fastcall MaskEdit1KeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
    __fastcall TPassForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPassForm *PassForm;
//---------------------------------------------------------------------------
#endif
