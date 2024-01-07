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

#ifndef EDITOR_H
#define EDITOR_H

#include <windows.h>

#include "ste.hpp"
#include "FileHandler.hpp"
#include "TextBuffer.hpp"


namespace ste
{
    class Editor
    {
    private:
        bool _running = true;
        unsigned int _textOffset = 0;
        FileHandler _fileHandle;

        static constexpr unsigned int  EDITOR_WORKSPACE_OFFSET_Y = 2;
        static constexpr unsigned int  EDITOR_WORKSPACE_OFFSET_X = 5;
        
        //  console porperties
        HANDLE _console;
        DWORD _consoleMode;
        DWORD _originalConsoleMode;
        CONSOLE_CURSOR_INFO _cursorInfo;
        mutable CONSOLE_SCREEN_BUFFER_INFO _consoleInfo;

        void updateTextOffset(unsigned int windowHeight) noexcept;

    public:
        Editor(const char* pathToFile);
        Editor(const std::string pathToFile);
        ~Editor();
        
        TextBuffer buffer;
        enum class exit_type {
            save,
            no_save
        };

        void start();
        void keyboardHandler() noexcept;
        void display() noexcept;
        void clearConsole() const;
        void save() const;
        void exit(exit_type type = exit_type::save);
        static void help() noexcept;
    };
} // namespace ste

#endif // EDITOR_H