#include "treemodel.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>
#include <QTreeWidgetItem>
TreeModel::TreeModel(const QJsonDocument &doc, QObject *parent) :
    QAbstractItemModel(parent)
{
    initHeaders();
    m_doc = doc;
    setupModelData(m_doc, rootItem);
}


TreeModel::~TreeModel()
{
    delete rootItem;
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    if (role != Qt::EditRole)
        return false;

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    bool isSuccess = item->setData(index.column(), value);

    emit dataChanged(index, index);
    return isSuccess;
}


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        QString caption = rootItem->data(section).toString();
        if(headerCaption.contains(caption))
        {
            return headerCaption[caption];
        }
        return rootItem->data(section);
    }
    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();
    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();
    if (parentItem == rootItem)
        return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
        return 0;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

bool TreeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, 0, 0);
    TreeItem *parentItem;
    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    bool isFile = parent.child(0, 0).isValid();
    qDebug() << isFile;
    QList<QVariant> fileValues = { "Name", "", "", "", "", "", false, false};
    QList<QVariant> dirValues = { "Name", "", "", "", "", ""};
    TreeItem *childItem = new TreeItem(isFile ? fileValues : dirValues, parentItem);
    parentItem->appendChild(childItem);
    endInsertRows();
    return true;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, 0, 0);
    if (!parent.isValid())
        return false;
    TreeItem *item = static_cast<TreeItem*>(parent.internalPointer());
    item->remove();
    endRemoveRows();
    return true;
}

QJsonDocument TreeModel::toJsonDocument()
{
    QJsonDocument doc;
    QJsonArray array;
    for (int i = 0; i < rootItem->childCount(); ++i)
    {
        QJsonObject *obj = itemToJsonObject(rootItem->child(i));
        array.append(*obj);
    }
    doc.setArray(array);
    return doc;
}

QJsonObject *TreeModel::itemToJsonObject(TreeItem *item)
{
    QJsonObject *obj = new QJsonObject();
    QJsonArray *subItems = new QJsonArray();
    for (int i = 0; i < headerList.size(); ++i)
    {
        obj->insert(headerList.at(i).toString(), item->data(i).toString());
    }
    if(item->childCount() > 0)
    {
        for (int j = 0; j < item->childCount(); ++j) {
            QJsonObject *obj1 = itemToJsonObject(item->child(j));
            subItems->append(*obj1);
        }
    }
    obj->insert("Menus", *subItems);
    return obj;
}

void TreeModel::objectToItem(QJsonObject object, TreeItem *parent)
{
    QList<QVariant> values;
    for (int j = 0; j < headerList.size(); ++j)
    {
        values.append(object.value(headerList.at(j).toString()));
    }
    TreeItem* mainItem = new TreeItem(values, parent);
    QJsonArray folders = object.value("Menus").toArray();
    parent->appendChild(mainItem);
    for (int i = 0; i < folders.size(); ++i) {
        objectToItem(folders.at(i).toObject(), mainItem);
    }
}

void TreeModel::setupModelData(const QJsonDocument &doc, TreeItem *parent)
{
    QJsonArray array = doc.array();
    for (int i = 0; i < array.size(); ++i)
    {
        objectToItem(array.at(i).toObject(), parent);
    }
}

void TreeModel::initHeaders()
{
     headerList << "Caption" << "VideoUrl";
     headerCaption.insert("Caption", "视频列表");
     headerCaption.insert("VideoUrl", "视频地址");
     rootItem = new TreeItem(headerList);
}
