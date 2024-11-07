using SelectLineWAWIApiCore.Shared.Models;
using System.ComponentModel.DataAnnotations;

namespace SelectLineWAWIApiCore.Server
{
    public class Beleg
    {
        [Key]
        public int BELEG_ID { get; set; }

        public string? Belegtyp { get; set; }
        public string? Belegnummer { get; set; }
        public DateTime? Datum { get; set; }
        public string? Adressnummer { get; set; }
        public string? Name { get; set; }
        public string? Firma { get; set; }
        public string? Anrede { get; set; }
        public string? Vorname { get; set; }
        public string? Zusatz { get; set; }
        public string? Zusatz2 { get; set; }
        public string? Zusatz3 { get; set; }
        public string? Strasse { get; set; }
        public string? Land { get; set; }
        public string? Plz { get; set; }
        public string? Ort { get; set; }
        public short? BankVerb { get; set; }
        public string? KasseBank { get; set; }
        public bool? Barverkauf { get; set; }
        public bool? Gedruckt { get; set; }
        public string? Preisgruppe { get; set; }
        public string? PreisTyp { get; set; }
        public string? Rabattgruppe { get; set; }
        public double? Belegrabatt { get; set; }
        public string? Zahlungsbedingung { get; set; }
        public short? Zahlungsziel { get; set; }
        public short? Zahlungszieltyp { get; set; }
        public string? ESRReferenznummer { get; set; }
        public short? Skontotage1 { get; set; }
        public short? Skontotage1typ { get; set; }
        public double? Skontoprozent1 { get; set; }
        public short? Skontotage2 { get; set; }
        public short? Skontotage2typ { get; set; }
        public double? Skontoprozent2 { get; set; }
        public string? Mitarbeiter { get; set; }
        public string? Vertreter { get; set; }
        public string? Lager { get; set; }
        public int? Ansprechpartner { get; set; }
        public string? Waehrungscode { get; set; }
        public double? Waehrungsfaktor { get; set; }
        public string? Lieferbedingung { get; set; }
        public string? LText1 { get; set; }
        public string? LText2 { get; set; }
        public double? Brutto { get; set; }
        public double? Netto { get; set; }
        public double? Steuer { get; set; }
        public double? FremdwaehrungBrutto { get; set; }
        public double? FremdwaehrungNetto { get; set; }
        public double? FremdwaehrungSteuer { get; set; }
        public double? WIR { get; set; }
        public double? WIRProzent { get; set; }
        public string? WIRArt { get; set; }
        public DateTime? Liefertermin { get; set; }
        public string? Konto { get; set; }
        public string? Kontengruppe { get; set; }
        public string? Kostenstelle { get; set; }
        public string? Status { get; set; }
        public string? IhrZeichen { get; set; }
        public string? UnserZeichen { get; set; }
        public string? IhrAuftrag { get; set; }
        public DateTime? IhrAuftragVom { get; set; }
        public string? Sprache { get; set; }
        public DateTime? Fibuexport { get; set; }
        public string? Fibusatz { get; set; }
        public string? LieferBelegNr { get; set; }
        public string? FreierText1 { get; set; }
        public string? FreierText2 { get; set; }
        public double? FreieZahl1 { get; set; }
        public double? FreieZahl2 { get; set; }
        public int? FreieZahl3 { get; set; }
        public int? FreieZahl4 { get; set; }
        public DateTime? FreiesDatum1 { get; set; }
        public DateTime? FreiesDatum2 { get; set; }
        public bool? FreiesKennzeichen1 { get; set; }
        public bool? FreiesKennzeichen2 { get; set; }
        public bool? FreiesKennzeichen3 { get; set; }
        public bool? FreiesKennzeichen4 { get; set; }
        public string? PCKasse { get; set; }
        public short? PCKassenSitzung { get; set; }
        public string? PCKasseBarZahlart { get; set; }
        public string? PCKasseBankZahlart { get; set; }
        public string? PCKasseBarBankbezug { get; set; }
        public double? PCKasseBarZahlung { get; set; }
        public double? PCKasseBar2 { get; set; }
        public int? Vertrag { get; set; }
        public string? Vorlage { get; set; }
        public string? RefAdresse { get; set; }
        public string? RechAdresse { get; set; }
        public double? Erloes { get; set; }
        public double? ErloesEuro { get; set; }
        public double? ErloesAlt { get; set; }
        public double? UebernahmeOffen { get; set; }
        public DateTime? AngelegtAm { get; set; }
        public string? AngelegtVon { get; set; }
        public DateTime? BearbeitetAm { get; set; }
        public string? BearbeitetVon { get; set; }
        public short? KursTyp { get; set; }
        public double? FixKurs { get; set; }
        public double? NettoAlt { get; set; }
        public double? SteuerAlt { get; set; }
        public double? BruttoAlt { get; set; }
        public double? EuroNetto { get; set; }
        public double? EuroSteuer { get; set; }
        public double? EuroBrutto { get; set; }
        public string? EULand { get; set; }
        public string? Verkehrszweig { get; set; }
        public double? Gewicht { get; set; }
        public int? Projekt { get; set; }
        public string? RTyp { get; set; }
        public string? ILNNummer { get; set; }
        public string? Orignummer { get; set; }
        public string? EDIStatus { get; set; }
        public DateTime? ValutaDatum { get; set; }

        public bool WerkauftragErweitert { get; set; } // Non-nullable
        public string? Standort { get; set; }
        public int? RundungPos { get; set; }
        public int? RundungBel { get; set; }
        public string? VerbandsKLNr { get; set; }
        public double? DelkredereProzent { get; set; }
        public string? Umlagerung { get; set; }

        public int Version { get; set; } // Non-nullable
        public string? Belegaufschlagsartikel { get; set; }
        public string? UStID { get; set; }

        public bool Zahlsperre { get; set; } // Non-nullable
        public string? RMANummer { get; set; }
        public string? LRMANummer { get; set; }

        public short Herkunft { get; set; } // Non-nullable
        public string? Meldeland { get; set; }
        public DateTime? Leistungsdatum { get; set; }
        public string? Zahlungsreferenz { get; set; }

        [Timestamp]
        public byte[] TS { get; set; } // Non-nullable timestamp for concurrency

        public static Beleg MapToBeleg(BelegReport report)
        {
            return new Beleg
            {
                Belegnummer = report.OrderId,            // "OrderId" -> "Belegnummer" (Document number)
                Datum = report.PurchaseDate,             // "PurchaseDate" -> "Datum" (Date)
                Firma = report.BuyerName,                // "BuyerName" -> "Name"
                Vorname = report.RecipientName,          // "RecipientName" -> "Vorname" (First name)
                Zusatz = report.BillAddress1,            // "BillAddress1" -> "Zusatz" (Address line 1)
                Zusatz2 = report.BillAddress2,           // "BillAddress2" -> "Zusatz2" (Address line 2)
                Zusatz3 = report.BillAddress3,           // "BillAddress3" -> "Zusatz3" (Address line 3)
                Strasse = report.ShipAddress1,           // "ShipAddress1" -> "Strasse" (Street)
                Land = report.ShipCountry,               // "ShipCountry" -> "Land" (Country)
                Plz = report.ShipPostalCode,             // "ShipPostalCode" -> "Plz" (Postal code)
                Ort = report.ShipCity,                   // "ShipCity" -> "Ort" (City)
                Waehrungscode = report.Currency,         // "Currency" -> "Waehrungscode" (Currency code)
                Brutto = (double?)report.ItemPrice,      // "ItemPrice" -> "Brutto" (Gross amount)
                Steuer = (double?)report.ItemTax,        // "ItemTax" -> "Steuer" (Tax)
                FreierText1 = report.ShipServiceLevel,   // "ShipServiceLevel" -> "FreierText1" (Free text field 1)
                FreierText2 = report.SalesChannel,       // "SalesChannel" -> "FreierText2" (Free text field 2)
                Liefertermin = report.DeliveryEndDate,   // "DeliveryEndDate" -> "Liefertermin" (Delivery date)
                Zahlsperre = false,                      // Default to false, no direct mapping provided

                // Handling required fields with default values
                WerkauftragErweitert = false,            // Required field, no corresponding field in BelegReport
                Herkunft = 0,                            // Required field, set default as no source specified
                Version = 1                              // Default to 1 as no versioning specified in BelegReport
            };
        }
    }
}
