#include "treemodel.hpp"

TreeModel::TreeModel(std::vector<ZipEntry> data, QObject *parent)
    : QAbstractItemModel(parent)
{
    rootItem = new TreeItem(QString::fromStdString("Name"),
                            QString::fromStdString("Comp size"),
                            QString::fromStdString("Uncomp size"));

    setupModelData(data, rootItem);
}

// dur to fs::parent_path() return same path for dirs
// this func return is level up dir
static std::string parent_path(const fs::path& path)
{
    std::string str = path;
    str = str.substr(0, str.find_last_of("/"));
    if (!path.has_filename())
        str = str.substr(0, str.find_last_of("/"));
    return str;
}

// check if nested more than 1 dir
static bool is_complicated_path(const fs::path& path)
{
    int count = 0;

    for (int i = 0; i < path.string().size(); i++)
    {
      if (path.string()[i] == '/')
      {
          count++;
      }
    }

    return count > 1;
}

static bool is_subpath(const fs::path& path, const fs::path& base)
{
    if (path.string() == base.string())
    {
        return false;
    }

    std::string parent;

    if (path.has_filename())
    {
        parent = path.parent_path().string() + "/";
    }
    else
    {
        parent = parent_path(path) + "/";

    }

    return parent == base;
}

void TreeModel::setupModelData(std::vector<ZipEntry> data, TreeItem *parent)
{
    for (size_t i = 0; i < data.size(); ++i)
    {
      if (parent->fullPath().toStdString() == rootItem->fullPath().toStdString())
      {
          if (is_complicated_path(parent_path(data[i].m_path) + "/"))
          {
              continue;
          }
      }
      else if (!is_subpath(data[i].m_path, parent->fullPath().toStdString()))
      {
          continue;
      }

       TreeItem *cur_entry = new TreeItem(QString::fromStdString(data[i].m_path.string()),
                                          QString::fromStdString(std::to_string(data[i].m_comp_size)),
                                          QString::fromStdString(std::to_string(data[i].m_uncomp_size)),
                                          parent
                                          );

       parent->appendChild(cur_entry);

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
