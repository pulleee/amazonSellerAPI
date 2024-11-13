//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

enum TGridPos {gpSEND_NAME1, gpSEND_NAME2, gpSEND_STREET, gpSEND_HOUSNUMBER, gpSEND_PLZ, gpSEND_CITY, gpSEND_COUNTRY, gpRECV_NAME1, gpRECV_NAME2, gpRECV_STREET, gpRECV_HOUSNUMBER, gpRECV_PLZ, gpRECV_CITY, gpRECV_COUNTRY, gpPRODUCT, gpCOUPON, gpSEND_EMAIL};
#include "FVersanddaten.h"

#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "AdvUtil"
#pragma link "BaseGrid"
#pragma resource "*.dfm"
TfrmVersanddaten *frmVersanddaten;
//---------------------------------------------------------------------------
__fastcall TfrmVersanddaten::TfrmVersanddaten(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmVersanddaten::loadData(void)
{
	UnicodeString s;
	int iRow;
	int iCount=0;
	TStringList *slSQL=new TStringList;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=frmMain->ADOConnection1;

	grd->RowCount=1;

	slSQL->LoadFromFile(ExtractFilePath(Application->ExeName)+"dhl.sql");
	//s=AnsiReplaceStr(slSQL->Text, ":Jahr", "'"+coJahr->Text+"' ");
	//s=AnsiReplaceStr(s, ":Monat", "'"+coMonat->Text+"' ");
  s=slSQL->Text;
	q->SQL->Text=s;
	q->Open();

	for(int i=0; i < q->RecordCount; i++)
		{
		grd->AddRow();
		iRow=grd->RowCount-1;

		grd->Cells[gpSEND_NAME1][iRow]=q->FieldByName("SEND_NAME1")->AsString;
		grd->Cells[gpSEND_NAME2][iRow]=q->FieldByName("SEND_NAME2")->AsString;
		grd->Cells[gpSEND_STREET][iRow]=q->FieldByName("SEND_STREET")->AsString;
		grd->Cells[gpSEND_HOUSNUMBER][iRow]=q->FieldByName("SEND_HOUSNUMBER")->AsString;
		grd->Cells[gpSEND_PLZ][iRow]=q->FieldByName("SEND_PLZ")->AsString;
		grd->Cells[gpSEND_CITY][iRow]=q->FieldByName("SEND_CITY")->AsString;
		grd->Cells[gpSEND_COUNTRY][iRow]=q->FieldByName("SEND_COUNTRY")->AsString;
		grd->Cells[gpRECV_NAME1][iRow]=q->FieldByName("RECV_NAME1")->AsString;
		grd->Cells[gpRECV_NAME2][iRow]=q->FieldByName("RECV_NAME2")->AsString;
		grd->Cells[gpRECV_STREET][iRow]=q->FieldByName("RECV_STREET")->AsString;
		grd->Cells[gpRECV_HOUSNUMBER][iRow]=q->FieldByName("RECV_HOUSNUMBER")->AsString;
		grd->Cells[gpRECV_PLZ][iRow]=q->FieldByName("RECV_PLZ")->AsString;
		grd->Cells[gpRECV_CITY][iRow]=q->FieldByName("RECV_CITY")->AsString;
		grd->Cells[gpRECV_COUNTRY][iRow]=q->FieldByName("RECV_COUNTRY")->AsString;
		grd->Cells[gpPRODUCT][iRow]=q->FieldByName("PRODUCT")->AsString;
		grd->Cells[gpCOUPON][iRow]=q->FieldByName("COUPON")->AsString;
		grd->Cells[gpSEND_EMAIL][iRow]=q->FieldByName("SEND_EMAIL")->AsString;

		iCount++;
		q->Next();
		}
	q->Close();

	grd->AutoSizeColumns(false, 10);

	StatusBar1->Panels->Items[0]->Text="Anzahl:"+IntToStr(iCount);

	if(iCount > 100)
		{
		Application->MessageBoxW(L"Mehr als 100 Versendungen ausgewäht!\r\nÜberzählige Sendungen werden verworfen!", L"Warnung", MB_OK);
    }

	delete q;
	delete slSQL;
}

//---------------------------------------------------------------------------

void __fastcall TfrmVersanddaten::Button2Click(TObject *Sender)
{
	loadData();
}

//---------------------------------------------------------------------------
void __fastcall TfrmVersanddaten::Button1Click(TObject *Sender)
{
	grd->Delimiter=';';
	SaveDialog1->DefaultExt=".csv";
	SaveDialog1->Filter="csv Datei (*.csv)|*.CSV";
	if(SaveDialog1->Execute())
		{
		grd->SaveToCSV(SaveDialog1->FileName, false);
		}
}
//---------------------------------------------------------------------------
