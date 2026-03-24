#pragma once
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <Windows.h>
#include <limits> // Required for numeric_limits

#ifdef _WIN32
#include <stdlib.h> // For _putenv_s
#endif

// This function executes the patch commands for a specified C4D version (e.g., "2025" or "2026")
void execute_rspatch_commands(const std::string& version) {
    std::cout << "\n--- Starting Patch Execution for Cinema 4D " << version << " ---" << std::endl;

    // 1. Set the PERLIO environment variable (SET PERLIO=:unix:perlio)
#ifdef _WIN32
    if (_putenv_s("PERLIO", ":unix:perlio") != 0) {
        std::cerr << "Error setting PERLIO environment variable. Patching may fail." << std::endl;
    }
    else {
        std::cout << "Environment variable PERLIO set successfully for this process." << std::endl;
    }
#endif

    // The base Perl command, using a C++ raw string literal (R"()") for the complex hex pattern.
    const std::string perl_command_base =
        R"(%TEMP%\perl.exe -0777 -w -pi -e "s/\xC7\x40\x58\xDC\xFF\xFF\xFF\x33\xC0\xEB\x6D\x44\x8B\x4C\x24\x68\x45\x33\xC0\x48\x8B\x54\x24\x48\x48\x8B\x4C\x24\x40\xE8..../\xC7\x40\x58\xDC\xFF\xFF\xFF\x33\xC0\xEB\x6D\x44\x8B\x4C\x24\x68\x45\x33\xC0\x48\x8B\x54\x24\x48\x48\x8B\x4C\x24\x40\xB8\x00\x00\x00\x00/g" )";

    // 2. List of target files. Paths are constructed using the selected version.
    const std::vector<std::string> files_to_patch = {
        // C4D Core Lib
       // R"("c:\Program Files\Maxon Cinema 4D )" + version + R"(\corelibs\c4d_base.xdl64")",

        // Redshift Core DLL (C4D Install Folder)
        R"("c:\Program Files\Maxon Cinema 4D )" + version + R"(\Redshift\res\libs\win64\redshift-core-vc140.dll")",

        // Redshift Licensing Tool (ProgramData)
        R"("c:\ProgramData\redshift\Tools\RedshiftLicensingTool.exe")",

        // Altus CLI (ProgramData)
        R"("c:\ProgramData\redshift\bin\altus-cli.exe")",

        // Redshift Core DLL (ProgramData)
        R"("c:\ProgramData\redshift\bin\redshift-core-vc140.dll")",

        // Redshift Core DLL (C4D Plugins Folder)
        R"("c:\Program Files\Maxon Cinema 4D )" + version + R"(\plugins\Redshift\res\libs\win64\redshift-core-vc140.dll")"
    };

    std::cout << "\nExecuting 6 binary patching operations for version " << version << "..." << std::endl;

    // 3. Loop through all files and execute the Perl command for each.
    for (size_t i = 0; i < files_to_patch.size(); ++i) {
        // Construct the full command: base + file path
        std::string command = perl_command_base + files_to_patch[i];

        // Extract the filename for display
        size_t last_slash = files_to_patch[i].find_last_of("\\/");
        std::string display_name = (last_slash == std::string::npos) ? files_to_patch[i] : files_to_patch[i].substr(last_slash + 1);

        std::cout << "\n(" << i + 1 << "/" << files_to_patch.size() << ") Patching: " << display_name << std::endl;

        // Execute the command using the system shell
        int result = std::system(command.c_str());

        if (result == 0) {
            std::cout << "-> SUCCESS: Patch applied." << std::endl;
        }
        else {
            std::cerr << "-> FAILURE: Command returned error code " << result << ". Ensure you have run as Administrator and files exist." << std::endl;
        }
    }

    MessageBoxA(0, "Redshift License Activated!", "Success", MB_OK);
}
