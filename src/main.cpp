#include <algorithm>
#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

#include <SDL.h>

#ifdef __WIIU__
#include <coreinit/memory.h>
#include <sysapp/title.h>
#endif

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"


#include "sdl_debug.hpp"


using std::cout;
using std::endl;
using std::filesystem::path;


std::vector<char>
load_file(const path& filename)
{
    auto size = file_size(filename);
    std::vector<char> result(size);
    std::filebuf fb;
    if (!fb.open(filename, std::ios::in | std::ios::binary))
        throw std::runtime_error{"failed to open " + filename.string()};
    std::size_t read = fb.sgetn(result.data(), result.size());
    if (read < result.size())
        result.resize(read);
    return result;
}


ImVector<ImWchar>
build_glyph_range()
{
    ImVector<ImWchar> result;
    ImFontGlyphRangesBuilder builder;
    builder.AddRanges(ImFontAtlas::GetGlyphRangesDefault());
#if 1
    builder.AddRanges(ImFontAtlas::GetGlyphRangesGreek());
    builder.AddRanges(ImFontAtlas::GetGlyphRangesCyrillic());
    builder.AddRanges(ImFontAtlas::GetGlyphRangesJapanese());

    builder.AddChar(U'ƒ');
    builder.AddChar(U'€');
    builder.AddChar(U'₯');

    // Spacing Modifier Letters
    for (char32_t c = 0x02b0; c < 0x0300; ++c)
        builder.AddChar(c);
    // General Punctuation block
    for (char32_t c = 0x2000; c < 0x2070; ++c)
        builder.AddChar(c);
    // Letterlike Symbols block
    for (char32_t c = 0x2100; c < 0x2150; ++c)
        builder.AddChar(c);
    // Number Forms block
    for (char32_t c = 0x2150; c < 0x2190; ++c)
        builder.AddChar(c);
    // Arrows block
    for (char32_t c = 0x2190; c < 0x2200; ++c)
        builder.AddChar(c);
    // Mathematical Operators block
    for (char32_t c = 0x2200; c < 0x2300; ++c)
        builder.AddChar(c);
    // Enclosed Alphanumerics block
    for (char32_t c = 0x2460; c < 0x2500; ++c)
        builder.AddChar(c);
    // Box Drawing block
    for (char32_t c = 0x2500; c < 0x2580; ++c)
        builder.AddChar(c);
    // Block Elements block
    for (char32_t c = 0x2580; c < 0x25a0; ++c)
        builder.AddChar(c);
    // Geometric Shapes block
    for (char32_t c = 0x25a0; c < 0x2600; ++c)
        builder.AddChar(c);
    // Miscellaneous Symbols block
    for (char32_t c = 0x2600; c < 0x2700; ++c)
        builder.AddChar(c);
    // Dingbats block
    for (char32_t c = 0x2700; c < 0x27c0; ++c)
        builder.AddChar(c);

    // Private Use Area block
    for (char32_t c = 0xe000; c < 0xe100; ++c)
        builder.AddChar(c);
#endif

    builder.BuildRanges(&result);
    return result;
}


int screen_width;
int screen_height;


#ifdef __WIIU__

bool enable_swkbd = true;


void
show_wiiu_options()
{
    ImGui::SetNextWindowPos(ImVec2(screen_width, screen_height),
                            ImGuiCond_Always,
                            {1.0f, 1.0f});
    if (ImGui::Begin("SWKBD options",
                     nullptr,
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse |
                     ImGuiWindowFlags_NoMove |
                     ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoSavedSettings |
                     ImGuiWindowFlags_NoTitleBar)) {

        if (ImGui::Checkbox("Enable SWKBD", &enable_swkbd))
            SDL_WiiUSetSWKBDEnabled(enable_swkbd ? SDL_TRUE : SDL_FALSE);
    }
    ImGui::End();
}

#endif


int main(int, char* [])
try
{
#ifdef __WIIU__
    SYSCheckTitleExists(0);
#endif

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO)) {
        cout << "Error in SDL_Init(): " << SDL_GetError() << endl;
        return -1;
    }

#ifdef __WIIU__
    // To stop the boot sound, we must open an audio device.
    SDL_AudioSpec desired{};
    desired.freq = 44100;
    desired.format = AUDIO_S16SYS;
    desired.channels = 2;
    desired.samples = 1024;
    SDL_AudioSpec obtained;
    auto audio_dev = SDL_OpenAudioDevice(nullptr,
                                         false,
                                         &desired,
                                         &obtained,
                                         true);
#endif

    SDL_EventState(SDL_CLIPBOARDUPDATE, SDL_ENABLE);

    SDL_Window* window = SDL_CreateWindow("ImGui Test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          // 854, 480,
                                          //1280, 720,
                                          0, 0,
                                          SDL_WINDOW_FULLSCREEN);
    if (!window) {
        cout << "Error in SDL_CreateWindow(): " << SDL_GetError() << endl;
        return -1;
    }

    SDL_GetWindowSize(window, &screen_width, &screen_height);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Error in SDL_CreateRenderer(): " << SDL_GetError() << endl;
        return -1;
    }

    SDL_RenderSetLogicalSize(renderer, screen_width, screen_height);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigNavSwapGamepadButtons = false;

#ifdef __WIIU__
       ImGui::LoadIniSettingsFromDisk("/vol/content/imgui.ini");
#endif
    io.IniFilename = nullptr;
    io.LogFilename = nullptr;

    ImVector<ImWchar> glyph_ranges = build_glyph_range();
    ImFontConfig cafe_cfg;
    void* cafe_font_ptr = nullptr;
    uint32_t cafe_font_size = 0;
    cafe_cfg.FontDataOwnedByAtlas = false;
#ifdef __WIIU__
    OSGetSharedData(OS_SHAREDDATATYPE_FONT_STANDARD, 0, &cafe_font_ptr, &cafe_font_size);
#else
    auto cafe_blob = load_file("CafeStd.ttf");
    cafe_font_ptr = cafe_blob.data();
    cafe_font_size = cafe_blob.size();
#endif
    io.Fonts->AddFontFromMemoryTTF(cafe_font_ptr,
                                   cafe_font_size,
                                   50,
                                   &cafe_cfg,
                                   glyph_ranges.Data);

    ImFontConfig symbola_cfg;
    symbola_cfg.MergeMode = true;
#ifdef __WIIU__
    // if running the .rpx, outside the .wuhb archive, disable this
    io.Fonts->AddFontFromFileTTF("/vol/content/Symbola.ttf", 60,
                                 &symbola_cfg, glyph_ranges.Data);
#else
    io.Fonts->AddFontFromFileTTF("Symbola.ttf", 60,
                                 &symbola_cfg, glyph_ranges.Data);
#endif

    //ImGui::StyleColorsDark();

    auto& style = ImGui::GetStyle();
    style.ScrollbarSize = 50;
    style.GrabMinSize = 20;
    // style.TouchExtraPadding = ImVec2(8.0f, 8.0f);

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);


    cout << "Running main loop..." << endl;

    bool done = false;
    while (!done) {

        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            sdl::debug::print(event);

            ImGui_ImplSDL2_ProcessEvent(&event);

            switch (event.type) {

                case SDL_QUIT:
                    done = true;
                    break;

                case SDL_CONTROLLERDEVICEADDED:
                {
                    auto ctr = SDL_GameControllerOpen(event.cdevice.which);
                    if (ctr)
                        cout << "Controller opened: " << SDL_GameControllerName(ctr) << endl;
                    break;
                }
                case SDL_CONTROLLERDEVICEREMOVED:
                {
                    auto ctr = SDL_GameControllerFromInstanceID(event.cdevice.which);
                    if (ctr) {
                        cout << "removed controller: " << SDL_GameControllerName(ctr) << endl;
                        SDL_GameControllerClose(ctr);
                    }
                    break;
                }
            } // switch


        }
        // if (SDL_IsTextInputActive()) {
        //     cout << "text input is active" << endl;
        // }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

#ifdef __WIIU__
        show_wiiu_options();
#endif

        ImGui::EndFrame();

        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 128, 104, 0, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

#ifdef __WIIU__
        // WORKAROUND: SDL does not update clipping until the next draw call.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawPoint(renderer, 1, 1);
#endif


        SDL_RenderPresent(renderer);
    }

    cout << "Stopped processing events" << endl;


    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();


    ImGui::DestroyContext();


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

#ifdef __WIIU__
    if (audio_dev)
        SDL_CloseAudioDevice(audio_dev);
#endif

    SDL_Quit();

    cout << "Returning from main()" << endl;

    return 0;
}
catch (std::exception& e) {
    cout << "Exception: " << e.what() << endl;
    return -1;
}
