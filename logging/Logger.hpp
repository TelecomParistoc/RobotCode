/**
Copyright (C)

creation date : 05/11/2015
file : logger.hpp
author : Julien Schoumacher

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
**/


#ifndef LOGGER_HPP
#define LOGGER_HPP


#include <memory>
#include <vector>

#include "Handler.hpp"


class Logger
{
    public:
        Logger();
        Logger(const std::vector<std::shared_ptr<Handler> >& handlers);

        void addHandler(const std::shared_ptr<Handler>& handler);

        static std::string date();

    protected:
        std::vector<std::shared_ptr<Handler> > handlers;
};


#endif
