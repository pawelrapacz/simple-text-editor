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

#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cstddef>
#include <stdexcept>


namespace ste
{
    class FileHandler
    {
    public:
        FileHandler(const char* pathToFile);
        FileHandler(const std::string pathToFile);
        ~FileHandler();

        std::size_t numOfLines() const noexcept;
        std::filesystem::path path() const noexcept;
        void path(const char* pathToFile) noexcept;
        void path(const std::string pathToFile) noexcept;
        void read(std::vector<std::string>& text) const noexcept;
        void write(const std::vector <std::string>& text) const;


    private:
        std::filesystem::path _path;
        mutable std::fstream _file;
    };
} // namespace ste

#endif // FILEHANDLER_H