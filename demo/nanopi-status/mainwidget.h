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

#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui>
class TMainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TMainWidget(QWidget *parent = 0);
    ~TMainWidget() {}
private slots:
    void onKeepAlive();
    void qtdemoButtonClicked();
private:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent *);
private:
    QTimer* mpKeepAliveTimer;
    QString loadAvg;
    QString currentCPUTemp;
    QString maxCPUTemp;
    QString freqStr;
    QString memInfo;
    QPixmap bg;
    QString eth0IP;
    QString wlan0IP;
    QPushButton* qtdemoButton;
};

#endif // WIDGET_H
