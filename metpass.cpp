//---------------------------------------------------------------------------
#include <vcl.h>
#include <string.h>
#pragma hdrstop

#include "metpass.h"
#include "metpass1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TPassForm *PassForm;

char ASCpassword[25];
char BINpassword[24];
char ResultPassword[18];

//---------------------------------------------------------------------------
__fastcall TPassForm::TPassForm(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------



void __fastcall TPassForm::BitBtn1Click(TObject *Sender)
{

if (ConvertPasswordASCBin() == false)
    {
    Application->MessageBox("Illegal character(s) in password.", "Input error", 0);
    return;
    }

ConvertPassword6bit8bit();
DecodePassword();
char chksum = CalcCheckSum();
if (chksum != ResultPassword[17])
    {
    Application->MessageBox("Invalid password. Checksum is not correct.","Password error",0);
    return;
    }

for (int i=0; i<128; i++)
    CheckListBox1->Checked[i] = (bool)(ResultPassword[i >> 3] & (1 << (i & 7)));

}
//---------------------------------------------------------------------------

void __fastcall TPassForm::FormCreate(TObject *Sender)
{
ResultPassword[16] = 8;
}
//---------------------------------------------------------------------------




void __fastcall TPassForm::CheckListBox1ClickCheck(TObject *Sender)
{

for (int i=0; i<16; i++)
    ResultPassword[i] = 0;

for (int i=0; i<128; i++)
    ResultPassword[i >> 3] |= (char)(CheckListBox1->State[i] << (i & 7));

ResultPassword[17] = CalcCheckSum();

EncodePassword();
ConvertPassword8bit6bit();
ConvertPasswordBinASC();
MaskEdit1->Text = ASCpassword;
BitBtn1->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TPassForm::MaskEdit1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
wsprintf(ASCpassword, "%s", MaskEdit1->Text);
int pwdlength = strlen(ASCpassword);
if (pwdlength == 24)
    BitBtn1->Enabled = true;
    else BitBtn1->Enabled = false;
}
//---------------------------------------------------------------------------

