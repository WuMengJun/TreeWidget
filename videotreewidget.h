#ifndef VIDEOTREEWIDGET_H
#define VIDEOTREEWIDGET_H

#include <QTreeWidget>

class VideoTreeWidget : public QTreeWidget
{
    Q_OBJECT
public:
    explicit VideoTreeWidget(const QByteArray data, QWidget *parent = 0);
    void treeInit();
    void setItemFromArray(QJsonArray array, QTreeWidgetItem *item);
    void setItemFromJson(QByteArray json);
signals:

public slots:
private:
    QByteArray m_data;
};

#endif // VIDEOTREEWIDGET_H
