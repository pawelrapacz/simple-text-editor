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

#ifndef TEXTBUFFER_H
#define TEXTBUFFER_H

#include <string>
#include <vector>

#include "FileHandler.hpp"


namespace ste
{
    class TextBuffer
    {
    public:
        struct Cursor {
            unsigned int x = 0;
            unsigned int y = 0;
            enum class pos {
                begin,
                end
            };
        };

        const std::vector <std::string>& text = _text;
        
        TextBuffer(FileHandler& fileHandle);
        ~TextBuffer();
        unsigned int cursorPositionX() const noexcept;
        unsigned int cursorPositionY() const noexcept;
        void moveCursorX(int offset) noexcept;
        void moveCursorX(Cursor::pos) noexcept;
        void moveCursorY(int offset) noexcept;
        void moveCursorY(Cursor::pos) noexcept;
        void setCursorX(unsigned int pos);
        void setCursorY(unsigned int pos);
        void setCursor(unsigned int posX, unsigned int posY);
        void insertChar(const char) noexcept;
        void deleteChar() noexcept;


    private:
        Cursor _cursor;
        std::vector <std::string> _text;

        void newLineBreak() noexcept;
        void deleteLineBreak() noexcept;
    };
} // namespace ste

#endif // TEXTBUFFER_H