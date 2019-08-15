#ifndef TREEITEM_H
#define TREEITEM_H
#include <QList>
#include <QVariant>
class TreeItem
{
public:
    TreeItem(const QList<QVariant> &data, TreeItem *parentItem = nullptr);
    ~TreeItem();
    void appendChild(TreeItem *childItem);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool setData(int column, QVariant value);


    int row() const;
    TreeItem *parentItem();
    void remove();
private:
    QList<TreeItem*> m_childItems;
    QList<QVariant> m_itemData;
    TreeItem *m_parentItem;
};

#endif // TREEITEM_H
