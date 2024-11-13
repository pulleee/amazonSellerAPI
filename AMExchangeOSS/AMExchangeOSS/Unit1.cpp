//---------------------------------------------------------------------------
// V1.18 pk 11072022 Fehler in Versionsabfrage bei OSS behoben

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <Xml.hpp>

#include <inifiles.hpp>
#include <strutils.hpp>
#include <dateutils.hpp>
#include <fstream.h>
#include <System.Hash.hpp>
#include <System.NetEncoding.hpp>

#include "wawi.h"
#include "FEingabeDialog.h"
#include "FMemoEingabeDialog.h"
#include "FSQLAuswahl.h"

#include "FVersanddaten.h"

using namespace std;

static int _iOrdersSekunden=0;
static int _iArtikelSekunden=0;
static int _iVersandSekunden=0;
static int _iCheckStornoSekunden=0;

#include <map>
#include <vector>

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma link "BaseGrid"
#pragma link "AdvEdit"
#pragma resource "*.dfm"
#pragma link "XML"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
	AnsiString s;

	slMarketPlaces=new TStringList();

	_slUpdateBeleg=new TStringList();

	TIniFile *ini;
	ini=new TIniFile(ChangeFileExt(Application->ExeName, ".INI"));

	_strServer=ini->ReadString("APP", "Server", "");
	_strMandant=ini->ReadString("APP", "Mandant", "SL_MWAWI");
	if(!AnsiContainsStr(_strMandant.UpperCase(), "SL_M"))
		_strMandant="SL_M"+_strMandant;
	_strPassword=ini->ReadString("APP", "Passwort", "wawi");
	_strUser=ini->ReadString("APP", "User", "sa");
	_bSQLWindowsLogin=ini->ReadBool("APP", "Login", true);
	_bLogFlag=ini->ReadBool("APP", "Log", true);
	_strLogFile=ini->ReadString("APP", "Logfile", ChangeFileExt(Application->ExeName, ".log"));

	_iDebugLevel=ini->ReadInteger("APP", "DebugLevel", 1);
	_bUseCOM=ini->ReadBool("APP", "UseCOM", true);

	// Amazon
	aws.sAWSAccessKeyId=ini->ReadString("AWS", "AcessKey", "AKIAJY66HSM2BXQE2GAA");
	aws.sAWSSecret=ini->ReadString("AWS", "Secret", "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");
	aws.sMWSAuthToken=ini->ReadString("AWS", "Token", "");
	aws.sSellerId=ini->ReadString("AWS", "SellerId", "");

	ini->ReadSectionValues("MARKETPLACES", slMarketPlaces);

	_strAMArtikelSQL=ini->ReadString("APP", "Artikel", "");
	_strAMGruppenSQL=ini->ReadString("APP", "Gruppen", "");
	_strAMKundenSQL=ini->ReadString("APP", "Kunden", "");
	_strAMStatusSQL=ini->ReadString("APP", "Status", "");
	_strAMRechnunsUploadSQL=ini->ReadString("APP", "RechnungUpload", "");

	_tdAMLastArticleUpload=ini->ReadDateTime("APP", "LetzterArtikelUpload", IncHour(Now(), -1));

	_strAMOrderUpdateProzedure=ini->ReadString("APP", "OrderAMUpdateProzedure", "");
	_strAMOrderVersendetProzedure=ini->ReadString("APP", "OrderAMVersendetProzedure", "");
	_strAMRechungUploadProzedure=ini->ReadString("APP", "OrderAMRechnungUploadProzedure", "");

	_bUseSLAdrFirma=ini->ReadBool("APP", "UseSLAdrFirma", false);

	// Automatismen
	_iAutoOrderCheckMinutes=ini->ReadInteger("APP", "AutoOrderMinutes", 5);
	edAutoOrderTime->FloatValue=_iAutoOrderCheckMinutes;
	_iAutoArtikelUploadMinutes=ini->ReadInteger("APP", "AutoArtikelUploadMinutes", 60);
	edAutoArtikelTime->FloatValue=_iAutoArtikelUploadMinutes;
	_iAutoVersandRueckmeldungsMinutes=ini->ReadInteger("APP", "AutoVersandRueckmeldungMinutes", 60);
	edAutoVersandRTime->FloatValue=_iAutoVersandRueckmeldungsMinutes;

	_iAutoCheckStornosMinutes=ini->ReadInteger("APP", "AutoCheckStornosMinutes", 15);
	edAutoCheckStronosTime->FloatValue=_iAutoCheckStornosMinutes;

	_tdLastArticleUpload=ini->ReadDateTime("APP", "LetzterArtikelUpload", IncHour(Now(), -1));

	cbxGetOrderAuto->Checked=ini->ReadBool("APP", "AutoOrder", false);
	cbxSendArtikelAuto->Checked=ini->ReadBool("APP", "AutoArtikel", false);
	cbxSendVersandRAuto->Checked=ini->ReadBool("APP", "AutoVersandRueckmeldung", false);

	cbxCheckStornosAuto->Checked=ini->ReadBool("APP", "AutoCheckStornos", false);

	ini->ReadSectionValues("UpdateBeleg", _slUpdateBeleg);
	s=_slUpdateBeleg->Text;

	grboxStammdatenVeraenderung->Enabled=ini->ReadBool("APP", "ArtikelUploadEnabled", true);
	_bRechnungsUploadEnabled=ini->ReadBool("APP", "RechnungsUploadEnabled", true);
	_bAMPreisUploadEnabled=ini->ReadBool("APP", "PreisUploadEnabled", false);

	_strSteuerQuelle=ini->ReadString("Order", "SteuerQuelle", "SL");

	delete ini;

	StatusBar1->Panels->Items[0]->Text="DB:" + _strMandant;
	StatusBar1->Panels->Items[1]->Text="User:" + _strUser;

	// locale Connection setzen
	if(!_bSQLWindowsLogin)
		{
		ADOConnection1->ConnectionString=
			"Provider=SQLOLEDB.1;Persist Security Info=True;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Use Encryption for Data=False;Tag with column collation when possible=False;";
		s="User ID=" + _strUser;
		ADOConnection1->ConnectionString=s + ";" + ADOConnection1->ConnectionString;
		s="Password=" + _strPassword;
		ADOConnection1->ConnectionString=s + ";" + ADOConnection1->ConnectionString;
		s="Data Source=" + _strServer;
		ADOConnection1->ConnectionString=ADOConnection1->ConnectionString + ";" + s;
		s="Initial Catalog=" + _strMandant;
		ADOConnection1->ConnectionString=ADOConnection1->ConnectionString + ";" + s;
		ADOConnection1->ConnectionString=ADOConnection1->ConnectionString + ";";
		}
	else
		{
		ADOConnection1->ConnectionString=
			"Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Use Procedure for Prepare=1;Auto Translate=True;Packet Size=4096;Use Encryption for Data=False;Tag with column collation when possible=False;";
		s="Data Source=" + _strServer;
		ADOConnection1->ConnectionString=ADOConnection1->ConnectionString + ";" + s;
		s="Initial Catalog=" + _strMandant;
		ADOConnection1->ConnectionString=ADOConnection1->ConnectionString + ";" + s;
		ADOConnection1->ConnectionString=ADOConnection1->ConnectionString + ";";
		}

	TRect tr;
	ProgressBar1->Parent=StatusBar1;
	SendMessageA(StatusBar1->Handle, SB_GETRECT, 2, (long) &tr);
	ProgressBar1->SetBounds(tr.left, tr.top, tr.Width(), tr.Height());
	ProgressBar1->Visible=false;

	StatusBar1->Panels->Items[0]->Text="Mandant: "+AnsiReplaceStr(_strMandant, "SL_M", "");
	StatusBar1->Panels->Items[1]->Text="User: "+_strUser;

	log("Startlocation:"+ExtractFileDir(Application->ExeName));

	timerAMGeneral->Enabled=true;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::FormDestroy(TObject *Sender)
{
	ADOConnection1->Close();

	TIniFile *ini;
	ini=new TIniFile(ChangeFileExt(Application->ExeName, ".INI"));

	ini->WriteInteger("APP", "AutoOrderMinutes", edAutoOrderTime->FloatValue);
	ini->WriteInteger("APP", "AutoArtikelUploadMinutes",edAutoArtikelTime->FloatValue);
	ini->WriteInteger("APP", "AutoVersandRueckmeldungMinutes", edAutoVersandRTime->FloatValue);
	ini->WriteInteger("APP", "AutoCheckStornosMinutes", edAutoCheckStronosTime->FloatValue);

	ini->WriteBool("APP", "AutoOrder", cbxGetOrderAuto->Checked);
	ini->WriteBool("APP", "AutoArtikel", cbxSendArtikelAuto->Checked);
	ini->WriteBool("APP", "AutoVersandRueckmeldung", cbxSendVersandRAuto->Checked);
	ini->WriteBool("APP", "AutoCheckStornos", cbxCheckStornosAuto->Checked);

	delete ini;

	delete _slUpdateBeleg;

  stopCOM();

	exit(1);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::log(AnsiString l, bool bTimeStamp, bool bHidden)
{
	ofstream log;

	if(bTimeStamp)
		{
		l=Now().FormatString("dd.mm hh:nn:ss")+" => "+l;
		}

	if(_bLogFlag)
		{
		log.open(_strLogFile.c_str(), ios::out|ios::app|ios::binary);
		if(!log)
			memo->Lines->Add("Fehler beim öffnen der Logdatei!");
		else
			{
			log << l.c_str() << "\r\n";
			log.close();
			}
		}
	if(!bHidden)
		memo->Lines->Add(l);

	return(true);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Beenden1Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
// Basisroutinen
//---------------------------------------------------------------------------

bool __fastcall TfrmMain::doAMOrderDownload(AnsiString strOrderId)
{
	AnsiString s, strReportId;
	int iUnloadedReports, iUnprocessedReports;
	int iNewOrders;
	bool bSt=true;

	TStringList *slUnprocessedReports=new TStringList();

	iUnloadedReports=checkUnloadedReports();                           // prüfe alle ungeladenen Reports

	iUnprocessedReports=getUnprocessedReports(slUnprocessedReports);   // aus eigener Reportsverwaltung laden

	/*
	if(iUnprocessedReports == 0)                                       // wenn kein Report mehr da ist zum Abarbeiten, dann einen anfordern
		{
		bSt=requestAMReport("_GET_FLAT_FILE_ORDERS_DATA_", 3);

		delete slUnprocessedReports;
		return(bSt);
		}
	*/

	for(int i=0; i < slUnprocessedReports->Count && i < 10; i++)        // maximal 5 Reports in einem Durchlauf
		{
		strReportId=slUnprocessedReports->Strings[i];

		bSt=processReport(strReportId);                                  // alle Orders werden in die interne Tabelle abgelegt
		if(!bSt)
			{
			log("Report:"+strReportId+" mit Fehlern verarbeitet!");
			acknowledgeReportError(strReportId);
			}
		else
			acknowledgeReportOK(strReportId);                              // wenn der Report sauber verarbeitet wurde, wird er bestätig und taucht dann nicht mehr auf
		}

	iNewOrders=checkAndCreatePendingOrders(true);                      // unverarbeitete Orders prüfen und anlegen

	delete slUnprocessedReports;
	return(bSt);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMArtikelUpload(AnsiString strArtikel, bool bCheckMediaUpload)
{
	AnsiString s, strFile, strMediaFiles,strMediaDescription;
	bool bSt;
	int iMediaId;
	int iChangedCount;
	bool bSingleMode;
	TDateTime tdAbfrageZeitpunkt=Now();

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;
	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;

	if(strArtikel == "")
		{
		bSingleMode=false;
		s=_strAMArtikelSQL;
		if(cbNurGeaenderte->Checked)
			s+=" and geaendertam > '"+_tdLastArticleUpload.DateTimeString()+"' ";
		}
	else
		{
		bSingleMode=true;
		s="select artikelnummer from art where artikelnummer='"+strArtikel+"'";
		}

	q->SQL->Text=s;
	q->Open();
	if(!bSingleMode)
		{
		ProgressBar1->Visible=true;
		ProgressBar1->Position=0;
		ProgressBar1->Max=q->RecordCount;
		}
	for(int i=0; i < q->RecordCount; i++)
		{
		if(!bSingleMode)
			ProgressBar1->Position=i;

		strArtikel=q->FieldByName("Artikelnummer")->AsString;

		log("Artikel:"+strArtikel);

		q->Next();
		}
	q->Close();

	delete q;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMBestandBatchUpload(void)
{
	AnsiString s, strArtikel;
	AnsiString strStueckliste, strVariantenartikel;
	UnicodeString sInventoryFeed, sXML;
	bool bSt;
	int iUpdateCount=0;
	int iInStock;

	TStringList *slUpdates=new TStringList();

	TDateTime tdAbfrageZeitpunkt=Now();

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="select replace(sku, '&', '&amp;') sku, InStock from dbo.pkAM_art "
		"where isnull(asin, '') != ''";
	q->SQL->Text=s;
	q->Open();

	for(int i=0; i < q->RecordCount; i++)
		{
		strArtikel=q->FieldByName("SKU")->AsString;
		iInStock=q->FieldByName("InStock")->AsInteger;
		if(iInStock < 0)
			iInStock=0;

		sXML+=
			"<Message>"
			"<MessageID>"+IntToStr(i+1)+"</MessageID>"
			"<OperationType>Update</OperationType>"
			"<Inventory>"
			"<SKU>"+strArtikel+"</SKU>"
			"<Quantity>"+IntToStr(iInStock)+"</Quantity>"
			"<FulfillmentLatency>2</FulfillmentLatency>"
			"</Inventory>"
			"</Message>";

		iUpdateCount++;

		q->Next();
		}

	if(iUpdateCount)  // wenn es was zum Updaten gab
		{
		sInventoryFeed=
			//"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
			"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
			"<AmazonEnvelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"amzn-envelope.xsd\">"
			"<Header>"
			"<DocumentVersion>1.01</DocumentVersion>"
			"<MerchantIdentifier>"+aws.sSellerId+"</MerchantIdentifier>"
			"</Header>"
			"<MessageType>Inventory</MessageType>";

		sInventoryFeed+=sXML;

		sInventoryFeed+=
			"</AmazonEnvelope>";

		s=submitAMFeed("_POST_INVENTORY_AVAILABILITY_DATA_", sInventoryFeed);
		if(s == "_SUBMITTED_")
			log("Bestandsaktualisierung erfolgreich übertragen!");
		}

	log(IntToStr(iUpdateCount)+" Artikel upgedated!");

	delete q;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMPreisFeedUpload(void)
{
	AnsiString s, strArtikel;
	AnsiString strStueckliste, strVariantenartikel;
	UnicodeString sPriceFeed, sXML;
	bool bSt;
	int iUpdateCount=0;
	double dPreis;

	TStringList *slUpdates=new TStringList();

	TDateTime tdAbfrageZeitpunkt=Now();

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="select replace(sku, '&', '&amp;') sku, AmazonPreis from dbo.pkAM_art "
		"where isnull(asin, '') != ''";
	q->SQL->Text=s;
	q->Open();

	for(int i=0; i < q->RecordCount; i++)
		{
		strArtikel=q->FieldByName("SKU")->AsString;
		dPreis=q->FieldByName("AmazonPreis")->AsFloat;

		sXML+=
			"<Message>"
			"<MessageID>"+IntToStr(i+1)+"</MessageID>"
			"<OperationType>Update</OperationType>"
			"<Price>"
			"<SKU>"+strArtikel+"</SKU>"
			"<StandardPrice currency=\"EUR\">"+AnsiReplaceStr(FloatToStr(dPreis), ",", ".")+"</StandardPrice>"
			"</Price>"
			"</Message>";

		iUpdateCount++;

		q->Next();
		}

	if(iUpdateCount)  // wenn es was zum Updaten gab
		{
		sPriceFeed=
			//"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
			"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
			"<AmazonEnvelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"amzn-envelope.xsd\">"
			"<Header>"
			"<DocumentVersion>1.01</DocumentVersion>"
			"<MerchantIdentifier>"+aws.sSellerId+"</MerchantIdentifier>"
			"</Header>"
			"<MessageType>Price</MessageType>";

		sPriceFeed+=sXML;

		sPriceFeed+=
			"</AmazonEnvelope>";

		s=submitAMFeed("_POST_PRODUCT_PRICING_DATA_", sPriceFeed);
		if(s == "_SUBMITTED_")
			log("Preisaktualisierung erfolgreich übertragen!");
		}

	log(IntToStr(iUpdateCount)+" Artikel upgedated!");

	delete q;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMBestandBatchUploadInc(void)
{
	AnsiString s, strArtikel;
	AnsiString strStueckliste, strVariantenartikel;
	UnicodeString sInventoryFeed, sXML;
	bool bSt;
	int iUpdateCount=0;
	int iInStock;

	TStringList *slUpdates=new TStringList();

	TDateTime tdAbfrageZeitpunkt=Now();

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;
	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;

	// Normale Artikel (keine Varianten)
	s="select s.id, s.artikelnummer, a.stueckliste, a.Variantenartikel, a.InStock from pk_amupdate s "
		"left join dbo.pkAM_art a on (a.artikelnummer=s.artikelnummer) "
		//"left join pk_amartikel ar on (ar.artikelnummer=s.artikelnummer) "
		"where s.status='1' and isnull(a.asin, '') != '' ";
	q->SQL->Text=s;
	q->Open();

	for(int i=0; i < q->RecordCount; i++)
		{
		strArtikel=q->FieldByName("Artikelnummer")->AsString;
		iInStock=q->FieldByName("InStock")->AsInteger;

		if(slUpdates->IndexOf(strArtikel) < 0)
			{
			slUpdates->Add(strArtikel);

			log("Artikel:"+strArtikel);

			sXML+=
				"<Message>"
				"<MessageID>"+IntToStr(i+1)+"</MessageID>"
				"<OperationType>Update</OperationType>"
				"<Inventory>"
				"<SKU>"+strArtikel+"</SKU>"
				"<Quantity>"+IntToStr(iInStock)+"</Quantity>"
				"<FulfillmentLatency>2</FulfillmentLatency>"
				"</Inventory>"
				"</Message>";

			iUpdateCount++;
			}

		q->Next();
		}

	if(iUpdateCount)  // wenn es was zum Updaten gab
		{
		sInventoryFeed=
			//"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
			"<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>"
			"<AmazonEnvelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"amzn-envelope.xsd\">"
			"<Header>"
			"<DocumentVersion>1.01</DocumentVersion>"
			"<MerchantIdentifier>"+aws.sSellerId+"</MerchantIdentifier>"
			"</Header>"
			"<MessageType>Inventory</MessageType>";

		sInventoryFeed+=sXML;

		sInventoryFeed+=
			"</AmazonEnvelope>";

		s=submitAMFeed("_POST_INVENTORY_AVAILABILITY_DATA_", sInventoryFeed);
		if(s == "_SUBMITTED_")
			{
			q->First();
			for(int i=0; i < q->RecordCount; i++)
				{
				s="update pk_amupdate set status='2', done=GetDate() where id='"+q->FieldByName("id")->AsString+"'";
				q2->SQL->Text=s;
				q2->ExecSQL();

				q->Next();
				}
			q->Close();
			}
		}

	log(IntToStr(iUpdateCount)+" Artikel upgedated!");

	delete q2;
	delete q;

	delete slUpdates;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMCheckStornos(void)
{
	AnsiString s, strStatus, strBelegtyp, strBelegnummer, strOrderId;
	bool bSt;

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
	strBelegtyp=ini->ReadString("Order", "Belegtyp", "");
	delete ini;

	if(strBelegtyp == "")
		{
		Application->MessageBoxW(L"Belegtyp muss konfiguriert sein!", L"Fehler", MB_OK);
		return;
		}

	TStringList *slStornos=new TStringList();
	TStringList *slContent=new TStringList();
	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;
	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;

	log("Prüfe stornierte Belege!");

	bSt=getAMOrderListCanceled(3, slContent);
	if(bSt)
		{
		for(int i=0; i < slContent->Count; i++)
			{
			if(slContent->Names[i] == "ListOrdersResponse.ListOrdersResult.Orders.Order.AmazonOrderId")
				slStornos->Add(slContent->ValueFromIndex[i]);
			}

		for(int i=0; i < slStornos->Count; i++)
			{
			strOrderId=slStornos->Strings[i];

			s="select * from beleg where belegtyp='"+strBelegtyp+"' and ihrauftrag='"+strOrderId+"' and status < '3'";
			q->SQL->Text=s;
			q->Open();
			if(q->RecordCount)
				{
				strBelegnummer=q->FieldByName("Belegnummer")->AsString;
				strStatus=q->FieldByName("Status")->AsString;
				if(strStatus == "2")
					log("Bestellung:"+strBelegnummer+" mit OrderId:"+strOrderId+" bereits weiterverarbeitet! Bitte Folgebelege prüfen!");
				else if(strStatus == "0")
					{
					log("Bestellung:"+strBelegnummer+" mit OrderId:"+strOrderId+" wird auf manuell erledigt gestellt!");

					TBeleg *beleg=new TBeleg(ADOConnection1);
					beleg->edit(strBelegtyp, strBelegnummer);
					beleg->dispose();
					delete beleg;
					}
				}
			}
		}

	delete q2;
	delete q;
	delete slContent;
	delete slStornos;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMVersandStatusUpdate(AnsiString strOrderNumber)
{
	AnsiString s, strBelegtyp, strBelegnummer, strSendungsnummer, strCarrier;
	UnicodeString sOrderFulfillmentFeed, sXML;
	bool bSt;
	int iUpdateCount=0;
	TStringList *slContent=new TStringList();
	TStringList *slStornos=new TStringList();

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;
	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;

	log("Prüfe Stornos!");

	if(getAMOrderListCanceled(3, slContent))
		{
		for(int i=0; i < slContent->Count; i++)
			{
			if(slContent->Names[i] == "ListOrdersResponse.ListOrdersResult.Orders.Order.AmazonOrderId")
				slStornos->Add(slContent->ValueFromIndex[i]);
			}
		}

	log("Versandstatusprüfung...");

	s=_strAMStatusSQL;
	if(AnsiContainsStr(s, ".sql"))
		{
		TStringList *slSQL=new TStringList();
		//slSQL->LoadFromFile(s);
		slSQL->LoadFromFile(ExtractFilePath(Application->ExeName)+s);
		s=slSQL->Text;
		delete slSQL;
		}
	if(strOrderNumber != "")
		s+=" and IhrAufrag='"+strOrderNumber+"' ";
	q->SQL->Text=s;
	q->Open();

	ProgressBar1->Visible=true;
	ProgressBar1->Position=0;
	ProgressBar1->Max=q->RecordCount;

	for(int i=0; i < q->RecordCount; i++)
		{
		ProgressBar1->Position=i;

		iUpdateCount++;

		strOrderNumber=q->FieldByName("OrderNumber")->AsString;

		if(slStornos->IndexOf(strOrderNumber) >= 0)
			{
			log("Order:"+strOrderNumber+" wurde wegen Storno nicht berücksichtigt!");
			q->Next();
			continue;
			}

		strBelegtyp=q->FieldByName("Belegtyp")->AsString;
		strBelegnummer=q->FieldByName("Belegnummer")->AsString;
		strSendungsnummer=q->FieldByName("Sendungsnummer")->AsString;
		strCarrier=q->FieldByName("Versender")->AsString;

		sXML+=
			"<Message>"
			"<MessageID>"+IntToStr(i+1)+"</MessageID>"
			"<OperationType>Update</OperationType>"
			"<OrderFulfillment>"
			"<AmazonOrderID>"+strOrderNumber+"</AmazonOrderID>"
			"<FulfillmentDate>"+DateToISO8601(Now(), false)+"</FulfillmentDate>"
			"<FulfillmentData>"
			"<CarrierName>"+strCarrier+"</CarrierName>"
			//"<ShippingMethod>xxxx</ShippingMethod>"
			"<ShipperTrackingNumber>"+strSendungsnummer+"</ShipperTrackingNumber>"
			"</FulfillmentData>"
			//<Item>
			//	<AmazonOrderItemCode>1233</AmazonOrderItemCode>
			//	<Quantity>1</Quantity>
			//</Item>
			"</OrderFulfillment>"
			"</Message>";

		q->Next();
		}

	ProgressBar1->Visible=false;

	if(iUpdateCount)
		{
		sOrderFulfillmentFeed=
			"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
			"<AmazonEnvelope xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xsi:noNamespaceSchemaLocation=\"amzn-envelope.xsd\">"
			"<Header>"
			"<DocumentVersion>1.01</DocumentVersion>"
			"<MerchantIdentifier>"+aws.sSellerId+"</MerchantIdentifier>"
			"</Header>"
			"<MessageType>OrderFulfillment</MessageType>";

		sOrderFulfillmentFeed+=sXML;

		sOrderFulfillmentFeed+=
			"</AmazonEnvelope>";

		s=submitAMFeed("_POST_ORDER_FULFILLMENT_DATA_", sOrderFulfillmentFeed);
		if(s == "_SUBMITTED_")
			{
			log("Versandstatus erfolgreich übertragen!");

			if(_strAMOrderVersendetProzedure != "")
				{
				q->First();
				for(int i=0; i < q->RecordCount; i++)
					{
					strOrderNumber=q->FieldByName("OrderNumber")->AsString;
					strBelegtyp=q->FieldByName("Belegtyp")->AsString;
					strBelegnummer=q->FieldByName("Belegnummer")->AsString;

					s="exec "+_strAMOrderVersendetProzedure+" N'"+strBelegtyp+"', N'"+strBelegnummer+"'";
					q2->SQL->Text=s;
					q2->ExecSQL();

					log("Order:"+strOrderNumber+" mit Beleg:"+strBelegnummer+" Belegtyp:"+strBelegtyp+" ausgeliefert!");

					q->Next();
					}
				q->Close();
				}
			}
		}

	log(IntToStr(iUpdateCount)+" Orders upgedated!");

	delete q2;
	delete q;
	delete slStornos;
	delete slContent;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::doAMRechnungsUpload(AnsiString strOrderNumber)
{
	AnsiString s, strBelegtyp, strBelegnummer, strSendungsnummer, strCarrier, strPDFDatei;
	bool bSt;
	int iUpdateCount=0;

	TDateTime td, tdUploadThrottle;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;
	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;

	log("Amazon Rechnungsupload...");

	s=_strAMRechnunsUploadSQL;
	if(AnsiContainsStr(s, ".sql"))
		{
		TStringList *slSQL=new TStringList();
		//slSQL->LoadFromFile(s);
		slSQL->LoadFromFile(ExtractFilePath(Application->ExeName)+s);
		s=slSQL->Text;
		delete slSQL;
		}
	if(strOrderNumber != "")
		s+=" and IhrAufrag='"+strOrderNumber+"' ";
	q->SQL->Text=s;
	q->Open();

	ProgressBar1->Visible=true;
	ProgressBar1->Position=0;
	ProgressBar1->Max=q->RecordCount;

	tdUploadThrottle=IncSecond(Now(), -3);

	for(int i=0; i < q->RecordCount && i < 20; i++)
		{
		ProgressBar1->Position=i;

		strOrderNumber=q->FieldByName("OrderNumber")->AsString;
		strBelegtyp=q->FieldByName("Belegtyp")->AsString;
		strBelegnummer=q->FieldByName("Belegnummer")->AsString;
		strSendungsnummer=q->FieldByName("Sendungsnummer")->AsString;
		strCarrier=q->FieldByName("Versender")->AsString;

		strPDFDatei=createSLPDF(strBelegtyp, strBelegnummer, "");
		if(strPDFDatei != "")
			{
			while(1)
				{
				if(tdUploadThrottle < Now())
					break;
				Sleep(1000);
				}
			bSt=uploadAMInvoiceEx(strOrderNumber, strBelegnummer, strPDFDatei);
			bSt=1;
			if(bSt)
				{
				tdUploadThrottle=IncSecond(Now(), 3);

				s="exec "+_strAMRechungUploadProzedure+" N'"+strBelegtyp+"', N'"+strBelegnummer+"'";
				q2->SQL->Text=s;
				q2->ExecSQL();

				log("Order:"+strOrderNumber+" mit Beleg:"+strBelegnummer+" Belegtyp:"+strBelegtyp+" Rechnung hochgeladen!");
				iUpdateCount++;
				}
			}

		q->Next();
		}

	ProgressBar1->Visible=false;

	log(IntToStr(iUpdateCount)+" Rechnungen upgeloaded!");

	delete q2;
	delete q;
}

//---------------------------------------------------------------------------
// Unterfunktionen
//---------------------------------------------------------------------------

bool __fastcall TfrmMain::checkAndCreatePendingOrders(bool bDoImport)
{
	AnsiString s, sFields, sOrderId;
	int iId;
	bool bSt;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	//s="select * from pk_amorders where processed is null order by id";
	s="select distinct order_id, count(*) as items from pk_amorders where processed is null group by order_id ";
	q->SQL->Text=s;
	q->Open();
	for(int o=0; o < q->RecordCount; o++)
		{
		//iId=q->FieldByName("id")->AsInteger;
		sOrderId=q->FieldByName("order_id")->AsString;

		bSt=checkAndCreateOrder(sOrderId, true);

		q->Next();
		}
	q->Close();

	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::checkAndCreateOrder(AnsiString sOrderId, bool bDoImport)
{
	AnsiString s, sSKU, strArtikelnummer, sOrderItemId;
	TDateTime td;
	int iId;
	bool bSt;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="select * from pk_amorders where order_id='"+sOrderId+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount == 0)
		{
		delete q;
		return(false);
		}

	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;
	TADOQuery *q3=new TADOQuery(this);
	q3->Connection=ADOConnection1;

	TStringList *slTaxIds=new TStringList();

	// Kopfdaten ermitteln
	shBData.sOrderNumber=sOrderId;
	log("OrderId:"+shBData.sOrderNumber);
	//shBData.tdOderDateTime=grd->Dates[2][r];
	//shBData.sKundennummer=grd->Cells[42][r];
	shBData.sKundennummer="guest";
	//log("Kundennummer:"+shBData.sKundennummer);
	shBData.sEMail=q->FieldByName("buyer_email")->AsString;
	adrSLBill.sEMail=q->FieldByName("buyer_email")->AsString;
	log("EMail:"+shBData.sEMail);

	s=q->FieldByName("purchase_date")->AsString;
	td=ISO8601ToDate(s, true);
	shBData.strOderDateTime=td.DateTimeString();

	shBData.sWaehrung=q->FieldByName("currency")->AsString;

	shBData.sZB="Amazon";

	s=q->FieldByName("is_prime")->AsString;
	if(s == "true")
		{
		s=q->FieldByName("is_premium_order")->AsString;
		if(s == "true")
			shBData.sLB="AmazonPrimePremium";
		else
			shBData.sLB="AmazonPrime";
		}
	else
		{
		s=q->FieldByName("is_premium_order")->AsString;
		if(s == "true")
			shBData.sLB="AmazonPremium";
		else
			shBData.sLB="Amazon";
		}

	adrSLBill.sAnrede="";
	adrSLBill.sFirma="";
	s=q->FieldByName("bill_name")->AsString;
	if(_bUseSLAdrFirma)
		{
		adrSLBill.sFirma=s;
		adrSLBill.sName="";
		adrSLBill.sVorname="";
		}
	else
		{
		if(AnsiContainsStr(s, " "))
			{
			adrSLBill.sVorname=s.SubString(1, s.AnsiPos(" ")-1);
			s.Delete(1, adrSLBill.sVorname.Length()+1);
			if(adrSLBill.sVorname.UpperCase() == "HERR" || adrSLBill.sVorname.UpperCase() == "FRAU" || adrSLBill.sVorname.UpperCase() == "DR." || adrSLBill.sVorname.UpperCase() == "FIRMA")
				{
				adrSLBill.sAnrede=adrSLBill.sVorname;
				adrSLBill.sVorname="";
				if(AnsiContainsStr(s, " "))
					{
					adrSLBill.sVorname=s.SubString(1, s.AnsiPos(" ")-1);
					s.Delete(1, adrSLBill.sVorname.Length()+1);
					}
				}
			adrSLBill.sName=s;
			}
		else
			{
			adrSLBill.sVorname="";
			adrSLBill.sName=s;
			}
		}
	s=q->FieldByName("bill_address_2")->AsString;
	if(s != "")
		{
		adrSLBill.sStrasse=s;
		adrSLBill.sZusatz=q->FieldByName("bill_address_1")->AsString;
		adrSLBill.sZusatz2=q->FieldByName("bill_address_3")->AsString;
		}
	else
		{
		adrSLBill.sStrasse=q->FieldByName("bill_address_1")->AsString;
		adrSLBill.sZusatz=q->FieldByName("bill_address_3")->AsString;
		adrSLBill.sZusatz2="";
		}
	adrSLBill.sPLZ=q->FieldByName("bill_postal_code")->AsString;
	adrSLBill.sOrt=q->FieldByName("bill_city")->AsString;
	adrSLBill.sLand=q->FieldByName("bill_country")->AsString;
	adrSLBill.sTelefon=q->FieldByName("buyer_phone_number")->AsString;
	//adrSLBill.sUstId=q->FieldByName("buyer_vat_number")->AsString;

	// UStId checken
	AnsiString sLieferLand, sUstId;
	sLieferLand=q->FieldByName("ship_country")->AsString;
	s=q->FieldByName("buyer_tax_registration_id")->AsString;
	slTaxIds->Text=AnsiReplaceStr(s, ",", "\r\n");  // in StringList falls mehrere durch , getrennt
	if(slTaxIds->Count == 0)
		sUstId=slTaxIds->Text;
	else
		{
		for(int i=0; i < slTaxIds->Count; i++)
			{
			s=slTaxIds->Strings[i];
			if(AnsiContainsStr(s, sLieferLand))
				{
				sUstId=s;
				break;
				}
			}
		if(sUstId == "")
			sUstId=slTaxIds->Strings[0];
		}

	adrSLBill.sUstId=sUstId;
	adrSLBill.sTelefon=q->FieldByName("buyer_phone_number")->AsString;
	//if(sUstId != "")
	//	log("UstId:"+sUstId+" LieferLand:"+sLieferLand);
	log("Rechnungsadresse:"+adrSLBill.sVorname+" - "+adrSLBill.sName+" - "+adrSLBill.sZusatz+" - "+adrSLBill.sStrasse+" - "+adrSLBill.sPLZ+" - "+adrSLBill.sOrt);

	//adrSLDel.sAnrede=grd->Cells[56][r];
	//adrSLDel.sFirma=grd->Cells[54][r];
	adrSLDel.sAnrede="";
	s=q->FieldByName("recipient_name")->AsString;
	if(_bUseSLAdrFirma)
		{
		adrSLDel.sFirma=s;
		adrSLDel.sName="";
		adrSLDel.sVorname="";
		}
	else
		{
		if(AnsiContainsStr(s, " "))
			{
			adrSLDel.sVorname=s.SubString(1, s.AnsiPos(" ")-1);
			s.Delete(1, adrSLDel.sVorname.Length()+1);
			if(adrSLDel.sVorname.UpperCase() == "HERR" || adrSLDel.sVorname.UpperCase() == "FRAU" || adrSLDel.sVorname.UpperCase() == "DR." || adrSLDel.sVorname.UpperCase() == "FIRMA")
				{
				adrSLDel.sAnrede=adrSLDel.sVorname;
				adrSLDel.sVorname="";
				if(AnsiContainsStr(s, " "))
					{
					adrSLDel.sVorname=s.SubString(1, s.AnsiPos(" ")-1);
					s.Delete(1, adrSLDel.sVorname.Length()+1);
					}
				}
			adrSLDel.sName=s;
			}
		else
			{
			adrSLDel.sVorname="";
			adrSLDel.sName=s;
			}
		}
	s=q->FieldByName("ship_address_2")->AsString;
	if(s != "")
		{
		adrSLDel.sStrasse=s;
		adrSLDel.sZusatz=q->FieldByName("ship_address_1")->AsString;
		adrSLDel.sZusatz2=q->FieldByName("ship_address_3")->AsString;
		}
	else
		{
		adrSLDel.sStrasse=q->FieldByName("ship_address_1")->AsString;
		adrSLDel.sZusatz=q->FieldByName("ship_address_3")->AsString;
		adrSLDel.sZusatz2="";
		}
	adrSLDel.sPLZ=q->FieldByName("ship_postal_code")->AsString;
	adrSLDel.sOrt=q->FieldByName("ship_city")->AsString;
	adrSLDel.sLand=q->FieldByName("ship_country")->AsString;
	//adrSLDel.sUstId=grd->Cells[65][r];
	log("Lieferadresse:"+adrSLDel.sVorname+" - "+adrSLDel.sName+" - "+adrSLDel.sZusatz+" - "+adrSLDel.sStrasse+" - "+adrSLDel.sPLZ+" - "+adrSLDel.sOrt);

	shBData.sKommentar=q->FieldByName("delivery_Instructions")->AsString;
	if(shBData.sKommentar != "")
		log("Kommentar:"+shBData.sKommentar);

	// Belegsummen ermitteln
	//s="select sum(item_price) GesamtBrutto, sum(shipping_price) GesamtVersandBrutto from pk_amorders where order_id='"+sOrderId+"' ";
	s="select sum(item_price) GesamtBrutto, sum(item_tax) GesamtSteuer, sum(shipping_price) GesamtVersandBrutto, sum(shipping_tax) VersandSteuer from pk_amorders where order_id='"+sOrderId+"' ";
	q2->SQL->Text=s;
	q2->Open();
	if(q2->RecordCount)
		{
		shBData.dGesamtBetragBrutto=q2->FieldByName("GesamtBrutto")->AsFloat;
		if(_strSteuerQuelle == "SL")  // dann muss der Brutto-Betrag zwingend stimmen
			{
			if(YearOf(Now()) >= 2021)
				shBData.dGesamtBetragNetto=shBData.dGesamtBetragBrutto/1.19;
			else
				shBData.dGesamtBetragNetto=shBData.dGesamtBetragBrutto/1.16;

			shBData.dVersandkostenBrutto=q2->FieldByName("GesamtVersandBrutto")->AsFloat;
			if(YearOf(Now()) >= 2021)
				shBData.dVersandkostenNetto=shBData.dVersandkostenBrutto/1.19;
			else
				shBData.dVersandkostenNetto=shBData.dVersandkostenBrutto/1.16;
			}
		else // wenn von Amazon genommen
			{
			shBData.dSteuerGesamtBetrag=q2->FieldByName("GesamtSteuer")->AsFloat;
			shBData.dGesamtBetragNetto=shBData.dGesamtBetragBrutto-shBData.dSteuerGesamtBetrag;

			shBData.dVersandkostenBrutto=q2->FieldByName("GesamtVersandBrutto")->AsFloat;
			shBData.dVersandkostenNetto=shBData.dVersandkostenBrutto-q2->FieldByName("VersandSteuer")->AsFloat;
			}
		}
	else
		{
		shBData.dGesamtBetragBrutto=0;
		shBData.dGesamtBetragNetto=0;
		shBData.dVersandkostenBrutto=0;
		shBData.dVersandkostenNetto=0;
		}
	q2->Close();

	if(bDoImport)
		{
		bSt=createSLOrder();
		if(!bSt)
			{
			log("Fehler beim Anlegen der Bestellung!");
			delete q2;
			delete q;
			return(bSt);
			}
		}

	// Positionen abarbeiten
	double dGesamtpreis;
	double dSteuerbetrag;
	//s="select * from pk_amorders where processed is null and order_id='"+sOrderId+"' order by id";
	s="select * from pk_amorders where order_id='"+sOrderId+"' order by id";
	q2->SQL->Text=s;
	q2->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		iId=q2->FieldByName("id")->AsInteger;
		sOrderItemId=q2->FieldByName("order_item_id")->AsString;

		sSKU=q2->FieldByName("sku")->AsString;

		s="select * from pkAM_art where sku='"+sSKU+"'";
		q3->SQL->Text=s;
		q3->Open();
		if(q3->RecordCount)
			strArtikelnummer=q3->FieldByName("artikelnummer")->AsString;
		else
			{
			log("Fehler: Artikel für SKU:"+sSKU+" wurde nicht gefunden!");
			strArtikelnummer="";
			}
		q3->Close();

		shPData.sArtikel=strArtikelnummer;
		shPData.sBezeichnung=q2->FieldByName("product_name")->AsString;
		shPData.dMenge=q2->FieldByName("quantity_purchased")->AsFloat;
		dGesamtpreis=q2->FieldByName("item_price")->AsFloat;
		shPData.dEinzelpreis=dGesamtpreis/shPData.dMenge;
		shPData.dVersandPerArtikel=q2->FieldByName("shipping_price")->AsFloat;
		dSteuerbetrag=q2->FieldByName("item_tax")->AsFloat;
		if(dSteuerbetrag != 0.0)
			shPData.dSteuerprozent=dSteuerbetrag/((dGesamtpreis-dSteuerbetrag)/100);
		else
			shPData.dSteuerprozent=0;

		shPData.dSteuerprozent=RoundTo(shPData.dSteuerprozent, 0);
		shPData.sMessage="";

		//log("Artikel:"+shPData.sArtikel+" - Bezeichnung:"+shPData.sBezeichnung+" - Menge:"+FloatToStr(shPData.dMenge)+" - EP:"+FloatToStr(shPData.dEinzelpreis));

		if(bDoImport)
			{
			bSt=addSLPos();
			if(bSt)
				{
				s="update pk_amorders set "
					"processed=GetDate(), "
					"belegnummer='"+shBData.strBelegnummer+"',"
					"kennung='"+shPData.strKennung+"' "
					"where id='"+IntToStr(iId)+"'";
				q3->SQL->Text=s;
				q3->ExecSQL();
				}
			}

		q2->Next();
		}
	q2->Close();

	if(bDoImport)
		{
		if(shBData.dVersandkostenBrutto > 0)
			{
			addSLVersand(shBData.sLB, shBData.dVersandkostenNetto, shBData.dVersandkostenBrutto, "A");
			}

		bSt=closeSLOrder();

		if(bSt)
			{
			UnicodeString sBelegField, sBelegValue;
			for(int i=0; i < _slUpdateBeleg->Count; i++)
				{
				sBelegField=_slUpdateBeleg->Names[i];
				sBelegValue=_slUpdateBeleg->ValueFromIndex[i];

				if(ContainsStr(sBelegValue, "'"))
					{
					sBelegValue=ReplaceStr(sBelegValue, "'", "");
					}
				else
					{
					s="select "+sBelegValue+" from pk_amorders where id='"+iId+"'";
					q2->SQL->Text=s;
					try
						{
						q2->Open();
						if(q2->RecordCount)
							{
							sBelegValue=q2->FieldByName(sBelegValue)->AsString;
							q2->Close();
							}
						}
					catch(Exception &e)
						{
						log("Exception BelegUpdate:"+e.Message);
						log(q2->SQL->Text);
						sBelegValue="";
						}
					}

				if(sBelegValue != "")
					{
					s="update beleg set "+sBelegField+"='"+getCheckedSQLStr(sBelegValue)+"' where belegtyp='"+shBData.strBelegtyp+"' and belegnummer='"+shBData.strBelegnummer+"'";
					q2->SQL->Text=s;
					try
						{
						q2->ExecSQL();
						if(_iDebugLevel > 1)
							log("Belegupdate:"+q2->SQL->Text);
						}
					catch(Exception &e)
						{
						log("Exception BelegUpdate:"+e.Message);
						log(q2->SQL->Text);
						}
					}
				}
			}

		}
	q->Close();

	delete slTaxIds;
	delete q3;
	delete q2;
	delete q;
	return(bSt);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::processReport(AnsiString strReportId)
{
	AnsiString s;
	bool bSt;

	TStringList *slContent=new TStringList();

	bSt=getAMReport(strReportId, slContent);
	if(bSt)
		{
		bSt=createInternalOrders(slContent);
		}
	else
		log("Fehler beim Einlesen des Reports:"+strReportId);

	delete slContent;
	return(bSt);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::createInternalOrders(TStringList *slContent)
{
	AnsiString s, sFields, sOrderId, sOrderItemId;
	UnicodeString sLine, sValues;
	int iTabIndex;
	int iNew=0, iVorhanden=0, iFehler=0;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s=slContent->Strings[0];

	s=AnsiReplaceStr(s, "amazon-", "");

	s=AnsiReplaceStr(s, "-", "_");
	s=AnsiReplaceStr(s, "\t", ",");
	sFields=s;

	for(int i=1; i < slContent->Count; i++)
		{
		sLine=slContent->Strings[i];
		s=sLine;
		sOrderId=s.SubString(1, s.AnsiPos("\t")-1);
		s.Delete(1, sOrderId.Length()+1);
		sOrderItemId=s.SubString(1, s.AnsiPos("\t")-1);
		sLine=ReplaceStr(sLine, "'", "''");
		sValues=ReplaceStr(sLine, "\t", "',N'");
		if(sValues != "")
			{
			s="select * from pk_amorders where order_id='"+sOrderId+"' and order_item_id='"+sOrderItemId+"'";
			q->SQL->Text=s;
			q->Open();
			if(q->RecordCount)
				{
				q->Close();
				if(_iDebugLevel > 1)
					log(sOrderId+"-"+sOrderItemId+" existieren bereits!");

				iVorhanden++;
				continue;
				}

			q->Close();
			//s="insert into pk_amorders ("+ORDERS_FLAT_FIELDS+") "
			s="SET ANSI_WARNINGS  OFF; "
				"insert into pk_amorders ("+sFields+") "
				"Values(N'"+sValues+"') "
				"SET ANSI_WARNINGS  ON; ";
			q->SQL->Text=s;
			try
				{
				q->ExecSQL();

				s="delete from pk_amorders where order_id='"+sOrderId+"' and order_item_id='"+sOrderItemId+"' and isnull(buyer_name, '')='' ";
				q->SQL->Text=s;
				q->ExecSQL();
				if(q->RowsAffected)
					{
					log(sOrderId+"-"+sOrderItemId+" mit leeren PII Daten wird übergangen!");
					iFehler++;
					break;
					}
				else
					iNew++;
				}
			catch(Exception &e)
				{
				log("Exception:"+e.Message);
				log("SQL:"+s);
				log("Fehler beim Einfügen Orders-Eintrag:"+sValues);
				iFehler++;
				}
			}
		}

	log("InternalOrderCreate -> Eingefügt:"+IntToStr(iNew)+" - Vorhanden:"+IntToStr(iVorhanden)+" - Fehler:"+IntToStr(iFehler));

	delete q;
	if(iFehler)
		return(false);
	else
		return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::updateInternalOrders(TStringList *slContent)
{
	AnsiString s, sFields, sOrderId, sOrderItemId;
	UnicodeString sLine, sValues;
	int iTabIndex;
	int iUpdate=0;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s=slContent->Strings[0];
	s=AnsiReplaceStr(s, "-", "_");
	s=AnsiReplaceStr(s, "\t", ",");
	sFields=s;

	for(int i=1; i < slContent->Count; i++)
		{
		sLine=slContent->Strings[i];
		s=sLine;
		sOrderId=s.SubString(1, s.AnsiPos("\t")-1);
		s.Delete(1, sOrderId.Length()+1);
		sOrderItemId=s.SubString(1, s.AnsiPos("\t")-1);
		sLine=ReplaceStr(sLine, "'", "''");
		sValues=ReplaceStr(sLine, "\t", "',N'");
		if(sValues != "")
			{
			s="select * from pk_amorders where order_id='"+sOrderId+"' and order_item_id='"+sOrderItemId+"'";
			q->SQL->Text=s;
			q->Open();
			if(q->RecordCount)
				{
				q->Close();

				s="update pk_amorders set quantity_to_ship=quantity_purchased where order_id='"+sOrderId+"' and order_item_id='"+sOrderItemId+"'";
				q->SQL->Text=s;
				q->ExecSQL();

				iUpdate++;
				}
			q->Close();
			}
		}

	log("InternalOrderUpdate -> Updatedt:"+IntToStr(iUpdate));

	delete q;
	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::acknowledgeReportOK(AnsiString strReportId)
{
	AnsiString s;
	bool bSt;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	//bSt=setAMReportAcknowledged(strReportId);
	bSt=true;
	if(bSt)
		{
		s="update pk_amreports set processed=GetDate(), acknowledged='1' where reportId='"+strReportId+"'";
		q->SQL->Text=s;
		q->ExecSQL();
		}

	delete q;
	return(bSt);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::acknowledgeReportError(AnsiString strReportId)
{
	AnsiString s;
	bool bSt;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="update pk_amreports set processed=GetDate(), acknowledged='0' where reportId='"+strReportId+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;
	return(bSt);
}

//---------------------------------------------------------------------------

int __fastcall TfrmMain::getUnprocessedReports(TStringList *slReports)
{
	AnsiString s;
	AnsiString strReportId;
	bool bSt;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="select * from pk_amreports where acknowledged='0' and processed is null order by availabledate ";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		slReports->Add(q->FieldByName("reportid")->AsString);

		q->Next();
		}
	q->Close();

	delete q;
	return(slReports->Count);
}

//---------------------------------------------------------------------------

int __fastcall TfrmMain::checkUnloadedReports(void)
{
	AnsiString s, strName, strValue;
	AnsiString strReportId, strReportRequestId;
	bool bAcknowledged;
	TDateTime tdAvailableDate;
	int iWaitingReports=0;
	bool bSt;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	TStringList *slListResult=new TStringList();

	bSt=getAMReportList("_GET_FLAT_FILE_ORDERS_DATA_", slListResult);
	if(bSt)
		{                   // prüfen welche Reports schon verarbeitet wurden und welche noch offen sind
		for(int i=0; i < slListResult->Count; i++)
			{
			strName=AnsiReplaceStr(slListResult->Names[i], "GetReportListResponse.GetReportListResult.ReportInfo", "Result");
			strValue=slListResult->ValueFromIndex[i];
			if(strName == "Result.Acknowledged")
				{
				if(strValue == "false")
					bAcknowledged=false;
				else
					bAcknowledged=true;
				}
			else if(strName == "Result.ReportId")
					strReportId=strValue;
			else if(strName == "Result.ReportRequestId")
					strReportRequestId=strValue;
			else if(strName == "Result.AvailableDate")
				{
				if(!TryISO8601ToDate(strValue, tdAvailableDate))
					{
					log("Ungültiges Datum:"+strValue);
					continue;
					}
				else
					{
					if(IncDay(Now(), -3) > tdAvailableDate)
						{
						log("Report älter als 3 Tage -> wird verworfen!");
						continue;
						}
					}

				s="select * from pk_amreports where reportid='"+strReportId+"'";
				q->SQL->Text=s;
				q->Open();
				if(!q->RecordCount)
					{
					q->Close();
					s="insert into pk_amreports (reportid, reportrequestid, acknowledged, availabledate) "
						"values('"+strReportId+"','"+strReportRequestId+"','"+IntToStr((int)bAcknowledged)+"','"+tdAvailableDate.DateTimeString()+"')";
					q->SQL->Text=s;
					q->ExecSQL();
					log("Report:"+strReportId+" AvailableDate:"+tdAvailableDate.DateTimeString()+" hinzugefügt!");

					iWaitingReports++;
					}
				else
					{
					q->Close();
					log("Report:"+strReportId+" AvailableDate:"+tdAvailableDate.DateTimeString()+" bereits vorhanden!");
					}
				}
			}
		}

	delete slListResult;
	delete q;

	return(iWaitingReports);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmMain::Debuglevel1Click(TObject *Sender)
{
	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="DebugLevel";
	frmEingabeDialog->edValue->Text=_iDebugLevel;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_iDebugLevel=frmEingabeDialog->edValue->Text.ToInt();
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Artikel1Click(TObject *Sender)
{
	AnsiString s;

	frmSQLAuswahl->_q->Connection=ADOConnection1;
	frmSQLAuswahl->Tag=0;
	frmSQLAuswahl->_bCanFilter=true;

	s="select Artikelnummer, Bezeichnung, Zusatz from art ";
	frmSQLAuswahl->openQuery(s, "Artikelnummer", "", "Artikelnummer");

	frmSQLAuswahl->ShowModal();
	if(frmSQLAuswahl->Tag)
		{
		s=frmSQLAuswahl->grd->Cells[1][frmSQLAuswahl->grd->Row];
		}

}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::CheckServiceStatus1Click(TObject *Sender)
{
	AnsiString s;

	s=getAMServiceStatus();

	log("ServiceStatus:"+s);
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetRequestReportList1Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strRequestReportId="";

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="RequestReportId";
	frmEingabeDialog->edValue->Text=_strRequestReportId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strRequestReportId=frmEingabeDialog->edValue->Text;

		getAMReportRequestList(_strRequestReportId);
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetReport1Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strGetReportId="";

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="ReportId";
	frmEingabeDialog->edValue->Text=_strGetReportId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strGetReportId=frmEingabeDialog->edValue->Text;

		TStringList *slContent=new TStringList();

		getAMReport(_strGetReportId, slContent);

		log("Content:");
		log(slContent->Text);

    delete slContent;
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetReportList1Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strGetReportType="_GET_FLAT_FILE_ORDERS_DATA_";

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="ReportType";
	frmEingabeDialog->edValue->Text=_strGetReportType;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strGetReportType=frmEingabeDialog->edValue->Text;

		TStringList *slListResult=new TStringList();
		getAMReportList(_strGetReportType, slListResult);
		delete slListResult;
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N4Click(TObject *Sender)
{
	AnsiString s;
	UnicodeString sInventoryXML;
	static AnsiString _strSFISKU="";
	static AnsiString _strSFIQuantity="1";

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="SKU";
	frmEingabeDialog->edValue->Text=_strSFISKU;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strSFISKU=frmEingabeDialog->edValue->Text;

		frmEingabeDialog->lbLabel->Caption="Quantity";
		frmEingabeDialog->edValue->Text=_strSFIQuantity;
		frmEingabeDialog->Tag=0;
		frmEingabeDialog->ShowModal();
		if(frmEingabeDialog->Tag)
			{
			_strSFIQuantity=frmEingabeDialog->edValue->Text;

			sInventoryXML=getInventoryFeedXML(_strSFISKU, _strSFIQuantity.ToInt());

			submitAMFeed("_POST_INVENTORY_AVAILABILITY_DATA_", sInventoryXML);
			}
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetOrder1Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strGetOrderId="";
	TStringList *slContent=new TStringList();

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="OrderId";
	frmEingabeDialog->edValue->Text=_strGetOrderId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strGetOrderId=frmEingabeDialog->edValue->Text;

		getAMOrder(_strGetOrderId, slContent);
		}

	delete slContent;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetOrderItems1Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strGetOrderId="";
	TStringList *slContent=new TStringList();

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="OrderId";
	frmEingabeDialog->edValue->Text=_strGetOrderId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strGetOrderId=frmEingabeDialog->edValue->Text;

		getAMOrderItems(_strGetOrderId, slContent);
		}

	delete slContent;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnGetShopOrdersClick(TObject *Sender)
{
	doAMOrderDownload("");
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbxSendArtikelAutoClick(TObject *Sender)
{
	if(cbxSendArtikelAuto->Checked)
		{
		btnArtikelAktualisieren->Enabled=false;
		_iArtikelSekunden=0;
		timerAMGeneral->Enabled=true;
		}
	else
		btnArtikelAktualisieren->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbxSendVersandRAutoClick(TObject *Sender)
{
	if(cbxSendVersandRAuto->Checked)
		{
		btnUploadVersand->Enabled=false;
		_iVersandSekunden=0;
		timerAMGeneral->Enabled=true;
		}
	else
		btnUploadVersand->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbxGetOrderAutoClick(TObject *Sender)
{
	if(cbxGetOrderAuto->Checked)
		{
		btnGetShopOrders->Enabled=false;
		_iOrdersSekunden=0;
		timerAMGeneral->Enabled=true;
		}
	else
		btnGetShopOrders->Enabled=true;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::timerAMGeneralTimer(TObject *Sender)
{
	timerAMGeneral->Enabled=false;

	if(cbxGetOrderAuto->Checked && ++_iOrdersSekunden >= edAutoOrderTime->FloatValue*60)
		{
		log("Start Auto Order Download!");

		_iOrdersSekunden=0;

		doAMOrderDownload("");

		log("Ende Auto Order Download!");
		}

	if(cbxCheckStornosAuto->Checked && ++_iCheckStornoSekunden >= edAutoCheckStronosTime->FloatValue*60)
		{
		log("Start Auto Storno Check!");

		_iCheckStornoSekunden=0;

		doAMCheckStornos();

		log("Ende Auto Storno Check!");
		}

	if(cbxSendVersandRAuto->Checked && ++_iVersandSekunden >= edAutoVersandRTime->FloatValue*60)
		{
		log("Start Auto Versand Upload!");

		_iVersandSekunden=0;

		doAMVersandStatusUpdate("");

		if(_bRechnungsUploadEnabled)
			{
			doAMRechnungsUpload("");
			}

		log("Ende Auto Versand Upload!");
		}

	if(cbxSendArtikelAuto->Checked && ++_iArtikelSekunden >= edAutoArtikelTime->FloatValue*60)
		{
		log("Start Auto Artikel Upload!");

		_iArtikelSekunden=0;

		// Artikel hochladen
		//doAMArtikelUpload("", true);
		doAMBestandBatchUpload();
		if(_bAMPreisUploadEnabled)
		  doAMPreisFeedUpload();


		log("Ende Auto Artikel Upload!");
		}

	timerAMGeneral->Enabled=true;
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::btnArtikelAktualisierenClick(TObject *Sender)
{
	doAMBestandBatchUpload();
	if(_bAMPreisUploadEnabled)
		doAMPreisFeedUpload();
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::btnUploadVersandClick(TObject *Sender)
{
	doAMVersandStatusUpdate("");

	if(_bRechnungsUploadEnabled)
		doAMRechnungsUpload("");
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::N7Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strCheckOrderId="";

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="OrderId";
	frmEingabeDialog->edValue->Text=_strCheckOrderId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strCheckOrderId=frmEingabeDialog->edValue->Text;

		checkAndCreateOrder(_strCheckOrderId, true);
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::ImportFLATReport1Click(TObject *Sender)
{
	if(OpenDialog1->Execute())
		{
		TStringList *slContent=new TStringList();

		//slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::UTF8);
		//slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::Unicode);
		slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::UTF8);   // Amazon-Export ist UTF8
		if(slContent->Count)
			{
			createInternalOrders(slContent);
			log("Manuelle Orderdaten importiert!");
			}
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::Logleeren1Click(TObject *Sender)
{
	memo->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetOrderList1Click(TObject *Sender)
{
	AnsiString s;
	static int _iTageZurueck=7;
	TStringList *slContent=new TStringList();

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="Tage zurück";
	frmEingabeDialog->edValue->Text=IntToStr(_iTageZurueck);
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_iTageZurueck=frmEingabeDialog->edValue->Text.ToInt();

		getAMOrderList(_iTageZurueck, slContent);
		}

	delete slContent;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N9Click(TObject *Sender)
{
	AnsiString s;
	static UnicodeString _sTestXML;
	TStringList *slContent=new TStringList();

	frmMemoEingabeDialog->Caption="Eingabe";
	frmMemoEingabeDialog->lbLabel->Caption="XML";
	frmMemoEingabeDialog->memo->Text=_sTestXML;
	frmMemoEingabeDialog->Tag=0;
	frmMemoEingabeDialog->ShowModal();
	if(frmMemoEingabeDialog->Tag)
		{
		_sTestXML=frmMemoEingabeDialog->memo->Text;

		serializeXMLStr(_sTestXML, slContent);

		log(slContent->Text);
		}

	delete slContent;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetFeedSubmissionList1Click(TObject *Sender)
{
	getAMFeedSubmissionList("");
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::UpdateReportAcknowledgements1Click(TObject *Sender)
{
	AnsiString s;
	bool bSt;
	static AnsiString _strAcknReportId="";

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="ReportId";
	frmEingabeDialog->edValue->Text=_strAcknReportId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strAcknReportId=frmEingabeDialog->edValue->Text;

		bSt=setAMReportAcknowledged(_strAcknReportId);
		if(bSt)
			log("ReportId:"+_strAcknReportId+" acknowleded!");
		else
			log("SetAcknowledgeReport:"+_strAcknReportId+" -> Fehler!");
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::N2Click(TObject *Sender)
{
	AnsiString s;
	bool bSt;
	static AnsiString _strGetReportType="_GET_FLAT_FILE_ORDERS_DATA_";
	static int _iDaysBack=2;

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="ReportType";
	frmEingabeDialog->edValue->Text=_strGetReportType;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strGetReportType=frmEingabeDialog->edValue->Text;

		if(_strGetReportType == "_GET_FLAT_FILE_ORDERS_DATA_")
			{
			frmEingabeDialog->lbLabel->Caption="Tage zurück";
			frmEingabeDialog->edValue->Text=IntToStr(_iDaysBack);
			frmEingabeDialog->Tag=0;
			frmEingabeDialog->ShowModal();
			if(frmEingabeDialog->Tag)
				{
				_iDaysBack=frmEingabeDialog->edValue->Text.ToInt();

				TStringList *slListResult=new TStringList();

				bSt=requestAMReport(_strGetReportType, _iDaysBack);
				if(bSt)
					log("RequestReport Type:"+_strGetReportType+" -> erfolgreich übertragen!");
				else
					log("RequestReport Type:"+_strGetReportType+" -> Fehler!");

				delete slListResult;
				}
			}
		}
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::UploadInvoiceToOrderr1Click(TObject *Sender)
{
	AnsiString s, strPDFFile;
	bool bSt;
	static AnsiString _strInvoiceOrderId="";
	static AnsiString _strInvoiceNumber="";

	if(!OpenDialog1->Execute())
		return;

	strPDFFile=OpenDialog1->FileName;

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="OrderId";
	frmEingabeDialog->edValue->Text=_strInvoiceOrderId;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strInvoiceOrderId=frmEingabeDialog->edValue->Text;

		frmEingabeDialog->lbLabel->Caption="Rechnungsnummer";
		frmEingabeDialog->edValue->Text=_strInvoiceNumber;
		frmEingabeDialog->Tag=0;
		frmEingabeDialog->ShowModal();
		if(frmEingabeDialog->Tag)
			{
			_strInvoiceNumber=frmEingabeDialog->edValue->Text;

			bSt=uploadAMInvoice(_strInvoiceOrderId, _strInvoiceNumber, strPDFFile);
			if(bSt)
				log("PDFUpload für OrderId:"+_strInvoiceOrderId+" -> erfolgreich übertragen!");
			else
				log("PDFUpload für OrderId:"+_strInvoiceOrderId+" -> fehlgeschlagen!");
			}
		}

}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::CheckRechnungsupload1Click(TObject *Sender)
{
	doAMRechnungsUpload("");
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::UpdateInternalOrdeswithFLATReport1Click(TObject *Sender)

{
	if(OpenDialog1->Execute())
		{
		TStringList *slContent=new TStringList();

		slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::UTF8);
		if(slContent->Count)
			{
			updateInternalOrders(slContent);
			log("Manuelle Orderdaten upgedated!");
			}
		}

}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::CheckAndCreatePendingOrders1Click(TObject *Sender)
{
	AnsiString s;

	if(Application->MessageBoxW(L"Wirklich alle ausstehenen Orders erstellen?", L"Warnung", MB_YESNO) != ID_YES)
		return;

	checkAndCreatePendingOrders(true);
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::Versand2Click(TObject *Sender)
{
	frmVersanddaten->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TfrmMain::RESTRequest1AfterExecute(TCustomRESTRequest *Sender)
{
	/*
	AnsiString s, sBody;
	sBody=Sender->Body->ToString();


	ofstream feed;
	s="after-execute-"+AnsiReplaceStr(AnsiReplaceStr(Now().DateTimeString(), ":", "_"), ".", "_")+".log";
	feed.open(s.c_str(), ios::out|ios::binary);
	if(!feed)
		log("log-file kann nicht angelegt werden!");
	else
		{
		feed << AnsiString(sBody).c_str();
		feed.close();
		}
  */
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::N13Click(TObject *Sender)
{
	if(OpenDialog1->Execute())
		{
		TStringList *slContent=new TStringList();

		//slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::UTF8);
		//slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::Unicode);
		slContent->LoadFromFile(OpenDialog1->FileName, TEncoding::UTF8);   // Amazon-Export ist UTF8
		if(slContent->Count)
			{
			createInternalOrders(slContent);
			log("Manuelle Orderdaten importiert!");

      checkAndCreatePendingOrders(true);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::CheckProcedure1Click(TObject *Sender)
{
	AnsiString s;
	static AnsiString _strBelegtyp, _strBelegnummer;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="Belegtyp";
	frmEingabeDialog->edValue->Text=_strBelegtyp;
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_strBelegtyp=frmEingabeDialog->edValue->Text;

		frmEingabeDialog->lbLabel->Caption="Belegnummeer";
		frmEingabeDialog->edValue->Text=_strBelegnummer;
		frmEingabeDialog->Tag=0;
		frmEingabeDialog->ShowModal();
		if(frmEingabeDialog->Tag)
			{
			_strBelegnummer=frmEingabeDialog->edValue->Text;

			s="exec "+_strAMOrderVersendetProzedure+" N'"+_strBelegtyp+"', N'"+_strBelegnummer+"'";
			q->SQL->Text=s;
			q->ExecSQL();
			}
		}

	delete q;
}

//---------------------------------------------------------------------------

void __fastcall TfrmMain::GetOrderListCanceled1Click(TObject *Sender)
{
	AnsiString s;
	static int _iTageZurueck=7;
	TStringList *slContent=new TStringList();

	frmEingabeDialog->Caption="Eingabe";
	frmEingabeDialog->lbLabel->Caption="Tage zurück";
	frmEingabeDialog->edValue->Text=IntToStr(_iTageZurueck);
	frmEingabeDialog->Tag=0;
	frmEingabeDialog->ShowModal();
	if(frmEingabeDialog->Tag)
		{
		_iTageZurueck=frmEingabeDialog->edValue->Text.ToInt();

		getAMOrderListCanceled(_iTageZurueck, slContent);
		}

	delete slContent;
}
//---------------------------------------------------------------------------


void __fastcall TfrmMain::btnAutoCheckStornosClick(TObject *Sender)
{
  doAMCheckStornos();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::cbxCheckStornosAutoClick(TObject *Sender)
{
	if(cbxCheckStornosAuto->Checked)
		{
		btnAutoCheckStornos->Enabled=false;
		_iCheckStornoSekunden=0;
		timerAMGeneral->Enabled=true;
		}
	else
		btnAutoCheckStornos->Enabled=true;
}
//---------------------------------------------------------------------------

