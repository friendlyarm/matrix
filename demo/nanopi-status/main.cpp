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

#include <QApplication> 
#include <QLabel> 
#include <QWSServer>
#include "mainwidget.h"
int main(int argc, char **argv) { 
	QApplication app (argc, argv); 
	QWSServer::setCursorVisible(false);
	TMainWidget w;
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.showFullScreen();
	return app.exec(); 
}
