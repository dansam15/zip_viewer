#ifndef TREEITEM_HPP
#define TREEITEM_HPP

#include <QVariant>
#include <QVector>

#include <QString>

class TreeItem
{
public:
    explicit TreeItem(const QString& path, const QString& comp_size,
                      const QString& uncomp_size, TreeItem *parentItem = nullptr);
    ~TreeItem();

    void appendChild(TreeItem *child);

    TreeItem *child(int row);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    int row() const;
    TreeItem *parentItem();

// \fixme revert
//private:
    QVector<TreeItem*> m_childItems;
    std::array<QString, 3> m_itemData;
    TreeItem *m_parentItem;
};
#endif // TREEITEM_HPP
