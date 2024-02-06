#ifndef TREEMODEL_HPP
#define TREEMODEL_HPP

#include <QAbstractItemModel>
#include <QStringList>

#include <iostream>

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    TreeModel(const std::vector<QStringList> &strings, QObject *parent = nullptr)
        : QAbstractItemModel(parent), stringList_(strings)
    {

    }

    bool insertColumn(int column, const QModelIndex &parent = QModelIndex())
    {
        std::cerr << "Invalid insertColumns() usage for predefined case\n";
        return false;
    };

    virtual bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override final
    {
        std::cerr << "Invalid insertColumns() usage for predefined case\n";
        return false;
    };

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    virtual QModelIndex parent(const QModelIndex &index) const override;

private:
    std::vector<QStringList> stringList_; // \todo make another (hier.) structure
};

#endif // TREEMODEL_HPP
