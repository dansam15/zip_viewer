#include "treemodel.hpp"

bool TreeModel::insertColumn(int column, const QModelIndex &parent)
{
    throw std::runtime_error("Invalid insertColumn() usage for predefined case");
    return false;
};

bool TreeModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    throw std::runtime_error("Invalid insertColumns() usage for predefined case");
    return false;
};

int TreeModel::rowCount(const QModelIndex &parent) const
{
    return stringList_.size();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= stringList_.size())
        return QVariant();

    if (index.column() >=3)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        return stringList_.at(index.row()).at(index.column());
    }
    else
    {
        return QVariant();
    }
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                    int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    //if (orientation == Qt::Horizontal)
    {
        if (section == 0)
        {
          return QStringLiteral("Full path");
        }
        else  if (section == 1)
        {
            return QStringLiteral("Packed size");
        }
        else if (section == 2)
        {
            return QStringLiteral("Unpacked size");
        }
        else
        {
            return QVariant();
        }

        //return QStringLiteral("Column %1").arg(section);
    }
}


QModelIndex 	TreeModel::index(int row, int column, const QModelIndex &parent) const
{
   return parent.isValid() ? QModelIndex() : createIndex(row, column); // without hierarchy due to restrictions
    //return createIndex(row, column);
}

int 	TreeModel::columnCount(const QModelIndex &parent) const
{
    return 3;
}

QModelIndex 	TreeModel::parent(const QModelIndex &index) const
{
    return QModelIndex();
}

