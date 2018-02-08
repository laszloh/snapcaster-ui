/***
    This file was originally part of snapcast
    Modified to fit the snapcaster-ui project
    
    Copyright (C) 2014-2017  Johannes Pohl
    Copyright (C) 2018 Laszlo Hegedüs
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef BROWSEMDNS_H
#define BROWSEMDNS_H

#include <string>

struct mDNSResult
{
	int proto_;
	std::string ip_;
	std::string host_;
	uint16_t port_;
	bool valid_;
};

class BrowsemDNS
{
public:
	virtual bool browse(const std::string& serviceName, mDNSResult& result, int timeout) = 0;
};

#if defined(HAS_AVAHI)
#include "browseAvahi.h"
typedef BrowseAvahi BrowseZeroConf;
#elif defined(HAS_BONJOUR)
#include "browseBonjour.h"
typedef BrowseBonjour BrowseZeroConf;
#endif

#endif
