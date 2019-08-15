#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QJsonDocument>
#include "treeitem.h"

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TreeModel(const QJsonDocument &doc, QObject *parent = 0);
    ~TreeModel();
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    QJsonDocument toJsonDocument();
    QJsonObject *itemToJsonObject(TreeItem* item);

signals:

public slots:

private:
    QHash<QString, QString> headerCaption;
    QJsonDocument m_doc;
    TreeItem *rootItem;
    QList<QVariant> headerList;
    void objectToItem(QJsonObject object, TreeItem *parent);
    void setupModelData(const QJsonDocument &doc, TreeItem *parent);
    void initHeaders();

};

#endif // TREEMODEL_H
