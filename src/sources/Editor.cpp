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
#include <iomanip>
#include <string>
#include <conio.h>
#include <windows.h>

#include "ste.hpp"
#include "Editor.hpp"


#define ESC "\x1b"
#define CSI "\x1b["
#define OSC "\x1b]"


ste::Editor::Editor(const char* pathToFile)
    : _fileHandle(pathToFile), buffer(_fileHandle)
{
    _console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(_console, &_consoleMode);
    _consoleOriginalMode = _consoleMode;
    _consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(_console, _consoleMode);
    GetConsoleScreenBufferInfo(_console, &_consoleInfo);
    GetConsoleCursorInfo(_console, &_cursorInfo);

    std::cout << OSC "2;ste\x07";   // set window title
    std::cout << CSI "?1049h";      // use alternate buffer
    std::cout << CSI "5 q";         // set cursor shape
    std::cout << CSI "1;1H";        // set cursor position
}

ste::Editor::Editor(const std::string pathToFile)
    : _fileHandle(pathToFile), buffer(_fileHandle)
{
    _console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleMode(_console, &_consoleMode);
    _consoleOriginalMode = _consoleMode;
    _consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(_console, _consoleMode);
    GetConsoleScreenBufferInfo(_console, &_consoleInfo);
    GetConsoleCursorInfo(_console, &_cursorInfo);

    std::cout << OSC "2;ste\x07";   // set window title
    std::cout << CSI "?1049h";      // use alternate buffer
    std::cout << CSI "5 q";         // set cursor shape
    std::cout << CSI "1;1H";        // set cursor position
}

ste::Editor::~Editor()
{
    std::cout << CSI "?1049l";      // exit alternate buffer
    std::cout << CSI "m";           // reset text formatting
    std::cout << CSI "0 q";         // user cursor shape
    std::cout << CSI "?25h";        // show cursor
    SetConsoleMode(_console, _consoleOriginalMode);
}



void ste::Editor::start()
{
    while (_running)
    {
        display();
        keyboardHandler();
    }
}

void ste::Editor::keyboardHandler() noexcept
{
    typedef TextBuffer::Cursor Cursor;
    switch (int ch = _getch())
    {
    case 23: // save and exit (CTRL + W)
        exit();
        break;

    case 24: // don't save, exit (CTRL + X)
        exit(exit_type::no_save);
        break;

    case 19: // save (CTRL + S)
        save();
        break;

    case '\n': // enter
    case '\r':
        buffer.insertChar('\n');
        break;

    case '\t':
        buffer.insertChar(' ');
        buffer.insertChar(' ');
        buffer.insertChar(' ');
        buffer.insertChar(' ');
        break;

    case '\b': // backspace
        buffer.deleteChar();
        break;
    
    case 0: // special controls
    case 224:
        switch (_getch())
        {
        case 72: // up arrow
            buffer.moveCursorY(-1);
            break;

        case 80: // down arrow
            buffer.moveCursorY(1);
            break;

        case 75: // left arrow
            buffer.moveCursorX(-1);
            break;

        case 77: // right arrow
            buffer.moveCursorX(1);
            break;

        case 83: // delete
            buffer.moveCursorX(1);
            buffer.deleteChar();
            break;

        case 71: // home
            buffer.moveCursorX(Cursor::pos::begin);
            break;

        case 79: // end
            buffer.moveCursorX(Cursor::pos::end);
            break;

        case 119: // CTRL + home                        doesn't work everywhere
            buffer.moveCursorY(Cursor::pos::begin);
            break;

        case 117: // CTRL + end                         doesn't work everywhere
            buffer.moveCursorY(Cursor::pos::end);
            break;
        
        case 73: // page up
            buffer.moveCursorY(-20);
            break;

        case 81: // page down
            buffer.moveCursorY(20);
            break;
        
        default:
            break;
        }
        break;
    
    default:
        if ((32 <= ch && 126 >= ch))
            buffer.insertChar(ch);
        break;
    }
}

void ste::Editor::display() noexcept
{
    std::cout << CSI "?25l";        // hide cursor
    std::cout << CSI "1;1H";        // set cursor position
    GetConsoleScreenBufferInfo(_console, &_consoleInfo);

    unsigned int workspaceHeight = _consoleInfo.srWindow.Bottom + 1 - EDITOR_WORKSPACE_OFFSET_Y;
    updateTextOffset(workspaceHeight);

    unsigned int displayedLines =
        ((buffer.text.size() - _textOffset) < workspaceHeight) ? buffer.text.size() - _textOffset : workspaceHeight;


    // display top bar
    std::cout << CSI "38;2;255;255;255m";
    std::cout << CSI "48;2;45;114;135m";
    std::wcout << CSI "2K" << L"ste.exe          file: " << _fileHandle.path().c_str() << L"    lines: " << buffer.text.size();
    std::cout << CSI "m";

    // display text
    for (std::size_t i = _textOffset; i < buffer.text.size() && i < workspaceHeight + _textOffset; i++) {
        std::cout << CSI "38;2;255;255;255m";
        std::cout << CSI "48;2;45;114;135m";
        std::cout << '\n' << std::setfill(' ') << std::setw(EDITOR_WORKSPACE_OFFSET_X - 1) << i + 1 << " "; // display line number
        std::cout << CSI "m" << CSI "0K";
        std::cout << buffer.text.at(i);
    }

    // display free line indicators
    if (workspaceHeight > displayedLines) {
        std::cout << CSI "38;2;121;0;145m";
        for (unsigned int i = 0; i < workspaceHeight - displayedLines; i++)
            std::cout << '\n' << CSI "2K" << '~';
        std::cout << CSI "m";
    }


    COORD cursorPos;
    cursorPos.X = buffer.cursorPositionX() + EDITOR_WORKSPACE_OFFSET_X;
    cursorPos.Y = buffer.cursorPositionY() - _textOffset + EDITOR_WORKSPACE_OFFSET_Y;
    SetConsoleCursorPosition(_console, cursorPos);
    std::cout << CSI "?25h";        // show cursor
}

void ste::Editor::updateTextOffset(unsigned int windowHeight) noexcept
{
    if (buffer.cursorPositionY() < _textOffset)
        _textOffset = buffer.cursorPositionY();
    else if (buffer.cursorPositionY() >= _textOffset + windowHeight)
        _textOffset = buffer.cursorPositionY() - windowHeight + 1;
}

void ste::Editor::save() const
{ _fileHandle.write( buffer.text ); }

void ste::Editor::exit(exit_type type)
{
    // TODO: exception handling
    if (exit_type::save == type) _fileHandle.write( buffer.text );
    _running = false;
}


void ste::Editor::help() noexcept
{
    std::cout <<
R"(save and exit       (CTRL + W)
don't save, exit    (CTRL + X)
save                (CTRL + S))";
}