// Copyright (C) Guangzhou FriendlyARM Computer Tech. Co., Ltd.
// (http://www.friendlyarm.com)
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, you can access it online at
// http://www.gnu.org/licenses/gpl-2.0.html.

#ifndef UTIL_H
#define UTIL_H

#include <QtGui>

class Util
{
public:
    static QString GetBoardType();
    static QString readFile (const QString& filename);
    static QString seconds_to_DHMS(unsigned long duration);
    static int GetMemInfoMB();
    static QString seconds_to_DHMS_US(unsigned long duration);
    static int getIPAddress(const char* ifaceName, char* ip, size_t maxLen);
};
#endif // UTIL_H
