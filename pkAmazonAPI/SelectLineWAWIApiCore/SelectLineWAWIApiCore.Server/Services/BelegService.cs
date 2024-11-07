using Microsoft.EntityFrameworkCore;
using SelectLineWAWIApiCore.Server.Database;
using SelectLineWAWIApiCore.Shared.DTOs;
using SelectLineWAWIApiCore.Shared.Models;

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
            List<BelegReport> belegList = new List<BelegReport>();

            var lines = textContent.Split('\n').Skip(1);
            foreach (var line in lines)
            {
                if (string.IsNullOrWhiteSpace(line)) continue;

                belegList.Add(BelegReport.ParseTextToBeleg(line));
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

        public async Task AddBelegFromReportAsync(BelegReport belegReport)
        {           
            _context.Belege.Add(Beleg.MapToBeleg(belegReport));
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
    }
}