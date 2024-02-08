#include "treeitem.hpp"

TreeItem::TreeItem(const QString& path, const QString& comp_size,
                   const QString& uncomp_size, TreeItem *parentItem)
    : m_parentItem(parentItem)
{
   m_itemData = {path, comp_size, uncomp_size};
}


TreeItem::TreeItem(const std::array<QString, 3> &data, TreeItem *parentItem)
    : m_itemData(data), m_parentItem(parentItem)
{
}

TreeItem::~TreeItem()
{
    qDeleteAll(m_childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
    m_childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
    if (row < 0 || row >= m_childItems.size())
        return nullptr;
    return m_childItems.at(row);
}

int TreeItem::childCount() const
{
    return m_childItems.count();
}

int TreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}

int TreeItem::columnCount() const
{
    return 3;
}

QVariant TreeItem::data(int column) const
{
    if (column < 0 || column >= 3)
        return QVariant();

    return m_itemData.at(column);
}

TreeItem *TreeItem::parentItem()
{
    return m_parentItem;
}
