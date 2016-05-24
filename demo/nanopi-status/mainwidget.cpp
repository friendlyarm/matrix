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

#include "mainwidget.h"
#include "util.h"
#include "sys/sysinfo.h"

TMainWidget::TMainWidget(QWidget *parent) :
    QWidget(parent),bg(QPixmap(":/bg.png"))
{
    mpKeepAliveTimer = new QTimer();
    mpKeepAliveTimer->setSingleShot(false);
    QObject::connect(mpKeepAliveTimer, SIGNAL(timeout()), this, SLOT(onKeepAlive()));
    mpKeepAliveTimer->start(1500);

    qtdemoButton = new QPushButton("Start Qt Demo >>", this);
    connect(qtdemoButton, SIGNAL(clicked()), this, SLOT(qtdemoButtonClicked()));
}

void TMainWidget::qtdemoButtonClicked() {
    system("/opt/QtE-Demo/run-fluidlauncher.sh&");
    exit(0);
}

void TMainWidget::resizeEvent(QResizeEvent*) {
    const int buttonWidth = width()/4;
    const int buttonHeight = height()/12;
    qtdemoButton->setGeometry(width()-buttonWidth-10,height()-5-buttonHeight,buttonWidth, buttonHeight);

    if (width() < 800) {
        qtdemoButton->hide();
    }
}

void TMainWidget::onKeepAlive() {
    static char ipStr[50];
    memset(ipStr, 0, sizeof(ipStr));
    int ret = Util::getIPAddress("eth0", ipStr, 49);
    if (ret == 0) {
        eth0IP = QString(ipStr);
    } else {
        eth0IP = "0.0.0.0";
    }

    memset(ipStr, 0, sizeof(ipStr));
    ret = Util::getIPAddress("wlan0", ipStr, 49);
    if (ret == 0) {
        wlan0IP = QString(ipStr);
    } else {
        wlan0IP = "0.0.0.0";
    }

    struct sysinfo sys_info;
    if (sysinfo(&sys_info) == 0) {
        qint32 totalmem=(qint32)(sys_info.totalram/1048576);
        qint32 freemem=(qint32)(sys_info.freeram/1048576); // divide by 1024*1024 = 1048576
        // float f = ((sys_info.totalram-sys_info.freeram)*1.0/sys_info.totalram)*100;
        // memInfo = QString("%1%,F%2MB").arg(int(f)).arg(freemem);
         memInfo = QString("%1/%2 MB").arg(totalmem-freemem).arg(totalmem);
    }

        if (Util::GetBoardType().toUpper().startsWith("NANOPC-T") || Util::GetBoardType().toUpper() == "NANOPI-M3") {
		currentCPUTemp = Util::readFile("/sys/class/hwmon/hwmon0/device/temp_label").simplified();
		maxCPUTemp = Util::readFile("/sys/class/hwmon/hwmon0/device/temp_max").simplified();
	} else if (Util::GetBoardType().toUpper() == "NANOPI-M1") {
		currentCPUTemp = Util::readFile("/sys/class/thermal/thermal_zone0/temp").simplified();
		maxCPUTemp = Util::readFile("/sys/class/thermal/thermal_zone0/temp").simplified();
	}  else if (Util::GetBoardType().toUpper().startsWith("RASPBERRYPI3")) {
		QString str;
		bool ok=false;
		float _currentCPUTemp = Util::readFile("/sys/class/thermal/thermal_zone0/temp").simplified().toInt(&ok)/1000.0;
		currentCPUTemp = str.sprintf("%.1f",_currentCPUTemp);
		maxCPUTemp = currentCPUTemp;
	}

    QString contents = Util::readFile("/proc/loadavg").simplified();
    QStringList values = contents.split(" ");
    int vCount = 3;
    loadAvg = "";
    foreach (const QString &v, values) {
        QString str = v.simplified();
        if (!str.isEmpty()) {
            if (!loadAvg.isEmpty()) {
                loadAvg += "/";
            }
            bool ok = false;
            float f = str.toFloat(&ok);
            if (ok) {
                loadAvg += QString("%1").arg(int(f));
            }
            vCount--;
            if (vCount <= 0) {
                break;
            }
        }
    }


    QString fileName = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
    QString str = Util::readFile(fileName).simplified();
    bool ok = false;
    int freq = str.toInt(&ok,10);
    freqStr = "";
    if (ok) {
	    QString str;
	    if (freq > 1000000) {
		    freqStr = str.sprintf("%.1fG",freq*1.0/1000000);
	    } else if (freq > 1000) {
		    freqStr = str.sprintf("%d",freq/1000);
	    } else {
		    freqStr = str.sprintf("%d",freq);
	    }
    }
    update();
}

void TMainWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int space = 3;
    int itemHeight = 20;
    p.fillRect(0,0,width(),height(),QBrush(QColor(0,0,0)));
    p.drawPixmap(0, 0, width(), height(), bg);

    QString ip = eth0IP;
    if (ip == "0.0.0.0") {
        ip = wlan0IP;
    }

    p.setPen(QPen(QColor(255,255,255)));
    if (Util::GetBoardType().toUpper() == "NANOPI-M1")
        p.drawText(space,itemHeight*0,width()-space*2,itemHeight,Qt::AlignLeft | Qt::AlignVCenter,QString("CPU: %1MHz %2C").arg(freqStr).arg(currentCPUTemp));
    else 
        p.drawText(space,itemHeight*0,width()-space*2,itemHeight,Qt::AlignLeft | Qt::AlignVCenter,QString("CPU: %1MHz").arg(freqStr));
    p.drawText(space,itemHeight*1,width()-space*2,itemHeight,Qt::AlignLeft | Qt::AlignVCenter,QString("Mem: %1").arg(memInfo));
    p.drawText(space,itemHeight*2,width()-space*2,itemHeight,Qt::AlignLeft | Qt::AlignVCenter,QString("LoadAvg: %1").arg(loadAvg));
    p.drawText(space,itemHeight*3,width()-space*2,itemHeight,Qt::AlignLeft | Qt::AlignVCenter,QString("IP: %1").arg(ip));

    if (width() >= 800) {
        p.setPen(QPen(QColor(192,192,192)));
        // const int buttonWidth = width()/4;                                                                   
        const int buttonHeight = height()/12;                                                                
        p.drawText(10,height()-5-buttonHeight-5-buttonHeight, "This is an open source project, you can find the source code in /opt/QtE-Demo.");
    }
}

