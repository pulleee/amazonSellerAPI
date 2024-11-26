using SelectLineWAWIApiCore.Server.Amazon;
using SelectLineWAWIApiCore.Server.Database;
using SelectLineWAWIApiCore.Server.Services;

var builder = WebApplication.CreateBuilder(args);

// Load config
builder.Configuration.AddJsonFile("appsettings.json", optional: false, reloadOnChange: true);

// Add services to the container.
builder.Services.AddControllersWithViews();
builder.Services.AddRazorPages();
// Learn more about configuring Swagger/OpenAPI at https://aka.ms/aspnetcore/swashbuckle
builder.Services.AddEndpointsApiExplorer();
builder.Services.AddSwaggerGen();

// Configure CORS to allow requests from Blazor client.
builder.Services.AddCors(options =>
{
    options.AddDefaultPolicy(
        builder => builder
            .WithOrigins("https://localhost:7064") 
            .AllowAnyHeader()
            .AllowAnyMethod()
            .AllowCredentials());
});

builder.Services.AddScoped<BelegService>();
builder.Services.AddSingleton<WAWIDbContext>();
builder.Services.AddSingleton<AmazonAuthContext>();

var app = builder.Build();

// Use
// Configure the HTTP request pipeline.
if (app.Environment.IsDevelopment())
{
    app.UseSwagger();
    app.UseSwaggerUI();
    app.UseWebAssemblyDebugging();
}

app.UseHttpsRedirection();
app.UseBlazorFrameworkFiles();
app.UseStaticFiles();
app.UseCors();
//app.UseAuthorization();

// Mapping
app.MapRazorPages();
app.MapControllers();
app.MapFallbackToFile("index.html");

app.Run();
