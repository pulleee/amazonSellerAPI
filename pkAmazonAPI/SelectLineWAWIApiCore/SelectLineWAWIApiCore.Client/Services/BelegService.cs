using System.Net.Http.Json;
using SelectLineWAWIApiCore.Shared.DTOs;

namespace SelectLineWAWIApiCore.Client.Services
{
    public class BelegService
    {
        private readonly HttpClient _httpClient;
        private readonly LogService _logService;

        public BelegService(HttpClient httpClient, LogService logService)
        {
            _httpClient = httpClient;
            _logService = logService;
        }

        // Get a list of all Belege
        public async Task<List<BelegDTO>> GetAllBelegeAsync()
        {
            try
            {
                _logService.AddLog(LogLevel.Info, "Try getting Beleg-Entities from: api/beleg");

                var result = await _httpClient.GetFromJsonAsync<List<BelegDTO>>("api/beleg");

                _logService.AddLog(LogLevel.Info, $"Successfully GetAllBelegeAsync with {result.Count} Beleg-Entities");

                return result;
            }
            catch (Exception ex)
            {
                _logService.AddLog(LogLevel.Error, $"Failed to GetAllBelegeAsync with message: {ex.Message}");

                return new List<BelegDTO>();
            }            
        }

        // Get a single Beleg by ID
        public async Task<BelegDTO> GetBelegByIdAsync(int id)
        {
            return await _httpClient.GetFromJsonAsync<BelegDTO>($"api/beleg/{id}");
        }

        // Add a new Beleg
        public async Task AddBelegAsync(BelegDTO belegDto)
        {
            await _httpClient.PostAsJsonAsync("api/beleg", belegDto);
        }

        // Update an existing Beleg
        public async Task UpdateBelegAsync(BelegDTO belegDto)
        {
            await _httpClient.PutAsJsonAsync($"api/beleg/{belegDto.Belegnummer}", belegDto);
        }

        // Delete a Beleg by ID
        public async Task DeleteBelegAsync(int id)
        {
            await _httpClient.DeleteAsync($"api/beleg/{id}");
        }
    }
}
