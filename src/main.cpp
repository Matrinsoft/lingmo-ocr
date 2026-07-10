/*
* Copyright (C) 2020 ~ 2021 Deepin Technology Co., Ltd.
*
* Author: He MingYang Hao<hemingyang@uniontech.com>
*
* Maintainer: He MingYang <hemingyang@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
#include "ocrapplication.h"
#include "service/ocrinterface.h"
#include "service/dbusocr_adaptor.h"

#include <QWidget>
//#include <QLog>
//#include <DWindowManagerHelper>
//#include <DWidgetUtil>
//#include <DGuiApplicationHelper>
#include <QApplication>

#include <QCoreApplication>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QScreen>
#include <QGuiApplication>

//鍒ゆ柇鏄惁鏄痺ayland
bool CheckWayland()
{
    auto e = QProcessEnvironment::systemEnvironment();
    QString XDG_SESSION_TYPE = e.value(QStringLiteral("XDG_SESSION_TYPE"));
    QString WAYLAND_DISPLAY = e.value(QStringLiteral("WAYLAND_DISPLAY"));

    if (XDG_SESSION_TYPE == QLatin1String("wayland") || WAYLAND_DISPLAY.contains(QLatin1String("wayland"), Qt::CaseInsensitive))
        return true;
    else {
        return false;
    }
}

int main(int argc, char *argv[])
{


    //鍒ゆ柇鏄惁鏄痺ayland
    if (CheckWayland()) {
        //榛樿璧皒dgv6,璇ュ簱娌℃湁缁存姢浜嗭紝鍥犳闇€瑕佹坊鍔犺浠ｇ爜
        qputenv("QT_WAYLAND_SHELL_INTEGRATION", "kwayland-shell");
    }

//    DGuiApplicationHelper::setUseInactiveColorGroup(false);
//#if(DTK_VERSION < DTK_VERSION_CHECK(5,4,0,0))
//    DApplication::loadDXcbPlugin();
//    QScopedPointer<DApplication> app(new DApplication(argc, argv));
//#else
//    QScopedPointer<DApplication> app(DApplication::globalApplication(argc, argv));
//#endif
    QApplication app(argc, argv);
    app.setOrganizationName("lingmoos");
    app.setApplicationName("lingmo-ocr");
//    app.setProductName(QObject::tr("OCR Tool"));
    app.setApplicationVersion("1.9.9");

//    Dtk::Core::DLogManager::registerConsoleAppender();
//    Dtk::Core::DLogManager::registerFileAppender();

    QCommandLineOption dbusOption(QStringList() << "u" << "dbus", "Start  from dbus.");
    QCommandLineParser cmdParser;
    cmdParser.setApplicationDescription("lingmo-Ocr");
    cmdParser.addHelpOption();
    cmdParser.addVersionOption();
    cmdParser.addOption(dbusOption);
    cmdParser.process(app);

//    app->loadTranslator();


    OcrApplication* instance = OcrApplication::instance();
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (dbus.registerService("org.lingmo.Ocr")) {
        // 绗竴娆″惎鍔?
        // 娉ㄥ唽Dbus鏈嶅姟鍜屽璞?
        dbus.registerObject("/com/lingmo/Ocr", instance);
        // 鍒濆鍖栭€傞厤鍣?
        new DbusOcrAdaptor(instance);

        if(QString(argv[1]) != "")
        {
            instance->openFile(QString(argv[1]));
        }

    } else {
            // 绗簩娆¤繍琛屾搴旂敤锛?
            // 璋冪敤DBus鎺ュ彛锛屽鐞嗕氦缁欑涓€娆¤皟鐢ㄧ殑杩涚▼
            // 鏈繘绋嬮€€閫€鍑?
            OcrInterface *pOcr = new OcrInterface("org.lingmo.Ocr", "/com/lingmo/Ocr", QDBusConnection::sessionBus(), instance);
            qDebug() << __FUNCTION__ << __LINE__;
            pOcr->openFile(QString(argv[1]));
            delete pOcr;
            return 0;
        }



    return app.exec();
}


