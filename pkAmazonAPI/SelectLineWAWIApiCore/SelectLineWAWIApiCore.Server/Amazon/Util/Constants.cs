namespace SelectLineWAWIApiCore.Server.Amazon.Util
{
    public static class Constants
    {
        public static readonly string DATE_FORMAT = "yyyy-MM-ddTHH:mm:ssZ";

        public static readonly Dictionary<string, string> MARKETPLACE_IDs = new Dictionary<string, string>
        {
            { "CA", "A2EUQ1WTGCTBG2" },
            { "US", "ATVPDKIKX0DER" },
            { "MX", "A1AM78C64UM0Y8" },
            { "BR", "A2Q3Y263D00KWC" },
            { "ES", "A1RKKUPIHCS9HS" },
            { "UK", "A1F83G8C2ARO7P" },
            { "FR", "A13V1IB3VIYZZH" },
            { "BE", "AMEN7PMS3EDWL" },
            { "NL", "A1805IZSGTT6HS" },
            { "DE", "A1PA6795UKMFR9" },
            { "IT", "APJ6JRA9NG5V4" },
            { "SE", "A2NODRKZP88ZB9" },
            { "ZA", "AE08WJ6YKNBMC" },
            { "PL", "A1C3SOZRARQ6R3" },
            { "EG", "ARBP9OOSHTCHU" },
            { "TR", "A33AVAJ2PDY3EV" },
            { "SA", "A17E79C6D8DWNP" },
            { "AE", "A2VIGQ35RCS4UG" },
            { "IN", "A21TJRUUN4KGV" },
            { "SG", "A19VAU5U5O7RUS" },
            { "AU", "A39IBJ37TRP1C6" },
            { "JP", "A1VC38T7YXB528" }
        };

        public static Region NorthAmerica { get { return new Region("us-east-1", "https://sellingpartnerapi-na.amazon.com", "https://sandbox.sellingpartnerapi-na.amazon.com"); } }
        public static Region Europe { get { return new Region("eu-west-1", "https://sellingpartnerapi-eu.amazon.com", "https://sandbox.sellingpartnerapi-eu.amazon.com"); } }
        public static Region FarEast { get { return new Region("us-west-2", "https://sellingpartnerapi-fe.amazon.com", "https://sandbox.sellingpartnerapi-fe.amazon.com"); } }
    }
}
