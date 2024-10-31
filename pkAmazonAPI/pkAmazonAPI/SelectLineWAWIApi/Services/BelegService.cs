using Microsoft.EntityFrameworkCore;
using SelectLineWAWIDataAccess.Database;
using SelectLineWAWIDataAccess.DTOs;
using SelectLineWAWILib.Models;

public class BelegService
{
    private readonly WAWIDbContext _context;

    public BelegService(WAWIDbContext context)
    {
        _context = context;
    }

    public List<BelegDTO> GetTestBelege()
    {
        return new List<BelegDTO>
        {
            new BelegDTO { BELEG_ID = 1, Name = "TestBeleg1", Datum = DateTime.Now, Brutto = 75000 },
            new BelegDTO { BELEG_ID = 2, Name = "TestBeleg2", Datum = DateTime.Now, Brutto = 65000 },
            new BelegDTO { BELEG_ID = 3, Name = "TestBeleg3", Datum = DateTime.Now, Brutto = 55000 }
        };
    }

    // Get a list of all Belege
    public async Task<List<Beleg>> GetAllBelegeAsync()
    {
        return await _context.Belege.ToListAsync();
    }

    // Get a single Beleg by ID
    public async Task<Beleg> GetBelegByIdAsync(int id)
    {
        return await _context.Belege.FindAsync(id);
    }

    // Add a new Beleg
    public async Task AddBelegAsync(Beleg beleg)
    {
        _context.Belege.Add(beleg);
        await _context.SaveChangesAsync();
    }

    // Update an existing Beleg
    public async Task UpdateBelegAsync(Beleg beleg)
    {
        _context.Belege.Update(beleg);
        await _context.SaveChangesAsync();
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
}
