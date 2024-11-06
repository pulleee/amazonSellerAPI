using Microsoft.EntityFrameworkCore;
using SelectLineWAWIApiCore.Server.Database;
using SelectLineWAWIApiCore.Shared.DTOs;
using SelectLineWAWIApiCore.Shared.Models;
using static System.Runtime.InteropServices.JavaScript.JSType;
using System.Globalization;

namespace SelectLineWAWIApiCore.Server.Services
{
    public class BelegService
    {
        private readonly WAWIDbContext _context;

        public BelegService(WAWIDbContext context)
        {
            _context = context;
        }

        public List<BelegReport> ParseTextToBelegList(string textContent)
        {
            // Implement your parsing logic here to create a Beleg instance
            List<BelegReport> belegList = new List<BelegReport>();

            // Example parsing logic (adjust as per your content structure)
            var lines = textContent.Split('\n').Skip(1);
            foreach (var line in lines)
            {
                if (string.IsNullOrWhiteSpace(line)) continue;

                belegList.Add(parseTextToBeleg(line));
            }

            return belegList;
        }

        public async Task<List<BelegDTO>> GetBelegDtosAsync()
        {
            return await _context.Belege
                .Select(b => new BelegDTO
                {
                    BELEG_ID = b.BELEG_ID,
                    Belegnummer = b.Belegnummer,
                    Datum = b.Datum,
                    Name = b.Name,
                    Firma = b.Firma,
                    Ort = b.Ort,
                    Brutto = b.Brutto,
                    Netto = b.Netto
                })
            .ToListAsync();
        }

        public async Task<BelegDTO> GetBelegByIdAsync(int id)
        {
            var beleg = await _context.Belege.FindAsync(id);
            if (beleg == null) return null;

            return new BelegDTO
            {
                BELEG_ID = beleg.BELEG_ID,
                Belegnummer = beleg.Belegnummer,
                Datum = beleg.Datum,
                Name = beleg.Name,
                Firma = beleg.Firma,
                Ort = beleg.Ort,
                Brutto = beleg.Brutto,
                Netto = beleg.Netto
            };
        }

        public async Task AddBelegAsync(BelegDTO belegDTO)
        {
            var beleg = new Beleg
            {
                BELEG_ID = belegDTO.BELEG_ID,
                Belegnummer = belegDTO.Belegnummer,
                Datum = belegDTO.Datum,
                Name = belegDTO.Name,
                Firma = belegDTO.Firma,
                Ort = belegDTO.Ort,
                Brutto = belegDTO.Brutto,
                Netto = belegDTO.Netto
            };

            _context.Belege.Add(beleg);
            await _context.SaveChangesAsync();
        }

        public async Task UpdateBelegAsync(BelegDTO belegDTO)
        {
            var beleg = await _context.Belege.FindAsync(belegDTO.BELEG_ID);
            if (beleg == null) throw new KeyNotFoundException("Beleg not found");

            beleg.BELEG_ID = belegDTO.BELEG_ID;
            beleg.Belegnummer = belegDTO.Belegnummer;
            beleg.Datum = belegDTO.Datum;
            beleg.Name = belegDTO.Name;
            beleg.Firma = belegDTO.Firma;
            beleg.Ort = belegDTO.Ort;
            beleg.Brutto = belegDTO.Brutto;
            beleg.Netto = belegDTO.Netto;

            await _context.SaveChangesAsync();
        }

        public async Task DeleteBelegAsync(int id)
        {
            var beleg = await _context.Belege.FindAsync(id);
            if (beleg == null) throw new KeyNotFoundException("Beleg not found");

            _context.Belege.Remove(beleg);
            await _context.SaveChangesAsync();
        }

        private BelegReport parseTextToBeleg(string data)
        {
            var fields = data.Split('\t');

            return new BelegReport
            {
                OrderId = fields[0],
                OrderItemId = fields[1],
                PurchaseDate = DateTime.Parse(fields[2], null, DateTimeStyles.RoundtripKind),
                PaymentsDate = DateTime.Parse(fields[3], null, DateTimeStyles.RoundtripKind),
                BuyerEmail = fields[4],
                BuyerName = fields[5],
                BuyerPhoneNumber = fields[6],
                Sku = fields[7],
                ProductName = fields[8],
                QuantityPurchased = int.Parse(fields[9]),
                Currency = fields[10],
                ItemPrice = decimal.Parse(fields[11], CultureInfo.InvariantCulture),
                ItemTax = decimal.Parse(fields[12], CultureInfo.InvariantCulture),
                ShippingPrice = decimal.Parse(fields[13], CultureInfo.InvariantCulture),
                ShippingTax = decimal.Parse(fields[14], CultureInfo.InvariantCulture),
                ShipServiceLevel = fields[15],
                RecipientName = fields[16],
                ShipAddress1 = fields[17],
                ShipAddress2 = fields[18],
                ShipAddress3 = fields[19],
                ShipCity = fields[20],
                ShipState = fields[21],
                ShipPostalCode = fields[22],
                ShipCountry = fields[23],
                ShipPhoneNumber = fields[24],
                BillName = fields[25],
                BillAddress1 = fields[26],
                BillAddress2 = fields[27],
                BillAddress3 = fields[28],
                BillCity = fields[29],
                BillState = fields[30],
                BillPostalCode = fields[31],
                BillCountry = fields[32],
                ItemPromotionDiscount = decimal.Parse(fields[33], CultureInfo.InvariantCulture),
                ItemPromotionId = fields[34],
                ShipPromotionDiscount = decimal.Parse(fields[35], CultureInfo.InvariantCulture),
                ShipPromotionId = fields[36],
                DeliveryStartDate = string.IsNullOrWhiteSpace(fields[37]) ? (DateTime?)null : DateTime.Parse(fields[37], null, DateTimeStyles.RoundtripKind),
                DeliveryEndDate = string.IsNullOrWhiteSpace(fields[38]) ? (DateTime?)null : DateTime.Parse(fields[38], null, DateTimeStyles.RoundtripKind),
                DeliveryTimeZone = fields[39],
                DeliveryInstructions = fields[40],
                SalesChannel = fields[41],
                ShipmentStatus = fields[42]
            };
        }
    }
}