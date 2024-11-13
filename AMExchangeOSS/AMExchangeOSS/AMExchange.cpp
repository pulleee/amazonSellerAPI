//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
USEFORM("Unit1.cpp", frmMain);
USEFORM("FVersanddaten.cpp", frmVersanddaten);
USEFORM("..\..\..\Packages\SLNativ\FEingabeDialog.cpp", frmEingabeDialog);
USEFORM("..\..\..\Packages\SLNativ\FMemoEingabeDialog.cpp", frmMemoEingabeDialog);
USEFORM("..\..\..\Packages\SLNativ\FSQLAuswahl.cpp", frmSQLAuswahl);
//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		TStyleManager::TrySetStyle("Windows10");
		Application->CreateForm(__classid(TfrmMain), &frmMain);
		Application->CreateForm(__classid(TfrmVersanddaten), &frmVersanddaten);
		Application->CreateForm(__classid(TfrmEingabeDialog), &frmEingabeDialog);
		Application->CreateForm(__classid(TfrmMemoEingabeDialog), &frmMemoEingabeDialog);
		Application->CreateForm(__classid(TfrmSQLAuswahl), &frmSQLAuswahl);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
