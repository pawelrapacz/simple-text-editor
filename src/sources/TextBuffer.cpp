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

#include <string>
#include <cstddef>
#include <cmath>
#include <vector>


#include "TextBuffer.hpp"
#include "FileHandler.hpp"



ste::TextBuffer::TextBuffer(FileHandler& fileHandle)
{
    _text.reserve(fileHandle.numOfLines() + 100);
    fileHandle.read(_text);
}

ste::TextBuffer::~TextBuffer() {}



unsigned int ste::TextBuffer::cursorPositionX() const noexcept
{ return _cursor.x; }

unsigned int ste::TextBuffer::cursorPositionY() const noexcept
{ return _cursor.y; }

void ste::TextBuffer::moveCursorX(int offset) noexcept
{
    if (0 > offset && std::abs(offset) > _cursor.x) {
        if (0 != _cursor.y) {
            moveCursorY(-1);
            _cursor.x = _text.at(_cursor.y).length();
        }
    }
    else if (_text.at(_cursor.y).length() < _cursor.x + offset) {
        if (_text.size() - 1 != _cursor.y) {
            moveCursorY(1);
            _cursor.x = 0;
        }
    }
    else {
        _cursor.x += offset;
    }
}

void ste::TextBuffer::moveCursorX(Cursor::pos pos) noexcept
{
    if (Cursor::pos::begin == pos) _cursor.x = 0;
    else if (Cursor::pos::end == pos) _cursor.x = _text.at(_cursor.y).length();
}

void ste::TextBuffer::moveCursorY(int offset) noexcept
{
    if (0 > offset && std::abs(offset) > _cursor.y) {
        _cursor.y = 0;
    }
    else if (_text.size() <= _cursor.y + offset) {
        _cursor.y = _text.size() - 1;
        if (_cursor.x > _text.at(_cursor.y).length()) _cursor.x = _text.at(_cursor.y).length();
    }
    else {
        _cursor.y += offset;
        if (_cursor.x > _text.at(_cursor.y).length()) _cursor.x = _text.at(_cursor.y).length();
    }
}

void ste::TextBuffer::moveCursorY(Cursor::pos pos) noexcept
{
    if (Cursor::pos::begin == pos) _cursor.y = 0;
    else if (Cursor::pos::end == pos) _cursor.y = _text.size() - 1;
}


void ste::TextBuffer::setCursorX(unsigned int pos)
{
    if (_text.at(_cursor.y).length() < pos) 
        throw std::overflow_error("Given number is too large: cannot set the cursor to that position");
    else _cursor.x = pos;
}

void ste::TextBuffer::setCursorY(unsigned int pos)
{
    if (_text.size() < pos)
        throw std::overflow_error("Given number is too large: cannot set the cursor to that position");
    else _cursor.y = pos;
}

void ste::TextBuffer::setCursor(unsigned int posX, unsigned int posY)
{
    setCursorY(posY);
    setCursorX(posX);
}

void ste::TextBuffer::newLineBreak() noexcept
{
    std::string line = _text.at(cursorPositionY());
    std::string dbg = line.substr(0, cursorPositionX());
    _text.insert( _text.begin() + cursorPositionY(), line.substr(0, cursorPositionX()) ); // create new line
    moveCursorY(1);
    _text.at(cursorPositionY()).erase(0, cursorPositionX()); // delete text that was moved to a new line
    setCursorX(0);
}

void ste::TextBuffer::deleteLineBreak() noexcept
{
    if (0 == cursorPositionY()) return;
    else {
        unsigned int newCursorPositionX = _text.at(cursorPositionY() - 1).length();
        _text.at(cursorPositionY() - 1) += _text.at(cursorPositionY());
        _text.erase(_text.begin() + cursorPositionY());
        moveCursorY(-1);
        setCursorX(newCursorPositionX);
    }
}

void ste::TextBuffer::insertChar(const char letter) noexcept
{
    if ('\n' != letter){
        _text.at(cursorPositionY()).insert(cursorPositionX(), 1, letter);
        moveCursorX(1);
    }
    else {
        newLineBreak();
    }
}

void ste::TextBuffer::deleteChar() noexcept
{
    if (0 != cursorPositionX()) {
        _text.at(cursorPositionY()).erase(cursorPositionX() - 1, 1);
        moveCursorX(-1);
    }
    else {
        deleteLineBreak();
    }
}