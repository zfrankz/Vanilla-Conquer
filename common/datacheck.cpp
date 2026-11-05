#include "datacheck.h"
#include "ccfile.h"
#include "paths.h"
#include "rawfile.h"
#include <stdio.h>
#include <string>

#if defined(SDL2_BUILD)
#include <SDL.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#if defined(SDL2_BUILD) && defined(__APPLE__)
#include <objc/message.h>
#include <objc/runtime.h>

// The box is shown before the game window exists, so macOS hasn't activated the app and puts it behind other windows.
static void Bring_App_To_Front()
{
    id app = ((id(*)(Class, SEL))objc_msgSend)(objc_getClass("NSApplication"), sel_registerName("sharedApplication"));
    ((void (*)(id, SEL, long))objc_msgSend)(app, sel_registerName("setActivationPolicy:"), 0); // Regular.
    ((void (*)(id, SEL, BOOL))objc_msgSend)(app, sel_registerName("activateIgnoringOtherApps:"), YES);
}
#else
static void Bring_App_To_Front()
{
}
#endif

static bool Disc_File_Available(const char* disc_file, const char* const disc_dirs[])
{
    std::string bases[3] = {Paths.User_Path(), Paths.Data_Path(), Paths.Program_Path()};

    for (int i = 0; disc_dirs[i] != nullptr; ++i) {
        for (int j = 0; j < 3; ++j) {
            std::string dir = Paths.Concatenate_Paths(bases[j].c_str(), disc_dirs[i]);
            if (RawFileClass(Paths.Concatenate_Paths(dir.c_str(), disc_file).c_str()).Is_Available()) {
                return true;
            }
        }
    }

    return false;
}

static void Show_Missing_Data(const char* game_name, const std::string& text)
{
    fprintf(stderr, "%s\n", text.c_str());

#if defined(SDL2_BUILD)
    std::string title = std::string(game_name) + " - game data not found";
    Bring_App_To_Front();
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, title.c_str(), text.c_str(), nullptr);
#elif defined(_WIN32)
    std::string title = std::string(game_name) + " - game data not found";
    MessageBoxA(nullptr, text.c_str(), title.c_str(), MB_OK | MB_ICONERROR);
#endif
}

bool Check_Game_Data(const char* game_name,
                     const char* const required[],
                     const char* disc_file,
                     const char* const disc_dirs[])
{
    std::string missing;

    for (int i = 0; required[i] != nullptr; ++i) {
        if (!CCFileClass(required[i]).Is_Available()) {
            missing += std::string("    ") + required[i] + "\n";
        }
    }

    if (disc_file != nullptr && !Disc_File_Available(disc_file, disc_dirs)) {
        missing += std::string("    ") + disc_file + " (inside one of:";
        for (int i = 0; disc_dirs[i] != nullptr; ++i) {
            missing += std::string(" ") + disc_dirs[i] + "/";
        }
        missing += ")\n";
    }

    if (missing.empty()) {
        return true;
    }

    std::string text = std::string(game_name) + " can't start because these game data files are missing:\n\n" + missing
                       + "\nCopy the files from the game discs into this folder:\n\n    " + Paths.User_Path()
                       + "\n\nLEEME.txt, next to the app, explains where to download the discs and which files to "
                         "copy.";
    Show_Missing_Data(game_name, text);

    return false;
}
