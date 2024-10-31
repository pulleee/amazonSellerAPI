using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;
using SelectLineWAWIApi;
using SelectLineWAWIDataAccess.Database;
using Syncfusion.Blazor;

var builder = WebAssemblyHostBuilder.CreateDefault(args);
builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

// Load config
builder.Configuration.AddJsonFile("appsettings.json", optional: false, reloadOnChange: true);

// Register services
builder.Services.AddSingleton<WAWIDbContext>();
builder.Services.AddScoped<BelegService>();
builder.Services.AddScoped(sp => new HttpClient { BaseAddress = new Uri(builder.HostEnvironment.BaseAddress) });

// Add Syncfusion
builder.Services.AddSyncfusionBlazor();

// Run
await builder.Build().RunAsync();
