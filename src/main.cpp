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

#include "ste.hpp"



int main(int argc, char const *argv[])
{
    if (2 > argc) return 1;

    SetConsoleCP( 65001 );
    SetConsoleOutputCP( 65001 );
    setlocale( LC_ALL, "65001" );
    
    if (2 == argc && ( strcmp(argv[1], "--version") || strcmp(argv[1], "-v") ))
    {
        std::cout << 
R"(ste.exe 1.0.0
Copyright (C) 2023  Paweł Rapacz
This program comes with ABSOLUTELY NO WARRANTY, without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. This is free software,
and you are welcome to redistribute it under certain conditions.)" << std::endl;
        return 0;
    }
    else {
        std::cout << "Hello👋 World🌎!\n🦢 Zażółć gęślą jaźń" << std::endl;
        return 0;
    }
}
