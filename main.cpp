#include "raylib.h"

// Define our low-fi retro resolution (scaled up later)
const int VIRTUAL_WIDTH = 320;
const int VIRTUAL_HEIGHT = 240;
const int TILE_SIZE = 16;

struct Player {
    Vector2 gridPos;      // Target grid position
    Vector2 screenPos;    // Actual pixel position floating toward target
    float speed;
    bool isMoving;
};

int main() {
    // 1. Initialize window with a scaled factor for crisp retro pixels
    const int scale = 3;
    InitWindow(VIRTUAL_WIDTH * scale, VIRTUAL_HEIGHT * scale, "Zali Investigation: Clinic-04");
    SetTargetFPS(60);

    // 2. Setup virtual render texture (lets us render at 320x240, then upscale perfectly)
    RenderTexture2D target = LoadRenderTexture(VIRTUAL_WIDTH, VIRTUAL_HEIGHT);

    // 3. Initialize Zali
    Player zali = { {5, 5}, {5 * TILE_SIZE, 5 * TILE_SIZE}, 4.0f, false };
    Vector2 dir = { 0, 0 };

    // Simple map bounds: 0 to 19 columns, 0 to 14 rows
    const int MAP_WIDTH = 20;
    const int MAP_HEIGHT = 15;

    while (!WindowShouldClose()) {
        // --- UPDATE LOGIC ---
        
        if (!zali.isMoving) {
            // Check for keyboard inputs
            if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))    { dir = { 0, -1 }; zali.isMoving = true; }
            else if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))  { dir = { 0, 1 };  zali.isMoving = true; }
            else if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  { dir = { -1, 0 }; zali.isMoving = true; }
            else if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) { dir = { 1, 0 };  zali.isMoving = true; }
            
            if (zali.isMoving) {
                // Calculate prospective grid step
                Vector2 nextPos = { zali.gridPos.x + dir.x, zali.gridPos.y + dir.y };
                
                // Simple boundary check (add custom wall logic here!)
                if (nextPos.x >= 0 && nextPos.x < MAP_WIDTH && nextPos.y >= 0 && nextPos.y < MAP_HEIGHT) {
                    zali.gridPos = nextPos;
                } else {
                    zali.isMoving = false; // Hit outer map bounds
                }
            }
        }

        // Interpolate pixel position smoothly toward target grid block
        if (zali.isMoving) {
            Vector2 targetPixels = { zali.gridPos.x * TILE_SIZE, zali.gridPos.y * TILE_SIZE };
            
            if (zali.screenPos.x < targetPixels.x) zali.screenPos.x += zali.speed;
            if (zali.screenPos.x > targetPixels.x) zali.screenPos.x -= zali.speed;
            if (zali.screenPos.y < targetPixels.y) zali.screenPos.y += zali.speed;
            if (zali.screenPos.y > targetPixels.y) zali.screenPos.y -= zali.speed;

            // Arrived at target block
            if (zali.screenPos.x == targetPixels.x && zali.screenPos.y == targetPixels.y) {
                zali.isMoving = false;
            }
        }

        // --- RENDER LOGIC ---
        
        // A. Draw your low-res game assets into the virtual canvas
        BeginTextureMode(target);
            ClearBackground(BLACK);

            // Draw a creepy retro grid background
            for (int x = 0; x < MAP_WIDTH; x++) {
                for (int y = 0; y < MAP_HEIGHT; y++) {
                    // Just an example floor tile layout
                    if ((x + y) % 2 == 0) {
                        DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, GetColor(0x110b18FF)); // Very dark purple
                    }
                }
            }

            // Draw an eerie object to inspect (e.g., a medical table or lore document)
            DrawRectangle(12 * TILE_SIZE, 8 * TILE_SIZE, TILE_SIZE, TILE_SIZE, DARKGRAY);
            DrawCircle(12 * TILE_SIZE + 8, 8 * TILE_SIZE + 8, 3, MAROON); // "Crimson bandage fluid"

            // Draw Zali (Using a 16x16 placeholder block for his sprite)
            // Color scheme: Medical white coat, pastel purple hair accents
            DrawRectangleV(zali.screenPos, Vector2{ (float)TILE_SIZE, (float)TILE_SIZE }, RAYWHITE);
            DrawRectangle(zali.screenPos.x + 4, zali.screenPos.y + 2, 8, 4, PURPLE); // Hair line

            // B. Apply the Flashlight Masking Layer
            // We use standard alpha blending to paint the darkness, skipping the center circle
            BeginBlendMode(BLEND_ALPHA);
                // Center flashlight circle on Zali's core
                Vector2 center = { zali.screenPos.x + 8, zali.screenPos.y + 8 };
                float radius = 45.0f;

                // Draw darkness masks around the flashlight ring using a simple layout geometry
                // Top block
                DrawRectangle(0, 0, VIRTUAL_WIDTH, center.y - radius, BLACK);
                // Bottom block
                DrawRectangle(0, center.y + radius, VIRTUAL_WIDTH, VIRTUAL_HEIGHT - (center.y + radius), BLACK);
                // Left block
                DrawRectangle(0, center.y - radius, center.x - radius, radius * 2, BLACK);
                // Right block
                DrawRectangle(center.x + radius, center.y - radius, VIRTUAL_WIDTH - (center.x + radius), radius * 2, BLACK);

                // Optional: Draw a faint, translucent ring to simulate a dirty flashlight beam edge
                DrawCircleLines(center.x, center.y, radius, GetColor(0x55331133)); 
            EndBlendMode();

        EndTextureMode();

        // C. Render the low-res layout upscaled to your actual window size
        BeginDrawing();
            ClearBackground(BLACK);
            // Draw the texture flipped vertically because Raylib render textures are upside down by default
            DrawTexturePro(target.texture, 
                Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height },
                Rectangle{ 0, 0, (float)VIRTUAL_WIDTH * scale, (float)VIRTUAL_HEIGHT * scale },
                Vector2{ 0, 0 }, 0.0f, WHITE);
        EndDrawing();
    }

    // Unload assets from GPU memory
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}