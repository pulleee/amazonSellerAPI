//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Data.Bind.Components.hpp>
#include <Data.Bind.ObjectScope.hpp>
#include <IPPeerClient.hpp>
#include <REST.Client.hpp>
#include <REST.Types.hpp>
#include "AdvGrid.hpp"
#include "AdvObj.hpp"
#include "AdvUtil.hpp"
#include "BaseGrid.hpp"
#include <Vcl.Grids.hpp>
#include "AdvEdit.hpp"
#include <Vcl.Dialogs.hpp>

using namespace std;

//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// Von der IDE verwaltete Komponenten
	TStatusBar *StatusBar1;
	TMainMenu *MainMenu1;
	TMenuItem *Datei1;
	TMenuItem *Beenden1;
	TADOConnection *ADOConnection1;
	TProgressBar *ProgressBar1;
	TPanel *Panel1;
	TMemo *memo;
	TRESTResponse *RESTResponse1;
	TRESTRequest *RESTRequest1;
	TRESTClient *RESTClient1;
	TMenuItem *API1;
	TMenuItem *CheckServiceStatus1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *GetRequestReportList1;
	TMenuItem *GetReport1;
	TMenuItem *GetReportList1;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *GetOrderList1;
	TMenuItem *GetOrder1;
	TGroupBox *grboxShopOrders;
	TLabel *Label3;
	TLabel *Label9;
	TButton *btnGetShopOrders;
	TCheckBox *cbxGetOrderAuto;
	TAdvEdit *edAutoOrderTime;
	TButton *btnUploadVersand;
	TCheckBox *cbxSendVersandRAuto;
	TAdvEdit *edAutoVersandRTime;
	TGroupBox *grboxStammdatenVeraenderung;
	TLabel *Label2;
	TButton *btnArtikelAktualisieren;
	TCheckBox *cbxSendArtikelAuto;
	TAdvEdit *edAutoArtikelTime;
	TCheckBox *cbNurGeaenderte;
	TTimer *timerAMGeneral;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *ImportFLATReport1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Logleeren1;
	TMenuItem *GetOrderItems1;
	TMenuItem *N8;
	TMenuItem *N9;
	TMenuItem *GetFeedSubmissionList1;
	TMenuItem *UpdateReportAcknowledgements1;
	TMenuItem *UploadInvoiceToOrderr1;
	TMenuItem *Debuglevel2;
	TMenuItem *N10;
	TMenuItem *CheckRechnungsupload1;
	TMenuItem *UpdateInternalOrdeswithFLATReport1;
	TMenuItem *CheckAndCreatePendingOrders1;
	TMenuItem *Versand1;
	TMenuItem *Versand2;
	TMenuItem *N11;
	TMenuItem *N12;
	TMenuItem *N13;
	TMenuItem *CheckProcedure1;
	void __fastcall Beenden1Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall Debuglevel1Click(TObject *Sender);
	void __fastcall Artikel1Click(TObject *Sender);
	void __fastcall CheckServiceStatus1Click(TObject *Sender);
	void __fastcall GetRequestReportList1Click(TObject *Sender);
	void __fastcall GetReport1Click(TObject *Sender);
	void __fastcall GetReportList1Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall GetOrder1Click(TObject *Sender);
	void __fastcall btnGetShopOrdersClick(TObject *Sender);
	void __fastcall cbxSendArtikelAutoClick(TObject *Sender);
	void __fastcall cbxSendVersandRAutoClick(TObject *Sender);
	void __fastcall cbxGetOrderAutoClick(TObject *Sender);
	void __fastcall timerAMGeneralTimer(TObject *Sender);
	void __fastcall btnArtikelAktualisierenClick(TObject *Sender);
	void __fastcall btnUploadVersandClick(TObject *Sender);
	void __fastcall N7Click(TObject *Sender);
	void __fastcall ImportFLATReport1Click(TObject *Sender);
	void __fastcall Logleeren1Click(TObject *Sender);
	void __fastcall GetOrderItems1Click(TObject *Sender);
	void __fastcall GetOrderList1Click(TObject *Sender);
	void __fastcall N9Click(TObject *Sender);
	void __fastcall GetFeedSubmissionList1Click(TObject *Sender);
	void __fastcall UpdateReportAcknowledgements1Click(TObject *Sender);
	void __fastcall N2Click(TObject *Sender);
	void __fastcall UploadInvoiceToOrderr1Click(TObject *Sender);
	void __fastcall CheckRechnungsupload1Click(TObject *Sender);
	void __fastcall UpdateInternalOrdeswithFLATReport1Click(TObject *Sender);
	void __fastcall CheckAndCreatePendingOrders1Click(TObject *Sender);
	void __fastcall Versand2Click(TObject *Sender);
	void __fastcall RESTRequest1AfterExecute(TCustomRESTRequest *Sender);
	void __fastcall N13Click(TObject *Sender);
	void __fastcall CheckProcedure1Click(TObject *Sender);
private:	// Benutzer-Deklarationen
public:		// Benutzer-Deklarationen
	__fastcall TfrmMain(TComponent* Owner);
	bool __fastcall log(AnsiString l, bool bTimeStamp=true, bool bHidden=false);

	bool __fastcall doAMOrderDownload(AnsiString strOrderId);
	void __fastcall doAMArtikelUpload(AnsiString strArtikel, bool bCheckMediaUpload);
  void __fastcall doAMBestandBatchUpload(void);
	void __fastcall doAMBestandBatchUploadInc(void);
	void __fastcall doAMVersandStatusUpdate(AnsiString strOrderNumber);
  void __fastcall doAMRechnungsUpload(AnsiString strOrderNumber);

	bool __fastcall checkAndCreatePendingOrders(bool bDoImport);
	bool __fastcall checkAndCreateOrder(AnsiString sOrderId, bool bDoImport);
	bool __fastcall processReport(AnsiString strReportId);
	bool __fastcall createInternalOrders(TStringList *slContent);
  bool __fastcall updateInternalOrders(TStringList *slContent);
	bool __fastcall acknowledgeReportOK(AnsiString strReportId);
	bool __fastcall acknowledgeReportError(AnsiString strReportId);
	int  __fastcall checkUnloadedReports(void);
	int  __fastcall getUnprocessedReports(TStringList *slReports);

  //=====================================

	#include "AMRestAPI.h"

	#include "SLOrder.h"

	//=====================================


	int _iDebugLevel;
	bool _bSQLWindowsLogin, _bLogFlag;
	AnsiString _strServer, _strMandant, _strPassword, _strUser, _strLogFile;
  bool _bUseCOM;

	TDateTime _tdAMLastArticleUpload;
	TDateTime _tdAMLastKundenUpload;

	AnsiString _strAMArtikelSQL, _strAMGruppenSQL, _strAMKundenSQL, _strAMStatusSQL, _strAMRechnunsUploadSQL;

	AnsiString _strAMOrderUpdateProzedure;
	AnsiString _strAMOrderVersendetProzedure;
  AnsiString _strAMRechungUploadProzedure;

	int _iAutoOrderCheckMinutes, _iAutoArtikelUploadMinutes, _iAutoVersandRueckmeldungsMinutes;
	TDateTime _tdLastArticleUpload;

	TStringList *_slUpdateBeleg;
	bool _bRechnungsUploadEnabled;
	bool _bUseSLAdrFirma;

  AnsiString _strSteuerQuelle;

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
