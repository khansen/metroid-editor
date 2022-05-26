//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USERES("metedit.res");
USEFORM("met.cpp", EditForm);
USEFORM("metmap.cpp", MapForm);
USEFORM("metitem.cpp", ItemForm);
USEFORM("metpal.cpp", PalForm);
USEFORM("metpass.cpp", PassForm);
USEASM("metpass1.asm");
USEFORM("metabout.cpp", AboutForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    try
    {
        Application->Initialize();
        Application->HelpFile = "C:\\helll\\Kent.hlp";
        Application->CreateForm(__classid(TEditForm), &EditForm);
        Application->CreateForm(__classid(TMapForm), &MapForm);
        Application->CreateForm(__classid(TItemForm), &ItemForm);
        Application->CreateForm(__classid(TPalForm), &PalForm);
        Application->CreateForm(__classid(TPassForm), &PassForm);
        Application->CreateForm(__classid(TAboutForm), &AboutForm);
        Application->Run();
    }
    catch (Exception &exception)
    {
        Application->ShowException(&exception);
    }
    return 0;
}
//---------------------------------------------------------------------------
