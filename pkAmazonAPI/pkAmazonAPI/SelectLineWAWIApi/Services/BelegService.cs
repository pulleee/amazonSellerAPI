using System.Net.Http.Json;
using SelectLineWAWIApiCore.Shared.DTOs;

namespace SelectLineWAWIApiCore.Client.Services
{
    public class BelegService
    {
        private readonly HttpClient _httpClient;

        public BelegService(HttpClient httpClient)
        {
            _httpClient = httpClient;
        }

        // Get a list of all Belege
        public async Task<List<BelegDTO>> GetAllBelegeAsync()
        {
            return await _httpClient.GetFromJsonAsync<List<BelegDTO>>("api/belege");
        }

        // Get a single Beleg by ID
        public async Task<BelegDTO> GetBelegByIdAsync(int id)
        {
            return await _httpClient.GetFromJsonAsync<BelegDTO>($"api/belege/{id}");
        }

        // Add a new Beleg
        public async Task AddBelegAsync(BelegDTO belegDto)
        {
            await _httpClient.PostAsJsonAsync("api/belege", belegDto);
        }

        // Update an existing Beleg
        public async Task UpdateBelegAsync(BelegDTO belegDto)
        {
            await _httpClient.PutAsJsonAsync($"api/belege/{belegDto.Belegnummer}", belegDto);
        }

        // Delete a Beleg by ID
        public async Task DeleteBelegAsync(int id)
        {
            await _httpClient.DeleteAsync($"api/belege/{id}");
        }
    }
}
