#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <Windows.h>
#include <chrono>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#include <thread>

using namespace std;

// Function to load texture from file
SDL_Texture* loadTexture(const string& path, SDL_Renderer* renderer) {
    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    return newTexture;
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // horizontal offset
            int dy = radius - h; // vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
            }
        }
    }
}

class Player {
public:
    int x, y;
    

    
};
class CustomType {
public:
    int x;
    CustomType(int val) : x(val) {}
};

std::string to_stringx(const CustomType& obj) {
    std::ostringstream oss;
    oss << "CustomType(" << obj.x << ")";
    return oss.str();
}
int main(int argc, char* argv[])
{
    auto start=std::chrono::high_resolution_clock::now();
    bool firstkey=1;
    Player player{ 322,544 };

    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    // Create window
    int windowWidth = 1;
    int windowHeight = 1;
    SDL_Window* window = SDL_CreateWindow("Window", 100,  100, windowWidth, windowHeight, SDL_WINDOW_SHOWN);

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    // Load image as texture
    SDL_Texture* imageTexture = loadTexture("main.png", renderer);

    // Get the texture dimensions
    int imageWidth, imageHeight;
    SDL_QueryTexture(imageTexture, nullptr, nullptr, &imageWidth, &imageHeight);

    windowHeight = imageHeight;
    windowWidth = imageWidth;
    //resize Window to Image Size
    SDL_SetWindowSize(window, windowWidth, windowHeight);

    //load font 
    TTF_Font* font = TTF_OpenFont("TNR.ttf", 50);

    //render text to surface 
    SDL_Color color = { 0,0,0 };//black
    SDL_Surface* textsurface = TTF_RenderText_Solid(font, "Move to start Timer", color);

    //surface -> texture
    SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);

    // Define where the text will be displayed
    SDL_Rect textRect;
    textRect.x = 100;  // x position
    textRect.y = 100;  // y position
    SDL_QueryTexture(texttexture, NULL, NULL, &textRect.w, &textRect.h);  // Set width and height

    
    // Define the destination rectangle for the texture (top-left corner)
    SDL_Rect destRect = { 0, 0, windowWidth, windowHeight };

    bool quit = false;
    SDL_Event event;

    SDL_RenderCopy(renderer, imageTexture, nullptr, &destRect);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color for the circle
    drawCircle(renderer, player.x, player.y, 5); // Draw circle at player's position
    SDL_RenderCopy(renderer, texttexture, NULL, &textRect);
    SDL_RenderPresent(renderer);

    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }

            // Mouse click detection
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    cout << "Mouse clicked at: (" << mouseX << ", " << mouseY << ")" << endl;
                }
            }

            // Key press handling
           // Get the state of the keyboard
            const Uint8* state = SDL_GetKeyboardState(nullptr);

            // Check for WASD keys and update the player's position
            if (state[26] || state[22] || state[4] || state[7])
            {
                if (firstkey == 1)
                {
                    firstkey = 0;
                }

                if (state[SDL_SCANCODE_W]) player.y -= 1;
                if (state[SDL_SCANCODE_S]) player.y += 1;
                if (state[SDL_SCANCODE_A]) player.x -= 1;
                if (state[SDL_SCANCODE_D]) player.x += 1;

                SDL_RenderCopy(renderer, imageTexture, nullptr, &destRect);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Set color for the circle
                drawCircle(renderer, player.x, player.y, 5); // Draw circle at player's position
                SDL_RenderPresent(renderer);
            }

            if (firstkey == 0)
            {

                auto now = std::chrono::high_resolution_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
                SDL_Surface* textsurface = TTF_RenderText_Solid(font, to_string(elapsed.count()).c_str(), color);
                //surface -> texture
                SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                SDL_RenderCopy(renderer, texttexture, 0, &textRect);
                SDL_RenderPresent(renderer);
            }
            for (size_t i = 0; i < 20; i++)
            {
                if (player.x == 392 + i)
                {
                    for (size_t i = 0; i < 20; i++)
                    {
                        if (player.y == 550 + i)
                        {
                            auto now = std::chrono::high_resolution_clock::now();
                            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
                            color = { 255,0,0 };
                            SDL_Surface* textsurface = TTF_RenderText_Solid(font,  to_string(elapsed.count()).c_str(), color);
                            SDL_Texture* texttexture = SDL_CreateTextureFromSurface(renderer, textsurface);
                            SDL_RenderCopy(renderer, texttexture, 0, &textRect);
                            SDL_RenderPresent(renderer);
                            std::this_thread::sleep_for(std::chrono::seconds(10));
                            // Cleanup
                            SDL_DestroyTexture(imageTexture);
                            SDL_DestroyRenderer(renderer);
                            SDL_DestroyWindow(window);
                            IMG_Quit();
                            SDL_Quit();
                            
                            return 0;

                        }
                    }


                }
            }
        
        }
        
    }
    // Cleanup
    SDL_DestroyTexture(imageTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
