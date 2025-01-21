#include <array>
#include <algorithm>
#include <iostream>
#include <vector>
#include <memory>
#include <cstring>
#include <filesystem>
#include <fstream>

#include <SDL.h>

#ifdef __WIIU__
#include <coreinit/filesystem.h>
#include <gx2/context.h>
#include <nn/swkbd.h>
#include <vpad/input.h>
#include <sys/iosupport.h>
#include <whb/log.h>
#include <whb/log_module.h>
#include <whb/log_udp.h>
#include <coreinit/memory.h>
#include <padscore/kpad.h>
#endif

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_sdlrenderer2.h"


#include "show.hpp"


using std::cout;
using std::endl;
using std::filesystem::path;


#ifdef __WIIU__
ssize_t
write_msg_to_whb_log(struct _reent*,
                     void*,
                     const char* buf,
                     size_t len)
    noexcept
{
    try {
        std::string msg(buf, len);
        WHBLogWrite(msg.data());
        return len;
    }
    catch (...) {
        return -1;
    }
}


__attribute__(( __constructor__ ))
void
init_stdout()
{
    WHBLogModuleInit();
    WHBLogUdpInit();
    static devoptab_t dev;
    dev.name = "STDOUT";
    dev.structSize = sizeof dev;
    dev.write_r = write_msg_to_whb_log;
    devoptab_list[STD_OUT] = &dev;
}

__attribute__(( __destructor__ ))
void
fini_stdout()
{
    WHBLogUdpDeinit();
    WHBLogModuleDeinit();
}
#endif



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


#if 0
bool
ImGui_ImplSDL2_ProcessEventEx(const SDL_Event* event)
{
    if (ImGui_ImplSDL2_ProcessEvent(event))
        return true;

    ImGuiIO& io = ImGui::GetIO();
    ImGuiViewport* vp = ImGui::GetMainViewport();

    switch (event->type) {

    case SDL_FINGERMOTION:
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent(event->tfinger.x * vp->Size.x,
                            event->tfinger.y * vp->Size.y);
        break;

    case SDL_FINGERDOWN:
    case SDL_FINGERUP:
        io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
        io.AddMousePosEvent(event->tfinger.x * vp->Size.x,
                            event->tfinger.y * vp->Size.y);
        io.AddMouseButtonEvent(ImGuiMouseButton_Left, event->type == SDL_FINGERDOWN);
        break;

    } // switch
    return false;
}
#endif


#ifdef __WIIU__


void
set_button(VPADStatus& vpad, VPADButtons flag, bool pressed)
{
    if (pressed) {
        vpad.hold    |= flag;
        vpad.trigger |= flag;
        vpad.release &= ~flag;
    } else {
        vpad.hold    &= ~flag;
        vpad.trigger &= ~flag;
        vpad.release |= flag;
    }
}


void
translate_buttons(const SDL_ControllerButtonEvent& event,
                  VPADStatus& vpad)
{
    bool pressed = event.state == SDL_PRESSED;

    switch (event.button) {
    case SDL_CONTROLLER_BUTTON_A:
        set_button(vpad, VPAD_BUTTON_A, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_B:
        set_button(vpad, VPAD_BUTTON_B, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_X:
        set_button(vpad, VPAD_BUTTON_X, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_Y:
        set_button(vpad, VPAD_BUTTON_Y, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_BACK:
        set_button(vpad, VPAD_BUTTON_MINUS, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_GUIDE:
        set_button(vpad, VPAD_BUTTON_HOME, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_START:
        set_button(vpad, VPAD_BUTTON_PLUS, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_LEFTSTICK:
        set_button(vpad, VPAD_BUTTON_STICK_L, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
        set_button(vpad, VPAD_BUTTON_STICK_R, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
        set_button(vpad, VPAD_BUTTON_L, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
        set_button(vpad, VPAD_BUTTON_R, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_UP:
        set_button(vpad, VPAD_BUTTON_UP, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
        set_button(vpad, VPAD_BUTTON_DOWN, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
        set_button(vpad, VPAD_BUTTON_LEFT, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
        set_button(vpad, VPAD_BUTTON_RIGHT, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_MISC1:
        set_button(vpad, VPAD_BUTTON_TV, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_PADDLE1:
        set_button(vpad, VPAD_BUTTON_ZL, pressed);
        break;
    case SDL_CONTROLLER_BUTTON_PADDLE2:
        set_button(vpad, VPAD_BUTTON_ZR, pressed);
        break;
    }
}


void
set_button(KPADStatus& kpad, WPADButton flag, bool pressed)
{
    if (pressed) {
        kpad.hold    |= flag;
        kpad.trigger |= flag;
        kpad.release &= ~flag;
    } else {
        kpad.hold    &= ~flag;
        kpad.trigger &= ~flag;
        kpad.release |= flag;
    }
}


void
translate_buttons(const SDL_ControllerButtonEvent& event,
                  KPADStatus& kpad)
{
    bool pressed = event.state == SDL_PRESSED;

    switch (event.button) {
        case SDL_CONTROLLER_BUTTON_A:
            set_button(kpad, WPAD_BUTTON_A, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_B:
            set_button(kpad, WPAD_BUTTON_B, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_X:
            set_button(kpad, WPAD_BUTTON_1, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_Y:
            set_button(kpad, WPAD_BUTTON_2, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_BACK:
            set_button(kpad, WPAD_BUTTON_MINUS, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_GUIDE:
            set_button(kpad, WPAD_BUTTON_HOME, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_START:
            set_button(kpad, WPAD_BUTTON_PLUS, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            set_button(kpad, WPAD_BUTTON_LEFT, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            set_button(kpad, WPAD_BUTTON_RIGHT, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            set_button(kpad, WPAD_BUTTON_UP, pressed);
            break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            set_button(kpad, WPAD_BUTTON_DOWN, pressed);
            break;
    }
}


void
populate_pads(const SDL_Event& event,
              VPADStatus& vpad,
              std::array<KPADStatus, 4>& kpads)
{
    ImGuiViewport* vp = ImGui::GetMainViewport();

    switch (event.type) {

        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
            vpad.tpNormal.touched = event.type == SDL_FINGERUP ? 0 : 1;
            vpad.tpNormal.validity = VPAD_VALID;
            vpad.tpNormal.x = event.tfinger.x * vp->Size.x;
            vpad.tpNormal.y = event.tfinger.y * vp->Size.y;
            break;

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            {
                auto controller = SDL_GameControllerFromInstanceID(event.cbutton.which);
                if (controller) {
                    int idx = SDL_GameControllerGetPlayerIndex(controller);
                    if (idx == 0)
                        translate_buttons(event.cbutton, vpad);
                    else
                        translate_buttons(event.cbutton, kpads[idx - 1]);
                }
            }
            break;
    }
}


std::u32string
to_utf32(const std::u8string& input)
{
    std::u32string output;
    char32_t out_c = 0;
    unsigned need = 0;

    constexpr char8_t one_prefix = 0x0000'0000;
    constexpr char8_t one_pmask  = 0b1000'0000;

    constexpr char8_t two_prefix = 0b1100'0000;
    constexpr char8_t two_pmask  = 0b1110'0000;
    constexpr char8_t two_dmask  = 0b0001'1111;

    constexpr char8_t three_prefix = 0b1110'0000;
    constexpr char8_t three_pmask  = 0b1111'0000;
    constexpr char8_t three_dmask  = 0b0000'1111;

    constexpr char8_t four_prefix = 0b1111'0000;
    constexpr char8_t four_pmask  = 0b1111'1000;
    constexpr char8_t four_dmask  = 0b0000'0111;

    constexpr char8_t cont_prefix = 0b1000'0000;
    constexpr char8_t cont_pmask  = 0b1100'0000;
    constexpr char8_t cont_dmask  = 0b0011'1111;

    for (char8_t c : input) {
        // if (!c) // null terminator, stop early
        //     break;
        if ((c & one_pmask) == one_prefix) {
            output.push_back(c);
        } else if ((c & two_pmask) == two_prefix) {
            out_c = c & two_dmask;
            need = 1;
        } else if ((c & three_pmask) == three_prefix) {
            out_c = c & three_dmask;
            need = 2;
        } else if ((c & four_pmask) == four_prefix) {
            out_c = c & four_dmask;
            need = 3;
        } else if ((c & cont_pmask) == cont_prefix) {
            if (need == 0)
                throw std::runtime_error{"invalid UTF-8 input"};
            out_c <<= 6;
            out_c |= c & cont_dmask;
            if (--need == 0)
                output.push_back(out_c);
        }
    }

    return output;
}


std::u8string
to_utf8(const std::u32string& input)
{
    std::u8string output;

    constexpr char8_t cont_mask   = 0b0011'1111;
    constexpr char8_t cont_prefix = 0b1000'0000;

    for (char32_t c : input) {
        // if (!c) // null terminator, stop early
        //     break;
        if (c < 0x0080) {
            output.push_back(c);
        } else if (c < 0x0800) {
            output.push_back(0b1100'0000 | (c >> 6));
            output.push_back(cont_prefix | (c & cont_mask));
        } else if (c < 0x010000) {
            output.push_back(0b1110'0000 |  (c >> 12));
            output.push_back(cont_prefix | ((c >>  6) & cont_mask));
            output.push_back(cont_prefix | ((c >>  0) & cont_mask));
        } else if (c < 0x110000) {
            output.push_back(0b1111'0000 |  (c >> 18));
            output.push_back(cont_prefix | ((c >> 12) & cont_mask));
            output.push_back(cont_prefix | ((c >>  6) & cont_mask));
            output.push_back(cont_prefix | ((c >>  0) & cont_mask));
        } else {
            throw std::runtime_error{"invalid UTF-32 input"};
        }
    }

    return output;
}


std::u16string
to_utf16(const std::u32string& input)
{
    std::u16string output;

    for (char32_t c : input) {
        // if (!c) // null terminator, stop early
        //     break;
        if (c < 0x10000) {
            if (c >= 0xd800 && c < 0xe000)
                throw std::runtime_error{"cannot encode this codepoint to UTF-16"};
            output.push_back(c);
        } else if (c < 0x110000) {
            char32_t u_prime = c - 0x1'0000;
            char16_t high = (u_prime >> 10) + 0xd800;
            char16_t low  = (u_prime & 0x3ff) + 0xdc00;
            output.push_back(high);
            output.push_back(low);
        } else {
            throw std::runtime_error{"invalid UTF-32 input"};
        }

    }

    return output;
}


std::u16string
to_utf16(const std::u8string& input)
{
    return to_utf16(to_utf32(input));
}

#endif // __WIIU__


int main(int, char* [])
try
{
#ifdef __WIIU__
    FSInit();
#endif


    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER | SDL_INIT_AUDIO)) {
        cout << "Error in SDL_Init(): " << SDL_GetError() << endl;
        return -1;
    }

    SDL_SetHint(SDL_HINT_TOUCH_MOUSE_EVENTS, "1");

    SDL_EventState(SDL_CLIPBOARDUPDATE, SDL_ENABLE);

    SDL_Window* window = SDL_CreateWindow("ImGui Test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          // 854, 480,
                                          1280, 720,
                                          0 /*SDL_WINDOW_WIIU_GAMEPAD_ONLY*/);
    if (!window) {
        cout << "Error in SDL_CreateWindow(): " << SDL_GetError() << endl;
        return -1;
    }


    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cout << "Error in SDL_CreateRenderer(): " << SDL_GetError() << endl;
        return -1;
    }

    std::vector<SDL_GameController*> controllers;


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

    ImVector<ImWchar> glyph_ranges;
    ImFontGlyphRangesBuilder ranges_builder;
    ranges_builder.AddRanges(io.Fonts->GetGlyphRangesDefault());
#if 1
    ranges_builder.AddRanges(io.Fonts->GetGlyphRangesGreek());
    ranges_builder.AddRanges(io.Fonts->GetGlyphRangesCyrillic());
    ranges_builder.AddRanges(io.Fonts->GetGlyphRangesJapanese());

    ranges_builder.AddChar(U'\u0192'); // ƒ
    ranges_builder.AddChar(U'\u20ac'); // €
    ranges_builder.AddChar(U'\u20af'); // ₯

    // Spacing Modifier Letters
    for (char32_t c = 0x02b0; c < 0x0300; ++c)
        ranges_builder.AddChar(c);
    // General Punctuation block
    for (char32_t c = 0x2000; c < 0x2070; ++c)
        ranges_builder.AddChar(c);
    // Letterlike Symbols block
    for (char32_t c = 0x2100; c < 0x2150; ++c)
        ranges_builder.AddChar(c);
    // Number Forms block
    for (char32_t c = 0x2150; c < 0x2190; ++c)
        ranges_builder.AddChar(c);
    // Arrows block
    for (char32_t c = 0x2190; c < 0x2200; ++c)
        ranges_builder.AddChar(c);
    // Mathematical Operators block
    for (char32_t c = 0x2200; c < 0x2300; ++c)
        ranges_builder.AddChar(c);
    // Enclosed Alphanumerics block
    for (char32_t c = 0x2460; c < 0x2500; ++c)
        ranges_builder.AddChar(c);
    // Box Drawing block
    for (char32_t c = 0x2500; c < 0x2580; ++c)
        ranges_builder.AddChar(c);
    // Block Elements block
    for (char32_t c = 0x2580; c < 0x25a0; ++c)
        ranges_builder.AddChar(c);
    // Geometric Shapes block
    for (char32_t c = 0x25a0; c < 0x2600; ++c)
        ranges_builder.AddChar(c);
    // Miscellaneous Symbols block
    for (char32_t c = 0x2600; c < 0x2700; ++c)
        ranges_builder.AddChar(c);
    // Dingbats block
    for (char32_t c = 0x2700; c < 0x27c0; ++c)
        ranges_builder.AddChar(c);

    // Private Use Area block
    for (char32_t c = 0xe000; c < 0xe100; ++c)
        ranges_builder.AddChar(c);
#endif

    ranges_builder.BuildRanges(&glyph_ranges);


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
                                   40,
                                   &cafe_cfg,
                                   glyph_ranges.Data);

    ImFontConfig symbola_cfg;
    symbola_cfg.MergeMode = true;
#ifdef __WIIU__
    // if running the .rpx, outside the .wuhb archive, disable this
    // io.Fonts->AddFontFromFileTTF("/vol/content/Symbola.ttf", 50,
    //                              &symbola_cfg, glyph_ranges.Data);
#else
    io.Fonts->AddFontFromFileTTF("Symbola.ttf", 50,
                                 &symbola_cfg, glyph_ranges.Data);
#endif

    //ImGui::StyleColorsDark();


#ifdef __WIIU__

    nn::swkbd::CreateArg kbd_create_arg;
    std::vector<char> kbd_memory(nn::swkbd::GetWorkMemorySize(0));
    kbd_create_arg.workMemory = kbd_memory.data();
    auto kbd_fsclient = std::make_unique<FSClient>();
    FSAddClient(kbd_fsclient.get(), FS_ERROR_FLAG_NONE);
    auto kbd_fscmdblock = std::make_unique<FSCmdBlock>();
    FSInitCmdBlock(kbd_fscmdblock.get());
    kbd_create_arg.fsClient = kbd_fsclient.get();
    kbd_create_arg.regionType = nn::swkbd::RegionType::USA;
    if (!nn::swkbd::Create(kbd_create_arg))
        throw std::runtime_error{"failed to create swkbd"};

#endif

    auto& style = ImGui::GetStyle();
    style.ScrollbarSize = 50;
    style.GrabMinSize = 20;
    // style.TouchExtraPadding = ImVec2(8.0f, 8.0f);

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);


    cout << "Running main loop..." << endl;

    bool done = false;
    while (!done) {

#ifdef __WIIU__
        static VPADStatus vpad;
        static std::array<KPADStatus, 4> kpads;

        // clear trigger/release fields from previous frame
        vpad.trigger = 0;
        vpad.release = 0;
        for (auto& kpad : kpads) {
            kpad.trigger = 0;
            kpad.release = 0;
        }

#endif

#ifdef __WIIU__

        if (io.WantTextInput) {
            if (nn::swkbd::GetStateInputForm() == nn::swkbd::State::Hidden) {
                nn::swkbd::AppearArg kbd_appear;
                cout << "Bringing up swkbd" << endl;
                std::u16string text;
                auto state = ImGui::GetInputTextState(ImGui::GetActiveID());
                if (state) {
                    auto text_utf8 = reinterpret_cast<char8_t*>(state->TextA.Data);
                    unsigned text_size = state->TextLen;
                    text = to_utf16(std::u8string(text_utf8, text_size));
                    kbd_appear.inputFormArg.initialText = text.data();
                }
                nn::swkbd::AppearInputForm(kbd_appear);
            }
        } else {
            if (nn::swkbd::GetStateInputForm() == nn::swkbd::State::Visible) {
                cout << "Hiding swkbd" << endl;
                nn::swkbd::DisappearInputForm();
            }
        }

        nn::swkbd::ControllerInfo info;
        info.vpad = &vpad;
        // info.kpad[0] = &kpads[0];
        // info.kpad[1] = &kpads[1];
        // info.kpad[2] = &kpads[2];
        // info.kpad[3] = &kpads[3];
        nn::swkbd::Calc(info);

        if (nn::swkbd::IsNeedCalcSubThreadFont())
            nn::swkbd::CalcSubThreadFont();

        if (nn::swkbd::IsNeedCalcSubThreadPredict())
            nn::swkbd::CalcSubThreadPredict();

        if (nn::swkbd::IsDecideOkButton(nullptr)) {
            cout << "Confirming SWKBD input!" << endl;
            if (auto state = ImGui::GetInputTextState(ImGui::GetActiveID()))
                state->ClearText();
            const char16_t* str = nn::swkbd::GetInputFormString();
            for (int i = 0; str[i]; ++i)
                io.AddInputCharacterUTF16(str[i]);
            auto button = io.ConfigNavSwapGamepadButtons
                ? ImGuiKey_GamepadFaceRight
                : ImGuiKey_GamepadFaceDown;
            io.ClearInputMouse();
            io.ClearInputKeys();
            io.AddKeyEvent(button, true);
            io.AddKeyEvent(button, false);
        }

        if (nn::swkbd::IsDecideCancelButton(nullptr)) {
            cout << "Canceling SWKBD input!" << endl;
            auto button = io.ConfigNavSwapGamepadButtons
                ? ImGuiKey_GamepadFaceDown
                : ImGuiKey_GamepadFaceRight;
            io.ClearInputMouse();
            io.ClearInputKeys();
            io.AddKeyEvent(button, true);
            io.AddKeyEvent(button, false);
        }

#endif // __WIIU__


        SDL_Event event;
        while (SDL_PollEvent(&event)) {

            show(event);

#ifdef __WIIU__
            if (nn::swkbd::GetStateInputForm() != nn::swkbd::State::Visible)
                ImGui_ImplSDL2_ProcessEvent(&event);
#else
            ImGui_ImplSDL2_ProcessEvent(&event);
#endif

#ifdef __WIIU__
            populate_pads(event, vpad, kpads);
#endif

            switch (event.type) {

                case SDL_QUIT:
                    done = true;
                    goto stop_events; // avoid handling bogus events

                case SDL_CONTROLLERDEVICEADDED:
                {
                    auto ctr = SDL_GameControllerOpen(event.cdevice.which);
                    if (ctr) {
                        cout << "Controller opened: " << SDL_GameControllerName(ctr) << endl;
                        controllers.push_back(ctr);
                    }
                    break;
                }
                case SDL_CONTROLLERDEVICEREMOVED:
                {
                    auto ctr = SDL_GameControllerFromInstanceID(event.cdevice.which);
                    if (ctr) {
                        cout << "removed controller: " << SDL_GameControllerName(ctr) << endl;
                        SDL_GameControllerClose(ctr);
                        std::erase(controllers, ctr);
                    }
                    break;
                }

#if 0 // disable to test if SDL_HINT_TOUCH_MOUSE_EVENTS is working
                case SDL_FINGERMOTION:
                {
                    ImGuiViewport* vp = ImGui::GetMainViewport();
                    IM_ASSERT(vp);
                    io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
                    io.AddMousePosEvent(event.tfinger.x * vp->Size.x,
                                        event.tfinger.y * vp->Size.y);
                    break;
                }

                case SDL_FINGERDOWN:
                case SDL_FINGERUP:
                {
                    ImGuiViewport* vp = ImGui::GetMainViewport();
                    IM_ASSERT(vp);
                    io.AddMouseSourceEvent(ImGuiMouseSource_TouchScreen);
                    io.AddMousePosEvent(event.tfinger.x * vp->Size.x,
                                        event.tfinger.y * vp->Size.y);
                    io.AddMouseButtonEvent(ImGuiMouseButton_Left, event.type == SDL_FINGERDOWN);
                    break;
                }
#endif

            } // switch


        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();

        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::EndFrame();



        ImGui::Render();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);

#ifdef __WIIU__
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderDrawPoint(renderer, 1, 1);
        SDL_RenderFlush(renderer);
        GX2SetContextState(nullptr);
        nn::swkbd::DrawDRC();
#endif // __WIIU__

        SDL_RenderPresent(renderer);
    }

 stop_events:

    cout << "Stopped processing events" << endl;


    for (auto& c : controllers)
        SDL_GameControllerClose(c);
    controllers.clear();


    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();


    ImGui::DestroyContext();


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

#ifdef __WIIU__
    nn::swkbd::Destroy();
    FSDelClient(kbd_fsclient.get(), FS_ERROR_FLAG_NONE);

    FSShutdown();
#endif

    cout << "Returning from main()" << endl;

    return 0;
}
catch (std::exception& e) {
    cout << "Exception: " << e.what() << endl;
    return -1;
}
