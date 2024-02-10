#include "treemodel.hpp"

TreeModel::TreeModel(std::vector<ZipEntry> data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(QString::fromStdString("Name"),
                            QString::fromStdString("Comp size"),
                            QString::fromStdString("Uncomp size"));

    setupModelData(data, rootItem);
}

// dur to fs::parent_path() return same path for dirs \todo more obvious
static std::string parent_path(const fs::path& path)
{
    std::string str = path;
    str = str.substr(0, str.find_last_of("/"));
    if (!path.has_filename())
        str = str.substr(0, str.find_last_of("/"));
    return str;// + "/";
}

bool is_complicated_path(const fs::path& path)
{ int count = 0;

    for (int i = 0; i < path.string().size(); i++)
      if (path.string()[i] == '/') count++;

    return count > 1;

}

void TreeModel::setupModelData(std::vector<ZipEntry> data, TreeItem *parent)
{
   // std::cout << "TEST TreeModel::setupModelData\n";

    std::string cur_dir = parent->m_itemData[0].toStdString();

    /*
    std::cout << parent->m_itemData[0].toStdString() << "\n";
    std::cout << parent->m_itemData[1].toStdString() << "\n";
    std::cout << parent->m_itemData[2].toStdString() << "\n";
    if (parent->m_parentItem)
        std::cout << "parent's parent:" << parent->m_parentItem->m_itemData[0].toStdString() << "\n";
    for (auto it: parent->m_childItems)
        std::cout << "parents child:" << it->m_itemData[0].toStdString() << "\n";

    std::cout << "===" << "\n";
*/
    for (size_t i = 0; i < data.size(); ++i)
    {
/*
        std::cout << "i = " << i << " << data[i].m_path.string()= " << data[i].m_path.string() << " for parent:" <<
                     parent->m_itemData[0].toStdString() << "\n";
        std::cout << "parent->m_itemData[0].toStdString()=" << parent->m_itemData[0].toStdString() << "\n";
        std::cout << "rootItem->m_itemData[0].toStdString()=" << rootItem->m_itemData[0].toStdString() << "\n";
       std::cout << "parent_path(data[i].m_path) = " << parent_path(data[i].m_path) << "\n";
       std::cout << "(parent->m_itemData[0].toStdString() != rootItem->m_itemData[0].toStdString()) = "
                 << (parent->m_itemData[0].toStdString() != rootItem->m_itemData[0].toStdString()) << "\n";
*/
      if(
          parent->m_itemData[0].toStdString() != rootItem->m_itemData[0].toStdString()
             &&  (
              !data[i].m_path.has_filename() &&
              parent->m_itemData[0].toStdString().find(data[i].m_path.string()) != std::string::npos
              ||
              data[i].m_path.has_filename() &&
          parent->m_itemData[0].toStdString() != parent_path(data[i].m_path) + "/"

          )
          )
      {
         // std::cout << "Continue 0\n\n";
          continue;
      }

      if (
              parent->m_itemData[0].toStdString() == rootItem->m_itemData[0].toStdString()
                 &&
             is_complicated_path(parent_path(data[i].m_path) + "/")

         )
       {
         //  std::cout << "Continue 1\n\n";
           continue;
       }

       if(
          parent->m_itemData[0].toStdString() != rootItem->m_itemData[0].toStdString()
             &&
              !data[i].m_path.has_filename() &&
               parent->m_itemData[0].toStdString() != parent_path(data[i].m_path) + "/"

         )
       {
          // std::cout << "Continue 2\n\n";
           continue;
       }


       TreeItem *cur_entry = new TreeItem(QString::fromStdString(data[i].m_path.string()),
                                              QString::fromStdString(std::to_string(data[i].m_comp_size)),
                                              QString::fromStdString(std::to_string(data[i].m_uncomp_size)),
                                              parent
                                              );
       parent->appendChild(cur_entry);
      // std::cout << "Added " << data[i].m_path.string() << "\n";

       if (data[i].m_path.has_filename())
       {
           continue;
       }
       else
       {
           setupModelData(data, cur_entry);
       }
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
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
    {
        return createIndex(row, column, childItem);
    }
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem *parentItem = childItem->parentItem();

    if (parentItem == rootItem)
    {
        return QModelIndex();
    }
    return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem;
    if (parent.column() > 0)
    {
        return 0;
    }
    if (!parent.isValid())
    {
        parentItem = rootItem;
    }
    else
    {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
    }
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }

    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    return item->data(index.column());
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return rootItem->data(section);
    }
    return QVariant();
}
