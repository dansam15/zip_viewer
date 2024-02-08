#include "treemodel.hpp"

TreeModel::TreeModel(std::vector<ZipEntry> data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(QString::fromStdString("Name"),
                            QString::fromStdString("Comp size"),
                            QString::fromStdString("Uncomp size"));

    setupModelData(data, rootItem);
}

void TreeModel::setupModelData(std::vector<ZipEntry> data, TreeItem *parent)
{
    std::cout << "TEST TreeModel::setupModelData\n";
    std::cout << parent->m_itemData[0].toStdString() << "\n";
    std::cout << parent->m_itemData[1].toStdString() << "\n";
    std::cout << parent->m_itemData[2].toStdString() << "\n";
    std::cout << parent->m_itemData[2].toStdString() << "\n";
    if (parent->m_parentItem)
        std::cout << "parent:" << parent->m_parentItem->m_itemData[0].toStdString() << "\n";
    std::cout << "===" << "\n";

    for (size_t i = 0; i < data.size(); ++i)
    {
       TreeItem *cur_entry = new TreeItem(QString::fromStdString(data[i].m_path.string()),
                                        QString::fromStdString(std::to_string(data[i].m_comp_size)),
                                        QString::fromStdString(std::to_string(data[i].m_uncomp_size)),
                                        parent);
       parent->appendChild(cur_entry);
       cur_entry->m_itemData = {QString::fromStdString(data[i].m_path.string()),
                               QString::fromStdString(std::to_string(data[i].m_comp_size)),
                               QString::fromStdString(std::to_string(data[i].m_uncomp_size))};

       if (data[i].m_path.has_filename())
       {
           continue;
       }

       std::vector<ZipEntry> sub_entries;
       fs::path parent_tmp = data[i].m_path.string().substr(0, data[i].m_path.string().size() - 1);
       std::cout << "DIR: " << parent_tmp << "\n";
       for (size_t j = 0; j < data.size(); ++j)
       {
           if (j == i)
           {
               continue;
           }

           fs::path child  = data[j].m_path;
           if (parent_tmp == child.parent_path()) // file from the subdir
           {
               std::cout << "sub: " << child << "\n";


               sub_entries.emplace_back(data[j].m_path.string(), data[j].m_comp_size, data[j].m_uncomp_size);
           }
       }
       setupModelData(sub_entries, cur_entry);
    }
}

TreeModel::~TreeModel()
{
    delete rootItem;
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
    return rootItem->columnCount();
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

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}
