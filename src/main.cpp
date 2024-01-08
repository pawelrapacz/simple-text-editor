/*
    ste - (simple text editor) a program that helps you edit text files 
    Copyright (C) 2023  Paweł Rapacz

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <windows.h>

#include "config.hpp"
#include "Editor.hpp"


int main(int argc, char const *argv[])
{
    SetConsoleCP( 65001 );
    SetConsoleOutputCP( 65001 );
    setlocale( LC_ALL, "65001" );

    if (2 != argc) {
        std::cout << "You have to give a file/filename to work on" << std::endl;
        return EXIT_FAILURE;
    }

    std::string arg = argv[1];
    if ( arg == "--version" || arg ==  "-v" ) {
        std::cout <<
        "ste.exe " << STE_VERSION << '\n' <<
        "Copyright (C) " << STE_PUBLICATION_YEAR << " Paweł Rapacz\n" <<
        "This program comes with ABSOLUTELY NO WARRANTY, without even the implied warranty of\n" <<
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. This is free software,\n" <<
        "and you are welcome to redistribute it under certain conditions." << std::endl;
    }
    else if ( arg == "--help" || arg ==  "-h" ) {
        ste::Editor::help();
    }
    else {
        /* MAIN FUNCTIONALITY */
        ste::Editor editor(arg);
        editor.start();
    }

    return EXIT_SUCCESS;
}