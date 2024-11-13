//---------------------------------------------------------------------------

#ifndef SLUnitH
#define SLUnitH

//---------------------------------------------------------------------------

typedef struct
{
	UnicodeString sAnrede;
	UnicodeString sName;
	UnicodeString sVorname;
	UnicodeString sFirma;
	UnicodeString sStrasse;
	//UnicodeString sHausnummer;
	UnicodeString sPLZ;
	UnicodeString sOrt;
	UnicodeString sLand;
	UnicodeString sLandname;
	UnicodeString sStaat;
	UnicodeString sGeburtsdatum;
	UnicodeString sUstId;
	UnicodeString sZusatz;
	UnicodeString sZusatz2;
	UnicodeString sZusatz3;
	UnicodeString sTelefon;
	UnicodeString sTelefon2;
	UnicodeString sTelefon3;
	UnicodeString sEMail;
	UnicodeString sPreistyp;
	UnicodeString sPreisgruppe;
	UnicodeString sKontengruppe;
	UnicodeString sZB;
	UnicodeString sLB;
	UnicodeString sLager;
	UnicodeString sCustomerId;
	UnicodeString sSalesChannelId;
	UnicodeString sSalesChannelName;

} SLAdress;

typedef struct
{
	UnicodeString sOrderNumber;
	UnicodeString sEMail;
	UnicodeString strOderDateTime;
	UnicodeString strLiefertermin;
	UnicodeString sWaehrung;
	UnicodeString sKundennummer;
	UnicodeString sZB;
	UnicodeString sLB;
	SLAdress adrShipping;
	UnicodeString sKommentar;
	double dGesamtBetragBrutto;
	double dGesamtBetragNetto;
	double dSteuerGesamtBetrag;
	double dVersandkostenNetto;
	double dVersandkostenBrutto;
	UnicodeString sOrderState;
	UnicodeString sIhrZeichen;

	// evtl. Rückgaben
	AnsiString strBelegnummer;
	AnsiString strBelegtyp;

} SLBelegData;


typedef struct
{
	UnicodeString sArtikel;
	UnicodeString sBezeichnung;
	UnicodeString sMessage;
	double dMenge;
	double dEinzelpreis;
	double dSteuerprozent;
	double dVersandPerArtikel;

	// evtl. Rückgaben
	AnsiString strKennung;

} SLPosData;

bool __fastcall startCOM(void);
bool __fastcall stopCOM(void);
bool __fastcall createSLOrder(void);
bool __fastcall addSLPos(void);
bool __fastcall addSLVersand(AnsiString strLB, double dVersandNetto, double dVersandBrutto, AnsiString strZeilentyp);
bool __fastcall addSLRabattPos(AnsiString strRabattArt, AnsiString strBezeichnung, double dMenge, double dPreisNetto, double dPreisBrutto);
bool __fastcall closeSLOrder();
AnsiString __fastcall createSLPDF(AnsiString strBelegtyp, AnsiString strBelegnummer, AnsiString strVorlage);
AnsiString __fastcall createKu(SLAdress *adr, bool bInterressent);
AnsiString __fastcall getSLNextKey(AnsiString strBlobKey);
AnsiString __fastcall setSLNextKey(AnsiString strBlobKey, AnsiString strNewKey);
UnicodeString getCheckedSQLStr(UnicodeString sSQL) {return(ReplaceStr(sSQL, "'", "''"));}

// SLUnit
SLAdress adrSLBill, adrSLDel;
SLBelegData shBData;
SLPosData shPData;
TStringList *slAdrBill, *slAdrDel;




#endif
