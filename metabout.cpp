//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "metabout.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::Button1Click(TObject *Sender)
{
AboutForm->Visible = false;
}
//---------------------------------------------------------------------------
