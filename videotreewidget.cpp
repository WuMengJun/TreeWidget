#include "videotreewidget.h"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
VideoTreeWidget::VideoTreeWidget(const QByteArray data, QWidget *parent) :
    QTreeWidget(parent)
{
    treeInit();
    m_data = data;
    setItemFromJson(m_data);
}

void VideoTreeWidget::treeInit()
{
    clear();
    headerItem()->setText(0,tr("视频列表"));
}

void VideoTreeWidget::setItemFromArray(QJsonArray array, QTreeWidgetItem *item)
{
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array.at(i).toObject();
        QTreeWidgetItem *subItem = new QTreeWidgetItem(item, QStringList(QString(obj.value("Caption").toString())));
        subItem->setData(0, Qt::UserRole+1, obj.value("VideoUrl").toString());
        if(!obj.value("Menus").isUndefined()){
            QJsonArray array = obj.value("Menus").toArray();
            setItemFromArray(array, subItem);
        }
    }
}

void VideoTreeWidget::setItemFromJson(QByteArray json)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);
    if(error.error == QJsonParseError::NoError){
        QJsonArray array = doc.array();
        for(int i=0; i<array.size(); i++){
            QJsonObject obj = array.at(i).toObject();
            QTreeWidgetItem *mainItem = new QTreeWidgetItem(this, QStringList(QString(obj.value("Caption").toString())));
            mainItem->setData(0, Qt::UserRole+1, obj.value("VideoUrl").toString());
            if(!obj.value("Menus").isUndefined()){
                QJsonArray array = obj.value("Menus").toArray();
                setItemFromArray(array, mainItem);
            }
        }
    }
}

