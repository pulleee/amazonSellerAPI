//---------------------------------------------------------------------------

#pragma hdrstop

//ToDos:
// Nicht mehr Lagern wenn der Vorgänger schon gelagert hat!
// Stücklisten in Belegp
// Umsatzsteuerschlüssel anhand des Belegdatums

#include "Wawi.h"

#include <DateUtils.hpp>
#include "Unit1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

TWawi::TWawi(TADOConnection *con)
{
	_con=con;
	AnsiString s;

	slExtraArBelegp=new TStringList();
	slExtraKuBeleg=new TStringList();

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	// User ermitteln
	s="select kuerzel, name from SL_Daten.dbo.[PASSWORT] where sqlserverbenutzer = (select login_name collate Latin1_General_CS_AS from sys.dm_exec_sessions where session_id = @@SPID)";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		_sUser=q->FieldByName("Kuerzel")->AsString;
		_sName=q->FieldByName("Name")->AsString;
		}
	else
		{
		_sUser="";
		_sName="";
		}

	// Standort ermitteln
	s="select wert from inifiles where inifilename='MANDANT.INI' and block='FAKT' and eintrag='STANDORT'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		_sStandort=q->FieldByName("Wert")->AsString;
	else
		_sStandort="";

	// Version ermitteln
	s="select wert from IniFiles where IniFileName='MANDANT.INI' and eintrag='VERSION'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		s=q->FieldByName("Wert")->AsString;
		_strVersion1=s.SubString(1, s.AnsiPos(".")-1);
		s.Delete(1, _strVersion1.Length()+1);
		_strVersion2=s.SubString(1, s.AnsiPos(".")-1);
		s.Delete(1, _strVersion2.Length()+1);
		_strVersion3=s.SubString(1, s.AnsiPos(" ")-1);
		}
	q->Close();


	delete q;
}

//---------------------------------------------------------------------------

TWawi::~TWawi(void)
{
	delete slExtraKuBeleg;
	delete slExtraArBelegp;
}

//---------------------------------------------------------------------------

UnicodeString TWawi::getNextKey(UnicodeString sBlobKey)
{
	UnicodeString s, sNewKey, sKey;
	char c;
	bool bShift=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from [key] where blobkey='"+sBlobKey+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		sNewKey=q->FieldByName("LastKey")->AsString;
	else
		sNewKey="";

	q->Close();
	delete q;

	if(sNewKey != "")
		{
		sKey=sNewKey;

		int iIndex=sKey.Length();
		while(iIndex > 0)
			{
			c=sKey[iIndex];
			if(c < '9')
				c++;
			else if(c == '9')
				{
				c='0';
				if(iIndex == 1)
					{
					sKey[iIndex]=c;
					sKey="1"+sKey;
          break;
					}
				else
					bShift=true;
				}
			else if(c >= 'a' && c < 'z')
				c++;
			else if(c >= 'A' && c < 'Z')
				c++;
			else if(c == 'Z')
				{
				c='0';
				bShift=true;
				}
			else if(c == 'z')
				{
				c='0';
				bShift=true;
				}

			sKey[iIndex]=c;

			if(bShift)
				{
				bShift=false;
				iIndex--;
				}
			else
				break;
			}
		sNewKey=sKey;
		}
	else
		{
    sNewKey=Now().FormatString("yy")+"000001";
    }

	return(sNewKey);
}

//---------------------------------------------------------------------------

UnicodeString TWawi::setNextKey(UnicodeString sBlobKey, UnicodeString sNewKey)
{
	UnicodeString s, sKey;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from [key] where blobkey='"+sBlobKey+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		sKey=q->FieldByName("LastKey")->AsString;
	else
		sKey="";

	q->Close();

	s="update [key] set PrevKey=LastKey, LastKey='"+sNewKey+"' where BlobKey='"+sBlobKey+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;

	return(sKey);
}

//---------------------------------------------------------------------------

/*
UnicodeString TWawi::getSQLFloat(double dValue)
{
	UnicodeString s;
	s.sprintf(L"%f", dValue);
	return(s);
}
*/

//---------------------------------------------------------------------------

double TWawi::getSteuerProzent(UnicodeString sSteuerCode)
{
	UnicodeString s;
	double dSteuerProzent;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select top 1 * from usdetail where steuercode='"+sSteuerCode+"' and gueltigab <= GetDate() order by gueltigab desc";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		dSteuerProzent=q->FieldByName("Steuer")->AsFloat;
	else
		dSteuerProzent=0.0;
	q->Close();
	delete q;

	return(dSteuerProzent);
}

//---------------------------------------------------------------------------

double TWawi::getArtikelVKPreis(UnicodeString sArtikelnummer, UnicodeString sWaehrung, double dMenge, int iPreisgruppe, UnicodeString sKundennummer)
{
	UnicodeString s, sPreisKunde;
	double dPreis=0;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select preisverweis from kunden where nummer='"+sKundennummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		sPreisKunde=q->FieldByName("preisverweis")->AsString;
	q->Close();

	if(sPreisKunde != "")
		sKundennummer=sPreisKunde;

	s="select top 1 * from arpreis where artikelnummer='"+sArtikelnummer+"' and preistyp='K' "
		"and typnummer='"+sKundennummer+"' and menge <= '"+getSQLFloat(dMenge)+"' "
		"and (VonDatum <= GetDate() or VonDatum is null) and (BisDatum > GETDATE() or BisDatum is null) "
		"order by menge desc";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		dPreis=q->FieldByName("Preis")->AsFloat;
	else if(iPreisgruppe > 0)
		{
		q->Close();
		s="select dbo.sl_fnPreis('"+sArtikelnummer+"',GetDate(), '"+sWaehrung+"', "+IntToStr(iPreisgruppe)+", 0, 100) Preis";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			dPreis=q->FieldByName("Preis")->AsFloat;
		}

	if(dPreis == 0.0)
		{
		q->Close();
		s="select dbo.SL_fnListenpreis('"+sArtikelnummer+"',GetDate(), '"+sWaehrung+"', 0, 100) Preis";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			dPreis=q->FieldByName("Preis")->AsFloat;
    }

	q->Close();

	delete q;

	return(dPreis);
}

//---------------------------------------------------------------------------
//ToDo: Preisfindung EK implementieren
double TWawi::getArtikelEKPreis(UnicodeString sArtikelnummer, double dMenge, UnicodeString sLieferantennummer)
{
	UnicodeString s;
	double dPreis=0;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select letzterek from artlief where lieferant='"+sLieferantennummer+"' and artikelnummer='"+sArtikelnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		dPreis=q->FieldByName("letzterek")->AsFloat;
  q->Close();

	delete q;
	return(dPreis);
}

//---------------------------------------------------------------------------

double TWawi::getWaehrungsfaktor(UnicodeString sWaehrung, TDateTime tdDatum)
{
	UnicodeString s;
	double dWaehrungsfaktor;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select top 1 * from waehrp where waehrungscode='"+sWaehrung+"' and datum >= GetDate()-1 order by datum desc";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		dWaehrungsfaktor=q->FieldByName("Kurs")->AsFloat;
	else
		{
		q->Close();
		s="select * from waehr where waehrungscode='"+sWaehrung+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			dWaehrungsfaktor=q->FieldByName("KalkulationsKurs")->AsFloat;
		else
			dWaehrungsfaktor=1.0;
		}
	q->Close();
	delete q;

	return(dWaehrungsfaktor);
}

//---------------------------------------------------------------------------

double TWawi::getBestand(UnicodeString sArtikelnummer, UnicodeString sLager)
{
	AnsiString s;
	double dBestand=0;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select sum(bestand) as bestand from lagerbestand where artikelnummer='"+sArtikelnummer+"' ";
	if(sLager != "")
		s+="and lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		dBestand=q->FieldByName("Bestand")->AsFloat;
	q->Close();
	delete q;

	return(dBestand);
}

//---------------------------------------------------------------------------

bool TWawi::addBestand(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert, AnsiString sSerieCharge, int iTyp)
{
	AnsiString s;
	UnicodeString sMenge, sWert, sSCTyp, sSerieId;
	UnicodeString sVerfallsdatum="NULL";
	double dBestand;

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge);
	sWert.sprintf(L"%f", dWert);

	if(sSerieCharge != "")
		{
		TADOQuery *q2=new TADOQuery(NULL);
		q2->Connection=_con;

		if(AnsiContainsStr(sSerieCharge, "|"))
			{
			s=sSerieCharge;
			sSerieCharge=s.SubString(1, s.AnsiPos("|")-1);
			s.Delete(1, sSerieCharge.Length()+1);
			sVerfallsdatum=s;
			}

		s="select * from serie where seriecharge='"+getCheckedSQLStr(sSerieCharge)+"'"; // and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			{
			sSerieId=q2->FieldByName("id")->AsString;
			sSCTyp=q2->FieldByName("SCTyp")->AsString;
			}
		q2->Close();
		delete q2;

		if(sBelegpKennung != "" && sSerieId == "")
			return(false);
		}

	if(sBelegpKennung == "") // wenn keine Kennung, dann manuelle Lagerbuchung anlegen
		{
		TBeleg *beleg=new TBeleg(_con);
		UnicodeString sBeleg;
		sBeleg=beleg->create("M", "");
		beleg->setDatum(tdDatum);

		TBelegp *belegp=new TBelegp(beleg);
		bool bSt=belegp->create("A");
		if(bSt)
			{
			belegp->setArtikel(sArtikelnummer);
			s=belegp->getSeriecharge();
			belegp->setLager(sLager);
			belegp->setMenge(dMenge);
			belegp->setEinzelpreis(0);
			if(belegp->getSeriecharge() == "S")
				belegp->addSNr(sSerieCharge);
			else if(belegp->getSeriecharge() == "C")
				belegp->addCharge(sSerieCharge, dMenge);
			belegp->save();   // Achtung - löst einen Rekursiven Aufruf dieser Funktion aus !!!!
			sBelegpKennung=belegp->getKennung();  // muss nach dem save aufgerufen werden

			delete belegp;
			}

		beleg->save();
		delete beleg;

		if(sBelegpKennung == "")
			return(false);
		else
			return(true);
		}


	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s=
		"BEGIN TRANSACTION;"
		" "
		"BEGIN TRY "
		"declare @returncode int "
		"declare @Id uniqueidentifier "
		"set @Id=NewID() "
		"declare @lagerplatzid int "
		"set @lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') "
		"declare @typ int "
		"set @typ='"+IntToStr(iTyp)+"' "
		"declare @lid int "
		"declare @bestand float "
		"declare @wert float "
		"set @wert='"+sWert+"' "
		"declare @rest float "
		"declare @menge float "
		"declare @zugang float "
		"set @zugang='"+sMenge+"' "
		"declare @TabId uniqueidentifier "
		"declare @verfallsdatum datetime ";
	if(sVerfallsdatum != "NULL")
		s+="set @verfallsdatum='"+sVerfallsdatum+"' ";
	else
		s+="set @verfallsdatum=NULL ";
	s+=
		" "
		"DECLARE cur CURSOR SCROLL FOR ";
	if(sVerfallsdatum != "NULL")
		s+="select lagerbestand_id, Bestand, Id, Wert from lagerbestand where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum=@verfallsdatum and bestand < 0 order by datum ";
	else
		s+="select lagerbestand_id, Bestand, Id, Wert from lagerbestand where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum is null and bestand < 0 order by datum ";
	s+=
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"WHILE @@FETCH_STATUS = 0 "
		"BEGIN "
		"  if(@bestand < @zugang) "
		"  begin "
		"    update lagerbestand set bestand=bestand+@zugang where lagerbestand_id=@lid "
		"    set @menge=@zugang "
		"    set @zugang=0 "
		"  end "
		"  else "
		"  begin "
		"    update lagerbestand set bestand=0 where lagerbestand_id=@lid "
		"    set @menge=@bestand*-1 "
		"    set @zugang=@zugang-@menge "
		"  end "
		" "
		"insert into lagerprotokoll (BELEGPKennung,	Typ, Tabelle,	Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																"SerieId, SCTyp, SerieCharge,	Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
		 "Values('"+sBelegpKennung+"', @typ, '"+IntToStr(ltaLagerbestand)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', '0', @menge, @wert, "
						"'"+sSerieId+"', '"+sSCTyp+"','"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @TabId, '"+this->_sUser+"', GetDate()) "
		" "
		"  if(@zugang <= 0) "
		"    break "
		"  FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"END "
		"DEALLOCATE cur "
		" "
		"if(@zugang != 0) " //wenn noch rest da ist diesen anlegen
		"BEGIN "
			//Lagerbestand
			//"declare @lagerbestand_id int "
			//"set @lagerbestand_id=(select lagerbestand_id from lagerbestand where id=(select top 1 tabid from lagerprotokoll where belegpkennung='"+sBelegpKennung+"')) "
			//"if(@lagerbestand_id is not null) " //wenn da dann updaten
			//"begin "
			//	"update lagerbestand set bestand=bestand+@zugang where lagerbestand_id=@lagerbestand_id "
			//	"set @Id=(select id from lagerbestand where lagerbestand_id=@lagerbestand_id) "
			//"end "
			//"else begin "
				"insert into lagerbestand (ID, Datum, Artikelnummer, Bestand, Wert, SerieId, Preismenge, Verfallsdatum, LagerplatzId, BelegpKennung, AngelegtVon, AngelegtAm) "
				"Values(@Id, '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @zugang, @wert, '"+sSerieId+"', '1', @verfallsdatum, @lagerplatzid, NULL, '"+this->_sUser+"', GetDate()) "
			//"end "
			//Protokoll
			"insert into lagerprotokoll (BELEGPKennung,	Typ, Tabelle,	Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																	"SerieId, SCTyp, SerieCharge,	Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
			"Values('"+sBelegpKennung+"', @typ, '"+IntToStr(ltaLagerbestand)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', '0', @zugang, @wert, "
							"'"+sSerieId+"', '"+sSCTyp+"', '"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @Id, '"+this->_sUser+"', GetDate()) "
		"END "
		"END TRY "
		"BEGIN CATCH "
		"IF @@TRANCOUNT > 0 "
		"  ROLLBACK TRANSACTION; "
		"set @returncode='0' "
		"END CATCH; "
		"IF @@TRANCOUNT > 0 "
		"BEGIN "
		"  COMMIT TRANSACTION; "
		"  set @returncode='1' "
		"END "
		"select @returncode as returncode";
	int iReturncode=0;
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		iReturncode=q->FieldByName("returncode")->AsInteger;
	q->Close();

	if(iReturncode && sSerieId != "0")
		{
		s="select * from sninfo where belegpkennung='"+sBelegpKennung+"' and typ='Z' and SerieId='"+sSerieId+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount == 0)
			{
			q->Close();
				 // Wenn alles OK war und Serie/Charge dabei, dann Info-Eintrag erstellen
			s="insert into sninfo (BelegpKennung, Typ, SerieId, AngelegtAm, AngelegtVon) "
				"Values ('"+sBelegpKennung+"','Z','"+sSerieId+"', GetDate(), '"+this->_sUser+"')";
			q->SQL->Text=s;
			q->ExecSQL();
			}
		}

	delete q;

	return(iReturncode);
}

//---------------------------------------------------------------------------

bool TWawi::addPack(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert, AnsiString sSerieCharge)
{
	AnsiString s;
	UnicodeString sMenge, sWert, sSCTyp, sSerieId;
	UnicodeString sVerfallsdatum="NULL";
	double dBestand;

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge);
	sWert.sprintf(L"%f", dWert);

	if(sSerieCharge != "")
		{
		TADOQuery *q2=new TADOQuery(NULL);
		q2->Connection=_con;

		if(AnsiContainsStr(sSerieCharge, "|"))
			{
			s=sSerieCharge;
			sSerieCharge=s.SubString(1, s.AnsiPos("|")-1);
			s.Delete(1, sSerieCharge.Length()+1);
			sVerfallsdatum=s;
			}

		s="select * from serie where seriecharge='"+getCheckedSQLStr(sSerieCharge)+"'"; // and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			{
			sSerieId=q2->FieldByName("id")->AsString;
			sSCTyp=q2->FieldByName("SCTyp")->AsString;
			}
		q2->Close();
		delete q2;

		if(sBelegpKennung != "" && sSerieId == "")
			return(false);
		}

	if(sBelegpKennung == "") // wenn keine Kennung, dann manuelle Lagerbuchung anlegen
		{
		TBeleg *beleg=new TBeleg(_con);
		UnicodeString sBeleg;
		sBeleg=beleg->create("M", "");
		beleg->setDatum(tdDatum);

		TBelegp *belegp=new TBelegp(beleg);
		bool bSt=belegp->create("A");
		if(bSt)
			{
			belegp->setArtikel(sArtikelnummer);
			s=belegp->getSeriecharge();
			belegp->setLager(sLager);
			belegp->setMenge(dMenge);
			belegp->setEinzelpreis(0);
			if(belegp->getSeriecharge() == "S")
				belegp->addSNr(sSerieCharge);
			else if(belegp->getSeriecharge() == "C")
				belegp->addCharge(sSerieCharge, dMenge);
			belegp->save();   // Achtung - löst einen Rekursiven Aufruf dieser Funktion aus !!!!
			sBelegpKennung=belegp->getKennung();  // muss nach dem save aufgerufen werden

			delete belegp;
			}

		beleg->save();
		delete beleg;

		if(sBelegpKennung == "")
			return(false);
		else
			return(true);
		}


	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;

	s=
		"BEGIN TRANSACTION;"
		" "
		"BEGIN TRY "
		"declare @returncode int "
		"declare @Id uniqueidentifier "
		"set @Id=NewID() "
		"declare @lagerplatzid int "
		"set @lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') "
		"declare @typ int "
		"set @typ='"+IntToStr(ltyPacken)+"' "
		"declare @lid int "
		"declare @bestand float "
		"declare @wert float "
		"set @wert='"+sWert+"' "
		"declare @rest float "
		"declare @menge float "
		"declare @zugang float "
		"set @zugang='"+sMenge+"' "
		"declare @TabId uniqueidentifier "
		"declare @verfallsdatum datetime ";
	if(sVerfallsdatum != "NULL")
		s+="set @verfallsdatum='"+sVerfallsdatum+"' ";
	else
		s+="set @verfallsdatum=NULL ";
	s+=
		" "
		"DECLARE cur CURSOR SCROLL FOR ";
	if(sVerfallsdatum != "NULL")
		s+="select lagerbestandpack_id, Bestand, Id, Wert from lagerbestandpack where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum=@verfallsdatum and bestand < 0 order by datum ";
	else
		s+="select lagerbestandpack_id, Bestand, Id, Wert from lagerbestandpack where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum is null and bestand < 0 order by datum ";
	s+=
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"WHILE @@FETCH_STATUS = 0 "
		"BEGIN "
		"  if(@bestand < @zugang) "
		"  begin "
		"    update lagerbestandpack set bestand=bestand+@zugang, menge=menge+@zugang where lagerbestandpack_id=@lid "
		"    set @menge=@zugang "
		"    set @zugang=0 "
		"  end "
		"  else "
		"  begin "
		"    update lagerbestandpack set bestand=0, menge=0 where lagerbestandpack_id=@lid "
		"    set @menge=@bestand*-1 "
		"    set @zugang=@zugang-@menge "
		"  end "
		" "
		"insert into lagerprotokoll (BELEGPKennung,	Typ, Tabelle,	Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																"SerieId, SCTyp, SerieCharge,	Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
		 "Values('"+sBelegpKennung+"', @typ, '"+IntToStr(ltaLagerbestandAndPack)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @menge, @menge, @wert, "
						"'"+sSerieId+"', '"+sSCTyp+"','"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @TabId, '"+this->_sUser+"', GetDate()) "
		" "
		"  if(@zugang <= 0) "
		"    break "
		"  FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"END "
		"DEALLOCATE cur "
		" "
		"if(@zugang != 0) " //wenn noch rest da ist diesen anlegen
		"BEGIN "
			//Lagerbestand
			//"declare @lagerbestand_id int "
			//"set @lagerbestand_id=(select lagerbestand_id from lagerbestand where id=(select top 1 tabid from lagerprotokoll where belegpkennung='"+sBelegpKennung+"')) "
			//"if(@lagerbestand_id is not null) " //wenn da dann updaten
			//"begin "
			//	"update lagerbestand set bestand=bestand+@zugang where lagerbestand_id=@lagerbestand_id "
			//	"set @Id=(select id from lagerbestand where lagerbestand_id=@lagerbestand_id) "
			//"end "
			//"else begin "
				"insert into lagerbestandpack (ID, Datum, Artikelnummer, Bestand, Menge, Wert, SerieId, Preismenge, Verfallsdatum, LagerplatzId, AngelegtVon, AngelegtAm) "
				"Values(@Id, '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @zugang, @zugang, @wert, '"+sSerieId+"', '1', @verfallsdatum, @lagerplatzid, '"+this->_sUser+"', GetDate()) "
			//"end "
			//Protokoll
			"insert into lagerprotokoll (BELEGPKennung,	Typ, Tabelle,	Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																	"SerieId, SCTyp, SerieCharge,	Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
			"Values('"+sBelegpKennung+"', @typ, '"+IntToStr(ltaLagerbestandAndPack)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @zugang, @zugang, @wert, "
							"'"+sSerieId+"', '"+sSCTyp+"', '"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @Id, '"+this->_sUser+"', GetDate()) "
		"END "
		"END TRY "
		"BEGIN CATCH "
		"IF @@TRANCOUNT > 0 "
		"  ROLLBACK TRANSACTION; "
		"set @returncode='0' "
		"END CATCH; "
		"IF @@TRANCOUNT > 0 "
		"BEGIN "
		"  COMMIT TRANSACTION; "
		"  set @returncode='1' "
		"END "
		"select @returncode as returncode";
	int iReturncode=0;
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		iReturncode=q->FieldByName("returncode")->AsInteger;
	q->Close();

	if(iReturncode && sSerieId != "0")
		{
		s="select * from sninfo where belegpkennung='"+sBelegpKennung+"' and typ='Z' and SerieId='"+sSerieId+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount == 0)
			{
			q->Close();
				 // Wenn alles OK war und Serie/Charge dabei, dann Info-Eintrag erstellen
			s="insert into sninfo (BelegpKennung, Typ, SerieId, AngelegtAm, AngelegtVon) "
				"Values ('"+sBelegpKennung+"','Z','"+sSerieId+"', GetDate(), '"+this->_sUser+"')";
			q->SQL->Text=s;
			q->ExecSQL();
			}
		}

	delete q;

	return(iReturncode);
}

//---------------------------------------------------------------------------

bool TWawi::subBestand(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, AnsiString sSerieCharge, int iTyp)
{
	AnsiString s;
	UnicodeString sMenge, sWert, sSerieId, sSCTyp;
	UnicodeString sVerfallsdatum="NULL";
	double dBestand;

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge);
	sWert.sprintf(L"%f", dMenge);

	if(sSerieCharge != "")
		{
		TADOQuery *q2=new TADOQuery(NULL);
		q2->Connection=_con;
        q2->LockType=ltReadOnly;
		if(AnsiContainsStr(sSerieCharge, "|"))
			{
			s=sSerieCharge;
			sSerieCharge=s.SubString(1, s.AnsiPos("|")-1);
			s.Delete(1, sSerieCharge.Length()+1);
			sVerfallsdatum=s;
			}

		s="select * from serie where seriecharge='"+getCheckedSQLStr(sSerieCharge)+"'"; // and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			{
			sSerieId=q2->FieldByName("id")->AsString;
			sSCTyp=q2->FieldByName("SCTyp")->AsString;
			}
		q2->Close();
		delete q2;

		if(sSerieId == "")
			return(false);
		}

	if(sBelegpKennung == "") // wenn keine Kennung, dann manuelle Lagerbuchung anlegen
		{
		TBeleg *beleg=new TBeleg(_con);
		UnicodeString sBeleg;
		sBeleg=beleg->create("M", "");
		beleg->setDatum(tdDatum);

		TBelegp *belegp=new TBelegp(beleg);
		bool bSt=belegp->create("A");
		if(bSt)
			{
			belegp->setArtikel(sArtikelnummer);
			belegp->setLager(sLager);
			belegp->setMenge(-dMenge);
			belegp->setEinzelpreis(0);
			if(sSCTyp == "S")
				belegp->addSNr(sSerieCharge);
			else if(sSCTyp == "C")
				belegp->addCharge(sSerieCharge, dMenge);
			belegp->save();
			sBelegpKennung=belegp->getKennung();  // muss nach dem save aufgerufen werden

			delete belegp;
			}

		beleg->save();
		delete beleg;

		if(sBelegpKennung == "")
			return(false);
		else
			return(true);
		}


	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="BEGIN TRANSACTION;"
		" "
		"BEGIN TRY "
		"declare @returncode int "
		"declare @lid int "
		"declare @bestand float "
		"declare @wert float "
		"set @wert='"+sWert+"' "
		"declare @rest float "
		"declare @menge float "
		"declare @abgang float "
		"set @abgang='"+sMenge+"' "
		"declare @lagerplatzid int "
		"set @lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') "
		"declare @TabId uniqueidentifier "
		"declare @Id uniqueidentifier "
		"set @Id=NewID() "
		"declare @verfallsdatum datetime ";
	if(sVerfallsdatum != "NULL")
		s+="set @verfallsdatum='"+sVerfallsdatum+"' ";
	else
		s+="set @verfallsdatum=NULL ";
	s+=
		" "
		"DECLARE cur CURSOR SCROLL FOR ";
	if(sVerfallsdatum != "NULL")
		s+="select lagerbestand_id, Bestand, Id, Wert from lagerbestand where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum=@verfallsdatum and bestand > 0 ""order by datum ";
	else
		s+="select lagerbestand_id, Bestand, Id, Wert from lagerbestand where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum is NULL and bestand > 0 ""order by datum ";
	s+=
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"WHILE @@FETCH_STATUS = 0 "
		"BEGIN "
		"  if(@bestand > @abgang) "
		"  begin "
		"    update lagerbestand set bestand=bestand-@abgang where lagerbestand_id=@lid "
		"    set @menge=@abgang*-1 "
		"    set @abgang=0 "
		"  end "
		"  else "
		"  begin "
		"    update lagerbestand set bestand=0 where lagerbestand_id=@lid "
		"    set @menge=@bestand*-1 "
		"    set @abgang=@abgang-@bestand "
		"  end "
		//Protokolleintrag einfügen
		"  insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																	"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
		"  Values('"+sBelegpKennung+"', '"+IntToStr(iTyp)+"', '"+IntToStr(ltaLagerbestand)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', '0', @menge, @wert,"
						"'"+sSerieId+"', '"+sSCTyp+"', '"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @TabId, '"+this->_sUser+"', GetDate()) "
		" "
		"  if(@abgang <= 0) "
		"    break "
		"  FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"END "
		"DEALLOCATE cur "
		" "
		"if(@abgang != 0) "
		"BEGIN "
			//Lagerbestand
			//"declare @lagerbestand_id int "
			//"set @lagerbestand_id=(select lagerbestand_id from lagerbestand where id=(select top 1 tabid from lagerprotokoll where belegpkennung='"+sBelegpKennung+"')) "
			//"if(@lagerbestand_id is not null) " //wenn da dann updaten
			//"begin "
			//	"update lagerbestand set bestand=bestand-@abgang where lagerbestand_id=@lagerbestand_id "
			//	"set @Id=(select id from lagerbestand where lagerbestand_id=@lagerbestand_id) "
			//"end "
			//"else begin "
				"insert into lagerbestand (ID, Datum, Artikelnummer, Bestand, Wert, SerieId, Preismenge, Verfallsdatum, LagerplatzId, BelegpKennung, AngelegtVon, AngelegtAm) "
				"Values(@Id, '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @abgang*-1, @wert, '"+sSerieId+"', '1', @verfallsdatum, @lagerplatzid, NULL, '"+this->_sUser+"', GetDate()) "
			//"end "
			//Protokolleintrag einfügen
			"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																	"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
			"Values('"+sBelegpKennung+"', '"+IntToStr(iTyp)+"', '"+IntToStr(ltaLagerbestand)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', '0', @abgang*-1, @Wert,"
						 "'"+sSerieId+"', '"+sSCTyp+"','"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @Id, '"+this->_sUser+"', GetDate()) "
		"END "
		"END TRY "
		"BEGIN CATCH "
		"IF @@TRANCOUNT > 0 "
		"  ROLLBACK TRANSACTION; "
		"set @returncode='0' "
		"END CATCH; "
		"IF @@TRANCOUNT > 0 "
		"BEGIN "
		"  COMMIT TRANSACTION; "
		"  set @returncode='1' "
		"END "
		"select @returncode as returncode";
	int iReturncode=0;
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		iReturncode=q->FieldByName("returncode")->AsInteger;
	q->Close();

	if(iReturncode && sSerieId != "0")
		{          // Wenn alles OK war und Serie/Charge dabei, dann Info-Eintrag erstellen
		s="select * from sninfo where belegpkennung='"+sBelegpKennung+"' and typ='A' and SerieId='"+sSerieId+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount == 0)
			{
			q->Close();
			s="insert into sninfo (BelegpKennung, Typ, SerieId, AngelegtAm, AngelegtVon) "
				"Values ('"+sBelegpKennung+"','A','"+sSerieId+"', GetDate(), '"+this->_sUser+"')";
			q->SQL->Text=s;
			q->ExecSQL();
			}
		}

	delete q;

	return(iReturncode);
}

//---------------------------------------------------------------------------

bool TWawi::subPack(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, AnsiString sSerieCharge)
{
	AnsiString s;
	UnicodeString sMenge, sWert, sSerieId, sSCTyp;
	UnicodeString sVerfallsdatum="NULL";
	double dBestand;

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge);
	sWert.sprintf(L"%f", dMenge);

	if(sSerieCharge != "")
		{
		TADOQuery *q2=new TADOQuery(NULL);
		q2->Connection=_con;

		if(AnsiContainsStr(sSerieCharge, "|"))
			{
			s=sSerieCharge;
			sSerieCharge=s.SubString(1, s.AnsiPos("|")-1);
			s.Delete(1, sSerieCharge.Length()+1);
			sVerfallsdatum=s;
			}

		s="select * from serie where seriecharge='"+getCheckedSQLStr(sSerieCharge)+"'"; // and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			{
			sSerieId=q2->FieldByName("id")->AsString;
			sSCTyp=q2->FieldByName("SCTyp")->AsString;
			}
		q2->Close();
		delete q2;

		if(sSerieId == "")
			return(false);
		}

	if(sBelegpKennung == "") // wenn keine Kennung, dann manuelle Lagerbuchung anlegen
		{
		TBeleg *beleg=new TBeleg(_con);
		UnicodeString sBeleg;
		sBeleg=beleg->create("M", "");
		beleg->setDatum(tdDatum);

		TBelegp *belegp=new TBelegp(beleg);
		bool bSt=belegp->create("A");
		if(bSt)
			{
			belegp->setArtikel(sArtikelnummer);
			belegp->setLager(sLager);
			belegp->setMenge(-dMenge);
			belegp->setEinzelpreis(0);
			if(sSCTyp == "S")
				belegp->addSNr(sSerieCharge);
			else if(sSCTyp == "C")
				belegp->addCharge(sSerieCharge, dMenge);
			belegp->save();
			sBelegpKennung=belegp->getKennung();  // muss nach dem save aufgerufen werden

			delete belegp;
			}

		beleg->save();
		delete beleg;

		if(sBelegpKennung == "")
			return(false);
		else
			return(true);
		}


	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;

	s="BEGIN TRANSACTION;"
		" "
		"BEGIN TRY "
		"declare @returncode int "
		"declare @lid int "
		"declare @bestand float "
		"declare @wert float "
		"set @wert='"+sWert+"' "
		"declare @rest float "
		"declare @menge float "
		"declare @abgang float "
		"set @abgang='"+sMenge+"' "
		"declare @lagerplatzid int "
		"set @lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') "
		"declare @TabId uniqueidentifier "
		"declare @Id uniqueidentifier "
		"set @Id=NewID() "
		"declare @verfallsdatum datetime ";
	if(sVerfallsdatum != "NULL")
		s+="set @verfallsdatum='"+sVerfallsdatum+"' ";
	else
		s+="set @verfallsdatum=NULL ";
	s+=
		" "
		"DECLARE cur CURSOR SCROLL FOR ";
	if(sVerfallsdatum != "NULL")
		s+="select lagerbestandpack_id, Bestand, Id, Wert from lagerbestandpack where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum=@verfallsdatum and bestand > 0 ""order by datum ";
	else
		s+="select lagerbestandpack_id, Bestand, Id, Wert from lagerbestandpack where artikelnummer='"+sArtikelnummer+"' and lagerplatzid=@lagerplatzid and serieid='"+sSerieId+"' and verfallsdatum is NULL and bestand > 0 ""order by datum ";
	s+=
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"WHILE @@FETCH_STATUS = 0 "
		"BEGIN "
		"  if(@bestand > @abgang) "
		"  begin "
		"    update lagerbestandpack set bestand=bestand-@abgang, menge=menge-@abgang where lagerbestandpack_id=@lid "
		"    set @menge=@abgang*-1 "
		"    set @abgang=0 "
		"  end "
		"  else "
		"  begin "
		"    update lagerbestandpack set bestand=0, menge=0 where lagerbestandpack_id=@lid "
		"    set @menge=@bestand*-1 "
		"    set @abgang=@abgang-@bestand "
		"  end "
		//Protokolleintrag einfügen
		"  insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																	"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
		"  Values('"+sBelegpKennung+"', '"+IntToStr(ltyAbgangFuerPack)+"', '"+IntToStr(ltaLagerbestandAndPack)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @menge, @menge, @wert,"
						"'"+sSerieId+"', '"+sSCTyp+"', '"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @TabId, '"+this->_sUser+"', GetDate()) "
		" "
		"  if(@abgang <= 0) "
		"    break "
		"  FETCH NEXT FROM cur INTO @lid, @bestand, @TabId, @Wert "
		"END "
		"DEALLOCATE cur "
		" "
		"if(@abgang != 0) "
		"BEGIN "
			//Lagerbestand
			//"declare @lagerbestand_id int "
			//"set @lagerbestand_id=(select lagerbestand_id from lagerbestand where id=(select top 1 tabid from lagerprotokoll where belegpkennung='"+sBelegpKennung+"')) "
			//"if(@lagerbestand_id is not null) " //wenn da dann updaten
			//"begin "
			//	"update lagerbestand set bestand=bestand-@abgang where lagerbestand_id=@lagerbestand_id "
			//	"set @Id=(select id from lagerbestand where lagerbestand_id=@lagerbestand_id) "
			//"end "
			//"else begin "
				"insert into lagerbestandpack (ID, Datum, Artikelnummer, Bestand, Menge, Wert, SerieId, Preismenge, Verfallsdatum, LagerplatzId, AngelegtVon, AngelegtAm) "
				"Values(@Id, '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @abgang*-1, @abgang*-1, @wert, '"+sSerieId+"', '1', @verfallsdatum, @lagerplatzid, '"+this->_sUser+"', GetDate()) "
			//"end "
			//Protokolleintrag einfügen
			"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																	"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
			"Values('"+sBelegpKennung+"', '"+IntToStr(ltyAbgangFuerPack)+"', '"+IntToStr(ltaLagerbestandAndPack)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @abgang*-1, @abgang*-1, @Wert,"
						 "'"+sSerieId+"', '"+sSCTyp+"','"+sSerieCharge+"', 1.0, @verfallsdatum, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @Id, '"+this->_sUser+"', GetDate()) "
		"END "
		"END TRY "
		"BEGIN CATCH "
		"IF @@TRANCOUNT > 0 "
		"  ROLLBACK TRANSACTION; "
		"set @returncode='0' "
		"END CATCH; "
		"IF @@TRANCOUNT > 0 "
		"BEGIN "
		"  COMMIT TRANSACTION; "
		"  set @returncode='1' "
		"END "
		"select @returncode as returncode";
	int iReturncode=0;
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		iReturncode=q->FieldByName("returncode")->AsInteger;
	q->Close();

	if(iReturncode && sSerieId != "0")
		{          // Wenn alles OK war und Serie/Charge dabei, dann Info-Eintrag erstellen
		s="select * from sninfo where belegpkennung='"+sBelegpKennung+"' and typ='A' and SerieId='"+sSerieId+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount == 0)
			{
			q->Close();
			s="insert into sninfo (BelegpKennung, Typ, SerieId, AngelegtAm, AngelegtVon) "
				"Values ('"+sBelegpKennung+"','A','"+sSerieId+"', GetDate(), '"+this->_sUser+"')";
			q->SQL->Text=s;
			q->ExecSQL();
			}
		}

	delete q;

	return(iReturncode);}

//---------------------------------------------------------------------------

bool TWawi::setReserviert(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert)
{
	UnicodeString s, sMenge, sWert, sSerieId;
	double dBestand;

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge*-1);
	sWert.sprintf(L"%f", dWert);

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s=
		"declare @Id uniqueidentifier "
		"set @Id=NewID() "
		"declare @lagerplatzid int "
		"set @lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') "
		"declare @typ int "
		"set @typ='"+IntToStr(ltyReservierung)+"' "
		"declare @lagerbedarf_id int "
		"set @lagerbedarf_id=(select lagerbedarf_id from lagerbedarf where menge != 0 and id in (select tabid from lagerprotokoll where belegpkennung='"+sBelegpKennung+"') and typ='1' and menge < 0) "
		"declare @wert float "
		"set @wert='"+sWert+"' "
		"declare @menge float "
		"set @menge='"+sMenge+"' "
		" "
		"if(@lagerbedarf_id is not null) " //wenn da dann updaten
		"begin "
			"update lagerbedarf set menge=menge+@menge, BearbeitetVon='"+this->_sUser+"', BearbeitetAm=GetDate() where lagerbedarf_id=@lagerbedarf_id "
			"set @Id=(select id from lagerbedarf where lagerbedarf_id=@lagerbedarf_id) "
		"end "
		"else begin "
			"insert into lagerbedarf (ID, Typ, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm) "
			"Values(@Id, 1, '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @menge, '"+sLager+"', @lagerplatzid, '"+this->_sStandort+"','"+this->_sUser+"', GetDate()) "
		"end "
		//Protokoll
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
		"Values('"+sBelegpKennung+"', @typ, '"+IntToStr(ltaLagerbedarf)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @menge, 0, '"+sWert+"',"
						"'"+sSerieId+"', '', '', 1.0, NULL, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @Id, '"+this->_sUser+"', GetDate()) ";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;
}

//---------------------------------------------------------------------------

bool TWawi::setWKReserviert(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sLager, double dMenge, UnicodeString sELager, UnicodeString sUser)
{
	UnicodeString s, sMenge, sWert, sSerieId;

	// Reservieren der Teile (für jede Reservierung wird ein Datensatz erstellt)
	// Eintrag in Werkauftragdetails für Teilepositionen mit Typ 1 (Reservierend)
	// Eintrag in Lagerprotokoll Typ 1 für Reservierung
	// Eintrag in lagerbedarf

	// Eintrag in Werkauftragdetails für Hauptposition mit Typ 6 (für jede Reservierung wird ein Datensatz erzeugt)
	// Eintrag in Lagerprotokoll Typ 6 für Fertigung
	// Eintrag in Lagerfertigung

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge*-1);

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @returncode int "
		"declare @belegpkennung uniqueidentifier "
		"set @belegpkennung='"+sBelegpKennung+"' "
		"declare @belegpkennungprod uniqueidentifier "
		"set @belegpkennungprod=(select kennung from belegp where belegtyp='W' and postext='' and belegnummer=(select belegnummer from belegp where kennung=@belegpkennung)) "
		" "
		"declare @standort nvarchar(10) "
		"set @standort=(select standort from belegp where kennung=@belegpkennung) "
		"declare @user nvarchar(2) "
		"set @user='"+sUser+"' "
		"declare @artikel nvarchar(18) "
		"set @artikel=(select artikelnummer from belegp where kennung=@belegpkennung) "
		"declare @partikel nvarchar(18) "
		"set @partikel=(select artikelnummer from belegp where kennung=@belegpkennungprod) "
		"declare @RWKId uniqueidentifier  "
		"set @RWKId=NewID()  "
		"declare @RId uniqueidentifier  "
		"set @RId=NewID()  "
		"declare @lager nvarchar(10) "
		"set @lager='"+sLager+"' "
		"declare @lagerplatzid int  "
		"set @lagerplatzid=(select id from lagerplatz where lager=@lager)  "
		"declare @Elager nvarchar(10) "
		"set @Elager='"+sELager+"' "
		"declare @Elagerplatzid int  "
		"set @Elagerplatzid=(select id from lagerplatz where lager=@Elager)  "
		"declare @EWKId uniqueidentifier "
		"set @EWKId=NewID()  "
		"declare @EId uniqueidentifier "
		"set @EId=NewID()  "
		"declare @menge float  "
		"set @menge='"+sMenge+"'  "
		"declare @datum datetime  "
		"set @datum='"+tdDatum.FormatString("dd.mm.yyyy")+"' "
		" "
		"-- Reservierung "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@belegpkennung, 3, @RWKId) "
		"  "
		"insert into lagerbedarf (ID, Typ, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm)  "
		"Values(@RId, 1, @datum, @artikel, @menge, @lager, @lagerplatzid, @standort, @user, GetDate())  "
		" "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
			"	SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@RWKId, '1', '1', CONVERT(date, GETDATE()), @datum, @artikel, @menge, 0, '0', "
			"'0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @RId, @user, GetDate()) "
		" "
		"-- später einzulagerndes fertiges teil "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@belegpkennungprod, 6, @EWKId) "
		"  "
		"insert into lagerfertigung (ID, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm)  "
		"Values(@EId, @datum, @artikel, @menge, @Elager, @Elagerplatzid, @standort, @user, GetDate())  "
		" "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
			"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@EWKId, '6', '4', CONVERT(date, GETDATE()), @datum, @partikel, @menge, 0, '0', "
			"'0', '', '', 1.0, NULL, @standort, @Elager, @Elagerplatzid, NULL, @EId, @user, GetDate()) "
		" "
		"-- Reservierte Menge im Beleg setzen "
		"update beleg set  "
		"  skontoprozent1=skontoprozent1-@menge, "
		"  skontoprozent2=skontoprozent2+@menge "
		"where belegtyp='W' and belegnummer=(select belegnummer from belegp where kennung=@belegpkennung) "
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode as returncode ";
	q->SQL->Text=s;
	q->Open();


	delete q;
}

//---------------------------------------------------------------------------

bool TWawi::setBestellt(UnicodeString sBelegpKennung, TDateTime tdDatum, UnicodeString sArtikelnummer, UnicodeString sLager, double dMenge, double dWert)
{
	UnicodeString s, sMenge, sWert, sSerieId;
	double dBestand;

	sSerieId="0";
	sMenge.sprintf(L"%f", dMenge);
	sWert.sprintf(L"%f", dWert);

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s=
		"declare @Id uniqueidentifier "
		"set @Id=NewID() "
		"declare @lagerplatzid int "
		"set @lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') "
		"declare @typ int "
		"set @typ='"+IntToStr(ltyBestellung)+"' "
		"declare @lagerbedarf_id int "
		"set @lagerbedarf_id=(select lagerbedarf_id from lagerbedarf where id=(select top 1 tabid from lagerprotokoll where belegpkennung='"+sBelegpKennung+"' and menge > 0 order by id desc)) "
		"declare @wert float "
		"set @wert='"+sWert+"' "
		"declare @menge float "
		"set @menge='"+sMenge+"' "
		" "
		"if(@lagerbedarf_id is not null) " //wenn da dann updaten
		"begin "
			"update lagerbedarf set menge=menge+@menge, BearbeitetVon='"+this->_sUser+"', BearbeitetAm=GetDate() where lagerbedarf_id=@lagerbedarf_id "
			"set @Id=(select id from lagerbedarf where lagerbedarf_id=@lagerbedarf_id) "
		"end "
		"else begin "
			"insert into lagerbedarf (ID, Typ, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm) "
			"Values(@Id, 2, '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @menge, '"+sLager+"', @lagerplatzid, '"+this->_sStandort+"','"+this->_sUser+"', GetDate()) "
		"end "
		//Protokoll
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert, "
																"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm) "
		"Values('"+sBelegpKennung+"', @typ, '"+IntToStr(ltaLagerbedarf)+"', CONVERT(date, GETDATE()), '"+tdDatum.FormatString("dd.mm.yyyy")+"', '"+sArtikelnummer+"', @menge, 0, '"+sWert+"',"
						"'"+sSerieId+"', '', '', 1.0, NULL, '"+this->_sStandort+"', '"+sLager+"', @lagerplatzid, NULL, @Id, '"+this->_sUser+"', GetDate()) ";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;
}

//---------------------------------------------------------------------------

int TWawi::checkSerieCharge(UnicodeString sSerieCharge, UnicodeString &sArtikelnummer, UnicodeString &sSCTyp)
{
	UnicodeString s, sExistingSCTyp;
	int iId=0;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from serie where seriecharge='"+sSerieCharge+"' ";
	//if(sArtikelnummer != "")
	//	s+="and artikelnummer='"+sArtikelnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iId=q->FieldByName("id")->AsInteger;
		sExistingSCTyp=q->FieldByName("SCTyp")->AsString;
		sArtikelnummer=q->FieldByName("Artikelnummer")->AsString;
		}
	q->Close();

	delete q;

	return(iId);
}

//---------------------------------------------------------------------------

int TWawi::addSerieCharge(UnicodeString sSerieCharge, UnicodeString sArtikelnummer, UnicodeString sSCTyp)
{
	UnicodeString s, sExistingSCTyp;
	int iId=0;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from serie where seriecharge='"+sSerieCharge+"' and artikelnummer='"+sArtikelnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iId=q->FieldByName("id")->AsInteger;
		sExistingSCTyp=q->FieldByName("SCTyp")->AsString;
		}
	q->Close();

	if(sExistingSCTyp != "")
		{
		if(sExistingSCTyp != sSCTyp)
			return(-1);
		}
	else
		{
		s="insert into serie (Artikelnummer, SerieCharge, SCTyp, AngelegtVon, AngelegtAm) "
			"Values('"+sArtikelnummer+"', '"+sSerieCharge+"', '"+sSCTyp+"', '"+this->_sUser+"', GetDate())";
		q->SQL->Text=s;
		q->ExecSQL();
		if(q->RowsAffected == 1)
			{
			s="select * from serie where seriecharge='"+sSerieCharge+"' and artikelnummer='"+sArtikelnummer+"'";
			q->SQL->Text=s;
			q->Open();
			if(q->RecordCount)
				iId=q->FieldByName("id")->AsInteger;
			q->Close();
			}
		else
			iId=-1;
		}

	delete q;

	return(iId);
}

//---------------------------------------------------------------------------

bool TWawi::setUebergeben(UnicodeString sBelegpKennung, UnicodeString sVorgaengerpKennung, double dMenge)
{
	AnsiString s, strBelegnummer, strBelegtyp, strMenge, strStatus;
	bool bSt;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	strMenge=s.sprintf("%0.4f", dMenge);

	s=
		 "declare @menge float "
		 "set @menge='"+strMenge+"' "
		 // Vorgaenger aktuelle Postition eintragen
		 "update belegp set vorgaenger='"+sVorgaengerpKennung+"', uebernahme=isnull(uebernahme, 0)+@menge where kennung='"+sBelegpKennung+"' "
		 // Übernommene Postition anpassen
		 "update belegp set uebernahmeoffen=isnull(uebernahmeoffen, 0)-@menge, verwendet=isnull(verwendet, 0)+@menge where kennung='"+sVorgaengerpKennung+"' "
		 // Übernommene Postition erledigen wenn dUebernahmeoffen und dVerwendet >= menge
		 "update belegp set status='2' where kennung='"+sVorgaengerpKennung+"' and verwendet >= menge "
		 // Evtl. geschlossenen Beleg öffnen wenn schon zu
		 "update belegp set status='0' where kennung='"+sVorgaengerpKennung+"' and verwendet < menge and status='2' ";
	q->SQL->Text=s;
	q->ExecSQL();

	// Übernommenen Beleg pruefen und evtl. erledigen
	s="select b.belegnummer, b.belegtyp, b.status from belegp bp "
		"left join beleg b on (b.belegnummer=bp.belegnummer and b.belegtyp=bp.belegtyp) "
		"where bp.kennung='"+sVorgaengerpKennung+"' ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		strBelegnummer=q->FieldByName("belegnummer")->AsString;
		strBelegtyp=q->FieldByName("belegtyp")->AsString;
		strStatus=q->FieldByName("status")->AsString;
		}
	q->Close();

	bSt=true;
	//s="select count(*) anz from belegp where status < '2' and belegtyp='"+strBelegtyp+"' and belegnummer='"+strBelegnummer+"'";
	s="select count(*) anz from belegp where isnull(menge, 0)-isnull(verwendet, 0) > 0 and belegtyp='"+strBelegtyp+"' and belegnummer='"+strBelegnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		int iAnz=q->FieldByName("anz")->AsInteger;
		if(iAnz > 0)
			bSt=false;
		}
	q->Close();

	if(bSt == true)
		{
		s="update beleg set uebernahmeoffen=0, status='2' where belegnummer='"+strBelegnummer+"' and belegtyp='"+strBelegtyp+"'";
		q->SQL->Text=s;
		q->ExecSQL();

		s="update belegp set uebernahmeoffen=0, status='2' where belegnummer='"+strBelegnummer+"' and belegtyp='"+strBelegtyp+"'";
		q->SQL->Text=s;
		q->ExecSQL();
		}
	if(bSt != true && strStatus == '2')  // Geschlossenen Beleg öffnen, wenn noch eine Position offen ist und falls er schon zu ist (kann z.B. bei delete passieren)
		{
		s="update beleg set uebernahmeoffen=isnull((select sum(uebernahmeoffen) from belegp where belegnummer='"+strBelegnummer+"' and belegtyp='"+strBelegtyp+"' and Zeilentyp in ('A','H','E')), 0), "
			"status='0' "
			"where belegnummer='"+strBelegnummer+"' and belegtyp='"+strBelegtyp+"'";
		q->SQL->Text=s;
		q->ExecSQL();
		}

	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TWawi::deleteBeleg(UnicodeString sBelegtyp, UnicodeString sBelegNummer)
{
	AnsiString s, strKennung;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	TBeleg *beleg=new TBeleg(_con);

	s="select * from belegp where belegtyp='"+sBelegtyp+"' and belegnummer='"+sBelegNummer+"'";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		if(i == 0)
			{
			beleg->edit(sBelegtyp, sBelegNummer);
			}
		strKennung=q->FieldByName("Kennung")->AsString;

		TBelegp *belegp=new TBelegp(beleg);
		//belegp->edit(strKennung);
		belegp->del(strKennung);
		delete belegp;

		q->Next();
		}
	q->Close();

	s="delete from beleg where belegtyp='"+sBelegtyp+"' and belegnummer='"+sBelegNummer+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	delete beleg;

	delete q;

  return(true);
}

//---------------------------------------------------------------------------

bool TWawi::createKunde(TSLAdresse *adr, TSLKundenDaten *kuda, UnicodeString &sKundennummer)
{
	AnsiString s, strNewKey, strAdressnummer;

	strNewKey=getNextKey("KU");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="insert into kunden (nummer, fibukonto, standort, waehrung, angelegtvon, angelegtam) "
		"values('"+strNewKey+"','"+strNewKey+"','"+kuda->sStandort+"','"+kuda->sWaehrungscode+"','"+this->getUser()+"', GetDate())";
	q->SQL->Text=s;
	q->ExecSQL();
	if(q->RowsAffected != 1)
		{
		delete q;
		return(false);
		}

  setNextKey("KU", strNewKey);

	s="update kunden set "
		"anrede='"+getCheckedSQLStr(adr->sAnrede)+"',"
		"firma='"+getCheckedSQLStr(adr->sFirma.SubString(1, 80))+"',"
		"vorname='"+getCheckedSQLStr(adr->sVorname.SubString(1, 40))+"',"
		"name='"+getCheckedSQLStr(adr->sName.SubString(1, 80))+"',"
		"zusatz='"+getCheckedSQLStr(adr->sZusatz.SubString(1, 80))+"',"
		"zusatz2='"+getCheckedSQLStr(adr->sZusatz2.SubString(1, 80))+"',"
		"zusatz3='"+getCheckedSQLStr(adr->sZusatz3.SubString(1, 80))+"',"
		"email='"+getCheckedSQLStr(adr->sEMail.SubString(1, 80))+"',"
		"ort='"+getCheckedSQLStr(adr->sOrt.SubString(1, 40))+"',"
		"plz='"+getCheckedSQLStr(adr->sPLZ.SubString(1, 10))+"',"
		"strasse='"+getCheckedSQLStr(adr->sStrasse.SubString(1, 74))+"',"
		"telefon1='"+getCheckedSQLStr(adr->sTelefon1.SubString(1, 30))+"',"
		"telefon2='"+getCheckedSQLStr(adr->sTelefon2.SubString(1, 30))+"',"
		"fax='"+getCheckedSQLStr(adr->sFax.SubString(1, 30))+"',"
		"land='"+getCheckedSQLStr(adr->sLand.SubString(1, 6))+"',"
		"steueridentnr='"+getCheckedSQLStr(adr->sSteueridentNr.SubString(1, 20))+"',"
		"steuernummer='"+getCheckedSQLStr(adr->sSteuernummer.SubString(1, 20))+"',"
		"preisgruppe='"+kuda->sPreisgruppe+"',"
		"preistyp='"+kuda->sPreistyp+"',"
		"zahlungsbedingung='"+kuda->sZB+"',"
		"lieferbedingung='"+kuda->sLB+"',"
		"rabattgruppe='"+kuda->sRabattgruppe+"',"
		"gruppe='"+kuda->sGruppe+"',"
		"kontengruppe='"+kuda->sKontengruppe+"',"
		"kostenstelle='"+kuda->sKostenstelle+"',"
		"lager='"+kuda->sLager+"',"
		"mitarbeiter='"+kuda->sMitarbeiter+"',"
		"vertreter='"+kuda->sVertreter+"',"
		"sprache='"+kuda->sSprache+"' "
		"where nummer='"+strNewKey+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	sKundennummer=strNewKey;

	delete q;

	return(true);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TBeleg::TBeleg(TADOConnection *con) : TWawi(con)
{
	UnicodeString s;

	_bStandortChanged=_bHeaderChanged=_bWaehrungChanged=_bAdressChanged=_bLieferAdressChanged=_bLagerChanged=_bPosChanged=false;
	_bKopftextChanged=_bFusstextChanged=false;

	dNetto=dSteuer=dBrutto=dErloes=0;
	dTeilsumme=0;
	dVersand=0;
	sWaehrungscode="EUR";
	dWaehrungsfaktor=1.0;
	dBelegrabatt=0.0;
	iPreisgruppe=0;
	iKursTyp=1;
	iZahlungsziel=iZahlungszieltyp=0;
	//sStandort=_sStandort;
	dtDatum=DateOf(Now());
	sLeistungsdatum="";
	sLiefertermin="";
	sIhrauftragvomdatum="";
	dSkontoprozent1=dSkontoprozent2=0.0;
	iSkontotage1=iSkontotage2=iSkontotage1typ=iSkontotage2typ=0;
	sMeldeland="DE";
}

//---------------------------------------------------------------------------

void TBeleg::setZahlungsbedingung(UnicodeString sZB)
{
	UnicodeString s;

	this->sZB=sZB;
	_bHeaderChanged=true;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from zahlbed where zb='"+sZB+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iZahlungszieltyp=q->FieldByName("Zahlungszieltyp")->AsInteger;
		iZahlungsziel=q->FieldByName("Zahlungsziel")->AsInteger;
		dSkontoprozent1=q->FieldByName("Skontoprozent1")->AsFloat;
		dSkontoprozent2=q->FieldByName("Skontoprozent2")->AsFloat;
		iSkontotage1=q->FieldByName("Skontotage1")->AsInteger;
		iSkontotage1typ=q->FieldByName("Skontotage1typ")->AsInteger;
		iSkontotage2=q->FieldByName("Skontotage2")->AsInteger;
		iSkontotage2typ=q->FieldByName("Skontotage2typ")->AsInteger;
		}
	q->Close();

	delete q;
}

//---------------------------------------------------------------------------

void TBeleg::setLieferbedingung(UnicodeString sLB)
{
	AnsiString s;

	this->sLB=sLB;
	_bHeaderChanged=true;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;
	
	s="select * from lieferbed where nummer='"+sLB+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		sLText1=q->FieldByName("Bezeichnung")->AsString;
		sLText2=q->FieldByName("Zusatz")->AsString;
		}
	q->Close();

	delete q;
}

//---------------------------------------------------------------------------

UnicodeString TWawi::getErloeskontoFromMatrix(UnicodeString sKundenKontengruppe, UnicodeString sArtikelkontengruppe, UnicodeString &sAbweichenderSteuercode, int &iOSSVerfahren)
{
	UnicodeString s, sKonto;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from [KTOGRUPPENPOS] where aufwanderloes='"+sKundenKontengruppe+"' and gruppe='"+sArtikelkontengruppe+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		sKonto=q->FieldByName("GKonto")->AsString;
		sAbweichenderSteuercode=q->FieldByName("Steuercode")->AsString;
		//if(_strVersion1.ToInt() >= 21 && _strVersion2.ToInt() >= 2 && _strVersion3.ToInt() >= 1)
		if(_strVersion1.ToInt() > 21 || (_strVersion1.ToInt() == 21 && _strVersion2.ToInt() >= 2))
			iOSSVerfahren=q->FieldByName("OssVerfahren")->AsInteger;
		}
	else
		sKonto="";
	q->Close();

	delete q;
	return(sKonto);
}

//---------------------------------------------------------------------------

UnicodeString TWawi::getAufwandskontoFromMatrix(UnicodeString sLieferantenKontengruppe, UnicodeString sArtikelkontengruppe, UnicodeString &sAbweichenderSteuercode)
{
	UnicodeString s, sKonto;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from [KTOGRUPPENPOS] where aufwanderloes='"+sLieferantenKontengruppe+"' and gruppe='"+sArtikelkontengruppe+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		sKonto=q->FieldByName("GKonto")->AsString;
		sAbweichenderSteuercode=q->FieldByName("Steuercode")->AsString;
		}
	else
		sKonto="";
	q->Close();

	delete q;
	return(sKonto);
}

//---------------------------------------------------------------------------

bool TBeleg::edit(UnicodeString sBelegtyp, UnicodeString sBelegnummer)
{
	UnicodeString s;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from beleg where belegtyp='"+sBelegtyp+"' and belegnummer='"+sBelegnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		this->_iBelegId=q->FieldByName("Beleg_Id")->AsInteger;
		this->sBelegnummer=sBelegnummer;
		this->sBelegtyp=sBelegtyp;

		this->sUnserzeichen=q->FieldByName("UnserZeichen")->AsString;
		this->sIhrauftrag=q->FieldByName("IhrAuftrag")->AsString;
		this->sIhrzeichen=q->FieldByName("IhrZeichen")->AsString;

		if(!q->FieldByName("IhrAuftragVom")->IsNull)
			this->sIhrauftragvomdatum=q->FieldByName("IhrAuftragVom")->AsDateTime.FormatString("dd.mm.yyyy");

		this->dtDatum=q->FieldByName("Datum")->AsDateTime;

		if(!q->FieldByName("Liefertermin")->IsNull)
			this->sLiefertermin=q->FieldByName("Liefertermin")->AsDateTime.FormatString("dd.mm.yyyy");

		this->sAdressnummer=q->FieldByName("Adressnummer")->AsString;
		belegAdr.sAnrede=q->FieldByName("Anrede")->AsString;
		belegAdr.sName=q->FieldByName("Name")->AsString;
		belegAdr.sVorname=q->FieldByName("Vorname")->AsString;
		belegAdr.sFirma=q->FieldByName("Firma")->AsString;
		belegAdr.sStrasse=q->FieldByName("Strasse")->AsString;
		belegAdr.sZusatz=q->FieldByName("Zusatz")->AsString;
		belegAdr.sZusatz2=q->FieldByName("Zusatz2")->AsString;
		belegAdr.sZusatz3=q->FieldByName("Zusatz3")->AsString;
		belegAdr.sPLZ=q->FieldByName("PLZ")->AsString;
		belegAdr.sOrt=q->FieldByName("Ort")->AsString;
		belegAdr.sLand=q->FieldByName("Land")->AsString;

		this->sPreistyp=q->FieldByName("Preistyp")->AsString;
		this->iPreisgruppe=q->FieldByName("Preisgruppe")->AsInteger;
		this->dBelegrabatt=q->FieldByName("Belegrabatt")->AsFloat;
		this->sMitarbeiter=q->FieldByName("Mitarbeiter")->AsString;
		this->sVertreter=q->FieldByName("Vertreter")->AsString;
		this->sZB=q->FieldByName("Zahlungsbedingung")->AsString;
		this->sLB=q->FieldByName("Lieferbedingung")->AsString;
		this->sLieferbelegnr=q->FieldByName("Lieferbelegnr")->AsString;
		this->sLText1=q->FieldByName("LText1")->AsString;
		this->sLText2=q->FieldByName("LText2")->AsString;
		this->sRabattgruppe=q->FieldByName("Rabattgruppe")->AsString;
		this->sLager=q->FieldByName("Lager")->AsString;
		this->sSprache=q->FieldByName("Sprache")->AsString;
		this->sKonto=q->FieldByName("Konto")->AsString;
		this->sKontengruppe=q->FieldByName("Kontengruppe")->AsString;
		this->sEULand=q->FieldByName("EULand")->AsString;
		this->sMeldeland=q->FieldByName("Meldeland")->AsString;
		this->sVerkehrszweig=q->FieldByName("Verkehrszweig")->AsString;
		this->sUSTId=q->FieldByName("USTId")->AsString;
		this->iZahlungsziel=q->FieldByName("Zahlungsziel")->AsInteger;
		this->iZahlungszieltyp=q->FieldByName("Zahlungszieltyp")->AsInteger;
		this->dSkontoprozent1=q->FieldByName("Skontoprozent1")->AsFloat;
		this->dSkontoprozent2=q->FieldByName("Skontoprozent2")->AsFloat;
		this->iSkontotage1=q->FieldByName("Skontotage1")->AsInteger;
		this->iSkontotage2=q->FieldByName("Skontotage2")->AsInteger;
		this->iSkontotage1typ=q->FieldByName("Skontotage1typ")->AsInteger;
		this->iSkontotage2typ=q->FieldByName("Skontotage2typ")->AsInteger;
		this->sRechAdresse=q->FieldByName("RechAdresse")->AsString;
		this->sFreiertext1=q->FieldByName("Freiertext1")->AsString;
		this->sFreiertext2=q->FieldByName("Freiertext2")->AsString;

		this->sWaehrungscode=q->FieldByName("Waehrungscode")->AsString;
		this->dWaehrungsfaktor=q->FieldByName("Waehrungsfaktor")->AsFloat;
		this->_sStandort=q->FieldByName("Standort")->AsString;

		this->dNetto=q->FieldByName("Netto")->AsFloat;
		this->dSteuer=q->FieldByName("Steuer")->AsFloat;
		this->dBrutto=q->FieldByName("Brutto")->AsFloat;
		this->dErloes=q->FieldByName("Erloes")->AsFloat;
		q->Close();

		s="select * from belarten where belegtyp='"+sBelegtyp+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			this->sAdressBlobKey=q->FieldByName("AdressBlobKey")->AsString;
			this->sLagerFlag=q->FieldByName("LagerFlag")->AsString;
			this->sUmsatzKennzeichen=q->FieldByName("Umsatz")->AsString;
			this->iEA=q->FieldByName("EA")->AsInteger;
			}
		q->Close();

		// abw. Lieferadresse
		s="select * from adress where adresstyp='Q"+this->sBelegtyp+this->sBelegnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			lieferAdr.sAnrede=q->FieldByName("Anrede")->AsString;
			lieferAdr.sName=q->FieldByName("Name")->AsString;
			lieferAdr.sVorname=q->FieldByName("Vorname")->AsString;
			lieferAdr.sFirma=q->FieldByName("Firma")->AsString;
			lieferAdr.sZusatz=q->FieldByName("Zusatz")->AsString;
			lieferAdr.sZusatz2=q->FieldByName("Zusatz2")->AsString;
			lieferAdr.sZusatz3=q->FieldByName("Zusatz3")->AsString;
			lieferAdr.sStrasse=q->FieldByName("Strasse")->AsString;
			lieferAdr.sPLZ=q->FieldByName("PLZ")->AsString;
			lieferAdr.sOrt=q->FieldByName("Ort")->AsString;
			lieferAdr.sLand=q->FieldByName("Land")->AsString;
			lieferAdr.sILN=q->FieldByName("ILNNummer")->AsString;
			}

		// Kopftext
		s="select * from text where blobkey='Q"+sBelegtyp+"°"+sBelegnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			sKopftext=q->FieldByName("text")->AsString;
			}
		q->Close();

		// Fußtext
		s="select * from text where blobkey='Q"+sBelegtyp+sBelegnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			sFusstext=q->FieldByName("text")->AsString;
			}
		q->Close();


		bSt=true;
		}
	q->Close();
	delete q;

	return(bSt);
}
//---------------------------------------------------------------------------

bool TBeleg::copyHeader(UnicodeString sBelegtyp, UnicodeString sBelegnummer)
{
	UnicodeString s;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from beleg where belegtyp='"+sBelegtyp+"' and belegnummer='"+sBelegnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		this->sIhrauftrag=q->FieldByName("IhrAuftrag")->AsString;

		if(!q->FieldByName("IhrAuftragVom")->IsNull)
			this->sIhrauftragvomdatum=q->FieldByName("IhrAuftragVom")->AsString;

		this->sIhrzeichen=q->FieldByName("IhrZeichen")->AsString;
		this->sUnserzeichen=q->FieldByName("UnserZeichen")->AsString;

		if(!q->FieldByName("Liefertermin")->IsNull)
			this->sLiefertermin=q->FieldByName("Liefertermin")->AsDateTime.FormatString("dd.mm.yyyy");

		this->sAdressnummer=q->FieldByName("Adressnummer")->AsString;
		belegAdr.sName=q->FieldByName("Name")->AsString;
		belegAdr.sVorname=q->FieldByName("Vorname")->AsString;
		belegAdr.sFirma=q->FieldByName("Firma")->AsString;
		belegAdr.sStrasse=q->FieldByName("Strasse")->AsString;
		belegAdr.sZusatz=q->FieldByName("Zusatz")->AsString;
		belegAdr.sZusatz2=q->FieldByName("Zusatz2")->AsString;
		belegAdr.sZusatz3=q->FieldByName("Zusatz3")->AsString;
		belegAdr.sPLZ=q->FieldByName("PLZ")->AsString;
		belegAdr.sOrt=q->FieldByName("Ort")->AsString;
		belegAdr.sLand=q->FieldByName("Land")->AsString;

		this->sPreistyp=q->FieldByName("Preistyp")->AsString;
		//this->iPreisgruppe=q->FieldByName("Preisgruppe")->AsInteger;
		s=q->FieldByName("Preisgruppe")->AsInteger;
		if(!TryStrToInt(s, this->iPreisgruppe))
			this->iPreisgruppe=0;
		this->dBelegrabatt=q->FieldByName("Belegrabatt")->AsFloat;
		this->sMitarbeiter=q->FieldByName("Mitarbeiter")->AsString;
		this->sVertreter=q->FieldByName("Vertreter")->AsString;
		this->sZB=q->FieldByName("Zahlungsbedingung")->AsString;
		this->sLB=q->FieldByName("Lieferbedingung")->AsString;
		this->sLText1=q->FieldByName("LText1")->AsString;
		this->sLText2=q->FieldByName("LText2")->AsString;
		this->sRabattgruppe=q->FieldByName("Rabattgruppe")->AsString;
		this->sLager=q->FieldByName("Lager")->AsString;
		this->sSprache=q->FieldByName("Sprache")->AsString;
		this->sKonto=q->FieldByName("Konto")->AsString;
		this->sKontengruppe=q->FieldByName("Kontengruppe")->AsString;
		this->sEULand=q->FieldByName("EULand")->AsString;
		this->sMeldeland=q->FieldByName("Meldeland")->AsString;
		this->sVerkehrszweig=q->FieldByName("Verkehrszweig")->AsString;
		this->sUSTId=q->FieldByName("USTId")->AsString;
		this->iZahlungsziel=q->FieldByName("Zahlungsziel")->AsInteger;
		this->iZahlungszieltyp=q->FieldByName("Zahlungszieltyp")->AsInteger;
		this->dSkontoprozent1=q->FieldByName("Skontoprozent1")->AsFloat;
		this->dSkontoprozent2=q->FieldByName("Skontoprozent2")->AsFloat;
		this->iSkontotage1=q->FieldByName("Skontotage1")->AsInteger;
		this->iSkontotage2=q->FieldByName("Skontotage2")->AsInteger;
		this->iSkontotage1typ=q->FieldByName("Skontotage1typ")->AsInteger;
		this->iSkontotage2typ=q->FieldByName("Skontotage2typ")->AsInteger;
		this->sRechAdresse=q->FieldByName("RechAdresse")->AsString;
		this->sFreiertext1=q->FieldByName("Freiertext1")->AsString;
		this->sFreiertext2=q->FieldByName("Freiertext2")->AsString;

		this->sWaehrungscode=q->FieldByName("Waehrungscode")->AsString;
		this->dWaehrungsfaktor=q->FieldByName("Waehrungsfaktor")->AsFloat;
		this->iKursTyp=q->FieldByName("Kurstyp")->AsInteger;
		this->_sStandort=q->FieldByName("Standort")->AsString;

		_bAdressChanged=true;
		_bHeaderChanged=true;
		_bWaehrungChanged=true;
		_bStandortChanged=true;

		q->Close();
		s="select * from adress where adresstyp='Q"+sBelegtyp+sBelegnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			lieferAdr.sAnrede=q->FieldByName("Anrede")->AsString;
			lieferAdr.sName=q->FieldByName("Name")->AsString;
			lieferAdr.sVorname=q->FieldByName("Vorname")->AsString;
			lieferAdr.sFirma=q->FieldByName("Firma")->AsString;
			lieferAdr.sZusatz=q->FieldByName("Zusatz")->AsString;
			lieferAdr.sZusatz2=q->FieldByName("Zusatz2")->AsString;
			lieferAdr.sZusatz3=q->FieldByName("Zusatz3")->AsString;
			lieferAdr.sStrasse=q->FieldByName("Strasse")->AsString;
			lieferAdr.sPLZ=q->FieldByName("PLZ")->AsString;
			lieferAdr.sOrt=q->FieldByName("Ort")->AsString;
			lieferAdr.sLand=q->FieldByName("Land")->AsString;
			lieferAdr.sILN=q->FieldByName("ILNNummer")->AsString;
			_bLieferAdressChanged=true;
			}
		q->Close();

		// Kopftext
		s="select * from text where blobkey='Q"+sBelegtyp+"°"+sBelegnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			sKopftext=q->FieldByName("text")->AsString;
			_bKopftextChanged=true;
			}
		q->Close();

		// Fußtext
		s="select * from text where blobkey='Q"+sBelegtyp+sBelegnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			sFusstext=q->FieldByName("text")->AsString;
			_bFusstextChanged=true;
			}
		q->Close();

		}
	q->Close();
	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

UnicodeString TBeleg::create(UnicodeString sBelegtyp, UnicodeString sBelegnummer)
{
	#define MAXKEYTRIES 100
	UnicodeString s, sNextBelegnummer;
	int iID=0;
	int iTries;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;


	for(iTries=0; iTries < MAXKEYTRIES; iTries++)
		{
		if(sBelegnummer == "")
			sNextBelegnummer=getNextKey("Q"+sBelegtyp);
		else
			sNextBelegnummer=sBelegnummer;

		try
			{
			s="insert into beleg (Belegtyp, Belegnummer, Waehrungscode, Waehrungsfaktor, Standort, AngelegtAm, AngelegtVon, Herkunft) "
				"Values('"+sBelegtyp+"', '"+sNextBelegnummer+"', '"+sWaehrungscode+"', '"+getSQLFloat(dWaehrungsfaktor)+"', '"+_sStandort+"', GetDate(), '"+_sUser+"', '1') "
				" "
				"select @@IDENTITY id ";
			q->SQL->Text=s;
			q->Open();
			if(q->RecordCount)
				{
				iID=q->FieldByName("id")->AsInteger;
				this->_iBelegId=iID;
				this->sBelegnummer=sNextBelegnummer;
				this->sBelegtyp=sBelegtyp;
				q->Close();

				setNextKey("Q"+sBelegtyp, sNextBelegnummer);

				s="select * from belarten where belegtyp='"+sBelegtyp+"'";
				q->SQL->Text=s;
				q->Open();
				if(q->RecordCount)
					{
					this->sAdressBlobKey=q->FieldByName("AdressBlobKey")->AsString;
					this->sLagerFlag=q->FieldByName("LagerFlag")->AsString;
					this->sUmsatzKennzeichen=q->FieldByName("Umsatz")->AsString;
					this->iEA=q->FieldByName("EA")->AsInteger;
					}
				q->Close();

				break;
				}
			}
		catch(Exception &e)
			{
			setNextKey("Q"+sBelegtyp, sNextBelegnummer);
			}
		}

	if(iTries >= MAXKEYTRIES)
		{
		this->_iBelegId=0;
		this->sBelegnummer="";
		this->sBelegtyp=sBelegtyp;
		sNextBelegnummer="";
		}

	delete q;

	//ToDo: Variablen initialisieren
	this->setLiefertermin(this->getDatum());

	return(sNextBelegnummer);
}

//---------------------------------------------------------------------------

bool TBeleg::save(void)
{
	UnicodeString s, sql;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	sql="update beleg set ";

	if(_bAdressChanged)
		{
		s+=
			"Adressnummer='"+sAdressnummer+"',"
			"Anrede='"+getCheckedSQLStr(belegAdr.sAnrede.SubString(1, 30))+"',"
			"Name='"+getCheckedSQLStr(belegAdr.sName.SubString(1, 80))+"',"
			"Vorname='"+getCheckedSQLStr(belegAdr.sVorname.SubString(1, 40))+"',"
			"Firma='"+getCheckedSQLStr(belegAdr.sFirma.SubString(1, 80))+"',"
			"Strasse='"+getCheckedSQLStr(belegAdr.sStrasse.SubString(1, 80))+"',"
			"Zusatz='"+getCheckedSQLStr(belegAdr.sZusatz.SubString(1, 80))+"',"
			"Zusatz2='"+getCheckedSQLStr(belegAdr.sZusatz2.SubString(1, 80))+"',"
			"Zusatz3='"+getCheckedSQLStr(belegAdr.sZusatz3.SubString(1, 80))+"',"
			"PLZ='"+getCheckedSQLStr(belegAdr.sPLZ.SubString(1, 10))+"',"
			"Ort='"+getCheckedSQLStr(belegAdr.sOrt.SubString(1, 40))+"',"
			"Land='"+getCheckedSQLStr(belegAdr.sLand.SubString(1, 6))+"'";
		_bAdressChanged=false;
		}

	if(_bHeaderChanged)
		{
		if(s != "")
			s+=",";
		s+=
			"Zahlungsbedingung='"+sZB+"',"
			"Zahlungsziel='"+IntToStr(iZahlungsziel)+"',"
			"Zahlungszieltyp='"+IntToStr(iZahlungszieltyp)+"',"
			"Skontoprozent1='"+getSQLFloat(dSkontoprozent1)+"',"
			"Skontoprozent2='"+getSQLFloat(dSkontoprozent2)+"',"
			"Skontotage1='"+IntToStr(iSkontotage1)+"',"
			"Skontotage2='"+IntToStr(iSkontotage2)+"',"
			"Skontotage1typ='"+IntToStr(iSkontotage1typ)+"',"
			"Skontotage2typ='"+IntToStr(iSkontotage2typ)+"',"
			"Lieferbedingung='"+sLB+"',"
			"Lieferbelegnr='"+sLieferbelegnr+"',"
			"LText1='"+getCheckedSQLStr(sLText1)+"',"
			"LText2='"+getCheckedSQLStr(sLText2)+"',"
			"Rabattgruppe='"+sRabattgruppe+"',"
			"Preistyp='"+sPreistyp+"',"
			"Preisgruppe='"+IntToStr(iPreisgruppe)+"',"
			"Belegrabatt='"+getSQLFloat(dBelegrabatt)+"',"
			"Lager='"+sLager+"',"
			"Sprache='"+sSprache+"',"
			"Konto='"+sKonto+"',"
			"Kontengruppe='"+sKontengruppe+"',"
			"Waehrungscode='"+sWaehrungscode+"',"
			"Waehrungsfaktor='"+getSQLFloat(dWaehrungsfaktor)+"',"
			"Kurstyp='"+IntToStr(iKursTyp)+"',"
			"Fixkurs='"+getSQLFloat(dWaehrungsfaktor)+"',"
			"Mitarbeiter='"+sMitarbeiter+"',"
			"Vertreter='"+sVertreter+"',"
			"Standort='"+_sStandort+"',"
			"EULand='"+sEULand+"',"
			"Meldeland='"+sMeldeland+"',"
			"Verkehrszweig='"+sVerkehrszweig+"',"
			"USTId='"+sUSTId+"',"
			"IhrAuftrag='"+getCheckedSQLStr(sIhrauftrag)+"',"
			"Datum='"+dtDatum.FormatString("dd.mm.yyyy")+"',"
			"IhrZeichen='"+getCheckedSQLStr(sIhrzeichen)+"',"
			"UnserZeichen='"+getCheckedSQLStr(sUnserzeichen)+"', "
			"RechAdresse='"+getCheckedSQLStr(sRechAdresse)+"', "
			"Version='3', "
			"Orignummer='"+sAdressnummer+"',"
			"RefAdresse='"+sAdressnummer+"',"
			"Projekt='0',"
			"DelkredereProzent='0.0',"
			"Ansprechpartner='0', "
			"Freiertext1='"+getCheckedSQLStr(sFreiertext1)+"',"
			"Freiertext2='"+getCheckedSQLStr(sFreiertext2)+"' ";
		if(sLeistungsdatum != "")
			s+=",Leistungsdatum='"+sLeistungsdatum+"' ";
		if(sLiefertermin != "")
			s+=",Liefertermin='"+sLiefertermin+"' ";
		if(sIhrauftragvomdatum != "")
			s+=",IhrAuftragVom='"+sIhrauftragvomdatum+"' ";
		_bHeaderChanged=false;
		}

	if(s != "")
		{
		sql+=s;
		sql+=" where beleg_id='"+IntToStr(_iBelegId)+"'";

		q->SQL->Text=sql;
		q->ExecSQL();
		if(q->RowsAffected == 1)
			bSt=true;
		}

	if(_bLieferAdressChanged)
		{
		s="update adress set "
			"Anrede='"+getCheckedSQLStr(lieferAdr.sAnrede.SubString(1, 30))+"',"
			"Name='"+getCheckedSQLStr(lieferAdr.sName.SubString(1, 80))+"',"
			"Vorname='"+getCheckedSQLStr(lieferAdr.sVorname.SubString(1, 40))+"',"
			"Firma='"+getCheckedSQLStr(lieferAdr.sFirma.SubString(1, 80))+"',"
			"Zusatz='"+getCheckedSQLStr(lieferAdr.sZusatz.SubString(1, 80))+"',"
			"Zusatz2='"+getCheckedSQLStr(lieferAdr.sZusatz2.SubString(1, 80))+"',"
			"Zusatz3='"+getCheckedSQLStr(lieferAdr.sZusatz3.SubString(1, 80))+"',"
			"Strasse='"+getCheckedSQLStr(lieferAdr.sStrasse.SubString(1, 80))+"',"
			"PLZ='"+getCheckedSQLStr(lieferAdr.sPLZ.SubString(1, 10))+"',"
			"Ort='"+getCheckedSQLStr(lieferAdr.sOrt.SubString(1, 40))+"',"
			"Land='"+getCheckedSQLStr(lieferAdr.sLand.SubString(1, 6))+"',"
			"ILNNummer='"+lieferAdr.sILN+"', "
			"eMail='"+lieferAdr.sEMail+"' "
			"where adresstyp='Q"+this->sBelegtyp+this->sBelegnummer+"' "
			"if @@rowcount=0 "
				"insert into adress (Adresstyp, Name, Vorname, Firma, Zusatz, Zusatz2, Zusatz3, Strasse, PLZ, Ort, Land, ILNNummer, eMail) "
				"Values('Q"+this->sBelegtyp+this->sBelegnummer+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sName.SubString(1, 80))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sVorname.SubString(1, 40))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sFirma.SubString(1, 80))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sZusatz.SubString(1, 80))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sZusatz2.SubString(1, 80))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sZusatz3.SubString(1, 80))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sStrasse.SubString(1, 80))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sPLZ.SubString(1, 10))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sOrt.SubString(1, 40))+"',"
							 "'"+getCheckedSQLStr(lieferAdr.sLand.SubString(1, 6))+"',"
							 "'"+lieferAdr.sILN+"',"
							 "'"+lieferAdr.sEMail+"')";
		q->SQL->Text=s;
		q->ExecSQL();
		_bLieferAdressChanged=false;
		}

	if(_bKopftextChanged) 		// Kopftext
		{
		s="update text set text='"+getCheckedSQLStr(sKopftext)+"' where blobkey='Q"+this->sBelegtyp+"°"+this->sBelegnummer+"' "
			"if @@rowcount=0 "
				"insert into text (blobkey, text) "
				"Values('Q"+this->sBelegtyp+"°"+this->sBelegnummer+"','"+getCheckedSQLStr(sKopftext)+"') ";
		q->SQL->Text=s;
		q->ExecSQL();
		_bKopftextChanged=false;
		}

	if(_bFusstextChanged)    // Fußtext
		{
		s="update text set text='"+getCheckedSQLStr(sKopftext)+"' where blobkey='Q"+this->sBelegtyp+this->sBelegnummer+"' "
			"if @@rowcount=0 "
				"insert into text (blobkey, text) "
				"Values('Q"+this->sBelegtyp+this->sBelegnummer+"','"+getCheckedSQLStr(sFusstext)+"') ";
		q->SQL->Text=s;
		q->ExecSQL();
		_bFusstextChanged=false;
		}

	if(_bPosChanged)     // Die Beleg-Summen werden direkt per SQL berechnet
		{
		s="update beleg set "
			"EuroNetto= round((select sum(RNettoEuro) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')) * case when adressblobkey='KU' then EA else EA end,2), "
			"Netto= round((select sum(RNetto) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')) * case when adressblobkey='KU' then EA else EA end,2), "
			"Brutto= round((select sum(RNetto*(1+Steuerprozent/100)) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E'))*case when adressblobkey='KU' then EA else EA end,2), "
			"EuroBrutto= round((select sum(RNettoEuro*(1+Steuerprozent/100)) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E'))*case when adressblobkey='KU' then EA else EA end,2), "
			"EuroSteuer= (select sum(RNettoEuro*(Steuerprozent/100)) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')) * case when adressblobkey='KU' then EA else EA end, "
			"Steuer= (select sum(RNetto*(Steuerprozent/100)) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')) * case when adressblobkey='KU' then EA else EA end, "
			"Erloes= (select sum(RNetto-Menge/Preiseinheit*Kalkulationspreis) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')), "
			"ErloesEuro= (select sum(RNettoEuro-Menge/Preiseinheit*KalkpreisEuro) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')), ";
		if(getBelegtyp() != "W")
			{
			s+="Uebernahmeoffen= (select sum(uebernahmeoffen) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')), ";
			}
		s+=
			"FremdWaehrungbrutto= (select sum(RNetto*(1+Steuerprozent/100)) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E'))* Waehrungsfaktor *case when adressblobkey='KU' then EA else EA end, "
			"Fremdwaehrungnetto=(select sum(RNetto) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E')) * Waehrungsfaktor * case when adressblobkey='KU' then EA else EA end, "
			"Fremdwaehrungsteuer= (select sum(RNetto*(Steuerprozent/100)) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and Zeilentyp in ('A','H','E'))* Waehrungsfaktor * case when adressblobkey='KU' then EA else EA end, "
			"Gewicht=(select sum(gewicht*Menge) from belegp where Belegp.belegnummer=beleg.belegnummer and beleg.belegtyp=belegp.belegtyp and  Zeilentyp in ('A','H','E')) "
			"from beleg with(NOLOCK) "
			"inner join belarten on belarten.belegtyp=beleg.belegtyp "
			"where beleg.Belegtyp='"+this->sBelegtyp+"' and beleg.belegnummer='"+this->sBelegnummer+"' ";
		q->SQL->Text=s;
		AnsiString txt;
    	txt=s;
		q->ExecSQL();
		if(q->RowsAffected == 1)
			bSt=true;
		_bPosChanged=false;
		}

	delete q;

	return(bSt);
}


//---------------------------------------------------------------------------

bool TBeleg::setAdresse(UnicodeString sAdressNr)
{
	UnicodeString s, sSourceTable;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	if(sAdressBlobKey == "KU")
		sSourceTable="kunden";
	else if(sAdressBlobKey == "IN")
		sSourceTable="interess";
	else if(sAdressBlobKey == "LI")
		sSourceTable="liefer";

	if(sSourceTable != "")
		{
		s="select * from "+sSourceTable+" where nummer='"+sAdressNr+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			this->sAdressnummer=sAdressNr;
      belegAdr.sAnrede=q->FieldByName("Anrede")->AsString;
			belegAdr.sName=q->FieldByName("Name")->AsString;
			belegAdr.sVorname=q->FieldByName("Vorname")->AsString;
			belegAdr.sFirma=q->FieldByName("Firma")->AsString;
			belegAdr.sStrasse=q->FieldByName("Strasse")->AsString;
			belegAdr.sZusatz=q->FieldByName("Zusatz")->AsString;
			belegAdr.sZusatz2=q->FieldByName("Zusatz2")->AsString;
			belegAdr.sZusatz3=q->FieldByName("Zusatz3")->AsString;
			belegAdr.sPLZ=q->FieldByName("PLZ")->AsString;
			belegAdr.sOrt=q->FieldByName("Ort")->AsString;
			belegAdr.sLand=q->FieldByName("Land")->AsString;
			_bAdressChanged=true;

			this->sRabattgruppe=q->FieldByName("Rabattgruppe")->AsString;
			this->sPreistyp=q->FieldByName("Preistyp")->AsString;
			//this->iPreisgruppe=q->FieldByName("Preisgruppe")->AsInteger;
			s=q->FieldByName("Preisgruppe")->AsInteger;
			if(!TryStrToInt(s, this->iPreisgruppe))
				this->iPreisgruppe=0;
			this->sMitarbeiter=q->FieldByName("Mitarbeiter")->AsString;
			this->sVertreter=q->FieldByName("Vertreter")->AsString;
			this->sZB=q->FieldByName("Zahlungsbedingung")->AsString;
			setZahlungsbedingung(this->sZB);
			this->sLB=q->FieldByName("Lieferbedingung")->AsString;
			this->sLager=q->FieldByName("Lager")->AsString;
			this->sSprache=q->FieldByName("Sprache")->AsString;
			this->sKonto=q->FieldByName("Fibukonto")->AsString;
			this->sKontengruppe=q->FieldByName("Kontengruppe")->AsString;
			this->sUSTId=q->FieldByName("SteueridentNr")->AsString;
			this->sEULand=q->FieldByName("EULand")->AsString;
			this->sVerkehrszweig=q->FieldByName("Verkehrszweig")->AsString;
			this->sRechAdresse=q->FieldByName("RefAdresse")->AsString;

			_bHeaderChanged=true;

			if(this->sWaehrungscode != q->FieldByName("Waehrung")->AsString && q->FieldByName("Waehrung")->AsString != "")
				{
				_bWaehrungChanged=true;
				this->sWaehrungscode=q->FieldByName("Waehrung")->AsString;
				}
			if(this->_sStandort != q->FieldByName("Standort")->AsString && q->FieldByName("Standort")->AsString != "")
				{
				_bStandortChanged=true;
				this->_sStandort=q->FieldByName("Standort")->AsString;
				}

			bSt=true;
			}
		q->Close();
		}

	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TBeleg::dispose(void)
{
	AnsiString s, strKennung, strStatus;
	int iResult;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select kennung, status from belegp where belegtyp='"+this->sBelegtyp+"' and belegnummer='"+this->sBelegnummer+"' order by belegp_id";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		strKennung=q->FieldByName("Kennung")->AsString;
		strStatus=q->FieldByName("Status")->AsString;

		if(strStatus == 0 || strStatus == 1)
			{
			TBelegp *bp=new TBelegp(this);
			bp->edit(strKennung);
			bp->dispose();
			delete bp;
			}

		q->Next();
		}
	q->Close();

	s="select count(*) anz from belegp where belegtyp='"+this->sBelegtyp+"' and belegnummer='"+this->sBelegnummer+"' and status not in ('3', '2')";
	q->SQL->Text=s;
	q->Open();
	iResult=q->FieldByName("anz")->AsInteger;
	q->Close();

	if(iResult == 0) // wenn alles Positionen auf Status=3 sind
		{
		s="update beleg set status='3' where belegtyp='"+this->sBelegtyp+"' and belegnummer='"+this->sBelegnummer+"'";
		q->SQL->Text=s;
		q->ExecSQL();

		bSt=true;
		}

	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TBeleg::copyBelegExtra(UnicodeString sQuellBelegtyp, UnicodeString sQuellBeleg, UnicodeString sZielBelegtyp, UnicodeString sZielBeleg)
{
	AnsiString s, strField;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=getCon();
	q->LockType=ltReadOnly;
	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=getCon();
	q2->LockType=ltReadOnly;

	s="SELECT ORDINAL_POSITION, COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, IS_NULLABLE, COLUMN_DEFAULT FROM INFORMATION_SCHEMA.COLUMNS "
		"WHERE Upper(TABLE_NAME)='BELEG' and substring(Column_name, 1, 1)='_' "
		"ORDER BY ORDINAL_POSITION ASC";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		strField=q->FieldByName("COLUMN_NAME")->AsString;

		s="update beleg set "+strField+"=(select "+strField+" from beleg b where b.belegtyp='"+sQuellBelegtyp+"' and b.belegnummer='"+sQuellBeleg+"') "
			"where belegtyp='"+sZielBelegtyp+"' and belegnummer='"+sZielBeleg+"'";
		q2->SQL->Text=s;
		q2->ExecSQL();

		q->Next();
		}
	q->Close();

	delete q2;
	delete q;

  return(true);
}

//---------------------------------------------------------------------------

bool TBeleg::updateOP(void)
{
	AnsiString s;
	int iOPNummer;
	bool bSt;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_con;
	q->LockType=ltReadOnly;

	s="select * from opos where opbeleg='"+sBelegnummer+"' and buchbeleg='"+sBelegnummer+"' and OPTyp='1' and Quelle='F|"+sBelegtyp+"|"+sBelegnummer+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		iOPNummer=q->FieldByName("OPNummer")->AsInteger;
	else
		iOPNummer=0;
	q->Close();

	if(iOPNummer == 0) // neuen OP erzeugen
		{
		s="declare @nummer integer "
			"set @nummer=(select isnull(max(opnummer), 0)+1 from opos) "
			"insert into opos(opnummer, AngelegtAm, OPBeleg, Datum, BelegDatum, Text, Quelle, BuchBeleg, BankVerb, OPTyp, KLTyp, "
											 "KLNr, Fibukonto, Name, Waehrungscode, Waehrungsfaktor, FixKurs, Kurstyp, Ansprechpartner, Projekt) "
			"Values(@nummer, GetDate(), '"+sBelegnummer+"', '"+dtDatum.DateString()+"', '"+dtDatum.DateString()+"' ,'Stundenabrechnung:"+sBelegnummer+"','F|"+sBelegtyp+"|"+sBelegnummer+"', '"+sBelegnummer+"','0','1','K', "
						 "'"+sAdressnummer+"','"+sAdressnummer+"','"+getCheckedSQLStr(belegAdr.sFirma)+"','"+sWaehrungscode+"','"+getCheckedSQLStr(getWaehrungfaktor())+"','1','1','0','0')"
			" "
			"select @nummer nummer ";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			iOPNummer=q->FieldByName("nummer")->AsInteger;
		else
			iOPNummer=0;
		q->Close();
		}

	if(iOPNummer)
		{
		s="update opos set "
			"Betrag='"+getSQLFloat(this->dBrutto)+"', "
			"Offen='"+getSQLFloat(this->dBrutto)+"', "
			"ZuZahlen='"+getSQLFloat(this->dBrutto)+"', "
			"Skontofaehig='"+getSQLFloat(this->dBrutto)+"', "
			"Provision='0',"
			"ProvisionBMG='"+getSQLFloat(this->dBrutto)+"', "
			"FaelligAm='"+IncDay(dtDatum, iZahlungsziel).DateString()+"' "
			"where opnummer='"+IntToStr(iOPNummer)+"'";
		q->SQL->Text=s;
		q->ExecSQL();
		bSt=true;
		}
	else
		bSt=false;

	delete q;

	return(bSt);
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TBelegp::TBelegp(TBeleg *beleg)
{
	_beleg=beleg;

	tdDatum=_beleg->getDatum();
	sAdressnr=_beleg->getAdresse();
	sLager=_beleg->getLager();

	dMenge=dPE=1.0;
	dPreismenge=1.0;
	sEditMenge="1";
	dVerwendet=0.0;
	dEP=dRabatt=dRabatt2=dGP=0.0;
	dEPBasis=0.0;
		dKalk=0.0;
	dKalkBasis=0.0;
	bProvisionsfaehig=bRabattfaehig=bSkontofaehig=true;
	bLagerartikel=false;
	sTermin=_beleg->getLiefertermin();
	sVorgaengerpKennung="";
	dUebernahme=0.0;
	dFreizahl=0.0;
	sFreidatum="";
	dGewicht=0.0;
	sStueckliste="K";
	sStatus="0";
	sSerieCharge="O";
	dMEUmrechnung=1.0;
    iOSSVerfahren=0;

	slSeriennummern=new TStringList;
	slChargen=new TStringList;
}

//---------------------------------------------------------------------------

TBelegp::~TBelegp(void)
{
	delete slChargen;
	delete slSeriennummern;
}

//---------------------------------------------------------------------------

bool TBelegp::addSNr(UnicodeString sSerNr)
{
	slSeriennummern->Add(sSerNr);
}

//---------------------------------------------------------------------------

bool TBelegp::addArtExtra(UnicodeString sKennung)
{
	AnsiString s, strField;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;
	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=_beleg->getCon();
	q2->LockType=ltReadOnly;

	s="SELECT ORDINAL_POSITION, COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, IS_NULLABLE, COLUMN_DEFAULT FROM INFORMATION_SCHEMA.COLUMNS "
		"WHERE Upper(TABLE_NAME)='ART' and substring(Column_name, 1, 1)='_' and COLUMN_NAME in "
		"	(SELECT COLUMN_NAME FROM INFORMATION_SCHEMA.COLUMNS WHERE Upper(TABLE_NAME)='BELEGP' and substring(Column_name, 1, 1)='_') "
		"ORDER BY ORDINAL_POSITION ASC";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		strField=q->FieldByName("COLUMN_NAME")->AsString;

		s="update belegp set "+strField+"=(select "+strField+" from art where art.artikelnummer=belegp.artikelnummer) where kennung='"+sKennung+"'";
		q2->SQL->Text=s;
		q2->ExecSQL();

		q->Next();
		}
	q->Close();


	delete q2;
	delete q;

	return(true);
}

//---------------------------------------------------------------------------

bool TBelegp::copyBelegpExtra(UnicodeString sQuellKennung, UnicodeString sZielKennung)
{
	AnsiString s, strField;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;
	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=_beleg->getCon();
	q2->LockType=ltReadOnly;

	s="SELECT ORDINAL_POSITION, COLUMN_NAME, DATA_TYPE, CHARACTER_MAXIMUM_LENGTH, IS_NULLABLE, COLUMN_DEFAULT FROM INFORMATION_SCHEMA.COLUMNS "
		"WHERE Upper(TABLE_NAME)='BELEGP' and substring(Column_name, 1, 1)='_' "
		"ORDER BY ORDINAL_POSITION ASC";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		strField=q->FieldByName("COLUMN_NAME")->AsString;

		s="update belegp set "+strField+"=(select "+strField+" from belegp bpq where bpq.kennung='"+sQuellKennung+"') where kennung='"+sZielKennung+"'";
		q2->SQL->Text=s;
		q2->ExecSQL();

		q->Next();
		}
	q->Close();


	delete q2;
	delete q;

	return(true);
}

//---------------------------------------------------------------------------

bool TBelegp::addCharge(UnicodeString sCharge, double dMenge)
{
	slChargen->Add(sCharge+"="+FloatToStr(dMenge));
}

//---------------------------------------------------------------------------

double TBelegp::getEditMengeValue(UnicodeString sEditMenge)
{
	AnsiString s;
	double dVal;

	if(TryStrToFloat(AnsiReplaceStr(sEditMenge, ".", ","), dVal))
		return(dVal);
	else
		return(0);
}

//---------------------------------------------------------------------------

void TBelegp::setMengeneinheit(UnicodeString sME)
{
	UnicodeString s, sLieferant;
	double dBasis;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();

	if(this->_beleg->getAdressBlobKey() == "LI")
		sLieferant=this->sAdressnr;
	else
		sLieferant="";

	s="select * from mengum where artikel='"+this->sArtikelnummer+"' and mengeneinheit='"+sME+"' and isnull(lieferant, '')='"+sLieferant+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		dBasis=q->FieldByName("Basis")->AsFloat;
		dMEUmrechnung=dBasis;
		dEP=dEPBasis*dMEUmrechnung;
		dKalk=dKalkBasis*dMEUmrechnung;
		}
	else
		{
		dMEUmrechnung=1.0;
		dEP=dEPBasis;
		dKalk=dKalkBasis;
		}
	q->Close();

	this->sME=sME;

	delete q;

	calc();
}

//---------------------------------------------------------------------------

bool TBelegp::edit(UnicodeString sKennung)
{
	UnicodeString s;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;
	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=_beleg->getCon();
	q2->LockType=ltReadOnly;

	s="select * from belegp where kennung='"+sKennung+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		this->sKennung=sKennung;
		//this->sBelegnummer=sBelegnummer;
		//this->sBelegtyp=sBelegtyp;
		this->tdDatum=q->FieldByName("Datum")->AsDateTime;
		this->sAdressnr=q->FieldByName("Adressnr")->AsString;
		this->sPostext=q->FieldByName("Postext")->AsString;
		this->sArtikelnummer=q->FieldByName("Artikelnummer")->AsString;
		this->sZeilentyp=q->FieldByName("Zeilentyp")->AsString;
		this->setStueckliste(q->FieldByName("Stueckliste")->AsString);
		this->sBezeichnung=q->FieldByName("Bezeichnung")->AsString;
		this->sZusatz=q->FieldByName("Zusatz")->AsString;
		this->dMenge=q->FieldByName("Menge")->AsFloat;
		this->sEditMenge=q->FieldByName("EditMenge")->AsString;
		this->dVerwendet=q->FieldByName("Verwendet")->AsFloat;
		this->sStatus=q->FieldByName("Status")->AsString;
		this->sME=q->FieldByName("Mengeneinheit")->AsString;
		this->dEP=q->FieldByName("Einzelpreis")->AsFloat;
		this->dRabatt=q->FieldByName("Rabatt")->AsFloat;
		this->dGP=q->FieldByName("Gesamtpreis")->AsFloat;
		this->dPE=q->FieldByName("Preiseinheit")->AsFloat;

		this->sSerieCharge=q->FieldByName("SerieCharge")->AsString;
		this->setFrei1(q->FieldByName("Frei1")->AsString);
		this->setFrei2(q->FieldByName("Frei2")->AsString);
		this->setFreizahl(q->FieldByName("Freizahl")->AsFloat);
		this->setFreidatum(q->FieldByName("Freidatum")->AsDateTime);
		this->setKostenstelle(q->FieldByName("Kostenstelle")->AsString);
		this->setKostentraeger(q->FieldByName("Kostentraeger")->AsString);
		this->setWarencode(q->FieldByName("Warencode")->AsString);

		this->bRabattfaehig=q->FieldByName("Rabattfaehig")->AsBoolean;
		this->bSkontofaehig=q->FieldByName("Skontofaehig")->AsBoolean;
		this->bProvisionsfaehig=q->FieldByName("Provisionsartikel")->AsBoolean;
		this->bLagerartikel=q->FieldByName("Lagerartikel")->AsBoolean;
		this->sLager=q->FieldByName("Lager")->AsString;

		this->sSteuercode=q->FieldByName("Steuercode")->AsString;
		this->sKonto=q->FieldByName("Konto")->AsString;
		this->sMitarbeiter=q->FieldByName("Mitarbeiter")->AsString;

		this->dSteuerprozent=q->FieldByName("Steuerprozent")->AsFloat;

		this->sVorgaengerpKennung=q->FieldByName("Vorgaenger")->AsString;
		this->dUebernahme=q->FieldByName("Uebernahme")->AsFloat;
		this->sBestellnummer=q->FieldByName("Bestellnummer")->AsString;
		this->sPreismengeneinheit=q->FieldByName("Preismengeneinheit")->AsString;
		this->dPreismenge=q->FieldByName("Preismenge")->AsFloat;

		// Langtext holen
		s="select text from text where blobkey='BP"+this->sKennung+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			sLangtext=q2->FieldByName("text")->AsString;
		q2->Close();

		q->Close();

		bSt=true;
		}

	delete q2;
	delete q;
	return(bSt);
}

//---------------------------------------------------------------------------

bool TBelegp::create(UnicodeString sZeilentyp)
{
	UnicodeString s;
	int iAnzahl;
	bool bSt=false;

	this->sZeilentyp=sZeilentyp;
	this->sStandort=_beleg->getStandort();

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();

	s="select count(*) anzahl from belegp where belegtyp='"+_beleg->getBelegtyp()+"' and belegnummer='"+_beleg->getBelegnummer()+"' "
		"and zeilentyp in ('A', 'H', 'L')";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		iAnzahl=q->FieldByName("Anzahl")->AsInteger;
	else
		iAnzahl=0;
	q->Close();

	this->sMitarbeiter=_beleg->getVertreter();

	this->sPostext=IntToStr(++iAnzahl);
	bSt=true;

	delete q;
	return(bSt);
}

//---------------------------------------------------------------------------

bool TBelegp::save(void)
{
	UnicodeString s;
	bool bSt=false;
	double dNetto=0, dNettoEuro=0, dRNetto=0, dRNettoEuro=0;
	double dPosGewicht=0;
	int iRundung=-4;

	_beleg->setPosChanged(true);

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;

	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=_beleg->getCon();
	q2->LockType=ltReadOnly;

	// alle Summen berechnen
	if(this->sZeilentyp == "T")  // bei Teilsumme wird aus einer Zwischenvariablen des Beleg-Objektes gerechnet
		{
		dEP=_beleg->getTeilsumme();
		dGP=RoundTo(dEP *(1-dRabatt/100) , iRundung);
		_beleg->setTeilsumme(0);
		}
	else if(this->sZeilentyp == "A" || this->sZeilentyp == "G" || this->sZeilentyp == "E" || this->sZeilentyp == "H")
		{
		if(this->sZeilentyp != "G")
		  dGP=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100)*(1-dRabatt2/100) , iRundung);
		_beleg->setTeilsumme(_beleg->getTeilsumme()+dGP);
		if(_beleg->getPreistyp() == "B")
			{
			dNetto=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100)*(1-dRabatt2/100), iRundung);
			dNetto=dNetto-(dNetto/(100+dSteuerprozent)*dSteuerprozent);
			dNettoEuro=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100) * (1-dRabatt2/100), iRundung);
			dNettoEuro=dNettoEuro-(dNettoEuro/(100+dSteuerprozent)*dSteuerprozent);
			dRNetto=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100) * (1-dRabatt2/100)*(1-_beleg->getBelegrabatt()/100), iRundung);
			dRNetto=dRNetto-(dRNetto/(100+dSteuerprozent)*dSteuerprozent);
			dRNettoEuro=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100) * (1-dRabatt2/100)*(1-_beleg->getBelegrabatt()/100), iRundung);
			dRNettoEuro=dRNettoEuro-(dRNettoEuro/(100+dSteuerprozent)*dSteuerprozent);
			dNetto*=_beleg->getWaehrungfaktor();
			dNettoEuro*=_beleg->getWaehrungfaktor();
			dRNetto*=_beleg->getWaehrungfaktor();
			dRNettoEuro*=_beleg->getWaehrungfaktor();
			dRNetto*=_beleg->getEA();
			dRNettoEuro*=_beleg->getEA();
			}
		else
			{
			dNetto=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100)*(1-dRabatt2/100), iRundung);
			dNettoEuro=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100) * (1-dRabatt2/100), iRundung);
			dRNetto=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100) * (1-dRabatt2/100)*(1-_beleg->getBelegrabatt()/100), iRundung);
			dRNettoEuro=RoundTo(dMenge / dPE * dEP *(1-dRabatt/100) * (1-dRabatt2/100)*(1-_beleg->getBelegrabatt()/100), iRundung);
			dRNetto*=_beleg->getEA();
			dRNettoEuro*=_beleg->getEA();
			}
		}

	dPosGewicht=dGewicht*dMEUmrechnung;  // Mengenumrechnung berücksichtigen

	if(sKennung == "")  // die Position noch nicht existiert
		{
		s="DECLARE @myid uniqueidentifier "
			"SET @myid = NEWID() "
			"insert into belegp (Belegtyp, Belegnummer, Standort, Zeilentyp, Kennung, Vorgaenger, AngelegtAm, AngelegtVon) "
			"Values('"+_beleg->getBelegtyp()+"', '"+_beleg->getBelegnummer()+"', '"+sStandort+"', '"+sZeilentyp+"', @myid, NULL, GetDate(), '"+_beleg->getUser()+"') "
			" "
			"select @myid kennung";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			bSt=true;
			this->sKennung=q->FieldByName("kennung")->AsString;
			}
		q->Close();

		if(!bSt)
			{
			delete q;
			return(bSt);
			}
		}

	int iPosNum;
	if(AnsiContainsStr(sPostext, "."))
		{
		s=sPostext;
		s.Delete(1, sPostext.LastDelimiter("."));
		iPosNum=s.ToInt();
		}
	else
		{
		if(_beleg->getBelegtyp() != "W")
			{
			if(sPostext == "")
				sPostext="1";
			iPosNum=sPostext.ToInt();
			}
		else
			{
			iPosNum=sPostext.ToInt()+1;
			}
		}

	s="update belegp set ";

	s+=
		"adressnr='"+sAdressnr+"',"
		"datum='"+tdDatum.FormatString("dd.mm.yyyy")+"',"
		"postext='"+sPostext+"',"
		"zeilentyp='"+sZeilentyp+"',"
		"stueckliste='"+sStueckliste+"',"
		"posnummer='"+IntToStr(iPosNum)+"',"
		"menge='"+getSQLFloat(dMenge)+"',"
		"eingabemenge='"+getSQLFloat(getEditMengeValue(sEditMenge))+"',"
		"editmenge='"+sEditMenge+"',"
		"lagerfaktor='"+getSQLFloat(dMEUmrechnung)+"',"
		"artikelnummer='"+sArtikelnummer+"',"
		"mengeneinheit='"+sME+"',"
		"preiseinheit='"+getSQLFloat(dPE)+"',"
		"einzelpreis='"+getSQLFloat(dEP)+"',"
		"rabatt='"+getSQLFloat(dRabatt)+"',"
		"gesamtpreis='"+getSQLFloat(dGP)+"',"
		"kalkulationspreis='"+getSQLFloat(dKalk)+"',"
		"kalkpreiseuro='"+getSQLFloat(dKalk)+"',"
		"bezeichnung='"+getCheckedSQLStr(sBezeichnung.SubString(1, 80))+"',"
		"zusatz='"+getCheckedSQLStr(sZusatz.SubString(1, 80))+"',"
		"steuercode='"+sSteuercode+"',"
		"steuerprozent='"+getSQLFloat(dSteuerprozent)+"',"
		"konto='"+sKonto+"',"
		"mitarbeiter='"+sMitarbeiter+"',"
		"provisionsartikel='"+IntToStr((int)this->bProvisionsfaehig)+"',"
		"rabattfaehig='"+IntToStr((int)this->bRabattfaehig)+"',"
		"skontofaehig='"+IntToStr((int)this->bSkontofaehig)+"',"
		"netto='"+getSQLFloat(dNetto)+"', "
		"uebernahmeoffen='"+getSQLFloat(dRNetto)+"', "
		"nettoeuro='"+getSQLFloat(dNettoEuro)+"', "
		"rnetto='"+getSQLFloat(dRNetto)+"', "
		"rnettoeuro='"+getSQLFloat(dRNettoEuro)+"', "
		"umsatz='"+_beleg->getUmsatzKennzeichen()+"', "
		"gewicht='"+getSQLFloat(dPosGewicht)+"', "
		"frei1='"+getCheckedSQLStr(sFrei1.SubString(1, 80))+"', "
		"frei2='"+getCheckedSQLStr(sFrei2.SubString(1, 80))+"', "
		"freizahl='"+getSQLFloat(dFreizahl)+"', "
		"kostenstelle='"+getCheckedSQLStr(sKostenstelle)+"', "
		"kostentraeger='"+getCheckedSQLStr(sKostentraeger)+"', "
		"warencode='"+getCheckedSQLStr(sWarencode)+"', "
		"seriecharge='"+getCheckedSQLStr(sSerieCharge)+"', "
		"bestellnummer='"+getCheckedSQLStr(sBestellnummer)+"', "
		"preismenge='"+getSQLFloat(dPreismenge)+"', "
		"preismengeneinheit='"+getCheckedSQLStr(sPreismengeneinheit)+"', "
		"uebernahme='"+getSQLFloat(dUebernahme)+"', "
		"RefAdresse='"+_beleg->getAdresse()+"', "
		"KassFilialNr='0', "
		"Projekt='0' ";
	if(sTermin != "")
		s+=",termin='"+sTermin+"' ";
	if(sFreidatum != "")
		s+=",freidatum='"+sFreidatum+"' ";

	//if(_beleg->_strVersion1.ToInt() >= 21 &&_beleg-> _strVersion2.ToInt() >= 2 && _beleg->_strVersion3.ToInt() >= 1)
	if(_beleg->_strVersion1.ToInt() > 21 || (_beleg->_strVersion1.ToInt() == 21 && _beleg->_strVersion2.ToInt() >= 2))
		s+=",OSSVerfahren='"+IntToStr(iOSSVerfahren)+"' ";

	if(this->bLagerartikel)
		{
		s+=
		",lagerartikel='"+IntToStr((int)this->bLagerartikel)+"',"
		"lager='"+sLager+"', "
		"lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') ";
		}
	else
		s+=",lagerartikel='0' ";

	if(sVorgaengerpKennung != "")
		s+=",vorgaenger='"+sVorgaengerpKennung+"' ";

	if(this->sHauptkennung != "")
		{
		s+=",hauptkennung='"+sHauptkennung+"' ";
		}

	if(!bSt)
		s+=
			 ",BearbeitetAm=GetDate(),"
			 "BearbeitetVon='"+_beleg->getUser()+"' ";
	s+=
		"where kennung='"+sKennung+"'";

	q->SQL->Text=s;
	try
		{
		q->ExecSQL();
		}
	catch(Exception &e)
		{
		frmMain->log("Exception:"+e.Message);
    frmMain->log("SQL:"+s);
    }
	if(q->RowsAffected == 1)
		bSt=true;

	if(bSt && sLangtext != "")
		{
		s="update text set text='"+getCheckedSQLStr(sLangtext)+"' where blobkey=replace(replace('BP"+this->sKennung+"', '{', ''), '}', '') "
			"if @@rowcount=0 "
			"  insert into text (blobkey, text) Values(replace(replace('BP"+this->sKennung+"', '{', ''), '}', ''),'"+getCheckedSQLStr(sLangtext)+"')";
		q->SQL->Text=s;
		q->ExecSQL();
		}

	if(!bSt)
		{
		delete q2;
		delete q;
		return(false);
		}

	//****************************************************************************
	// hier prüfen ob eine Lageraktion notwendig ist
	float dVResult, dResult, dBereitsGelagert, dBereitsBestellt, dBereitsReserivert, dNochZuLagern, dNochZuBestellen, dNochZuReservieren, dUebernehmen;
	double dVMenge=0, dVVerwendet=0, dVUebernahmeoffen=0, dVOffen=0, dVReserviert=0, dVBestellt=0, dVGelagert=0;
	UnicodeString sTyp;
	double dChargenMenge;
	UnicodeString sSerieChargeArtikelnummer, sSerieChargeTyp, sTempSC;
	double dLagerMenge;

	// Übergabe/Übernahme checken
	if(sVorgaengerpKennung != "")
		{
		// Verwendete Menge des Vorgängers feststellen
		s="select belegnummer, menge, verwendet, uebernahmeoffen, menge-verwendet as offen from belegp where kennung='"+sVorgaengerpKennung+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			{
			dVMenge=q2->FieldByName("Menge")->AsFloat;
			dVVerwendet=q2->FieldByName("verwendet")->AsFloat;
			dVUebernahmeoffen=q2->FieldByName("uebernahmeoffen")->AsFloat;
			dVOffen=q2->FieldByName("offen")->AsFloat;
			}
		q2->Close();

		// Übernahmemenge berechnet - max. das was noch offen ist
		if(dVOffen < dMenge)
			dUebernehmen=dVOffen;
		else
			dUebernehmen=dMenge-this->dUebernahme;
		if(dUebernehmen != 0)
			_beleg->setUebergeben(this->sKennung, sVorgaengerpKennung, dUebernehmen);

		// Vorkasse-Positionen evtl. auch übernehmen
		s="IF EXISTS (SELECT * FROM VorkasseBelegLink WHERE BelegPKennung = '"+sVorgaengerpKennung+"') "
			"INSERT INTO VorkasseBelegLink (BelegPKennung, VorkasseBelegID) "
			"SELECT '"+this->sKennung+"', VorkasseBelegID "
			"FROM VorkasseBelegLink "
			"WHERE BelegPKennung = '"+sVorgaengerpKennung+"' ";
		q2->SQL->Text=s;
		q2->ExecSQL();
		}

	if(_beleg->getLagerFlag() == "A" && this->bLagerartikel)     // auslagern
		{
		// bereits gelagerte Mengen dieser Position checken
		s="select sum(bestand) as bestand from lagerprotokoll where typ in ('3','4') and belegpkennung='"+sKennung+"' and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			dBereitsGelagert=q2->FieldByName("bestand")->AsFloat*-1; // Abgänge kommen immer negativ zurück
		else
			dBereitsGelagert=0;
		q2->Close();

		//dMenge-=dBereitsGelagert;
		dLagerMenge=(dMenge*dMEUmrechnung)-dBereitsGelagert;
		//dNochZuLagern=dMenge;
		dNochZuLagern=dLagerMenge;

		if(sVorgaengerpKennung != "")
			{
			// prüfen ob Vorgaenger schon was gelagert oder reserviert hat - wirkt sich auf die noch zu lagernde Menge aus
			s="select typ, sum(bestand) as bestand, sum(menge) as menge from lagerprotokoll where belegpkennung='"+sVorgaengerpKennung+"' and artikelnummer='"+sArtikelnummer+"' group by typ";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				{
				sTyp=q2->FieldByName("typ")->AsString;
				if(sTyp == 1) // wenn eine Reservierung existiert
					{
					dVReserviert=q2->FieldByName("menge")->AsFloat*-1;   // Menge kommt im - -> gerechnet wird positiv
					if(dLagerMenge < dVReserviert)                            // wenn nicht die komplette Reserierung aufgelöst wird
						dVReserviert=dLagerMenge;                               // begrenzen auf die Menge
					if(dVReserviert != 0)                                // Reservierung zurück nehmen
						_beleg->setReserviert(sVorgaengerpKennung, this->tdDatum, sArtikelnummer, sLager, -dVReserviert);  // Aufruf positiv da im normalfall negatvi!
					dNochZuLagern=dLagerMenge;
					}
				else if(sTyp == 3)  // Lagerabgang
					{
					dVGelagert=q2->FieldByName("bestand")->AsFloat*-1;  // wird im Minus gemeldet -> gerechnet wird positiv
					dNochZuLagern=dLagerMenge-dVGelagert;
					}
				else if(sTyp == 8) // LAGERBESTANDPACK
					{
					dVGelagert=q2->FieldByName("bestand")->AsFloat*-1;  // wird im Minus gemeldet -> gerechnet wird positiv
					dNochZuLagern=dLagerMenge-dVGelagert;
					}
				else
					dNochZuLagern=dLagerMenge;
				}
			q2->Close();
			}

		if(dNochZuLagern < 0)
			{
			if(sSerieCharge == "S")
				{
				for(int x=0; x < dNochZuLagern; x++)
					{
					sTempSC=slSeriennummern->Strings[x];

					if(!_beleg->checkSerieCharge(sTempSC, sSerieChargeArtikelnummer, sSerieChargeTyp))
						_beleg->addSerieCharge(sTempSC, sArtikelnummer, sSerieCharge);

					bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, 1, 0, sTempSC);
					}
				}
			else if(sSerieCharge == "C")
				{
				for(int x=0; x < slChargen->Count; x++)
					{
					sTempSC=slChargen->Names[x];

					if(!_beleg->checkSerieCharge(sTempSC, sSerieChargeArtikelnummer, sSerieChargeTyp))
						_beleg->addSerieCharge(sTempSC, sArtikelnummer, sSerieCharge);

					dChargenMenge=StrToFloat(slChargen->ValueFromIndex[x]);
					bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, 0, sTempSC);
					}
				}
			else
				bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, -dNochZuLagern);
			}
		else
			{
			if(sSerieCharge == "S")
				{
				for(int x=0; x < dNochZuLagern; x++)
					{
					sTempSC=slSeriennummern->Strings[x];
					bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, 1, sTempSC);
					}
				}
			else if(sSerieCharge == "C")
				{
				for(int x=0; x < slChargen->Count; x++)
					{
					sTempSC=slChargen->Names[x];
					dChargenMenge=StrToFloat(slChargen->ValueFromIndex[x]);
					bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, sTempSC);
					}
				}
			else
				bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, dNochZuLagern);
			}
		}
	if(_beleg->getLagerFlag() == "P" && this->bLagerartikel)     // auslagern
		{
		// bereits gelagerte Mengen dieser Position checken
		s="select sum(bestand) as bestand from lagerprotokoll where typ in ('5','8') and belegpkennung='"+sKennung+"' and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			dBereitsGelagert=q2->FieldByName("bestand")->AsFloat*-1; // Abgänge kommen immer negativ zurück
		else
			dBereitsGelagert=0;
		q2->Close();

		//dMenge-=dBereitsGelagert;
		dLagerMenge=(dMenge*dMEUmrechnung)-dBereitsGelagert;
		//dNochZuLagern=dMenge;
		dNochZuLagern=dLagerMenge;

		if(sVorgaengerpKennung != "")
			{
			// prüfen ob Vorgaenger schon was gelagert oder reserviert hat - wirkt sich auf die noch zu lagernde Menge aus
			s="select typ, sum(bestand) as bestand, sum(menge) as menge from lagerprotokoll where belegpkennung='"+sVorgaengerpKennung+"' and artikelnummer='"+sArtikelnummer+"' group by typ";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				{
				sTyp=q2->FieldByName("typ")->AsString;
				if(sTyp == 1) // wenn eine Reservierung existiert
					{
					dVReserviert=q2->FieldByName("menge")->AsFloat*-1;   // Menge kommt im - -> gerechnet wird positiv
					if(dLagerMenge < dVReserviert)                            // wenn nicht die komplette Reserierung aufgelöst wird
						dVReserviert=dLagerMenge;                               // begrenzen auf die Menge
					if(dVReserviert != 0)                                // Reservierung zurück nehmen
						_beleg->setReserviert(sVorgaengerpKennung, this->tdDatum, sArtikelnummer, sLager, -dVReserviert);  // Aufruf positiv da im normalfall negatvi!
					dNochZuLagern=dLagerMenge;
					}
				else if(sTyp == 3)  // Lagerabgang

					{
					dVGelagert=q2->FieldByName("bestand")->AsFloat*-1;  // wird im Minus gemeldet -> gerechnet wird positiv
					dNochZuLagern=dLagerMenge-dVGelagert;
					}
				else if(sTyp == 8) // LAGERBESTANDPACK
					{
					dVGelagert=q2->FieldByName("bestand")->AsFloat*-1;  // wird im Minus gemeldet -> gerechnet wird positiv
					dNochZuLagern=dLagerMenge-dVGelagert;
					}
				else
					dNochZuLagern=dLagerMenge;
				}
			q2->Close();
			}

		if(dNochZuLagern < 0)
			{
			if(sSerieCharge == "S")
				{
				for(int x=0; x < dNochZuLagern; x++)
					{
					sTempSC=slSeriennummern->Strings[x];

					if(!_beleg->checkSerieCharge(sTempSC, sSerieChargeArtikelnummer, sSerieChargeTyp))
						_beleg->addSerieCharge(sTempSC, sArtikelnummer, sSerieCharge);

					bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, 1, 0, sTempSC, 7);
					bSt=_beleg->subPack(sKennung, tdDatum, sArtikelnummer, sLager, 1, sTempSC);
					}
				}
			else if(sSerieCharge == "C")
				{
				for(int x=0; x < slChargen->Count; x++)
					{
					sTempSC=slChargen->Names[x];

					if(!_beleg->checkSerieCharge(sTempSC, sSerieChargeArtikelnummer, sSerieChargeTyp))
						_beleg->addSerieCharge(sTempSC, sArtikelnummer, sSerieCharge);

					dChargenMenge=StrToFloat(slChargen->ValueFromIndex[x]);
					bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, 0, sTempSC, 7);
					bSt=_beleg->subPack(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, sTempSC);
					}
				}
			else
				{
				bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, -dNochZuLagern, 0, "", 7);
				bSt=_beleg->subPack(sKennung, tdDatum, sArtikelnummer, sLager, -dNochZuLagern);
				}
			}
		else
			{
			if(sSerieCharge == "S")
				{
				for(int x=0; x < dNochZuLagern; x++)
					{
					sTempSC=slSeriennummern->Strings[x];
					bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, 1, sTempSC, 7);
					bSt=_beleg->addPack(sKennung, tdDatum, sArtikelnummer, sLager, 1, 0, sTempSC);
					}
				}
			else if(sSerieCharge == "C")
				{
				for(int x=0; x < slChargen->Count; x++)
					{
					sTempSC=slChargen->Names[x];
					dChargenMenge=StrToFloat(slChargen->ValueFromIndex[x]);
					bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, sTempSC, 7);
					bSt=_beleg->addPack(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, 0, sTempSC);
					}
				}
			else
				{
				bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, dNochZuLagern, "", 7);
				bSt=_beleg->addPack(sKennung, tdDatum, sArtikelnummer, sLager, dNochZuLagern);
				}
			}
		}
	else if(_beleg->getLagerFlag() == "Z" && this->bLagerartikel)  // zulagern
		{
		// bereits gelagerte Mengen dieser Position checken
		s="select sum(bestand) as bestand from lagerprotokoll where typ in ('3','4') and belegpkennung='"+sKennung+"' and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			dBereitsGelagert=q2->FieldByName("bestand")->AsFloat;
		else
			dBereitsGelagert=0;
		q2->Close();

		//dMenge-=dBereitsGelagert;
		//dNochZuLagern=dMenge;
		dLagerMenge=(dMenge*dMEUmrechnung)-dBereitsGelagert;
		dNochZuLagern=dLagerMenge;

		if(sVorgaengerpKennung != "")
			{
			s="select typ, sum(bestand) as bestand, sum(menge) as menge from lagerprotokoll where belegpkennung='"+sVorgaengerpKennung+"' and artikelnummer='"+sArtikelnummer+"' group by typ";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				{
				sTyp=q2->FieldByName("typ")->AsString;
				if(sTyp == 2) // wenn eine Bestellung existiert
					{
					dVBestellt=q2->FieldByName("menge")->AsFloat;
					if(dLagerMenge < dVBestellt)                              // wenn nicht die komplette Bestellung aufgelöst wird
						dVBestellt=dLagerMenge;                                 // begrenzen auf die Menge
					if(dVBestellt != 0) 																 // Bestellung zurück nehmen
						_beleg->setBestellt(sVorgaengerpKennung, this->tdDatum, sArtikelnummer, sLager, -dVBestellt);
					dNochZuLagern=dLagerMenge;
					}
				else if(sTyp == 4)
					{
					dVGelagert=q2->FieldByName("bestand")->AsFloat;  		 // wird im Minus gemeldet -> gerechnet wird positiv
					dNochZuLagern=dLagerMenge-dVGelagert;
					}
				else
					dNochZuLagern=dLagerMenge;
				}
			q2->Close();
			}
		else
			dNochZuLagern=dLagerMenge;

		if(dNochZuLagern < 0)
			{
			if(sSerieCharge == "S")
				{
				for(int x=0; x < abs(dNochZuLagern); x++)
					{
					sTempSC=slSeriennummern->Strings[x];
					bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, 1, sTempSC);
					}
				}
			else if(sSerieCharge == "C")
				{
				for(int x=0; x < slChargen->Count; x++)
					{
					sTempSC=slChargen->Names[x];
					dChargenMenge=StrToFloat(slChargen->ValueFromIndex[x]);
					bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, sTempSC);
					}
				}
			else
				bSt=_beleg->subBestand(sKennung, tdDatum, sArtikelnummer, sLager, -dNochZuLagern);
			}
		else
			{
			if(sSerieCharge == "S")
				{
				for(int x=0; x < dNochZuLagern; x++)
					{
					sTempSC=slSeriennummern->Strings[x];

					if(!_beleg->checkSerieCharge(sTempSC, sSerieChargeArtikelnummer, sSerieChargeTyp))
						_beleg->addSerieCharge(sTempSC, sArtikelnummer, sSerieCharge);

					bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, 1, 0, sTempSC);
					}
				}
			else if(sSerieCharge == "C")
				{
				for(int x=0; x < slChargen->Count; x++)
					{
					sTempSC=slChargen->Names[x];

					if(!_beleg->checkSerieCharge(sTempSC, sSerieChargeArtikelnummer, sSerieChargeTyp))
						_beleg->addSerieCharge(sTempSC, sArtikelnummer, sSerieCharge);

					dChargenMenge=StrToFloat(slChargen->ValueFromIndex[x]);
					bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, dChargenMenge, 0, sTempSC);
					}
				}
			else
				bSt=_beleg->addBestand(sKennung, tdDatum, sArtikelnummer, sLager, dNochZuLagern);
			}
		}
	else if(_beleg->getLagerFlag() == "R" && this->bLagerartikel)  // reservieren
		{
		// Bereits reservierte Menge checken
		s="select sum(menge) as menge from lagerprotokoll where typ='1' and belegpkennung='"+sKennung+"' and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			dBereitsReserivert=q2->FieldByName("menge")->AsFloat*-1;  // Es kommt immer eine negative Menge=reserviert
		else
			dBereitsReserivert=0;
		q2->Close();

		//dMenge-=dBereitsReserivert;
		dLagerMenge=(dMenge*dMEUmrechnung)-dBereitsReserivert;

		if(sVorgaengerpKennung != "")
			{
			// prüfen ob Vorgaenger schon was gelagert oder reserviert hat
			/*
			s="select typ, sum(bestand) as bestand, sum(menge) as menge from lagerprotokoll where belegpkennung='"+sVorgaengerpKennung+"' and artikelnummer='"+sArtikelnummer+"' group by typ";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)  // wenn ja, dann bestimmt diese Menge auch die Ueberabe/Verwendet Mengen
				{
				sTyp=q2->FieldByName("typ")->AsString;
				if(sTyp == 1)      // wenn eine Reservierung existiert
					{
					dVReserviert=q2->FieldByName("menge")->AsFloat*-1;    // Menge kommt im - -> gerechnet wird positiv
					dNochZuReservieren=dLagerMenge-dVReserviert;
					}
				else if(sTyp == 3)
					{
					dNochZuReservieren=dLagerMenge;
					}
				else
					dNochZuReservieren=dLagerMenge;
				}
			q2->Close();
			*/
			if(dLagerMenge != 0)                                 // Rest Rerserieren
				_beleg->setReserviert(sVorgaengerpKennung, tdDatum, sArtikelnummer, sLager, -dLagerMenge);
      dNochZuReservieren=dLagerMenge;
			}
		else
			dNochZuReservieren=dLagerMenge;

		// Reservierung: immer relativ - d.h. es muss immer die differenz zu vorher geschrieben werden (+reserviert -löst auf)
		if(dNochZuReservieren != 0)                                 // Rest Rerserieren
			_beleg->setReserviert(sKennung, tdDatum, sArtikelnummer, sLager, dNochZuReservieren);
		bSt=true;
		}
	else if(_beleg->getLagerFlag() == "E" && this->bLagerartikel) // bestellt
		{
		// Bereits bestellte Menge checken
		s="select sum(menge) as menge from lagerprotokoll where typ='2' and belegpkennung='"+sKennung+"' and artikelnummer='"+sArtikelnummer+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			dBereitsBestellt=q2->FieldByName("menge")->AsFloat;
		else
			dBereitsBestellt=0;
		q2->Close();

		//dMenge-=dBereitsBestellt;
		dLagerMenge=(dMenge*dMEUmrechnung)-dBereitsBestellt;

		// prüfen ob Vorgaenger schon was gelagert oder reserviert hat
		if(sVorgaengerpKennung != "")
			{
			s="select typ, sum(bestand) as bestand, sum(menge) as menge from lagerprotokoll where belegpkennung='"+sVorgaengerpKennung+"' and artikelnummer='"+sArtikelnummer+"' group by typ";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				{
				sTyp=q2->FieldByName("typ")->AsString;
				if(sTyp == 2) // wenn eine Bestellung existiert
					{
					dVBestellt=q2->FieldByName("menge")->AsFloat;
					dNochZuBestellen=dLagerMenge-dVBestellt;
					}
				else if(sTyp == 4)  // Lagerbestand
					{
					dNochZuBestellen=dLagerMenge;
					}
				else
					dNochZuBestellen=dLagerMenge;
				}
			q2->Close();
			}
		else
			dNochZuBestellen=dLagerMenge;

		if(dNochZuBestellen != 0)
			_beleg->setBestellt(sKennung, tdDatum, sArtikelnummer, sLager, dNochZuBestellen);
		bSt=true;
		}

	//****************************************************************************
	if(sStueckliste == "H")                 // Handelsstückliste
		{
		AnsiString sSetArtikel;
		double dMenge;
		double dHPosEP;

		dHPosEP=this->getEinzelpreis();
		dMenge=this->getMenge();

		s="select s.*, a.preiseinheit, case when kalkulation='0' then zielpreis else dbo.SL_fnGetListenpreis(s.setartikelnummer, 0, 0) end as Preis, "
			"cast(0 as float) GGes, cast(0 as float) GProz, cast(0 as float) G "
			"into #temp from artset s "
			"left join art a on (a.artikelnummer=s.setartikelnummer) "
			"where s.artikelnummer='"+sArtikelnummer+"' "
			"order by s.position "
			" "
			"update #temp set "
			"	 GGes=(select sum(preis) from #temp) "
			" "
			"update #temp set "
			"  GProz=Preis/GGes*100 "
			" "
			"select * from #temp "
			" "
			"drop table #temp";
		q->SQL->Text=s;
		q->Open();
		double dGProz, dGPosEP, dGPosGP, dGMenge, dGPreiseinheit, dGKalkpreis;
		bool bKalkulation;
		for(int i=0; i < q->RecordCount; i++)
			{
			sSetArtikel=q->FieldByName("SetArtikelnummer")->AsString;
			dGKalkpreis=q->FieldByName("Ausgangspreis")->AsFloat;
			bKalkulation=q->FieldByName("Kalkulation")->AsBoolean;
			dGProz=q->FieldByName("GProz")->AsFloat;
			dGMenge=q->FieldByName("Menge")->AsFloat;
			//dGMenge=q->FieldByName("Menge")->AsFloat;
			dGPreiseinheit=q->FieldByName("Preiseinheit")->AsFloat;
			dGPosEP=((dHPosEP*dMenge)/100*dGProz)/(dGMenge*dMenge)*dGPreiseinheit;

			TBelegp *bp=new TBelegp(this->_beleg);
			bp->create("G");
			bp->setArtikel(sSetArtikel);
			bp->setMenge(dGMenge*dMenge);
			bp->setEinzelpreis(dGPosEP);
			if(!bKalkulation)
				bp->setKalkpreis(dGKalkpreis);
			bp->setEditMenge(AnsiReplaceStr(FloatToStr(dGMenge), ",", "."));
			if(dMenge > 1)
				bp->setGP(dGPosEP*dGMenge/dGPreiseinheit);

			/*
			sSetArtikel=q->FieldByName("SetArtikelnummer")->AsString;
			dGKalkpreis=q->FieldByName("Ausgangspreis")->AsFloat;
			bKalkulation=q->FieldByName("Kalkulation")->AsBoolean;
			dGProz=q->FieldByName("GProz")->AsFloat;
			dGMenge=q->FieldByName("Menge")->AsFloat*dMenge;
			//dGMenge=q->FieldByName("Menge")->AsFloat;
			dGPreiseinheit=q->FieldByName("Preiseinheit")->AsFloat;
			dGPosEP=((dHPosEP*dMenge)/100*dGProz)/dGMenge*dGPreiseinheit;

			TBelegp *bp=new TBelegp(this->_beleg);
			bp->create("G");
			bp->setArtikel(sSetArtikel);
			bp->setMenge(dGMenge);
			bp->setEinzelpreis(dGPosEP);
			if(!bKalkulation)
				bp->setKalkpreis(dGKalkpreis);
			if(dMenge > 1)
				{
				bp->setEditMenge("1");
				bp->setGP(dGPosEP);
				}
			*/
			bp->setHauptkennung(sKennung);
			bp->setPostext(sPostext+"."+IntToStr(i+1));
			bp->save();

			delete bp;

			q->Next();
			}
		q->Close();
		}

	//****************************************************************************

	delete q2;
	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TBelegp::del(UnicodeString sKennung)
{
	UnicodeString s, strArtikel, strLager, strVorgaenger;
	bool bSt=false;
	int iTyp;
	double dMenge, dBestand, dWert;
	TDateTime tdDatum;


	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;

	if(sKennung != "")
		this->edit(sKennung);
	else
    sKennung=this->getKennung();

	this->setMenge(0);

	this->save();

	/*
	// Prüfen ob was von einem Vorgänger übernommen wurde
	s="select vorgaenger, uebernahme from belegp where kennung='"+sKennung+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		dUebernahme=q->FieldByName("uebernahme")->AsFloat;
		strVorgaenger=q->FieldByName("Vorgaenger")->AsString;
		}
	q->Close();

	// Lagerungsmengen/Reservierungen/Bestellungen prüfen
	s="select typ, datum, artikelnummer, lager, wert, sum(menge) menge, sum(Bestand) bestand from lagerprotokoll where belegpkennung='"+sKennung+"' "
		"group by typ, datum, artikelnummer, lager, wert";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		iTyp=q->FieldByName("typ")->AsInteger;
		dMenge=q->FieldByName("menge")->AsFloat;
		dBestand=q->FieldByName("bestand")->AsFloat;
		dWert=q->FieldByName("wert")->AsFloat;
		tdDatum=q->FieldByName("datum")->AsDateTime;
		strArtikel=q->FieldByName("Artikelnummer")->AsString;
		strLager=q->FieldByName("Lager")->AsString;

		if(iTyp == ltyReservierung)
			{
			_beleg->setReserviert(sKennung, tdDatum, strArtikel, strLager, dMenge, dWert);
			}
		else if(iTyp == ltyBestellung)
			{
			_beleg->setBestellt(sKennung, tdDatum, strArtikel, strLager, -dMenge, dWert);
			}
		else if(iTyp == ltyAbgang)
			{
			_beleg->subBestand(sKennung, tdDatum, strArtikel, strLager, dBestand);
			}
		else if(iTyp == ltyZugang)
			{
			_beleg->addBestand(sKennung, tdDatum, strArtikel, strLager, -dBestand, dWert);
			}

		q->Next();
		}
	q->Close();

	// Übergabe/Übernahme checken

	if(strVorgaenger != "")
		{
		if(dUebernahme != 0)
			_beleg->setUebergeben(sKennung, strVorgaenger, -dUebernahme);
		}
	*/

	s="delete from belegp where kennung='"+sKennung+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;
	return(true);
}

//---------------------------------------------------------------------------

bool TBelegp::take(UnicodeString sQuellKennung, double dMenge)
{
	UnicodeString s, strArtikel, strLager;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;
	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=_beleg->getCon();
	q2->LockType=ltReadOnly;


	s="select * from belegp where kennung='"+sQuellKennung+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount == 0)
		{
		delete q;
		return(false);
		}

	this->setMenge(dMenge);

	this->sVorgaengerpKennung=q->FieldByName("Kennung")->AsString;
	//this->setArtikel(q->FieldByName("Artikelnummer")->AsString);   // SetArtikel würde mehr machen!
	this->sZeilentyp=q->FieldByName("Zeilentyp")->AsString;
	this->setStueckliste(q->FieldByName("Stueckliste")->AsString);
	this->sArtikelnummer=q->FieldByName("Artikelnummer")->AsString;
	this->setMengeneinheit(q->FieldByName("Mengeneinheit")->AsString);
	this->setBezeichnung(q->FieldByName("Bezeichnung")->AsString);
	this->setZusatz(q->FieldByName("Zusatz")->AsString);
	this->setEinzelpreis(q->FieldByName("Einzelpreis")->AsFloat);
	this->setPreiseinheit(q->FieldByName("Preiseinheit")->AsFloat);
	this->setRabatt(q->FieldByName("Rabatt")->AsFloat);
	//this->dGP=q->FieldByName("Gesamtpreis")->AsFloat;

	this->setTermin(q->FieldByName("Termin")->AsDateTime);
	this->setGewicht(q->FieldByName("Gewicht")->AsFloat);
	this->setRabatt2(q->FieldByName("Rabatt2")->AsFloat);

	this->setFrei1(q->FieldByName("Frei1")->AsString);
	this->setFrei2(q->FieldByName("Frei2")->AsString);
	this->setFreizahl(q->FieldByName("Freizahl")->AsFloat);
	if(!q->FieldByName("Freidatum")->IsNull)
		this->setFreidatum(q->FieldByName("Freidatum")->AsDateTime.FormatString("dd.mm.yyyy"));
	this->setKostenstelle(q->FieldByName("Kostenstelle")->AsString);
	this->setKostentraeger(q->FieldByName("Kostentraeger")->AsString);
	this->setWarencode(q->FieldByName("Warencode")->AsString);
	this->setBestellnummer(q->FieldByName("Bestellnummer")->AsString);
	this->setPreismengeneinheit(q->FieldByName("Preismengeneinheit")->AsString);
	this->setPreismenge(q->FieldByName("Preismenge")->AsFloat);
	this->sSerieCharge=q->FieldByName("SerieCharge")->AsString;


	this->bRabattfaehig=q->FieldByName("Rabattfaehig")->AsBoolean;
	this->bSkontofaehig=q->FieldByName("Skontofaehig")->AsBoolean;
	this->bProvisionsfaehig=q->FieldByName("Provisionsartikel")->AsBoolean;
	this->bLagerartikel=q->FieldByName("Lagerartikel")->AsBoolean;
	this->setLager(q->FieldByName("Lager")->AsString);

	this->setSteuercode(q->FieldByName("Steuercode")->AsString);
	this->setKonto(q->FieldByName("Konto")->AsString);
	this->setMitarbeiter(q->FieldByName("Mitarbeiter")->AsString);
	this->setSteuerprozent(q->FieldByName("Steuerprozent")->AsFloat);
	this->setKalkpreis(q->FieldByName("Kalkulationspreis")->AsFloat);

	// Langtext holen
	s="select text from text where blobkey=replace(replace('BP"+this->sVorgaengerpKennung+"', '{', ''), '}', '')";
	q2->SQL->Text=s;
	q2->Open();
	if(q2->RecordCount)
		this->sLangtext=q2->FieldByName("text")->AsString;
	q2->Close();

	// Handelsstücklisten
	AnsiString strQuellHauptkennung, strZielHauptkennung;
	//if(this->sZeilentyp == "G")
	if(!q->FieldByName("Hauptkennung")->IsNull)
		{
		strQuellHauptkennung=q->FieldByName("Hauptkennung")->AsString;

		s="select kennung from belegp where vorgaenger='"+strQuellHauptkennung+"'";
		q2->SQL->Text=s;
		q2->Open();
		if(q2->RecordCount)
			strZielHauptkennung=q2->FieldByName("Kennung")->AsString;
		else
			strZielHauptkennung="";
		q2->Close();

		if(strZielHauptkennung != "")
			this->setHauptkennung(strZielHauptkennung);
		}

	q->Close();

	//save();   // Position speichern

	delete q2;
	delete q;
	return(true);
}

//---------------------------------------------------------------------------

bool TBelegp::dispose(void)
{
	AnsiString s;
	bool bSt;
	double dOffen;

	dOffen=this->dMenge-this->dVerwendet;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;

	if(_beleg->getLagerFlag() == "R" && this->bLagerartikel)
		{
		_beleg->setReserviert(this->sKennung, this->tdDatum, this->sArtikelnummer, this->sLager, -dOffen);
		}

	s="update belegp set status='3' where kennung='"+this->sKennung+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

bool TBelegp::undispose(void)
{
	AnsiString s;
	bool bSt;
	double dOffen;

	dOffen=this->dMenge-this->dVerwendet;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;

	if(_beleg->getLagerFlag() == "R" && this->bLagerartikel)
		{
		_beleg->setReserviert(this->sKennung, this->tdDatum, this->sArtikelnummer, this->sLager, dOffen);
		}

	s="update belegp set status=case when verwendet='0' then '0' when menge-verwendet > 0 then '1' else '2' end where kennung='"+this->sKennung+"'";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

void TBelegp::setMenge(double dMenge)
{
	this->dMenge=dMenge;
	this->sEditMenge=FloatToStr(dMenge);

	calc();
}

//---------------------------------------------------------------------------

void TBelegp::setEditMenge(UnicodeString sEingabeMenge)
{
	this->sEditMenge=sEingabeMenge;
}

//---------------------------------------------------------------------------

bool TBelegp::setArtikel(UnicodeString sArtikel)
{
	UnicodeString s;
	bool bSt=false;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();
	q->LockType=ltReadOnly;
	TADOQuery *q2=new TADOQuery(NULL);
	q2->Connection=_beleg->getCon();
	q2->LockType=ltReadOnly;


	s="select * from art where artikelnummer='"+sArtikel+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount == 0)
		{
		q->Close();

		s="select * from art where eannummer='"+sArtikel+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount == 0)
			bSt=false;
		q->Close();
		}
	else
		bSt=true;

	if(bSt)
		{
		this->sArtikelnummer=q->FieldByName("Artikelnummer")->AsString;
		this->sBezeichnung=q->FieldByName("Bezeichnung")->AsString;
		this->sZusatz=q->FieldByName("Zusatz")->AsString;
		this->sME=q->FieldByName("Mengeneinheit")->AsString;
		this->sKostenstelle=q->FieldByName("Kostenstelle")->AsString;
		this->sKostentraeger=q->FieldByName("Kostentraeger")->AsString;
		this->sSerieCharge=q->FieldByName("SerieCharge")->AsString;
		this->dGewicht=q->FieldByName("Gewicht")->AsFloat;
		this->sStueckliste=q->FieldByName("Stueckliste")->AsString;
		if(this->sStueckliste == "H")
			this->sZeilentyp="H";

		this->dPE=q->FieldByName("Preiseinheit")->AsFloat;
		this->bRabattfaehig=q->FieldByName("Rabattfaehig")->AsBoolean;
		this->bSkontofaehig=q->FieldByName("Skontofaehig")->AsBoolean;
		this->bProvisionsfaehig=q->FieldByName("Provisionsfaehig")->AsBoolean;
		this->bLagerartikel=q->FieldByName("Lagerartikel")->AsBoolean;
		if(this->bLagerartikel)
			{
			s="select lager from artortlager where standort='"+_beleg->getStandort()+"' and artikelnummer='"+this->sArtikelnummer+"'";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				this->sLager=q2->FieldByName("Lager")->AsString;
      q2->Close();
			}


		// Fibukonto feststellen
		if(_beleg->getAdressBlobKey() == "KU")
			{
			if(_beleg->getPreistyp() != "S")
				{
				this->sSteuercode=q->FieldByName("SSVerkauf")->AsString;
				this->dSteuerprozent=_beleg->getSteuerProzent(this->sSteuercode);
				}
			else
				{
				this->sSteuercode="";
				this->dSteuerprozent=0;
				}
			if(q->FieldByName("KontoVerkauf")->AsString != "")
				this->sKonto=q->FieldByName("KontoVerkauf")->AsString;
			else if(q->FieldByName("Erfolgskontengruppe")->AsString != "")
				{
				UnicodeString sAbwSteuercode;
				int iOSSVerfahren;
				this->sKonto=_beleg->getErloeskontoFromMatrix(q->FieldByName("Erfolgskontengruppe")->AsString, _beleg->getKontengruppe(), sAbwSteuercode, iOSSVerfahren);
				if(sAbwSteuercode != "")
					this->sSteuercode=sAbwSteuercode;

				//if(_beleg->_strVersion1.ToInt() >= 21 && _beleg->_strVersion2.ToInt() >= 2 && _beleg->_strVersion3.ToInt() >= 1)
				if(_beleg->_strVersion1.ToInt() > 21 || (_beleg->_strVersion1.ToInt() == 21 && _beleg->_strVersion2.ToInt() >= 2))
					this->iOSSVerfahren=iOSSVerfahren;
				}

			// Einzelpreis Preis holen
			this->dEPBasis=_beleg->getArtikelVKPreis(this->sArtikelnummer, _beleg->getWaehrungscode(), this->dMenge, _beleg->getPreisgruppe(), _beleg->getAdresse());
			this->dEP=this->dEPBasis;
			//if(_beleg->getWaehrungscode() != "EUR")
			//	this->dEP/=_beleg->getWaehrungsfaktor(_beleg->getWaehrungscode(), _beleg->getDatum());

			double dRabatt;
			s="declare @art nvarchar(18) "
				"declare @kunde nvarchar(13) "
				"declare @rbgr nvarchar(13) "
				"declare @artgr nvarchar(10) "
				"set @art='"+this->sArtikelnummer+"' "
				"set @kunde='"+this->sAdressnr+"' "
				"set @rbgr=(select rabattgruppe from kunden where nummer=@kunde) "
				"set @artgr=(select artikelgruppe from art where artikelnummer=@art) "
				" "
				"if (select count(*) from rabgrup where nummer=@art and rabattgruppe=@rbgr) = 1 "
				"  select isnull(rabatt+rabatt2, 0) rabatt from [RABGRUP] where nummer=@art and rabattgruppe=@rbgr "
				"else "
				"	 select isnull(rabatt+rabatt2, 0) rabatt from [RABGRUP] where nummer=@artgr and rabattgruppe=@rbgr ";
			q->SQL->Text=s;
			q->Open();
			if(q->RecordCount)
				dRabatt=q->FieldByName("Rabatt")->AsFloat;
			else
				dRabatt=0.0;
			q->Close();

			if(dRabatt != 0.0)
				this->dRabatt=dRabatt;
			}
		else if(_beleg->getAdressBlobKey() == "LI")
			{
			if(_beleg->getPreistyp() != "S")
				{
				this->sSteuercode=q->FieldByName("SSEinkauf")->AsString;
				this->dSteuerprozent=_beleg->getSteuerProzent(this->sSteuercode);
				}
			else
				{
				this->sSteuercode="";
				this->dSteuerprozent=0;
				}
			if(q->FieldByName("KontoEinkauf")->AsString != "")
				this->sKonto=q->FieldByName("KontoEinkauf")->AsString;
			else if(q->FieldByName("Aufwandskontengruppe")->AsString != "")
				{
				UnicodeString sAbwSteuercode;
				this->sKonto=_beleg->getAufwandskontoFromMatrix(q->FieldByName("Aufwandskontengruppe")->AsString, _beleg->getKontengruppe(), sAbwSteuercode);
				if(sAbwSteuercode != "")
					this->sSteuercode=sAbwSteuercode;
				}
			// Einzelpreis Preis holen
			this->dEPBasis=_beleg->getArtikelEKPreis(this->sArtikelnummer, this->dMenge, _beleg->getAdresse());
			this->dEP=this->dEPBasis;
			//if(_beleg->getWaehrungscode() != "EUR")
			//	this->dEP/=_beleg->getWaehrungsfaktor(_beleg->getWaehrungscode(), _beleg->getDatum());

			// Prüfen ob Lieferant spezielle Skonto/Rabatt-Fähigkeit hat
			s="select * from artlief where artikelnummer='"+this->sArtikelnummer+"' and lieferant='"+_beleg->getAdresse()+"'";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				{
				this->bRabattfaehig=q2->FieldByName("Rabattfaehig")->AsBoolean;
				this->bSkontofaehig=q2->FieldByName("Skontofaehig")->AsBoolean;
				this->dRabatt=q2->FieldByName("Rabatt")->AsFloat;
				this->dRabatt2=q2->FieldByName("Rabatt2")->AsFloat;
				}
			q2->Close();
			}
		q->Close();

		// Steuerprozent feststellen
		this->dSteuerprozent=_beleg->getSteuerProzent(this->sSteuercode);

		// Kalkpreis holen
		//s="select kalkulationspreis from arkalk where artikelnummer='"+sArtikelnummer+"'";
		s="select dbo.SL_fnKalkpreis('"+sArtikelnummer+"', GetDate(), 'EUR', 0) kalkulationspreis ";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			this->dKalkBasis=q->FieldByName("Kalkulationspreis")->AsFloat;
			this->dKalk=this->dKalkBasis;
			}
		else
			{
			this->dKalkBasis=0;
			this->dKalk=0;
			}
		q->Close();

		// Langtext holen
		s="select text from text where blobkey='AR"+sArtikelnummer+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			this->sLangtext=q->FieldByName("Text")->AsString;
		else
			this->sLangtext="";
		q->Close();

		// Gesamtpreis berechnen
		calc();
		}

	delete q2;
	delete q;

	return(bSt);
}

//---------------------------------------------------------------------------

void TBelegp::setLager(UnicodeString sLager)
{
	AnsiString s;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=_beleg->getCon();

	this->sLager=sLager;

	s="select standort from lager where lager='"+sLager+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		this->sStandort=q->FieldByName("Standort")->AsString;
	q->Close();

	delete q;
}


//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

TWerkauftrag::TWerkauftrag(TADOConnection *con) : TBeleg(con)
{
}

//---------------------------------------------------------------------------

UnicodeString TWerkauftrag::create(UnicodeString sBelegnumemr)
{
	UnicodeString sNewBelegnummer;

	sNewBelegnummer=TBeleg::create("W", sBelegnummer);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::edit(UnicodeString sBelegnummer)
{
	TBeleg::edit("W", sBelegnummer);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setArtikel(UnicodeString sArtikelnummer, bool bOhneTeile)
{
	UnicodeString s, sSTLArtikel, sSTLME;
	double dSTLMenge;
	bool bSTLLagern;
	bool bSt;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	TBelegp *wkBelegp;

	wkBelegp=new TBelegp(this);
	bSt=wkBelegp->create("A");
	bSt=wkBelegp->setArtikel(sArtikelnummer);
	bSt=wkBelegp->save();
	delete wkBelegp;

	if(!bOhneTeile)
		{
		s="select * from artset where artikelnummer='"+sArtikelnummer+"' order by position";
		q->SQL->Text=s;
		q->Open();
		for(int i=0; i < q->RecordCount; i++)
			{
			sSTLArtikel=q->FieldByName("SetArtikelnummer")->AsString;
			sSTLME=q->FieldByName("Mengeneinheit")->AsString;
			dSTLMenge=q->FieldByName("Menge")->AsFloat;
			bSTLLagern=q->FieldByName("Lagern")->AsBoolean;

			wkBelegp=new TBelegp(this);
			bSt=wkBelegp->create("A");
			bSt=wkBelegp->setArtikel(sSTLArtikel);
			wkBelegp->setMenge(1);
			wkBelegp->setEditMenge(FloatToStr(dSTLMenge));
			wkBelegp->setMengeneinheit(sSTLME);
			bSt=wkBelegp->save();
			delete wkBelegp;

			q->Next();
			}
		q->Close();
		}

	setSkontoprozent1(1);
}

//---------------------------------------------------------------------------

UnicodeString TWerkauftrag::getArtikel(void)
{
	UnicodeString s, sArtikel;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	s="select artikelnummer from belegp where belegtyp='W' and belegnummer='"+this->getBelegnummer()+"' and postext=''";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		sArtikel=q->FieldByName("Artikelnummer")->AsString;
		}
	q->Close();
	delete q;

	return(sArtikel);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setMenge(double dMenge)  // nur möglich wenn Werkauftrag noch Status=0 hat (keine Reservierung....
{
	UnicodeString s, sKennung, sStatus, sEditMenge;
	bool bSt;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	s="select kennung, status from belegp where belegtyp='W' and belegnummer='"+this->getBelegnummer()+"' and postext='1'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		sKennung=q->FieldByName("kennung")->AsString;
		sStatus=q->FieldByName("status")->AsString;
		}
	q->Close();

	if(sStatus != "0")
		{
		delete q;
		return(false);
		}

	// Hauptposition setzen
	TBelegp *wkBelegp;
	wkBelegp=new TBelegp(this);
	wkBelegp->edit(sKennung);

	wkBelegp->setMenge(dMenge);
	bSt=wkBelegp->save();
	delete wkBelegp;

	setSkontoprozent1(dMenge);  // Reservierung offen im Werkauftrag

	// Teile setzen
	s="select kennung, status from belegp where belegtyp='W' and belegnummer='"+this->getBelegnummer()+"' and postext != ''";
	q->SQL->Text=s;
	q->Open();
	for(int i=0; i < q->RecordCount; i++)
		{
		sKennung=q->FieldByName("kennung")->AsString;

		wkBelegp=new TBelegp(this);
		wkBelegp->edit(sKennung);
		sEditMenge=wkBelegp->getEditMenge();
		wkBelegp->setMenge(dMenge);
		wkBelegp->setEditMenge(sEditMenge);
		bSt=wkBelegp->save();
		delete wkBelegp;

		q->Next();
		}
	q->Close();

	delete q;
	return(true);
}

//---------------------------------------------------------------------------

double TWerkauftrag::getMenge(void)
{
	UnicodeString s;
	double dMenge;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	s="select menge from belegp where belegtyp='W' and belegnummer='"+this->getBelegnummer()+"' and postext=''";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		dMenge=q->FieldByName("Menge")->AsFloat;
		}
	q->Close();
	delete q;

	return(dMenge);
}

//---------------------------------------------------------------------------

UnicodeString TWerkauftrag::getLagerAus(void)
{
	return(this->getLager());
}

//---------------------------------------------------------------------------

void TWerkauftrag::setLagerAus(UnicodeString sLager)
{
	this->setLager(sLager);
}

//---------------------------------------------------------------------------

UnicodeString TWerkauftrag::getLagerEin(void)
{
	UnicodeString s, sLager;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	s="select lager from belegp where belegtyp='W' and belegnummer='"+this->getBelegnummer()+"' and postext=''";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		sLager=q->FieldByName("Lager")->AsString;
		}
	q->Close();
	delete q;

	return(sLager);
}

//---------------------------------------------------------------------------

void TWerkauftrag::setLagerEin(UnicodeString sLager)
{
	UnicodeString s;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	s="update belegp set lager='"+sLager+"', lagerplatzid=(select id from lagerplatz where lager='"+sLager+"') where belegtyp='W' and belegnummer='"+this->getBelegnummer()+"' and postext=''";
	q->SQL->Text=s;
	q->ExecSQL();

	delete q;
}

//---------------------------------------------------------------------------

UnicodeString TWerkauftrag::getStatus(double &dResOffen, double &dAusOffen, double &dFertigOffen, double &dFertiggestellt)
{
	UnicodeString s, sStatus;

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	s="select b.Belegnummer, b.Datum, bp.termin, b.status Status, bp.status BPStatus, bp.Artikelnummer, bp.Menge, b.skontoprozent1 ResOffen, b.skontoprozent2 AusOffen, b.uebernahmeoffen FertigOffen, bp.verwendet Fertiggestellt from beleg b "
		"left join belegp bp on (bp.belegnummer=b.belegnummer and bp.belegtyp=b.belegtyp) "
		"where b.belegtyp='W' and b.belegnummer='"+this->getBelegnummer()+"' and bp.postext='' ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		dResOffen=q->FieldByName("ResOffen")->AsFloat;
		dAusOffen=q->FieldByName("AusOffen")->AsFloat;
		dFertigOffen=q->FieldByName("FertigOffen")->AsFloat;
		dFertiggestellt=q->FieldByName("Fertiggestellt")->AsFloat;
		sStatus=q->FieldByName("Status")->AsString;
		}
	q->Close();
	delete q;

	return(sStatus);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setReserviert(double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sLagerAus, sLagerEin, sMenge;
	int iReturnCode;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sLagerAus=this->getLagerAus();
	sLagerEin=this->getLagerEin();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @Elager='"+sLagerEin+"' "
		"set @pmenge='"+sMenge+"' "
	*/

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @belegnummer nvarchar(10) "
		"declare @returncode int "
		"declare @belegpkennung uniqueidentifier "
		"declare @belegpkennungprod uniqueidentifier "
		"declare @user nvarchar(2) "
		"declare @standort nvarchar(10) "
		"declare @artikel nvarchar(18) "
		"declare @lager nvarchar(10) "
		"declare @lagerplatzid int  "
		"declare @partikel nvarchar(18) "
		"declare @Elager nvarchar(10) "
		"declare @Elagerplatzid int  "
		"declare @RWKId uniqueidentifier  "
		"declare @RId uniqueidentifier  "
		"declare @EWKId uniqueidentifier "
		"declare @EId uniqueidentifier "
		"declare @pmenge float "
		"declare @menge float "
		"declare @mengelager float  "
		"declare @datum datetime  "
		" "
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @Elager='"+sLagerEin+"' "
		"set @pmenge='"+sMenge+"' "
		" "
		"set @lagerplatzid=(select id from lagerplatz where lager=@lager)  "
		"set @Elagerplatzid=(select id from lagerplatz where lager=@Elager)  "
		"set @belegpkennungprod=(select kennung from belegp where belegtyp='W' and text='1' and belegnummer=@belegnummer) "
		"set @standort=(select standort from beleg where belegtyp='W' and belegnummer=@belegnummer) "
		"set @partikel=(select artikelnummer from belegp where kennung=@belegpkennungprod) "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select kennung, artikelnummer, editmenge from belegp where belegtyp='W' and postext != '' and belegnummer=@belegnummer order by posnummer  "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @artikel, @menge  "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"set @RWKId=NewID()  "
		"set @RId=NewID()  "
		"set @EWKId=NewID()  "
		"set @EId=NewID()  "
		"set @mengelager=@menge*@pmenge  "
		" "
		"/* Reservierung für Teileartikel */ "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@belegpkennung, 1, @RWKId) "
		"  "
		"insert into lagerbedarf (ID, Typ, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm)  "
		"Values(@RId, 1, @datum, @artikel, -@mengelager, @lager, @lagerplatzid, @standort, @user, GetDate())  "
		" "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@RWKId, '1', '1', CONVERT(date, GETDATE()), @datum, @artikel, -@mengelager, 0, '0', "
		"'0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @RId, @user, GetDate()) "
		" "
		"/* später einzulagerndes fertiges teil */ "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@belegpkennungprod, 6, @EWKId) "
		"  "
		"insert into lagerfertigung (ID, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm)  "
		"Values(@EId, @datum, @partikel, @pmenge, @Elager, @Elagerplatzid, @standort, @user, GetDate())  "
		" "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@EWKId, '6', '4', CONVERT(date, GETDATE()), NULL, @partikel, @pmenge, 0, '0', "
		"'0', '', '', 1.0, NULL, @standort, @Elager, @Elagerplatzid, NULL, @EId, @user, GetDate()) "
		" "
		"FETCH NEXT FROM cur INTO @belegpkennung, @artikel, @menge "
		"END "
		"DEALLOCATE cur  "
		" "
		"/* Reservierte Menge im Beleg setzen */ "
		"update beleg set  "
		"  skontoprozent1=skontoprozent1-@pmenge, "
		"  skontoprozent2=skontoprozent2+@pmenge "
		"where belegtyp='W' and belegnummer=@belegnummer "
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode as returncode ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setUnReserviert(double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sMenge;
	int iReturnCode=0;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @Elager='"+sLagerEin+"' "
		"set @pmenge='"+sMenge+"' "
	*/

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @belegnummer nvarchar(10) "
		"declare @returncode int "
		"declare @belegpkennung uniqueidentifier "
		"declare @user nvarchar(2) "
		"declare @standort nvarchar(10) "
		"declare @artikel nvarchar(18) "
		"declare @lager nvarchar(10) "
		"declare @lagerplatzid int  "
		"declare @pmenge float "
		"declare @menge float "
		"declare @datum datetime  "
		"declare @tabid uniqueidentifier "
		" "
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select BELEGPKennung, Datum, Artikelnummer, Menge, Standort, Lager, LagerplatzId, TabID from lagerprotokoll  "
		"  where belegpkennung in (select lagerkennung from WERKAUFTRAGDETAILS where typ='1' and belegpkennung in  "
		"  (select kennung from belegp where belegtyp='W' and postext != '' and belegnummer=@belegnummer)) "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid  "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"/* Reservierung 0en */ "
		"update lagerbedarf set menge='0' where id=@tabid "
		" "
		"/* Werkauftragdetailseintrag löschen */ "
		"delete from WERKAUFTRAGDETAILS where lagerkennung=@belegpkennung "
		" "
		"/* Lagerprotokolleintrag dafür erstellen */ "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '1', '1', CONVERT(date, GETDATE()), @datum, @artikel, -@menge, 0, '0', "
		"'0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid  "
		"END "
		"DEALLOCATE cur  "
		" "
		"/* später einzulagerndes fertiges teil */ "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select BELEGPKennung, Datum, Artikelnummer, Menge, Standort, Lager, LagerplatzId, TabID from lagerprotokoll  "
		"  where belegpkennung in (select lagerkennung from WERKAUFTRAGDETAILS where typ='6' and belegpkennung in  "
		"  (select kennung from belegp where belegtyp='W' and postext='' and belegnummer=@belegnummer)) "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid  "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"/* lagerfertigungseintrag 0en */ "
		"update lagerfertigung set menge='0' where id=@tabid "
		" "
		"/* Werkauftragdetailseintrag löschen */ "
		"delete from WERKAUFTRAGDETAILS where lagerkennung=@belegpkennung "
		" "
		"/* Lagerprotokolleintrag dafür erstellen */ "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '6', '4', CONVERT(date, GETDATE()), NULL, @artikel, -@menge, 0, '0', "
		"'0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid  "
		"END "
		"DEALLOCATE cur  "
		" "
		"/* Reservierte Menge im Beleg rücksetzen */ "
		"update beleg set  "
		"  skontoprozent1=@pmenge, "
		"  skontoprozent2=0 "
		"where belegtyp='W' and belegnummer=@belegnummer "
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode as returncode ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setUnReserviertPos(UnicodeString sKennung, double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sMenge;
	int iReturnCode=0;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @Elager='"+sLagerEin+"' "
		"set @pmenge='"+sMenge+"' "
	*/

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @belegnummer nvarchar(10) "
		"declare @returncode int "
		"declare @belegpkennung uniqueidentifier "
		"declare @user nvarchar(2) "
		"declare @standort nvarchar(10) "
		"declare @artikel nvarchar(18) "
		"declare @lager nvarchar(10) "
		"declare @lagerplatzid int  "
		"declare @pmenge float "
		"declare @menge float "
		"declare @offen float "
		"declare @datum datetime  "
		"declare @tabid uniqueidentifier "
		" "
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select BELEGPKennung, Datum, Artikelnummer, Menge, Standort, Lager, LagerplatzId, TabID from lagerprotokoll  "
		"  where belegpkennung in (select lagerkennung from WERKAUFTRAGDETAILS where typ='1' and belegpkennung='"+sKennung+"') and menge < 0 "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid  "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"/* Reservierung 0en */ "
		"set @offen=(select -menge from lagerbedarf where id=@tabid) "
		"if(@pmenge > @offen) "
		"  set @pmenge=@offen "
		"update lagerbedarf set menge=menge+@pmenge where id=@tabid "
		"set @offen=(select menge from lagerbedarf where id=@tabid) "
		"/* Werkauftragdetailseintrag löschen */ "
		"if(@offen = 0) "
		"	 delete from WERKAUFTRAGDETAILS where lagerkennung=@belegpkennung "
		" "
		"/* Lagerprotokolleintrag dafür erstellen */ "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '1', '1', CONVERT(date, GETDATE()), @datum, @artikel, @pmenge, 0, '0', "
		"'0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid  "
		"END "
		"DEALLOCATE cur  "
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode as returncode ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}


//---------------------------------------------------------------------------

bool TWerkauftrag::setAusgelagert(double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sLagerAus, sLagerEin, sMenge;
	int iReturnCode=0;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sLagerAus=this->getLagerAus();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
	*/

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @belegnummer nvarchar(10) "
		"declare @returncode int "
		"declare @serieId int "
		"declare @scTyp nvarchar(1) "
		"declare @serieCharge nvarchar(40) "
		"declare @belegpkennung uniqueidentifier "
		"declare @user nvarchar(2) "
		"declare @standort nvarchar(10) "
		"declare @artikel nvarchar(18) "
		"declare @lager nvarchar(10) "
		"declare @lagerplatzid int  "
		"declare @pmenge float "
		"declare @menge float "
		"declare @datum datetime  "
		"declare @tabid uniqueidentifier "
		"declare @abgang float "
		"declare @bestand float "
		"declare @kennung uniqueidentifier "
		"declare @lid int "
		"declare @id uniqueidentifier "
		"declare @wert float "
		" "
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
		" "
		"set @wert=0 "
		"set @serieId='0' "
		"set @scTyp='' "
		"set @serieCharge='' "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select lp.BELEGPKennung, lp.Datum, lp.Artikelnummer, lp.Menge, lp.Standort, bp.Lager, (select id from lagerplatz where lager=bp.lager) as LagerplatzId, lp.TabID, bp.Kennung from lagerprotokoll lp  "
		"  left join WERKAUFTRAGDETAILS wd on (wd.LagerKennung=lp.BELEGPKennung) "
		"  left join belegp bp on (bp.kennung=wd.BELEGPKennung) "
		"where bp.belegtyp='W' and bp.belegnummer=@belegnummer and bp.postext != '' and wd.typ='1'  "
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid, @kennung "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"/* Reservierung 0en */ "
		"update lagerbedarf set menge='0' where id=@tabid "
		" "
		"/* Werkauftragdetailseintrag löschen */ "
		"delete from WERKAUFTRAGDETAILS where lagerkennung=@belegpkennung "
		" "
		"/* Lagerprotokolleintrag dafür erstellen */ "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"  SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '1', '1', CONVERT(date, GETDATE()), @datum, @artikel, -@menge, 0, '0', "
		"  '0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"/* Eintrag für Auslagerung erstellen */ "
		"set @belegpkennung=NEWID() "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@kennung, 3, @belegpkennung) "
		" "
		"set @abgang=@menge*-1 "
		"/* Lagerbestandsabgang erstellen */ "
		"DECLARE cur2 CURSOR SCROLL FOR  "
		"select lagerbestand_id, Bestand, Id, Wert from lagerbestand where artikelnummer=@artikel and lagerplatzid=@lagerplatzId and serieid=@serieId and bestand > 0 order by datum  "
		"  "
		"OPEN cur2  "
		"FETCH NEXT FROM cur2 INTO @lid, @bestand, @TabId, @Wert  "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		"  if(@bestand > @abgang)  "
		"  begin  "
		"    update lagerbestand set bestand=bestand-@abgang where lagerbestand_id=@lid  "
		"    set @menge=@abgang*-1  "
		"    set @abgang=0  "
		"  end  "
		"  else  "
		"  begin  "
		"    update lagerbestand set bestand=0 where lagerbestand_id=@lid  "
		"    set @menge=@bestand*-1  "
		"    set @abgang=@abgang-@bestand  "
		"  end  "
		"  /* Protokolleintrag einfügen */ "
		"  insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"    SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"  Values(@belegpkennung, '3', '2', CONVERT(date, GETDATE()), @datum, @artikel, '0', @menge, @wert, "
		"@serieId, @scTyp, @serieCharge, 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @TabId, @user, GetDate())  "
		"  "
		"  if(@abgang <= 0)  "
		"    break  "
		"  FETCH NEXT FROM cur2 INTO @lid, @bestand, @TabId, @Wert  "
		"END  "
		"DEALLOCATE cur2  "
		"  "
		"if(@abgang != 0)  "
		"BEGIN  "
		"  /* Lagerbestand */ "
		//"  declare @lagerbestand_id int  "
		//"  set @lagerbestand_id=(select lagerbestand_id from lagerbestand where id=(select top 1 tabid from lagerprotokoll where belegpkennung=@belegpKennung))  "
		//"  if(@lagerbestand_id is not null) "
		//"  begin  "
		//"update lagerbestand set bestand=bestand-@abgang where lagerbestand_id=@lagerbestand_id  "
		//"set @Id=(select id from lagerbestand where lagerbestand_id=@lagerbestand_id)  "
		//"  end  "
		//"  else begin  "
		"    set @id=NEWID() "
		"insert into lagerbestand (ID, Datum, Artikelnummer, Bestand, Wert, SerieId, Preismenge, Verfallsdatum, LagerplatzId, BelegpKennung, AngelegtVon, AngelegtAm)  "
		"Values(@Id, @datum, @artikel, @abgang*-1, @wert, @serieId, '1', NULL, @lagerplatzid, NULL, @user, GetDate())  "
		//"  end  "
		"  /* Protokolleintrag einfügen */ "
		"  insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"  Values(@belegpkennung, '3', '2', CONVERT(date, GETDATE()), @datum, @artikel, '0', @abgang*-1, @Wert, "
		"@serieId, @scTyp, @serieCharge, 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @Id, @user, GetDate())  "
		"END "
		" "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid, @kennung  "
		"END "
		"DEALLOCATE cur  "
		" "
		"/* AuslagerungOffen umd Menge vermindern */ "
		"update beleg set  "
		"  skontoprozent2=skontoprozent2-@pmenge, UebernahmeOffen=UebernahmeOffen+-@menge, status='1' "
		"where belegtyp='W' and belegnummer=@belegnummer "
		"/* "
		"-- Fertiggestellt umd Menge erhoehen "
		"--update belegp set  "
		"--  verwendet=verwendet+@pmenge, status='2' "
		"--where belegtyp='W' and belegnummer=@belegnummer "
		"*/ "
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode as returncode ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setUnAusgelagert(double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sLagerAus, sLagerEin, sMenge;
	int iReturnCode=0;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sLagerAus=this->getLagerAus();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
			*/

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @belegnummer nvarchar(10) "
		"declare @returncode int "
		"declare @serieId int "
		"declare @scTyp nvarchar(1) "
		"declare @serieCharge nvarchar(40) "
		"declare @belegpkennung uniqueidentifier "
		"declare @user nvarchar(2) "
		"declare @standort nvarchar(10) "
		"declare @artikel nvarchar(18) "
		"declare @lager nvarchar(10) "
		"declare @lagerplatzid int  "
		"declare @pmenge float "
		"declare @menge float "
		"declare @datum datetime  "
		"declare @tabid uniqueidentifier "
		"declare @abgang float "
		"declare @bestand float "
		"declare @kennung uniqueidentifier "
		"declare @lid int "
		"declare @id uniqueidentifier "
		"declare @wert float "
		" "
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
		" "
		"set @wert=0 "
		"set @serieId='0' "
		"set @scTyp='' "
		"set @serieCharge='' "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select lp.BELEGPKennung, lp.Datum, lp.Artikelnummer, lp.Bestand, lp.Standort, lp.Lager, lp.LagerplatzId, lp.TabID, bp.Kennung from lagerprotokoll lp  "
		"  left join WERKAUFTRAGDETAILS wd on (wd.LagerKennung=lp.BELEGPKennung) "
		"  left join belegp bp on (bp.kennung=wd.BELEGPKennung) "
		"where bp.belegtyp='W' and bp.belegnummer=@belegnummer and bp.postext != '' and wd.typ='3'  "
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid, @kennung "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"/* Lagerabgang 0en */ "
		"update lagerbestand set bestand='0' where id=@tabid "
		" "
		"/* Werkauftragdetailseintrag löschen */ "
		"delete from WERKAUFTRAGDETAILS where lagerkennung=@belegpkennung "
		" "
		"/* Lagerprotokolleintrag dafür erstellen */ "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"  SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '3', '2', CONVERT(date, GETDATE()), @datum, @artikel, 0, -@menge, '0', "
		"  '0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"set @belegpkennung=NEWID() "
		"set @tabid=NEWID() "
		" "
		"/* Reservierung für Teileartikel */ "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@kennung, 1, @belegpkennung) "
		"  "
		"insert into lagerbedarf (ID, Typ, Datum, Artikelnummer, Menge, Lager, LagerplatzId, Standort, AngelegtVon, AngelegtAm)  "
		"Values(@tabid, 1, @datum, @artikel, @menge, @lager, @lagerplatzid, @standort, @user, GetDate())  "
		" "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"  SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '1', '1', CONVERT(date, GETDATE()), @datum, @artikel, @menge, 0, '0', "
		"  '0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"        FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid, @kennung  "
		"END "
		"DEALLOCATE cur  "
		" "
		"/* AuslagerungOffen umd Menge vermindern */ "
		"update beleg set  "
		"  skontoprozent2=skontoprozent2+@pmenge, UebernahmeOffen=UebernahmeOffen-@pmenge, status='1' "
		"where belegtyp='W' and belegnummer=@belegnummer "
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode returncode ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setEingelagert(double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sLagerEin, sMenge;
	int iReturnCode=0;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sLagerEin=this->getLagerEin();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @Elager='"+sLagerAus+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
	*/

	s="BEGIN TRANSACTION; "
		"BEGIN TRY  "
		"declare @belegnummer nvarchar(10) "
		"declare @returncode int "
		"declare @serieId int "
		"declare @scTyp nvarchar(1) "
		"declare @serieCharge nvarchar(40) "
		"declare @belegpkennung uniqueidentifier "
		"declare @user nvarchar(2) "
		"declare @standort nvarchar(10) "
		"declare @artikel nvarchar(18) "
		"declare @lager nvarchar(10) "
		"declare @Elager nvarchar(10) "
		"declare @lagerplatzid int  "
		"declare @pmenge float "
		"declare @menge float "
		"declare @datum datetime  "
		"declare @tabid uniqueidentifier "
		"declare @zugang float "
		"declare @bestand float "
		"declare @kennung uniqueidentifier "
		"declare @lid int "
		"declare @id uniqueidentifier "
		"declare @wert float "
		" "
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @Elager='"+sLagerEin+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
		" "
		"set @wert=0 "
		"set @serieId='0' "
		"set @scTyp='' "
		"set @serieCharge='' "
		" "
		"DECLARE cur CURSOR SCROLL FOR  "
		"select top 1 lp.BELEGPKennung, lp.Datum, lp.Artikelnummer, lp.Menge, lp.Standort, bp.Lager, bp.LagerplatzId, lp.TabID, bp.Kennung from lagerprotokoll lp  "
		"  left join WERKAUFTRAGDETAILS wd on (wd.LagerKennung=lp.BELEGPKennung) "
		"  left join belegp bp on (bp.kennung=wd.BELEGPKennung) "
		"where bp.belegtyp='W' and bp.belegnummer=@belegnummer and bp.postext='' "
		" "
		"OPEN cur "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid, @kennung "
		"WHILE @@FETCH_STATUS = 0  "
		"BEGIN  "
		" "
		"/* Reservierung 0en */ "
		"update lagerfertigung set menge='0' where id=@tabid "
		" "
		//"/* Werkauftragdetailseintrag löschen */ "
		//"delete from WERKAUFTRAGDETAILS where lagerkennung=@belegpkennung "
		//" "
		"/* Lagerprotokolleintrag dafür erstellen */ "
		"insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"  SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"Values(@belegpkennung, '6', '4', CONVERT(date, GETDATE()), NULL, @artikel, -@pmenge, 0, '0', "
		"  '0', '', '', 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @tabid, @user, GetDate()) "
		" "
		"/* Eintrag für Einlagerung erstellen */ "
		"set @belegpkennung=NEWID() "
		"insert into werkauftragdetails (BELEGPKennung, Typ, LagerKennung) "
		"Values(@kennung, 4, @belegpkennung) "
		" "
		"set @zugang=@pmenge "
		" "
		"set @Id=NEWID() "
		"set @lager=@ELager "
		"set @lagerplatzid=(select Id from LAGERPLATZ where Lager=@ELager) "
		"insert into lagerbestand (ID, Datum, Artikelnummer, Bestand, Wert, SerieId, Preismenge, Verfallsdatum, LagerplatzId, BelegpKennung, AngelegtVon, AngelegtAm)  "
		"Values(@Id, @datum, @artikel, @zugang, @wert, @serieId, '1', NULL, @lagerplatzid, NULL, @user, GetDate())  "
		" "
		"  /* Protokolleintrag einfügen */ "
		"  insert into lagerprotokoll (BELEGPKennung, Typ, Tabelle, Datum, Bestandsdatum, Artikelnummer, Menge, Bestand, Wert,  "
		"SerieId, SCTyp, SerieCharge, Preismenge, Verfallsdatum, Standort, Lager, LagerplatzId, TabBELEGPKennung, TabID, AngelegtVon, AngelegtAm)  "
		"  Values(@belegpkennung, '4', '2', CONVERT(date, GETDATE()), @datum, @artikel, '0', @zugang, @Wert, "
		"@serieId, @scTyp, @serieCharge, 1.0, NULL, @standort, @lager, @lagerplatzid, NULL, @Id, @user, GetDate())  "
		" "
		"FETCH NEXT FROM cur INTO @belegpkennung, @datum, @artikel, @menge, @standort, @lager, @lagerplatzid, @tabid, @kennung  "
		"END "
		"DEALLOCATE cur  "
		" "
		"/* OffenFertig um Menge reduzieren */ "
		"update beleg set  "
		"  UebernahmeOffen=UebernahmeOffen-@pmenge "
		"where belegtyp='W' and belegnummer=@belegnummer "
		" "
		"/* Fertiggestellt umd Menge erhoehen */ "
		"update belegp set  "
		"  verwendet=verwendet+@pmenge "
		"where belegtyp='W' and belegnummer=@belegnummer and postext='' "
		" "
		"declare @offen float "
		"set @offen=(select menge-verwendet from belegp	where belegtyp='W' and belegnummer=@belegnummer and postext='') "
		" "
		"/* wenn alles erledigt, dann Status updaten */ "
		"if(@offen <= 0) "
		"begin "
		"  update belegp set status='2' where belegtyp='W' and belegnummer=@belegnummer and postext='' "
		"  update beleg  set status='2', freiesdatum1=GetDate() where belegtyp='W' and belegnummer=@belegnummer "
		"end"
		" "
		"END TRY "
		"BEGIN CATCH  "
		"IF @@TRANCOUNT > 0  "
		"ROLLBACK TRANSACTION;  "
		"set @returncode='0'  "
		"END CATCH;  "
		"IF @@TRANCOUNT > 0  "
		"BEGIN  "
		"  COMMIT TRANSACTION;  "
		"  set @returncode='1'  "
		"END  "
		"select @returncode as returncode ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}

//---------------------------------------------------------------------------

bool TWerkauftrag::setUnEingelagert(double dMenge)
{
	UnicodeString s, sErrorMSG;
	UnicodeString sBelegnummer, sLagerEin, sMenge;
	int iReturnCode=0;
	TDateTime td;

	td=Now();

	sBelegnummer=this->getBelegnummer();
	sLagerEin=this->getLagerEin();
	sMenge=AnsiReplaceStr(FloatToStr(dMenge), ",", ".");

	TADOQuery *q=new TADOQuery(NULL);
	q->Connection=this->getCon();

	/*
		"set @user='"+this->getUser()+"' "
		"set @datum='"+td.FormatString("dd.mm.yyyy")+"' "
		"set @belegnummer='"+sBelegnummer+"' "
		"set @lager='"+sLagerAus+"' "
		"set @pmenge='"+sMenge+"' "
		//"set @pmenge=(select menge from belegp where belegtyp='W' and posnummer='1' and belegnummer=@belegnummer) "
	*/

	s=" ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		iReturnCode=q->FieldByName("returncode")->AsInteger;
		sErrorMSG="";
		}
	q->Close();
	delete q;

	return(iReturnCode);
}

