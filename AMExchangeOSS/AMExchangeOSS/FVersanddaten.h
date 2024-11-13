//---------------------------------------------------------------------------

#ifndef FVersanddatenH
#define FVersanddatenH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
#include "BaseGrid.hpp"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TfrmVersanddaten : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TPanel *Panel1;
	TStatusBar *StatusBar1;
	TAdvStringGrid *grd;
	TButton *Button1;
	TButton *Button2;
	TSaveDialog *SaveDialog1;
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TfrmVersanddaten(TComponent* Owner);

  void loadData(void);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmVersanddaten *frmVersanddaten;
//---------------------------------------------------------------------------
#endif
