//---------------------------------------------------------------------------

#ifndef WawiH
#define WawiH


#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>

#include <System.Math.hpp>
#include <strutils.hpp>

typedef struct
{
	UnicodeString sAnrede;
	UnicodeString sName;
	UnicodeString sVorname;
	UnicodeString sFirma;
	UnicodeString sStrasse;
	UnicodeString sPLZ;
	UnicodeString sOrt;
	UnicodeString sLand;
	UnicodeString sSteueridentNr;
	UnicodeString sSteuernummer;
	UnicodeString sZusatz;
	UnicodeString sZusatz2;
	UnicodeString sZusatz3;
	UnicodeString sTelefon1;
	UnicodeString sTelefon2;
	UnicodeString sTelefon3;
  UnicodeString sFax;
	UnicodeString sEMail;
	UnicodeString sILN;
} TSLAdresse;

typedef struct
{
	UnicodeString sStandort;
	UnicodeString sWaehrungscode;
	UnicodeString sPreistyp;
	UnicodeString sPreisgruppe;
	UnicodeString sKontengruppe;
	UnicodeString sRabattgruppe;
	UnicodeString sZB;
	UnicodeString sLB;
	UnicodeString sKostenstelle;
	UnicodeString sLager;
	UnicodeString sBankbezug;
	UnicodeString sGruppe;
	UnicodeString sMitarbeiter;
	UnicodeString sVertreter;
	UnicodeString sSprache;

} TSLKundenDaten;

/*
Definition Lager Typ
Lagerungsaktion und Zieltabelle:
0 - keine
1 - LAGERBEDARF - Reservierung
2 - LAGERBEDARF - Bestellung
3 - LAGERBESTAND - Lagerabgang
4 - LAGERBESTAND - Lagerzugang
5 - LAGERBESTANDPACK - Packung
6 - LAGERFERTIGUNG - Fertigung
7 - LAGERBESTAND - Abgang für Packzettel
8 - LAGERBESTANDPACK - Abgang aus Packzettel
9 - LAGERBESTAND - Abgang für Umlagerung
10 - LAGERBESTAND - Zugang aus Umlagerung
*/
enum TLagerTypCodes {ltyReservierung=1, ltyBestellung, ltyAbgang, ltyZugang, ltyPacken, ltyFertigung, ltyAbgangFuerPack, ltyZugangAusPack, ltyAbgangUmlagerug, ltyZugangUmlagerung};

/*
Definition Tabelle

*/
enum TLagerTabelleCodes {ltaLagerbedarf=1, ltaLagerbestand, ltaLagerbestandAndPack, ltaLagerfertigung};

//---------------------------------------------------------------------------
//struct TAdresse
//{
//	UnicodeString sName, sVorname, sFirma, sStrasse, sZusatz, sZusatz2, sZusatz3, sPLZ, sOrt, sLand, sILN;
//};

//---------------------------------------------------------------------------

//UnicodeString getSQLFloat(double dValue) {UnicodeString s; s.sprintf(L"%f", dValue); return(s);}
UnicodeString getSQLFloat(double dValue) {return(AnsiReplaceStr(FloatToStr(dValue), ",", "."));}
UnicodeString getCheckedSQLStr(UnicodeString sSQL) {return(ReplaceStr(sSQL, "'", "''"));}


class TWawi
{
	protected:
	TADOConnection *_con;
	TADOQuery *q, *q2;
	UnicodeString _sUser;
	UnicodeString _sName;
	UnicodeString _sStandort;
	TStringList *slExtraKuBeleg;
	TStringList *slExtraArBelegp;

	public:
	TWawi(TADOConnection *con);
	~TWawi(void);

	TADOConnection * getCon(void) {return(_con);}
	UnicodeString getUser(void) {return(_sUser);}
	void setUser(UnicodeString sUser) {_sUser=sUser;}
	UnicodeString getStandort(void) {return _sStandort;}
	UnicodeString getName(void) {return(_sName);}

	UnicodeString getNextKey(UnicodeString sBlobKey);
	UnicodeString setNextKey(UnicodeString sBlobKey, UnicodeString sNewKey);

	UnicodeString getErloeskontoFromMatrix(UnicodeString sKundenKontengruppe, UnicodeString sArtikelkontengruppe, UnicodeString &sAbwieichenderSteuercode);
	UnicodeString getAufwandskontoFromMatrix(UnicodeString sLieferantenKontengruppe, UnicodeString sArtikelkontengruppe, UnicodeString &sAbwieichenderSteuercode);
	double getSteuerProzent(UnicodeString sSteuerCode);
	double getArtikelVKPreis(UnicodeString sArtikelnummer, UnicodeString sWaehrung, double dMenge, int iPreisgruppe, UnicodeString sKundennummer);

	double getArtikelEKPreis(UnicodeString sArtikelnummer, double dMenge, UnicodeString sLieferantennummer);
	double getWaehrungsfaktor(UnicodeString sWaehrung, TDateTime tdDatum);

  double getBestand(UnicodeString sArtikelnummer, UnicodeString sLager);
	bool addBestand(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert=0, AnsiString sSerieCharge="", int iTyp=ltyZugang);
	bool subBestand(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, AnsiString sSerieCharge="", int iTyp=ltyAbgang);

	bool addPack(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert=0, AnsiString sSerieCharge="");
	bool subPack(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, AnsiString sSerieCharge="");

	bool setReserviert(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert=0);
	bool setBestellt(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert=0);

	bool setWKReserviert(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sLager, double dMenge, UnicodeString sELager, UnicodeString sUser);

	int checkSerieCharge(UnicodeString sSerieCharge, UnicodeString &sArtikelnummer, UnicodeString &sSCTyp);
	int addSerieCharge(UnicodeString sSerieCharge, UnicodeString sArtikelnummer, UnicodeString sSCTyp);

	bool setUebergeben(UnicodeString sBelegpKennung, UnicodeString sVorgaengerpKennung, double Menge);

	bool deleteBeleg(UnicodeString sBelegtyp, UnicodeString sBelegNummer);

	bool createKunde(TSLAdresse *adr, TSLKundenDaten *kuda, UnicodeString &sKundennummer);

};

//---------------------------------------------------------------------------

class TBeleg : public TWawi
{
	protected:
	int _iBelegId;
	bool _bStandortChanged, _bHeaderChanged, _bWaehrungChanged, _bAdressChanged, _bLieferAdressChanged, _bLagerChanged, _bPosChanged;
	bool _bKopftextChanged, _bFusstextChanged;
	UnicodeString sAdressBlobKey, sLagerFlag;

	TDateTime dtDatum;
	UnicodeString sIhrauftrag;
	UnicodeString sLiefertermin, sIhrauftragvomdatum, sLeistungsdatum;
	UnicodeString sBelegtyp, sBelegnummer, sAdressnummer;
	TSLAdresse belegAdr, lieferAdr;
	UnicodeString sKonto, sKontengruppe;
	UnicodeString sUmsatzKennzeichen;

	UnicodeString sPreistyp, sZB, sLB, sLText1, sLText2, sWaehrungscode, sLager;
	UnicodeString sMitarbeiter, sVertreter, sSprache;
	UnicodeString sUnserzeichen, sIhrzeichen;
	UnicodeString sKopftext, sFusstext;
	UnicodeString sLieferbelegnr;
	int iPreisgruppe;
	int iZahlungsziel, iZahlungszieltyp;
	int iEA;
	double dBelegrabatt;
	UnicodeString sRabattgruppe;
	UnicodeString sEULand, sMeldeland, sVerkehrszweig, sUSTId;
	UnicodeString sRechAdresse;

	double dNetto, dSteuer, dBrutto, dErloes;
	double dWaehrungsfaktor, dVersand;
	int iKursTyp;

	double dSkontoprozent1, dSkontoprozent2;
	int iSkontotage1, iSkontotage2, iSkontotage1typ, iSkontotage2typ;

	double dTeilsumme;

	public:
	TBeleg(TADOConnection *con);

	TADOConnection *getCon(void) {return(_con);}
	UnicodeString getBelegnummer(void) {return(sBelegnummer);}
	UnicodeString getBelegtyp(void) {return(sBelegtyp);}
	int getBelegID(void) {return(_iBelegId);}

	bool edit(UnicodeString sBelegtyp, UnicodeString sBelegnummer);
	UnicodeString create(UnicodeString sBelegtyp, UnicodeString sBelegnummer);
	bool save(void);
	bool copyHeader(UnicodeString sBelegtyp, UnicodeString sBelegnummer);
	bool dispose(void);
	bool undisplose(void);

	// Setter
	bool setAdresse(UnicodeString sAdressNr);
	void setDatum(TDateTime dtDatum) {this->dtDatum=dtDatum; _bHeaderChanged=true;}
	void setLeistungsdatum(TDateTime dtLeistungsdatum) {this->sLeistungsdatum=dtLeistungsdatum.DateString(); _bHeaderChanged=true;}
	void setZahlungsbedingung(UnicodeString sZB);
	void setLiefertermin(TDateTime dtLiefertermin) {this->sLiefertermin=dtLiefertermin.FormatString("dd.mm.yyyy"); _bHeaderChanged=true;}
	void setLieferbelegnr(UnicodeString sLieferbelegnr) {this->sLieferbelegnr=sLieferbelegnr; _bHeaderChanged=true;}
	void setLieferbedingung(UnicodeString sLB);
	void setLiefertext1(UnicodeString sLText1) {this->sLText1=sLText1; _bHeaderChanged=true;}
	void setLiefertext2(UnicodeString sLText2) {this->sLText2=sLText2; _bHeaderChanged=true;}
	void setRabattgruppe(UnicodeString sRabattgruppe) {this->sRabattgruppe=sRabattgruppe; _bHeaderChanged=true;}
	void setIhrauftrag(UnicodeString sIhrauftrag) {this->sIhrauftrag=sIhrauftrag; _bHeaderChanged=true;}
	void setIhrauftragvom(TDateTime dtDate) {this->sIhrauftragvomdatum=dtDate.DateTimeString(); _bHeaderChanged=true;}
	void setKontengruppe(UnicodeString sKontengruppe) {this->sKontengruppe=sKontengruppe; _bHeaderChanged=true;}
	void setPreisgruppe(int iPreisgruppe) {this->iPreisgruppe=iPreisgruppe; _bHeaderChanged=true;}
	void setPreistyp(UnicodeString sPreistyp) {this->sPreistyp=sPreistyp; _bHeaderChanged=true;}
	void setLager(UnicodeString sLager) {this->sLager=sLager; _bHeaderChanged=true;}
	void setWaehrungscode(UnicodeString sWaehrungscode) {this->sWaehrungscode=sWaehrungscode; if(sWaehrungscode == "EUR") iKursTyp=1; else iKursTyp=3; _bHeaderChanged=true;}
	void setWaehrungsfaktor(double dWaehrungfaktorA) {this->dWaehrungsfaktor=dWaehrungfaktorA; _bHeaderChanged=true;}
	void setUnserzeichen(UnicodeString sUnserzeichen) {this->sUnserzeichen=sUnserzeichen; _bHeaderChanged=true;}
	void setIhrzeichen(UnicodeString sIhrzeichen) {this->sIhrzeichen=sIhrzeichen; _bHeaderChanged=true;}
	void setLieferadresse(TSLAdresse *adrLiefer) {this->lieferAdr.sAnrede=adrLiefer->sAnrede;
																							this->lieferAdr.sName=adrLiefer->sName;
																							this->lieferAdr.sVorname=adrLiefer->sVorname;
																							this->lieferAdr.sFirma=adrLiefer->sFirma;
																							this->lieferAdr.sStrasse=adrLiefer->sStrasse;
																							this->lieferAdr.sZusatz=adrLiefer->sZusatz;
																							this->lieferAdr.sZusatz2=adrLiefer->sZusatz2;
																							this->lieferAdr.sZusatz3=adrLiefer->sZusatz3;
																							this->lieferAdr.sPLZ=adrLiefer->sPLZ;
																							this->lieferAdr.sOrt=adrLiefer->sOrt;
																							this->lieferAdr.sLand=adrLiefer->sLand;
																							this->lieferAdr.sILN=adrLiefer->sILN; _bLieferAdressChanged=true;}
	void setBelegadresse(TSLAdresse *adrBeleg) {this->belegAdr.sAnrede=adrBeleg->sAnrede;
																							this->belegAdr.sName=adrBeleg->sName;
																							this->belegAdr.sVorname=adrBeleg->sVorname;
																							this->belegAdr.sFirma=adrBeleg->sFirma;
																							this->belegAdr.sStrasse=adrBeleg->sStrasse;
																							this->belegAdr.sZusatz=adrBeleg->sZusatz;
																							this->belegAdr.sZusatz2=adrBeleg->sZusatz2;
																							this->belegAdr.sZusatz3=adrBeleg->sZusatz3;
																							this->belegAdr.sPLZ=adrBeleg->sPLZ;
																							this->belegAdr.sOrt=adrBeleg->sOrt;
																							this->belegAdr.sLand=adrBeleg->sLand;
																							this->belegAdr.sILN=adrBeleg->sILN; _bAdressChanged=true;}
	void setKopftext(UnicodeString sKopftext) {this->sKopftext=sKopftext; _bKopftextChanged=true;}
	void setFusstext(UnicodeString sFusstext) {this->sFusstext=sFusstext; _bFusstextChanged=true;}
	void setBelegrabatt(double dBelegrabatt) {this->dBelegrabatt=dBelegrabatt; _bHeaderChanged=true;}
	void setEULand(UnicodeString sEULand) {this->sEULand=sEULand; _bHeaderChanged=true;}
	void setMeldeland(UnicodeString sMeldeland) {this->sMeldeland=sMeldeland; _bHeaderChanged=true;}
	void setUSTId(UnicodeString sUSTId) {this->sUSTId=sUSTId; _bHeaderChanged=true;}
	void setZahlungsziel(int iZahlungsziel) {this->iZahlungsziel=iZahlungsziel; _bHeaderChanged=true;}
	void setSkontoprozent1(double dSkontoprozent1) {this->dSkontoprozent1=dSkontoprozent1; _bHeaderChanged=true;}
	void setSkontoprozent2(double dSkontoprozent2) {this->dSkontoprozent2=dSkontoprozent2; _bHeaderChanged=true;}
	void setMitarbeiter(UnicodeString sMitarbeiter) {this->sMitarbeiter=sMitarbeiter;}
	void setVertreter(UnicodeString sVertreter) {this->sVertreter=sVertreter;}

	// Setter für interne Sachen
	void setPosChanged(bool bChanged) {this->_bPosChanged=bChanged;}
	void setTeilsumme(double dTeilsumme) {this->dTeilsumme=dTeilsumme;}

	// Getter
	UnicodeString getAdressBlobKey(void) {return(sAdressBlobKey);}
	UnicodeString getAdresse(void) {return(sAdressnummer);}
	TDateTime getDatum(void) {return(dtDatum);}
	UnicodeString getLeistungsdatum(void) {return(sLeistungsdatum);}

	UnicodeString getLiefertermin(void) {return(sLiefertermin);}
	UnicodeString getLieferbelegnr(void) {return(this->sLieferbelegnr);}
	UnicodeString getLieferbedingung(void) {return(this->sLB);}
	UnicodeString getLiefertext1(void) {return(this->sLText1);}
	UnicodeString getLiefertext2(void) {return(this->sLText2);}
	UnicodeString getRabattgrupe(void) {return(this->sRabattgruppe);}

	UnicodeString getIhrauftrag(void) {return(this->sIhrauftrag);}
	UnicodeString getIhrauftragvom(void) {return(this->sIhrauftragvomdatum);}
	UnicodeString getKontengruppe(void) {return(this->sKontengruppe);}
	int getPreisgruppe(void) {return(this->iPreisgruppe);}
	UnicodeString getPreistyp(void) {return(this->sPreistyp);}

	UnicodeString getMitarbeiter(void) {return(this->sMitarbeiter);}
	UnicodeString getVertreter(void) {return(this->sVertreter);}
	UnicodeString getLager(void) {return(this->sLager);}
	UnicodeString getLagerFlag(void) {return(this->sLagerFlag);}
	UnicodeString getWaehrungscode(void) {return(this->sWaehrungscode);}
	double getWaehrungfaktor(void) {return(this->dWaehrungsfaktor);}
	double getBelegrabatt(void) {return(this->dBelegrabatt);}
	UnicodeString getEULand() {return(this->sEULand);}
	UnicodeString getMeldeland() {return(this->sMeldeland);}
	UnicodeString getUSTId() {return(this->sUSTId);}
	int getZahlungsziel() {return(this->iZahlungsziel);}
	double getSkontoprozent1() {return(this->dSkontoprozent1);}
	double getSkontoprozent2() {return(this->dSkontoprozent2);}

	UnicodeString getUnserzeichen(void) {return(this->sUnserzeichen);}
	UnicodeString getIhrzeichen(void) {return(this->sIhrzeichen);}
	UnicodeString getKopftext() {return(sKopftext);}
	UnicodeString getFusstext() {return(sFusstext);}

	double getTeilsumme(void) {return(this->dTeilsumme);}
	UnicodeString getUmsatzKennzeichen(void) {return(this->sUmsatzKennzeichen);}
	int getEA(void) {return(this->iEA);}

	bool updateOP(void);

};

//---------------------------------------------------------------------------

class TBelegp
{
	protected:
	TBeleg *_beleg;
	bool _bChanged;
	UnicodeString sKennung, sHauptkennung;
	TDateTime tdDatum;
	UnicodeString sAdressnr, sZeilentyp, sStueckliste;
	UnicodeString sVorgaengerpKennung;
	TStringList *slSeriennummern;
	TStringList *slChargen;

	double dMenge, dVerwendet, dEP, dRabatt, dRabatt2, dGP, dPE, dKalk;
	double dGewicht;
	double dPreismenge;
	double dEPBasis, dKalkBasis;
	UnicodeString sStatus, sME, sSerieCharge;
	UnicodeString sPreismengeneinheit;
	UnicodeString sArtikelnummer;
	UnicodeString sBestellnummer;
	UnicodeString sBezeichnung, sZusatz, sLangtext, sPostext, sLager;
	UnicodeString sMitarbeiter;
	bool bRabattfaehig, bSkontofaehig, bProvisionsfaehig, bLagerartikel;
	UnicodeString sSteuercode, sKonto;
	double dSteuerprozent;
	UnicodeString sEditMenge;
	double dUebernahme;
	UnicodeString sFrei1, sFrei2;
	double dFreizahl;
	UnicodeString sFreidatum;
	UnicodeString sKostenstelle, sKostentraeger;
	UnicodeString sWarencode;
    UnicodeString sStandort;

	UnicodeString sTermin;
	double dMEUmrechnung;

	//void calc(void) {dGP=dMenge*dEP/dPE-(dEP/dPE/100*dRabatt);}
	void calc(void) {dGP=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100)*(1-dRabatt2/100) , -2);}

	double getEditMengeValue(UnicodeString sEditMenge);

	public:
	TBelegp(TBeleg *beleg);
	~TBelegp(void);
	bool edit(UnicodeString sKennung);
	bool create(UnicodeString sZeilentyp);
	bool save(void);
	bool del(UnicodeString sKennung);
	bool take(UnicodeString sQuellKennung, double dMenge);
	bool dispose(void);
	bool undispose(void);
	bool addSNr(UnicodeString sSerNr);
	void clearSNr(void) {slSeriennummern->Clear();}
	bool addCharge(UnicodeString sCharge, double dMenge);
	void clearCharge(void) {slChargen->Clear();}
	bool addArtExtra(UnicodeString sKennung);
	bool copyBelegpExtra(UnicodeString sQuellKennung, UnicodeString sZielKennung);

	// Setter
	bool setArtikel(UnicodeString sArtikel);
	void setMenge(double dMenge);
	void setZeilentyp(UnicodeString sZeilentyp) {this->sZeilentyp=sZeilentyp;}
	void setHauptkennung(UnicodeString sHauptkennung) {this->sHauptkennung=sHauptkennung;}
	void setStueckliste(UnicodeString sStueckliste) {this->sStueckliste=sStueckliste;}
	void setEditMenge(UnicodeString sEingabeMenge);
	void setMengeneinheit(UnicodeString sME);
	void setEinzelpreis(double dEinzelpreis) {this->dEP=dEinzelpreis; this->dEPBasis=dEinzelpreis/dMEUmrechnung; calc();}
	void setGP(double dGP) {this->dGP=dGP;}
	void setRabatt(double dRabatt) {this->dRabatt=dRabatt; calc();}
	void setPreiseinheit(double dPreiseinheit) {dPE=dPreiseinheit; calc();}
	void setTermin(TDateTime tdTermin) {this->sTermin=tdTermin.FormatString("dd.mm.yyyy");}
	bool setLagerartikel(bool bLagerartikel) {this->bLagerartikel=bLagerartikel;}
	void setBezeichnung(UnicodeString sBez) {this->sBezeichnung=sBez;}
	void setZusatz(UnicodeString sZus) {this->sZusatz=sZus;}
	void setLager(UnicodeString sLager);
	void setKalkpreis(double dKalk) {this->dKalk=dKalk; this->dKalkBasis=dKalk/dMEUmrechnung; calc();}
	void setSteuercode(UnicodeString sSteuercode) {this->sSteuercode=sSteuercode;}
	void setSteuerprozent(double dSteuerprozent) {this->dSteuerprozent=dSteuerprozent;}
	void setKonto(UnicodeString sKonto) {this->sKonto=sKonto;}
	void setLangtext(UnicodeString sLangtext) {this->sLangtext=sLangtext;}
	void setGewicht(double dGewicht) {this->dGewicht=dGewicht;}
	void setRabatt2(double dRabatt2) {this->dRabatt2=dRabatt2; calc();}
	void setFrei1(UnicodeString sFrei1) {this->sFrei1=sFrei1;}
	void setFrei2(UnicodeString sFrei2) {this->sFrei2=sFrei2;}
	void setFreizahl(double dFreizahl) {this->dFreizahl=dFreizahl;}
	void setFreidatum(TDateTime tdFreidatum) {this->sFreidatum=tdFreidatum.DateString();}
	void setKostenstelle(UnicodeString sKostenstelle) {this->sKostenstelle=sKostenstelle;}
	void setKostentraeger(UnicodeString sKostentraeger) {this->sKostentraeger=sKostentraeger;}
	void setWarencode(UnicodeString sWarencode) {this->sWarencode=sWarencode;}
	void setVerwendet(double dVerwendet) {this->dVerwendet=dVerwendet;}
	void setVorgaenger(UnicodeString sVorgaenger) {this->sVorgaengerpKennung=sVorgaenger;}
	void setPostext(UnicodeString sPostext) {this->sPostext=sPostext;}
	void setMitarbeiter(UnicodeString sMitarbeiter) {this->sMitarbeiter=sMitarbeiter;}
	void setBestellnummer(UnicodeString sBestellnummer) {this->sBestellnummer=sBestellnummer;}
	void setPreismengeneinheit(UnicodeString sPreismengeneinheit) {this->sPreismengeneinheit=sPreismengeneinheit;}
	void setPreismenge(double dPreismenge) {this->dPreismenge=dPreismenge;}

	// Getter
	UnicodeString getKennung(void) {return(sKennung);}
	UnicodeString getHauptkennung(void) {return(sHauptkennung);}
	UnicodeString getArtikel() {return(sArtikelnummer);}
	UnicodeString getZeilentyp() {return(sZeilentyp);}
	UnicodeString getStueckliste() {return(sStueckliste);}
	double getEinzelpreis() {return(dEP);}
	double getRabatt() {return(dRabatt);}
	double getGesamtpreis() {return(dGP);}
	double getPreiseinheit() {return(dPE);}
	double getMenge() {return(dMenge);}
	UnicodeString getEditMenge() {return(sEditMenge);}
	UnicodeString getMengeneinheit() {return(sME);}
	UnicodeString getBezeichnung() {return(sBezeichnung);}
	UnicodeString getZusatz() {return(sZusatz);}
	UnicodeString getPostext() {return(sPostext);}
	bool getProfisionsfaehig() {return(bProvisionsfaehig);}
	bool getSkontofaehig() {return(bSkontofaehig);}
	bool getRabattfaehig() {return(bRabattfaehig);}
	bool getLagerartikel() {return(bLagerartikel);}
	UnicodeString getSteuercode() {return(sSteuercode);}
	double getSteuerprozent() {return(dSteuerprozent);}
	UnicodeString getFibukonto() {return(sKonto);}
	UnicodeString getTermin() {return(sTermin);}
	UnicodeString getLager() {return(sLager);}
	double getKalkpreis() {return(dKalk);}
	UnicodeString getLangtext() {return(sLangtext);}
	UnicodeString getWarencode() {return(sWarencode);}
	double getVerwendet() {return(dVerwendet);}
	UnicodeString getStatus() {return(sStatus);}
	UnicodeString getMitarbeiter() {return(sMitarbeiter);}
	UnicodeString getSeriecharge() {return(sSerieCharge);}
	UnicodeString getBestellnummer() {return(sBestellnummer);}
	UnicodeString getPreismengeneinheit() {return(sPreismengeneinheit);}
	double getPreismenge() {return(dPreismenge);}
	double getEPBasis() {return(dEPBasis);}
	double getKalkBasis() {return(dKalkBasis);}
    double getGewicht() {return(dGewicht);}
};

//---------------------------------------------------------------------------

class TWerkauftrag : public TBeleg
{
	public:
	TWerkauftrag(TADOConnection *con);

	UnicodeString create(UnicodeString sBelegnummer);
	bool edit(UnicodeString sBelegnummer);

	void setLagerAus(UnicodeString sLager);
	UnicodeString getLagerAus(void);
	void setLagerEin(UnicodeString sLager);
	UnicodeString getLagerEin(void);

	UnicodeString getStatus(double &dResOffen, double &dAusOffen, double &dFertigOffen, double &dFertiggestellt);

	bool setArtikel(UnicodeString sArtikelnummer, bool bOhneTeile=false);
	UnicodeString getArtikel(void);
	bool setMenge(double dMenge);
    double getMenge(void);
	bool addTeil(UnicodeString sTeil, double dMenge);
	bool delTeil(UnicodeString sTeil, double dMenge);

	bool setReserviert(double dMenge);
	bool setUnReserviert(double dMenge);
	bool setUnReserviertPos(UnicodeString sKennung, double dMenge);
	bool setAusgelagert(double dMenge);
	bool setUnAusgelagert(double dMenge);

	bool setEingelagert(double dMenge);
	bool setUnEingelagert(double dMenge);

};

#endif
