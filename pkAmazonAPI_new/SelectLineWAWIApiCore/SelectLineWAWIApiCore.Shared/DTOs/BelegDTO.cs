namespace SelectLineWAWIApiCore.Shared.DTOs
{
    public class BelegDTO
    {
        public int BELEG_ID { get; set; }
        public string Belegnummer { get; set; }
        public DateTime Datum { get; set; }
        public string Name { get; set; }
        public string Firma { get; set; }
        public string Ort { get; set; }
        public double Brutto { get; set; }
        public double Netto { get; set; }
    }
}
