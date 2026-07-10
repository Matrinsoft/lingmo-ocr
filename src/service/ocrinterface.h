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
#ifndef OCRINTERFACE_H
#define OCRINTERFACE_H

#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtDBus/QtDBus>
#include <QImage>
#include <QBuffer>
#include <QDebug>

class OcrInterface: public QDBusAbstractInterface
{
    Q_OBJECT
public:
    static inline const char *staticInterfaceName()
    {
        return "org.lingmo.Ocr";
    }

public:
    /*
    * @param: serviceName QDBusConnection 娉ㄥ唽鐨勬湇鍔″悕瀛?
    * @param: ObjectPath QDBusConnection 娉ㄥ唽鐨勫璞¤矾寰?
    */
    OcrInterface(const QString &serviceName, const QString &ObjectPath,
                 const QDBusConnection &connection, QObject *parent = nullptr);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    ~OcrInterface();

public Q_SLOTS: // METHODS
    /*
    * @bref:openFile 閫氳繃璺緞鎵撳紑鍥剧墖鏂囦欢
    * @param: filePath 鍥剧墖鐨勮矾寰?
    * @return: QDBusPendingReply
    */
    inline QDBusPendingReply<> openFile(const QString &filePath)
    {
        return call(QStringLiteral("openFile"), filePath);
    }

    /*
    * @bref:openImages
    * @param: image 鍥剧墖
    * @return: QDBusPendingReply
    * @note:
    */
    inline QDBusPendingReply<> openImage(const QImage &image)
    {
        qDebug() << __FUNCTION__;
        QByteArray data;
        QBuffer buf(&data);
        if (image.save(&buf, "PNG")) {
            data = qCompress(data, 9);
            data = data.toBase64();
        }
        return call(QStringLiteral("openImage"), QVariant::fromValue(data));
    }

    /*
    * @bref:openImages
    * @param: image 鍥剧墖
    * @param: imageName 鍥剧墖鍚嶇О
    * @return: QDBusPendingReply
    * @note:
    */
    inline QDBusPendingReply<> openImageAndName(const QImage &image, const QString &imageName)
    {
        qDebug() << __FUNCTION__;
        QByteArray data;
        QBuffer buf(&data);
        if (image.save(&buf, "PNG")) {
            data = qCompress(data, 9);
            data = data.toBase64();
        }
        return call(QStringLiteral("openImageAndName"), QVariant::fromValue(data), imageName);
    }

Q_SIGNALS: // SIGNALS
};

namespace com {
namespace lingmo {
typedef ::OcrInterface Ocr;
}
}
#endif // DRAWINTERFACE_H
