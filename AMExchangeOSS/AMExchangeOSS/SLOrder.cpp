//---------------------------------------------------------------------------

#pragma hdrstop

#include "Unit1.h"
#include "c:\pk-ware\packages\10.4\pslwawi20\XFakt_OCX.h"
#include "Wawi.h"

#include <inifiles.hpp>

//---------------------------------------------------------------------------
#pragma package(smart_init)

static bool bInit=false;

IOleBeleg *beleg;
TApp *App1=NULL;

TBeleg *wawiBeleg=NULL;

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::startCOM(void)
{
	AnsiString s;

	if(!bInit)
		{
		bInit=true;

    App1=new TApp(this);

		Variant v;
		bool st;

		log("Start COM-Server...");

		App1->Connect();
		App1->InitServerData();

		while(1)
			{
			v=App1->ApplicationReady();
			st=v;

			if(st)
				break;

			Application->ProcessMessages();
			}

		v=App1->Mandant();
		s=_strMandant.SubString(5, 20);
		if(v != s)
			{
			log("Mandantenwechsel:"+s+" -> "+v);
			App1->GetMandant(WideString(s).c_bstr(), 0);
			}

		v=App1->Mandant();
		s="Mandant:" + v;
		log(s);

		v=App1->Benutzer();
		s="User:" + v;
		log(s);
		}
	else
		{
		log("COM-Server bereits gestartet!");
		}

	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::stopCOM(void)
{
	AnsiString s;

	if(bInit)
		{
		App1->Disconnect();
		delete App1;
		bInit=false;
		}
	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::createSLOrder(void)
{
	AnsiString s, strBelegtyp, strBeleg, strKontengruppe, strLand, strIhrZeichen;
	AnsiString strKunde, strZB, strLB, strPreisgruppe, strPreistyp;
	AnsiString strGastKunde, strGastKundeAU, strGastKundeEU;
	AnsiString strNeukundenBelegtyp, strSteuerQuelle;
	bool bAdresstausch=false;
	Variant v;

	TSLAdresse *adr=new TSLAdresse;
	TSLAdresse *adrLief=new TSLAdresse;
	TSLKundenDaten *kuda=new TSLKundenDaten;

	if(_bUseCOM)
		startCOM();

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;
	TADOQuery *q2=new TADOQuery(this);
	q2->Connection=ADOConnection1;

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );

	s=ini->ReadString("Order", "Belegtyp", strBelegtyp);
	if(s != "")
		strBelegtyp=s;

	s=ini->ReadString("Order", "NeukundenBelegtyp", strNeukundenBelegtyp);
	if(s != "")
		strBelegtyp=s;

	s=ini->ReadString("Order", "GastKunde", strGastKunde);
	if(s != "")
		strGastKunde=s;
	s=ini->ReadString("Order", "GastKundeAU", strGastKundeAU);
	if(s != "")
		strGastKundeAU=s;
	s=ini->ReadString("Order", "GastKundeEU", strGastKundeEU);
	if(s != "")
		strGastKundeEU=s;

	strSteuerQuelle=ini->ReadString("Order", "SteuerQuelle", "SL");

	if(strGastKundeEU == "")
		strGastKundeEU=strGastKunde;
	if(strGastKundeAU == "")
		strGastKundeAU=strGastKunde;

	s=ini->ReadString("Order", "Preisgruppe", "");
	if(s != "")
		strPreisgruppe=s;

	strZB=ini->ReadString("Zahlung", shBData.sZB, "");

	strLB=ini->ReadString("Lieferung", shBData.sLB, "");

	delete ini;

	//strIhrZeichen=ini->ReadString("Order", "IhrZeichen", "");
	strIhrZeichen=shBData.sIhrZeichen;

	adrSLBill.sZB=strZB;
	adrSLBill.sLB=strLB;

	// prüfen ob Beleg schon existiert
	s="select * from beleg where belegtyp='"+strBelegtyp+"' and IhrAuftrag='"+shBData.sOrderNumber+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		strBeleg=q->FieldByName("Belegnummer")->AsString;
		log("Order:"+shBData.sOrderNumber+" existiert bereits mit Beleg:"+strBeleg);
		q->Close();
		delete q;
		return(false);
		}
	q->Close();

	if(_bUseCOM)
		{
		beleg=App1->NeuerBelegAuto(WideString(strBelegtyp).c_bstr(), WideString("").c_bstr());
		v=beleg->GetField(WideString("Belegnummer").c_bstr());
		strBeleg=v;
		beleg->SetIhrAuftrag(WideString(shBData.sOrderNumber).c_bstr());
		}
	else
		{
		wawiBeleg=new TBeleg(ADOConnection1);
		strBeleg=wawiBeleg->create(strBelegtyp, "");
		wawiBeleg->setIhrauftrag(shBData.sOrderNumber);

		adr->sAnrede=adrSLBill.sAnrede;
		adr->sVorname=adrSLBill.sVorname;
		adr->sName=adrSLBill.sName;
		adr->sFirma=adrSLBill.sFirma;
		adr->sZusatz=adrSLBill.sZusatz;
		adr->sZusatz2=adrSLBill.sZusatz2;
		adr->sZusatz3=adrSLBill.sZusatz3;
		adr->sStrasse=adrSLBill.sStrasse;
		adr->sOrt=adrSLBill.sOrt;
		adr->sPLZ=adrSLBill.sPLZ;
		adr->sLand=adrSLBill.sLand;
		adr->sTelefon1=adrSLBill.sTelefon;
		adr->sEMail=adrSLBill.sEMail;
		adr->sSteueridentNr=adrSLBill.sUstId;

		adrLief->sAnrede=adrSLDel.sAnrede;
		adrLief->sVorname=adrSLDel.sVorname;
		adrLief->sName=adrSLDel.sName;
		adrLief->sFirma=adrSLDel.sFirma;
		adrLief->sZusatz=adrSLDel.sZusatz;
		adrLief->sZusatz2=adrSLDel.sZusatz2;
		adrLief->sZusatz3=adrSLDel.sZusatz3;
		adrLief->sStrasse=adrSLDel.sStrasse;
		adrLief->sOrt=adrSLDel.sOrt;
		adrLief->sPLZ=adrSLDel.sPLZ;
		adrLief->sLand=adrSLDel.sLand;
		adrLief->sTelefon1=adrSLDel.sTelefon;
		adrLief->sEMail=adrSLDel.sEMail;
		adrLief->sSteueridentNr=adrSLDel.sUstId;
		}
	shBData.strBelegnummer=strBeleg;
	shBData.strBelegtyp=strBelegtyp;
	log("Order:"+shBData.sOrderNumber+" mit Beleg:"+strBeleg+" angelegt!");

	if(shBData.sKundennummer == "guest")
		{
		AnsiString strBelegKunde, strIsEU;

		if(adrSLBill.sLand == "D" || adrSLBill.sLand == "DE" || adrSLBill.sUstId == "")   // Deutsch oder keine UstId
			strBelegKunde=strGastKunde;
		else
			{
			s="select * from sl_daten.dbo.intra where kuerzel='"+adrSLBill.sLand+"'";
			q2->SQL->Text=s;
			q2->Open();
			if(q2->RecordCount)
				strIsEU=q2->FieldByName("IsEU")->AsString;
			else
				strIsEU="N";
			q2->Close();
			if(strIsEU.UpperCase() == "J")
				strBelegKunde=strGastKundeEU;
			else
				strBelegKunde=strGastKundeAU;
			}

		log("GastBestellung mit Vorlage:"+strBelegKunde);

		if(_bUseCOM)
			{
			beleg->SetAdresse(WideString(strBelegKunde).c_bstr());

			v=adrSLBill.sAnrede;
			beleg->PutField(WideString("Anrede").c_bstr(), v);
			v=adrSLBill.sVorname.SubString(1, 80);
			beleg->PutField(WideString("VorName").c_bstr(), v);
			v=adrSLBill.sName.SubString(1, 80);
			beleg->PutField(WideString("Name").c_bstr(), v);
			v=adrSLBill.sFirma.SubString(1, 80);
			beleg->PutField(WideString("Firma").c_bstr(), v);
			//v=adrSLBill.sStrasse+" "+adrSLBill.sHausnummer;
			v=adrSLBill.sStrasse;
			beleg->PutField(WideString("Strasse").c_bstr(), v);
			v=adrSLBill.sPLZ;
			beleg->PutField(WideString("PLZ").c_bstr(), v);
			v=adrSLBill.sOrt;
			beleg->PutField(WideString("Ort").c_bstr(), v);
			v=adrSLBill.sLand;
			beleg->PutField(WideString("Land").c_bstr(), v);
			v=adrSLBill.sZusatz.SubString(1, 80);
			beleg->PutField(WideString("Zusatz").c_bstr(), v);
			v=adrSLBill.sZusatz2.SubString(1, 80);
			beleg->PutField(WideString("Zusatz2").c_bstr(), v);
			v=adrSLBill.sUstId;
			beleg->PutField(WideString("UstId").c_bstr(), v);
			}
		else
			{
			wawiBeleg->setAdresse(strBelegKunde);
			wawiBeleg->setBelegadresse(adr);
			if(adrSLBill.sUstId != "")
				{
				wawiBeleg->setUSTId(adrSLBill.sUstId);
				log("UstId:"+adrSLBill.sUstId);
				}
			}
		}
	else
		{
		if(shBData.sKundennummer == "" || AnsiContainsStr(shBData.sKundennummer, "new"))
			{
			strKunde=createKu(&adrSLBill, false);
			if(strKunde == "")
				{
				log("Fehler - Kunde mit Adresse: "+adrSLBill.sFirma+" "+adrSLBill.sName+" "+adrSLBill.sVorname+" konnte nicht angelegt werden!");
				}
			}
		else
			{
			strKunde=shBData.sKundennummer;
			}

		if(_bUseCOM)
			beleg->SetAdresse(WideString(strKunde).c_bstr());
		else
			wawiBeleg->setAdresse(strKunde);
		}

	if(_bUseCOM)
		{
		if(strIhrZeichen == "")
			v="";
		else
			v=strIhrZeichen;
		beleg->PutField(WideString("IhrZeichen").c_bstr(), v);
		}
	else
		wawiBeleg->setIhrzeichen(strIhrZeichen);

	// Orderdate einlesen/normieren
	TDateTime td;
	if(shBData.strOderDateTime != "")
		{
		if(TryStrToDateTime(shBData.strOderDateTime, td))
			{
			if(_bUseCOM)
				beleg->SetIhrAuftragVom(td.Val);
			else
				wawiBeleg->setIhrauftragvom(td);
			}
		}

	if(shBData.sKommentar != "")
		{
		if(_bUseCOM)
			beleg->AddKopftextLine(WideString(shBData.sKommentar).c_bstr());
		else
			wawiBeleg->setKopftext(shBData.sKommentar);
		}

	// Wenn eine ZB aus der INI-Datei gezogen wurde dann diese verwenden
	if(strZB != "")
		{
		if(_bUseCOM)
			beleg->SetZahlungsbedingung(WideString(strZB).c_bstr());
		else
			wawiBeleg->setZahlungsbedingung(strZB);
		}

	// Wenn eine LB aus der INI-Datei gezogen wurde dann diese verwenden
	if(strLB != "")
		{
		if(_bUseCOM)
			beleg->SetLieferbedingung(WideString(strLB).c_bstr());
		else
			wawiBeleg->setLieferbedingung(strLB);
		}

	// Liefertermin
	if(shBData.strLiefertermin != "")
		{
		if(TryStrToDateTime(shBData.strLiefertermin, td))
			{
			if(_bUseCOM)
				{
				v=td.Val;
				beleg->PutField(WideString("_Liefertermin").c_bstr(), v);
				}
			else
				wawiBeleg->setLiefertermin(td);
			}
		}

	// Preisgruppe evtl. durch INI-Einstellung überschreiben
	if(_bUseCOM)
		{
		v=beleg->GetField(WideString("Preisgruppe").c_bstr());
		if(strPreisgruppe != "" && v != strPreisgruppe)
			beleg->SetPreisgruppe(WideString(strPreisgruppe).c_bstr());
		}
	else
		{
		s=wawiBeleg->getPreisgruppe();
		if(strPreisgruppe != "" && s != strPreisgruppe)
			wawiBeleg->setPreisgruppe(strPreisgruppe.ToInt());
		}

	// Preistyp prüfen
	if(strSteuerQuelle != "SL")  // nur wenn die Steuerquelle nicht SL ist wird aus den Amazondaten die Steuer abgeleitet
		{
		if(shBData.dSteuerGesamtBetrag == 0.0)  // Wenn keine Steuer dabei ist, dann steuerfrei
			strPreistyp="S";
		}
	if(strPreistyp != "")
		{
		if(_bUseCOM)
			beleg->SetPreistyp(WideString(strPreistyp).c_bstr());
		else
			wawiBeleg->setPreistyp(strPreistyp);
		}

	// Währung checken
	if(shBData.sWaehrung != "")
		{
		if(_bUseCOM)
			beleg->SetWaehrung(WideString(shBData.sWaehrung).c_bstr());
		else
			wawiBeleg->setWaehrungscode(shBData.sWaehrung);
		}

	// Prüfen ob abweichender Lieferempfänger angegeben ist (Unterschiede in den Adressen)
	bool bLieferAbweichend=false;

	if(adrSLBill.sFirma != adrSLDel.sFirma ||
		 adrSLBill.sVorname != adrSLDel.sVorname ||
		 adrSLBill.sName != adrSLDel.sName ||
		 adrSLBill.sStrasse != adrSLDel.sStrasse ||
		 adrSLBill.sOrt != adrSLDel.sOrt ||
		 adrSLBill.sZusatz != adrSLDel.sZusatz  ||
		 adrSLBill.sZusatz2 != adrSLDel.sZusatz2)
		bLieferAbweichend=true;

	if(bLieferAbweichend)
		{
		// Kontengruppe checken
		log("Abweichende Lieferanschrift vorhanden!");
		log("Kontengruppe prüfen!");
		if(_bUseCOM)
			{
			v=beleg->GetField(WideString("Kontengruppe").c_bstr());
			strKontengruppe=v;
			}
		else
			strKontengruppe=wawiBeleg->getKontengruppe();

		ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
		if(adrSLBill.sUstId == "")
			s=ini->ReadString("KontengruppeOSS", adrSLDel.sLand, "");
		else
			s=ini->ReadString("Kontengruppe", adrSLDel.sLand, "");
		if(s != "")
			{
			if(s != strKontengruppe)
				{
				if(_bUseCOM)
					{
					v=s;
					beleg->PutField(WideString("Kontengruppe").c_bstr(), v);
					}
				else
					wawiBeleg->setKontengruppe(s);
				log("Kontengruppe:"+s+" von Land für Lieferempfänger eingetragen!");
				}
			}
		delete ini;

		if(!bAdresstausch)  // normales Verhalten
			{
			s="insert into adress (adresse, adresstyp, anrede, firma, name, vorname, zusatz, zusatz2, strasse, "
														"land, plz, ort, telefon, email) "
				"Values("
				"isnull((select max(adresse) from adress), 0)+1,"
				"'Q"+strBelegtyp+strBeleg+"',"
				"'"+adrSLDel.sAnrede+"',"
				"'"+getCheckedSQLStr(adrSLDel.sFirma.SubString(1, 80))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sName.SubString(1, 80))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sVorname.SubString(1, 80))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sZusatz.SubString(1, 80))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sZusatz2.SubString(1, 80))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sStrasse.SubString(1, 80))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sLand.SubString(1, 6))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sPLZ.SubString(1, 10))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sOrt.SubString(1, 40))+"',"
				"'"+getCheckedSQLStr(adrSLDel.sTelefon.SubString(1, 30))+"',"
				"'"+adrSLDel.sEMail.SubString(1, 80)+"'"
				")";
			q->SQL->Text=s;
			q->ExecSQL();

			/*
			if(_bLieferAdresseAnlegen && strFixKunde=="")
				{
				// prüfen ob die Lieferadresse nicht schon existiert
				s="select * from adress where adresstyp='KUL"+strKunde+"' and Upper(name)='"+adrSLDel.sName.UpperCase()+"' and Upper(vorname)='"+adrSLDel.sVorname.UpperCase()+"' "
					"and Upper(strasse)='"+adrSLDel.sStrasse.UpperCase()+"' and Upper(ort)='"+adrSLDel.sOrt.UpperCase()+"' and Upper(firma)='"+adrSLDel.sFirma.UpperCase()+"'";
				q->SetSqlText(WideString(s).c_bstr());
				q->OpenSql();
				v=q->RecordCount();
				if(v.intVal == 0)
					{
					q->CloseSql();

					s="insert into adress (adresse, adresstyp, anrede, firma, name, vorname, zusatz, strasse, land, plz, ort, telefon, email) "
					"Values("+IntToStr(++iAdresse)+", 'KUL"+strKunde+"', '"+adrSLDel.sAnrede+"', '"+adrSLDel.sFirma+"', '"+adrSLDel.sName+"', '"+adrSLDel.sVorname+"', '"+adrSLDel.sZusatz+"', "
					"'"+adrSLDel.sStrasse+" "+adrSLDel.sHausnummer.SubString(1, 80)+"', '"+adrSLDel.sLand+"', '"+adrSLDel.sPLZ+"', '"+adrSLDel.sOrt+"', '"+adrSLDel.sTelefon+"','"+adrSLDel.sEMail+"')";
					q->SetSqlText(WideString(s).c_bstr());
					q->ExecuteSql();

					log("Lieferadresse für Kunden angelegt!");
					}
				else
					q->CloseSql();
				}
			*/
			}
		else  // Bei Adresstausch wird die abweichende Lieferadresse in die Belegadresse geschrieben und keine abw. Lieferadresse angelegt
			{
			if(_bUseCOM)
				{
				v=adrSLDel.sAnrede;
				beleg->PutField(WideString("Anrede").c_bstr(), v);
				v=adrSLDel.sVorname.SubString(1, 80);
				beleg->PutField(WideString("VorName").c_bstr(), v);
				v=adrSLDel.sName.SubString(1, 80);
				beleg->PutField(WideString("Name").c_bstr(), v);
				v=adrSLDel.sFirma.SubString(1, 80);
				beleg->PutField(WideString("Firma").c_bstr(), v);
				//v=adrSLDel.sStrasse+" "+adrSLDel.sHausnummer;
				v=adrSLDel.sStrasse;
				beleg->PutField(WideString("Strasse").c_bstr(), v);
				v=adrSLDel.sPLZ;
				beleg->PutField(WideString("PLZ").c_bstr(), v);
				v=adrSLDel.sOrt;
				beleg->PutField(WideString("Ort").c_bstr(), v);
				v=adrSLDel.sLand;
				beleg->PutField(WideString("Land").c_bstr(), v);
				v=adrSLDel.sZusatz.SubString(1, 80);
				beleg->PutField(WideString("Zusatz").c_bstr(), v);
				v=adrSLDel.sZusatz2.SubString(1, 80);
				beleg->PutField(WideString("Zusatz2").c_bstr(), v);
				}
			else
				wawiBeleg->setBelegadresse(adrLief);
			}
		}
	else
		{
		log("Keine abweichende Lieferanschrift vorhanden!");
		log("Kontengruppe prüfen!");
		if(_bUseCOM)
			{
			v=beleg->GetField(WideString("Kontengruppe").c_bstr());
			strKontengruppe=v;
			}
		else
			strKontengruppe=wawiBeleg->getKontengruppe();

		ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
		if(adrSLBill.sUstId == "")
			s=ini->ReadString("KontengruppeOSS", adrSLBill.sLand, "");
		else
			s=ini->ReadString("Kontengruppe", adrSLBill.sLand, "");
		if(s != "")
			{
			if(s != strKontengruppe)
				{
				if(_bUseCOM)
					{
					v=s;
					beleg->PutField(WideString("Kontengruppe").c_bstr(), v);
					}
				else
					wawiBeleg->setKontengruppe(s);
				log("Kontengruppe:"+s+" von Land für Rechnungsempfänger eingetragen!");
				}
			}
		delete ini;
		}

	delete q2;
	delete q;
	delete adr;
	delete kuda;
	delete adrLief;
	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::addSLPos(void)
{
	AnsiString s, strArtikel, strPreistyp, strSteuercode, strSteuerQuelle;
	int iLosgroesse;
	double dSteuerProzent, dMenge;
	Variant v;

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
	strSteuerQuelle=ini->ReadString("Order", "SteuerQuelle", "SL");
	delete ini;

	log("Artikel:"+shPData.sArtikel+" Bez:"+shPData.sBezeichnung+" Anzahl:"+FloatToStr(RoundTo(shPData.dMenge, -2))+" Preis:"+FloatToStr(RoundTo(shPData.dEinzelpreis, -2)));

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	IOlePos *pos;
	TBelegp *belegp;

	strArtikel=shPData.sArtikel;
	dMenge=shPData.dMenge;

	if(_bUseCOM)
		{
		v=beleg->GetField(WideString("Preistyp").c_bstr());
		strPreistyp=v;

		pos=beleg->CreatePos(WideString("A").c_bstr());
		pos->SetArtikel(WideString(strArtikel).c_bstr());
		pos->SetEingabeMenge(WideString(FloatToStr(dMenge)).c_bstr());

		// Prüfen ob durch das Artikeleinfügen eine Beeichnung da ist
		v=pos->get_Bezeichnung();
		s=v;
		}
	else
		{
		strPreistyp=wawiBeleg->getPreistyp();
		belegp=new TBelegp(wawiBeleg);
		belegp->create("A");
		belegp->setArtikel(strArtikel);
		belegp->setMenge(dMenge);
		s=belegp->getBezeichnung();
		}
	if(s == "") // Artikel nicht gefunden!
		{
		s=AnsiReplaceStr(shPData.sBezeichnung, "&quot;", "\"");
		if(_bUseCOM)
			pos->SetBezeichnung(WideString(s).c_bstr());
		else
			belegp->setBezeichnung(s);
		}

	double dEP=RoundTo(shPData.dEinzelpreis, -2);
	if(_bUseCOM)
		{
		pos->SetEinzelPreis(RoundTo(dEP, -2));
		dSteuerProzent=pos->get_SteuerProzent();
		}
	else
		{
		belegp->setEinzelpreis(RoundTo(dEP, -2));
		dSteuerProzent=belegp->getSteuerprozent();
		}

	if(dSteuerProzent != shPData.dSteuerprozent && strSteuerQuelle != "SL")
		{                                                 // Steuer aus Import ermitteln
		if(shPData.dSteuerprozent == 0.0)
			{
			if(_bUseCOM)
				{
				pos->SetSteuerCode(WideString("").c_bstr());
				pos->SetSteuerProzent(0.0);
				}
			else
				{
				belegp->setSteuercode("");
				belegp->setSteuerprozent(0.0);
				}
			}
		else if(shPData.dSteuerprozent < 15.0)
			{
			if(_bUseCOM)
				{
				pos->SetSteuerCode(WideString("2").c_bstr());
				pos->SetSteuerProzent(shPData.dSteuerprozent);
				}
			else
				{
				belegp->setSteuercode("2");
				belegp->setSteuerprozent(shPData.dSteuerprozent);
				}
			}
		else
			{
			if(_bUseCOM)
				{
				pos->SetSteuerCode(WideString("3").c_bstr());
				pos->SetSteuerProzent(shPData.dSteuerprozent);
				}
			else
				{
				belegp->setSteuercode("3");
				belegp->setSteuerprozent(shPData.dSteuerprozent);
				}
			}
		}

	if(shPData.sMessage != "")
		{
		if(_bUseCOM)
			pos->AddKommentarLine(WideString(shPData.sMessage).c_bstr());
		else
			belegp->setLangtext(shPData.sMessage);
		}

	if(_bUseCOM)
		{
		pos->Save(1);
		v=pos->GetKennung();
		shPData.strKennung=v;
		pos->Release();
		}
	else
		{
		try
			{
			belegp->save();
			}
		catch(Exception &e)
			{
			s="Exception:"+e.Message;
			log("Exception:"+e.Message);
			Application->MessageBoxW(WideString(s).c_bstr(), L"Fehler", MB_OK);
      }
		shPData.strKennung=belegp->getKennung();
		delete belegp;
		}

	delete q;
	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::addSLVersand(AnsiString strLB, double dVersandNetto, double dVersandBrutto, AnsiString strZeilentyp)
{
	AnsiString s, strVersand, strVersandMinder;
	AnsiString strBelegtyp, strBelegnummer, strPreistyp, strSteuercode;
	int iSteuerAnzahl;
	Variant v;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	IOlePos *pos;
	TBelegp *belegp;

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
	strVersand=ini->ReadString("Order", "Versand", "");
	s=ini->ReadString("Order", "Versand", "");
	if(s != "")
		strVersand=s;
	strVersandMinder=ini->ReadString("Order", "VersandMinder", "");

	log("Lieferbedingung:"+strLB);

	if(strLB != "")
		{
		strVersand=ini->ReadString("Versand", strLB, "");
		strVersandMinder=ini->ReadString("VersandMinder", strLB, "");
		}
	delete ini;

	if(strVersand != "")
		log("Versandartikel:"+strVersand+" VersandMinder:"+strVersandMinder);

	if(_bUseCOM)
		{
		v=beleg->GetField(WideString("Belegtyp").c_bstr());
		strBelegtyp=v;
		v=beleg->GetField(WideString("Belegnummer").c_bstr());
		strBelegnummer=v;
		v=beleg->GetField(WideString("Preistyp").c_bstr());
		strPreistyp=v;
		}
	else
		{
		strBelegtyp=wawiBeleg->getBelegtyp();
		strBelegnummer=wawiBeleg->getBelegnummer();
		strPreistyp=wawiBeleg->getPreistyp();
		}

	// Steuercodes der Bestellung feststellen
	s="select steuercode from belegp where belegtyp='"+strBelegtyp+"' and belegnummer='"+strBelegnummer+"' "
		"group by steuercode";
	q->SQL->Text=s;
	q->Open();
	iSteuerAnzahl=q->RecordCount;
	if(iSteuerAnzahl == 1)
		{
		v=q->FieldByName("steuercode")->AsString;
		strSteuercode=v;
		}
	else
		strSteuercode="";
	q->Close();

	if(_bUseCOM)
		pos=beleg->CreatePos(WideString("E").c_bstr());
	else
		{
		belegp=new TBelegp(wawiBeleg);
		belegp->create("E");
		}

	if(iSteuerAnzahl > 1)
		{
		if(_bUseCOM)
			pos->SetArtikel(WideString(strVersand).c_bstr());
		else
			belegp->setArtikel(strVersand);
		if(strPreistyp != "S")
			{
			if(_bUseCOM)
				pos->SetSteuerCode(WideString("3").c_bstr());
			else
				belegp->setSteuercode("3");
			}
		}
	else
		{
		if(strSteuercode == "3")
			{
			if(_bUseCOM)
				pos->SetArtikel(WideString(strVersand).c_bstr());
			else
				belegp->setArtikel(strVersand);
			}
		else
			{
			if(strVersandMinder != "")
				{
				if(_bUseCOM)
					pos->SetArtikel(WideString(strVersandMinder).c_bstr());
				else
					belegp->setArtikel(strVersandMinder);
				}
			else
				{
				if(strPreistyp != "S")
					{
					if(_bUseCOM)
						pos->SetSteuerCode(WideString("2").c_bstr());
					else
						belegp->setSteuercode("2");
					}
				}
			}
		}
	if(_bUseCOM)
		pos->SetEingabeMenge(WideString("1").c_bstr());
	else
		belegp->setMenge(1);

	double dVersand;
	if(strPreistyp == "B")
		dVersand=dVersandBrutto;
	else
		dVersand=dVersandNetto;

	if(_bUseCOM)
		pos->SetEinzelPreis(dVersand);
	else
		belegp->setEinzelpreis(dVersand);

	log("Versandkosten:"+FloatToStr(RoundTo(dVersand, -2)));

	if(_bUseCOM)
		{
		pos->Save(1);
		pos->Release();
		}
	else
		{
		belegp->save();
		delete belegp;
		}

	delete q;
	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::addSLRabattPos(AnsiString strRabattArt, AnsiString strBezeichnung, double dMenge, double dPreisNetto, double dPreisBrutto)
{
	AnsiString s, strArtikel;
	AnsiString strBelegtyp, strBelegnummer, strPreistyp, strSteuercode;
	int iSteuerAnzahl;
	Variant v;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	IOlePos *pos;
	TBelegp *belegp;

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );
	strArtikel=ini->ReadString("Rabatt", strRabattArt, "");

	log("Rabattartikel:"+strArtikel);

	if(_bUseCOM)
		{
		v=beleg->GetField(WideString("Belegtyp").c_bstr());
		strBelegtyp=v;
		v=beleg->GetField(WideString("Belegnummer").c_bstr());
		strBelegnummer=v;
		v=beleg->GetField(WideString("Preistyp").c_bstr());
		strPreistyp=v;
		}
	else
		{
		strBelegtyp=wawiBeleg->getBelegtyp();
		strBelegnummer=wawiBeleg->getBelegnummer();
		strPreistyp=wawiBeleg->getPreistyp();
		}

	// Steuercodes der Bestellung feststellen
	s="select steuercode from belegp where belegtyp='"+strBelegtyp+"' and belegnummer='"+strBelegnummer+"' "
		"group by steuercode";
	q->SQL->Text=s;
	q->Open();
	iSteuerAnzahl=q->RecordCount;
	if(iSteuerAnzahl == 1)
		strSteuercode=q->FieldByName("Steuercode")->AsString;
	else
		strSteuercode="";
	q->Close();

	if(_bUseCOM)
		{
		pos=beleg->CreatePos(WideString("A").c_bstr());
		pos->SetArtikel(WideString(strArtikel).c_bstr());
		pos->SetEingabeMenge(WideString("1").c_bstr());
		}
	else
		{
		belegp=new TBelegp(wawiBeleg);
		belegp->create("E");
		belegp->setArtikel(strArtikel);
		belegp->setMenge(1);
		}

	if(strBezeichnung != "")
		{
		if(_bUseCOM)
			pos->SetBezeichnung(WideString(strBezeichnung).c_bstr());
		else
			belegp->setBezeichnung(strBezeichnung);
		}

	double dRabatt;
	if(strPreistyp == "B")
		dRabatt=dPreisBrutto;
	else
		dRabatt=dPreisNetto;

	if(_bUseCOM)
		pos->SetEinzelPreis(dRabatt);
	else
		belegp->setEinzelpreis(dRabatt);

	log("Rabatt:"+FloatToStr(RoundTo(dRabatt, -2))+" - "+strBezeichnung);

	if(_bUseCOM)
		{
		pos->Save(1);
		pos->Release();
		}
	else
		{
		belegp->save();
		delete belegp;
		}

	delete q;
	return(true);
}

//---------------------------------------------------------------------------

bool __fastcall TfrmMain::closeSLOrder()
{
	AnsiString s;
	AnsiString strBelegtyp, strBelegnummer;
	Variant v;

	if(_bUseCOM)
		{
		beleg->GetField(WideString("Belegtyp").c_bstr(), v);
		strBelegtyp=v;
		beleg->GetField(WideString("Belegnummer").c_bstr(), v);
		strBelegnummer=v;

		beleg->Save();
		beleg->Release();
		}
	else
		{
		strBelegtyp=wawiBeleg->getBelegtyp();
		strBelegnummer=wawiBeleg->getBelegnummer();

		wawiBeleg->save();
		delete wawiBeleg;
		}

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	if(_strAMOrderUpdateProzedure != "")
		{
		s="exec "+_strAMOrderUpdateProzedure+" N'"+strBelegtyp+"', N'"+strBelegnummer+"'";
		q->SQL->Text=s;
		q->ExecSQL();
		}

	delete q;

	return(true);
}

//---------------------------------------------------------------------------

AnsiString __fastcall TfrmMain::createKu(SLAdress *adr, bool bInterressent)
{
	AnsiString s, strBlobKey, strNewKey, strTable, strPreisTyp, strLand;
	AnsiString strNeukundenPreisgruppe, strNeukundenZB, strNeukundenLB, strNeukundenRabattgruppe, strNeukundenLager;
	AnsiString strNeukundenKundengruppe, strNeukundenKontengruppe, strNeukundenStandort, strNeukundenKostenstelle, strNeukundenBankbezug;
	AnsiString strNeukundenPreistyp;
	AnsiString strNeukundenFibukonto;
	AnsiString strSprache;
	AnsiString strName, strVorname, strFirma;
	Variant v;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	strSprache="D";

	if(!bInterressent)
		{
		strBlobKey="KU";
		strTable="kunden";
		}
	else
		{
		strBlobKey="IN";
		strTable="Interess";
		}

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );

	strNeukundenPreisgruppe=ini->ReadString("Neukunde", "Preisgruppe", "0");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Preisgruppe", "");
	if(s != "")
		strNeukundenPreisgruppe=s;

	strNeukundenPreistyp=ini->ReadString("Neukunde", "Preistyp", "B");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Preistyp", "");
	if(s != "")
		strNeukundenPreistyp=s;

	strNeukundenZB = ini->ReadString("Neukunde", "ZB", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "ZB", "");
	if(s != "")
		strNeukundenZB=s;
	if(strNeukundenZB == "")
		strNeukundenZB=adr->sZB;

	strNeukundenLB = ini->ReadString("Neukunde", "LB", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "LB", "");
	if(s != "")
		strNeukundenLB=s;
	if(strNeukundenLB == "")
		strNeukundenLB=adr->sLB;

	strNeukundenRabattgruppe = ini->ReadString("Neukunde", "Rabattgruppe", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Rabattgruppe", "");
	if(s != "")
		strNeukundenRabattgruppe=s;
	strNeukundenKundengruppe = ini->ReadString("Neukunde", "Kundengruppe", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Kundengruppe", "");
	if(s != "")
		strNeukundenKundengruppe=s;
	strNeukundenKontengruppe = ini->ReadString("Neukunde", "Kontengruppe", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Kontengruppe", "");
	if(s != "")
		strNeukundenKontengruppe=s;
	strNeukundenStandort = ini->ReadString("Neukunde", "Standort", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Standort", "");
	if(s != "")
		strNeukundenStandort=s;
	strNeukundenKostenstelle = ini->ReadString("Neukunde", "Kostenstelle", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Kostenstelle", "");
	if(s != "")
		strNeukundenKostenstelle=s;
	strNeukundenBankbezug = ini->ReadString("Neukunde", "Bankbezug", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Bankbezug", "");
	if(s != "")
		strNeukundenBankbezug=s;

	strNeukundenFibukonto = ini->ReadString("Neukunde", "Fibukonto", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Fibukonto", "");
	if(s != "")
		strNeukundenFibukonto=s;

	strNeukundenLager = ini->ReadString("Neukunde", "Lager", "");
	s=ini->ReadString("Neukunde:"+adr->sLand, "Lager", "");
	if(s != "")
		strNeukundenLager=s;
	adr->sLager=strNeukundenLager;

	if(strNeukundenPreisgruppe != "")
		{
		s="select zusatz from gruppen where id='PG' and nummer='"+strNeukundenPreisgruppe+"'";
		q->SQL->Text=s;
		q->Open();
		if(q->RecordCount)
			{
			s=q->FieldByName("Zusatz")->AsString;
			if(s.UpperCase() == "NETTO")
				strPreisTyp="N";
			else
				strPreisTyp="B";
			}
		else
			strPreisTyp=strNeukundenPreistyp;
		q->Close();
		}
	else
		{
		strNeukundenPreisgruppe="0";
		if(strNeukundenPreistyp != "")
			strPreisTyp=strNeukundenPreistyp;
		else
			strPreisTyp="B";
		}

	//if(shBData.dSteuerGesamtBetrag == 0.0)  // Wenn keine Steuer dabei ist, dann steuerfrei
	//	strPreisTyp="S";

	// Prüfen ob Kunde evtl. schon existiert
	bool bKuExist=false;
	s="select * from kunden where 1=1 ";
	/*
	if(slKundenPruefung->Values["Name"] == "1")
		s+="and Upper(name)='"+adr->sName.UpperCase()+"' ";
	if(slKundenPruefung->Values["Vorname"] == "1")
		s+="and Upper(vorname)='"+adr->sVorname.UpperCase()+"' ";
	if(slKundenPruefung->Values["Strasse"] == "1")
		s+="and Upper(strasse)='"+adr->sStrasse.UpperCase()+"' ";
	if(slKundenPruefung->Values["Ort"] == "1")
		s+="and Upper(ort)='"+adr->sOrt.UpperCase()+"' ";
	if(slKundenPruefung->Values["Firma"] == "1")
		s+="and Upper(firma)='"+adr->sFirma.UpperCase()+"' ";
	if(slKundenPruefung->Values["eMail"] == "1")
	*/
		s+="and Upper(eMail)='"+adr->sEMail.UpperCase()+"' ";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		strNewKey=q->FieldByName("Nummer")->AsString;
		q->Close();
		log("Existierenden Kunden:"+strNewKey+" gefunden!");
		log("Update Kundendaten...");

		s="update kunden set "
			"anrede='"+adr->sAnrede+"',"
			"firma='"+getCheckedSQLStr(adr->sFirma.SubString(1, 80))+"',"
			"vorname='"+getCheckedSQLStr(adr->sVorname.SubString(1, 80))+"',"
			"name='"+getCheckedSQLStr(adr->sName.SubString(1, 80))+"',"
			"zusatz='"+getCheckedSQLStr(adr->sZusatz.SubString(1, 80))+"',"
			"zusatz2='"+getCheckedSQLStr(adr->sZusatz2.SubString(1, 80))+"',"
			"email='"+adr->sEMail.SubString(1, 80)+"',"
			"ort='"+getCheckedSQLStr(adr->sOrt.SubString(1, 40))+"',"
			"plz='"+getCheckedSQLStr(adr->sPLZ.SubString(1, 10))+"',"
			"land='"+getCheckedSQLStr(adr->sLand.SubString(1, 6))+"',"
			"strasse='"+getCheckedSQLStr(adr->sStrasse.SubString(1, 80))+"',"
			"telefon1='"+getCheckedSQLStr(adr->sTelefon.SubString(1, 30))+"'"
			" "
			"where nummer='"+strNewKey+"'";
		q->SQL->Text=s;
		q->ExecSQL();

		delete q;
		return(strNewKey);
		}
	else
		q->Close();

	// Neuen Schlüssel suchen/belegen
	strNewKey=getSLNextKey(strBlobKey);

	log("Neue KdNr:"+strNewKey);

	// Kunden anlegen
	s="Insert into "+strTable+" (nummer, anrede, firma, vorname, name, zusatz, zusatz2, email, waehrung, ort, plz, strasse, telefon1, telefon2, fax, "
		"preisgruppe, preistyp, land, zahlungsbedingung, lieferbedingung, rabattgruppe, gruppe, kontengruppe, standort, kostenstelle, "
		"steueridentnr, lager, angelegtam, sprache, fibukonto) "
		"Values("
		"'"+strNewKey+"',"
		"'"+getCheckedSQLStr(adr->sAnrede)+"',"
		"'"+getCheckedSQLStr(adr->sFirma.SubString(1, 80))+"',"
		"'"+getCheckedSQLStr(adr->sVorname.SubString(1, 80))+"',"
		"'"+getCheckedSQLStr(adr->sName.SubString(1, 80))+"',"
		"'"+getCheckedSQLStr(adr->sZusatz.SubString(1, 80))+"'," // Zusatz
		"'"+getCheckedSQLStr(adr->sZusatz2.SubString(1, 80))+"'," // Zusatz2
		"'"+adr->sEMail.SubString(1, 80)+"',"
		"'EUR',"
		"'"+getCheckedSQLStr(adr->sOrt.SubString(1, 40))+"',"
		"'"+getCheckedSQLStr(adr->sPLZ.SubString(1, 10))+"',"
		"'"+getCheckedSQLStr(adr->sStrasse.SubString(1, 80))+"',"
		"'"+getCheckedSQLStr(adr->sTelefon.SubString(1, 30))+"',"
		"''," // Telefon2
		"''," // Fax
		"'"+strNeukundenPreisgruppe+"',"
		"'"+strPreisTyp+"',"
		"'"+getCheckedSQLStr(adr->sLand.SubString(1, 6))+"',"
		"'"+strNeukundenZB+"',"
		"'"+strNeukundenLB+"',"
		"'"+strNeukundenRabattgruppe+"',"
		"'"+strNeukundenKundengruppe+"',"
		"'"+strNeukundenKontengruppe+"',"
		"'"+strNeukundenStandort+"',"
		"'"+strNeukundenKostenstelle+"',"
		"'"+getCheckedSQLStr(adr->sUstId.SubString(1, 20))+"',"
		"'"+adr->sLager+"',"
		"GetDate(),"
		"'"+strSprache+"',"
		"'"+strNeukundenFibukonto+"')";
	q->SQL->Text=s;
	q->ExecSQL();
	AnsiString strLog=s;

	s="select eMail, nummer from "+strTable+" where nummer='"+strNewKey+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		{
		q->Close();
		setSLNextKey(strBlobKey, strNewKey);

		delete q;
		return(strNewKey);
		}
	q->Close();
	delete q;

	log("Fehler bei der Neukundenanlage!");
	log("SQL:"+strLog);
	return("");
}

//---------------------------------------------------------------------------

AnsiString __fastcall TfrmMain::createSLPDF(AnsiString strBelegtyp, AnsiString strBelegnummer, AnsiString strVorlage)
{
	AnsiString s, strResult, strOptionen, strPDFVorlage, strPDFOrdner;
	Variant v;

	TIniFile *ini;
	ini = new TIniFile( ChangeFileExt( Application->ExeName, ".INI" ) );

	if(strVorlage == "")
		strPDFVorlage=ini->ReadString("Order", "PDFRechnungsvorlage", "");
	else
		strPDFVorlage=strVorlage;

	strPDFOrdner=ini->ReadString("Order", "PDFOrdner", "");
	delete ini;

	strOptionen="Ausgabeziel;2\r\nDialogDruckziel;0\r\nDialogDruckereinrichtung;0\r\n";

	if(App1 == NULL)
		startCOM();

	try
		{
		log("DruckeBelegMitVorlage - Typ:"+strBelegtyp+" - Beleg:"+strBelegnummer+" - Vorlage:"+strPDFVorlage+" Optionen:"+strOptionen);
		App1->DruckeBelegMitVorlage(WideString(strBelegtyp).c_bstr(), WideString(strBelegnummer).c_bstr(), WideString(strPDFVorlage).c_bstr(), 0, WideString(strOptionen).c_bstr());

		s=strPDFOrdner+strBelegnummer+".pdf";
		log("PDFDatei:"+s);
		if(FileExists(s))
			strResult=s;
		}
	catch(Exception &e)
		{
		log("Exception:"+e.Message);
		}

	return(strResult);
}

//---------------------------------------------------------------------------

AnsiString __fastcall TfrmMain::getSLNextKey(AnsiString strBlobKey)
{
	AnsiString s, strNewKey, strKey;
	Variant v;
	char c;
	bool bShift=false;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="select * from [key] where blobkey='"+strBlobKey+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		strNewKey=q->FieldByName("LastKey")->AsString;
	else
		strNewKey="";

	q->Close();
	delete q;

	if(strNewKey != "")
		{
		strKey=strNewKey;

		int iIndex=strKey.Length();
		while(iIndex > 0)
			{
			c=strKey[iIndex];
			if(c < '9')
				c++;
			else if(c == '9')
				{
				c='0';
				bShift=true;
				}
			else if(c < 'Z')
				c++;
			else if(c == 'Z')
				{
				c='0';
				bShift=true;
				}

			strKey[iIndex]=c;

			if(bShift)
				{
				bShift=false;
				iIndex--;
				}
			else
				break;
			}
		strNewKey=strKey;
		}

	return(strNewKey);
}

//---------------------------------------------------------------------------

AnsiString __fastcall TfrmMain::setSLNextKey(AnsiString strBlobKey, AnsiString strNewKey)
{
	AnsiString s, strKey;
	Variant v;

	TADOQuery *q=new TADOQuery(this);
	q->Connection=ADOConnection1;

	s="select * from [key] where blobkey='"+strBlobKey+"'";
	q->SQL->Text=s;
	q->Open();
	if(q->RecordCount)
		strKey=q->FieldByName("Lastkey")->AsString;
	else
		strKey="";

	q->Close();

	s="update [key] set PrevKey=LastKey, LastKey='"+strNewKey+"' where BlobKey='"+strBlobKey+"'";
	q->SQL->Text=s;
	q->ExecSQL();
	delete q;

	return(strKey);
}
