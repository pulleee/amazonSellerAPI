using Microsoft.AspNetCore.Mvc;
using SelectLineWAWIApiCore.Server.Services;
using SelectLineWAWIApiCore.Shared.DTOs;
using SelectLineWAWIApiCore.Shared.Models;

namespace SelectLineWAWIApiCore.Server.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class BelegController : ControllerBase
    {
        private readonly BelegService _belegService;

        public BelegController(BelegService belegService)
        {
            _belegService = belegService;
        }

        // GET: api/beleg
        [HttpGet]
        public async Task<ActionResult<List<BelegDTO>>> GetAllBelege()
        {
            var belege = await _belegService.GetBelegDtosAsync();
            return Ok(belege);
        }

        // GET: api/beleg/{id}
        [HttpGet("{id}")]
        public async Task<ActionResult<BelegDTO>> GetBelegById(int id)
        {
            var beleg = await _belegService.GetBelegByIdAsync(id);
            if (beleg == null)
            {
                return NotFound();
            }
            return Ok(beleg);
        }

        // POST: api/beleg
        [HttpPost]
        public async Task<ActionResult> AddBeleg(BelegDTO belegDto)
        {
            await _belegService.AddBelegAsync(belegDto);
            return CreatedAtAction(nameof(GetBelegById), new { id = belegDto.BELEG_ID }, belegDto);
        }

        // POST: api/beleg/UploadFiles
        [HttpPost("[action]")]
        public async Task<IActionResult> UploadFiles(IFormFile UploadFiles)
        {
            if (UploadFiles.Length == 0)
            {
                var errorResponse = new
                {
                    Success = false,
                    Message = "No files given!"
                };

                return BadRequest(errorResponse);
            }

            try
            {
                using var reader = new StreamReader(UploadFiles.OpenReadStream());
                string data = await reader.ReadToEndAsync();

                // Parse to entity
                List<BelegReport> belegList = _belegService.ParseTextToBelegList(data);

                // Save in Database                
                foreach (var item in belegList)
                {
                    await _belegService.AddBelegFromReportAsync(item);
                }

                var successResponse = new
                {
                    Success = true,
                    Message = $"Files uploaded and parsed successfully - {belegList.Count} Entities added!",
                    BelegList = belegList
                };

                return Ok(successResponse);
            }
            catch (Exception ex)
            {
                var errorResponse = new
                {
                    Success = false,
                    Message = "File upload failed: " + ex.Message
                };

                return BadRequest(errorResponse);
            }           
        }

        // PUT: api/beleg/{id}
        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateBeleg(int id, BelegDTO belegDto)
        {
            if (id != belegDto.BELEG_ID)
            {
                return BadRequest();
            }

            await _belegService.UpdateBelegAsync(belegDto);
            return NoContent();
        }

        // DELETE: api/beleg/{id}
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteBeleg(int id)
        {
            await _belegService.DeleteBelegAsync(id);
            return NoContent();
        }
    }
}
