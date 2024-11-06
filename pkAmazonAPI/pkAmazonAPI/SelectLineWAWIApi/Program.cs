using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using SelectLineWAWIApiCore.Client;
using SelectLineWAWIApiCore.Client.Services;
using Syncfusion.Blazor;

var builder = WebAssemblyHostBuilder.CreateDefault(args);
builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

// Load config
builder.Configuration.AddJsonFile("appsettings.json", optional: false, reloadOnChange: true);

// Register services
builder.Services.AddSingleton<LogService>();
builder.Services.AddScoped<BelegService>();
builder.Services.AddScoped(sp => new HttpClient {BaseAddress = new Uri(builder.Configuration.GetConnectionString("Host")) });

// Add Syncfusion
builder.Services.AddSyncfusionBlazor();
Syncfusion.Licensing.SyncfusionLicenseProvider.RegisterLicense("Ngo9BigBOggjHTQxAR8/V1NDaF5cWGRCf1JpRmNGfV5ycEVFallWTnRaUiweQnxTdEFiWXxfcHNXQmRVVEd0Vw==");

// Build Host
var host = builder.Build();

// Add Logging
var logger = host.Services.GetRequiredService<ILoggerFactory>()
    .CreateLogger<Program>();

// Run Host
await host.RunAsync();

logger.LogInformation("Running...");
