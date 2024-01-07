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

#include <fstream>
#include <string>
#include <vector>
#include <cstddef>

#include "FileHandler.hpp"



ste::FileHandler::FileHandler(const char* pathToFile)
    : _path(pathToFile) {}

ste::FileHandler::FileHandler(const std::string pathToFile)
    : _path(pathToFile) {}

ste::FileHandler::~FileHandler() {}



std::size_t ste::FileHandler::numOfLines() const noexcept
{
    std::size_t count = 0;
    std::string line;
    _file.open(_path, std::ios::in);
    if (_file.good()) {
        while(std::getline(_file, line))
            count++;
    }
    _file.close();
    return count;
}

std::filesystem::path ste::FileHandler::path() const noexcept
{ return _path; }

void ste::FileHandler::path(const char* pathToFile) noexcept
{ _path = pathToFile; }

void ste::FileHandler::path(const std::string pathToFile) noexcept
{ _path = pathToFile; }

void ste::FileHandler::write(const std::vector <std::string>& text) const
{
    _file.open(_path, std::ios::out | std::ios::trunc);
    if (_file.fail()) throw std::runtime_error("Cannot save chnges to the file");

    for (std::size_t i = 0; i < text.size(); i++) {
        if ( text.size() - 1 == i) _file << text.at(i);
        else _file << text.at(i) << '\n';
    }

    _file.close();
}

void ste::FileHandler::read(std::vector<std::string>& text) const noexcept
{
    std::string line;
    _file.open(_path, std::ios::in);
    if (_file.good()) {
        while(std::getline(_file, line))
            text.push_back(line);
    }
    _file.close();

    if (text.empty())
        text.push_back(std::string());
}