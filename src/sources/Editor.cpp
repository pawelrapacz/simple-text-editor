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


// windows console colors
#define BLACK           0x00
#define BLUE            0x01
#define GREEN           0x02
#define AQUA            0x03
#define RED             0x04
#define PURPLE          0x05
#define YELLOW          0x06
#define WHITE           0x07
#define GRAY            0x08
#define LIGHT_BLUE      0x09
#define LIGHT_GREEN     0x0a
#define LIGHT_AQUA      0x0b
#define LIGHT_RED       0x0c
#define LIGHT_PURPLE    0x0d
#define LIGHT_YELLOW    0x0e
#define BRIGHT_WHITE    0x0f


ste::Editor::Editor(const char* pathToFile)
    : _fileHandle(pathToFile), buffer(_fileHandle)
{
    _console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(_console, &_consoleInfo);
    GetConsoleCursorInfo(_console, &_cursorInfo);
    SetConsoleTextAttribute(_console, BRIGHT_WHITE);
}

ste::Editor::Editor(const std::string pathToFile)
    : _fileHandle(pathToFile), buffer(_fileHandle)
{
    _console = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(_console, &_consoleInfo);
    GetConsoleCursorInfo(_console, &_cursorInfo);
    SetConsoleTextAttribute(_console, BRIGHT_WHITE);
}

ste::Editor::~Editor()
{
    _cursorInfo.bVisible = true;
    SetConsoleCursorInfo(_console, &_cursorInfo); 
    SetConsoleTextAttribute(_console, WHITE);
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
    unsigned int workspaceHeight = _consoleInfo.srWindow.Bottom + 1 - EDITOR_WORKSPACE_OFFSET_Y;
    updateTextOffset(workspaceHeight);
    clearConsole();

    unsigned int displayedLines =
        ((buffer.text.size() - _textOffset) < workspaceHeight) ? buffer.text.size() - _textOffset : workspaceHeight;

    // display top bar
    std::cout << "ste.exe          file: " << _fileHandle.path() << "    lines: " << buffer.text.size() << '\n';
    std::cout << std::setfill('-') << std::setw(_consoleInfo.dwSize.X) << '\r';

    for (std::size_t i = _textOffset; i < buffer.text.size() && i < workspaceHeight + _textOffset; i++) {
        SetConsoleTextAttribute(_console, AQUA);
        std::cout << '\n' << std::setfill(' ') << std::setw(EDITOR_WORKSPACE_OFFSET_X - 1) << i + 1 << " "; // display line number
        SetConsoleTextAttribute(_console, BRIGHT_WHITE);
        std::cout << buffer.text.at(i);
    }

    // display free line indicators
    if (workspaceHeight > displayedLines) {
        SetConsoleTextAttribute(_console, PURPLE);
        for (unsigned int i = 0; i < workspaceHeight - displayedLines; i++)
            std::cout << "\n~";
        SetConsoleTextAttribute(_console, BRIGHT_WHITE);
    }

    COORD cursorPos;
    cursorPos.X = buffer.cursorPositionX() + EDITOR_WORKSPACE_OFFSET_X;
    cursorPos.Y = buffer.cursorPositionY() - _textOffset + EDITOR_WORKSPACE_OFFSET_Y;
    SetConsoleCursorPosition(_console, cursorPos);
}

void ste::Editor::updateTextOffset(unsigned int windowHeight) noexcept
{
    if (buffer.cursorPositionY() < _textOffset)
        _textOffset = buffer.cursorPositionY();
    else if (buffer.cursorPositionY() >= _textOffset + windowHeight)
        _textOffset = buffer.cursorPositionY() - windowHeight + 1;
}

void ste::Editor::clearConsole() const
{
    COORD coordScreen = {0, 0}; // home for the cursor
    DWORD cCharsWritten;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(_console, &_consoleInfo))
        return;

    dwConSize = _consoleInfo.dwSize.X * _consoleInfo.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(_console, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten))
        return;

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(_console, &_consoleInfo))
        return;

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(_console, _consoleInfo.wAttributes, dwConSize, coordScreen, &cCharsWritten))
        return;

    SetConsoleCursorPosition(_console, coordScreen);
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