#ifndef TREEMODEL_HPP
#define TREEMODEL_HPP

#include <QAbstractItemModel>
#include <QStringList>
#include <QVariant>
#include <QVector>

#include <iostream>

#include "treeitem.hpp"

#include <filesystem>
namespace fs = std::filesystem;

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    class ZipEntry
    {
        friend class TreeModel;
        public:
            ZipEntry(std::string path, unsigned comp_size, unsigned uncomp_size):
                m_path(path), m_comp_size(comp_size), m_uncomp_size(uncomp_size) {};
        private:
            fs::path m_path;
            unsigned m_comp_size;
            unsigned m_uncomp_size;
    };
    explicit TreeModel(std::vector<ZipEntry> data, QObject *parent = nullptr);
    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

private:
    bool isRootDir(const std::string dir) const;
    void setupModelData(std::vector<ZipEntry> data, TreeItem *parent);

    TreeItem *rootItem;
};
#endif // TREEMODEL_HPP
