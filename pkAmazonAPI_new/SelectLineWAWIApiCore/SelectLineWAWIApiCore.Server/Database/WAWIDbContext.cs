using SelectLineWAWIApiCore.Shared.Models;
using Microsoft.EntityFrameworkCore;

namespace SelectLineWAWIApiCore.Server.Database
{
    public class WAWIDbContext : DbContext
    {
        public static readonly string DB_CONNECTION = "SelectLineWAWI";

        public DbSet<Beleg> Belege { get; set; }

        private IConfiguration _configuration;

        public WAWIDbContext(IConfiguration configuration)
        {
            _configuration = configuration;
        }

        protected override void OnConfiguring(DbContextOptionsBuilder optionsBuilder)
        {
            optionsBuilder.UseSqlServer(_configuration.GetConnectionString(DB_CONNECTION));
        }

        protected override void OnModelCreating(ModelBuilder modelBuilder)
        {
            modelBuilder.Entity<Beleg>().ToTable("Beleg");
        }
    }
}
