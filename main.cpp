
#include <QApplication>

#include <QtWidgets/QTreeView>
#include <QtWidgets/QFileSystemModel>

#include <QStringList>

#include <filesystem>
namespace fs = std::filesystem;

#include <iostream>
#include <string>

#include "treemodel.hpp"

#include "mz.h"
#include "mz_strm.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"

std::vector<TreeModel::ZipEntry> parse_zip(const std::string& path)
{
    std::vector<TreeModel::ZipEntry> retval;

    mz_zip_file *file_info = NULL;
    int32_t err = MZ_OK;

    void *reader = mz_zip_reader_create();
    if (!reader)
    {
        throw std::runtime_error("Error mz reader creating\n");
    }

    err = mz_zip_reader_open_file(reader, path.c_str());
    if (err != MZ_OK)
    {
        mz_zip_reader_delete(&reader);

        const std::string err_str = "Error" + std::to_string(err) + " opening archieve " + path;
        throw std::runtime_error(err_str);
    }

    err = mz_zip_reader_goto_first_entry(reader);
    if (err != MZ_OK && err != MZ_END_OF_LIST)
    {
        mz_zip_reader_delete(&reader);

        const std::string err_str = "Error" + std::to_string(err) + " going to first entry in archive";
        throw std::runtime_error(err_str);
    }

    /* Enumerate all entries in the archive */
    do {
        err = mz_zip_reader_entry_get_info(reader, &file_info);

        if (err != MZ_OK)
        {
            const std::string err_str = "Error" + std::to_string(err) + " getting entry info in archive";
            throw std::runtime_error(err_str);
        }

        // Read entry
        retval.emplace_back(file_info->filename, file_info->compressed_size, file_info->uncompressed_size);


        err = mz_zip_reader_goto_next_entry(reader);

        if (err != MZ_OK && err != MZ_END_OF_LIST)
        {
            const std::string err_str = "Error" + std::to_string(err) + " going to next entry in archive";
            throw std::runtime_error(err_str);
        }
    }
    while (err == MZ_OK);

    mz_zip_reader_delete(&reader);

    if (err != MZ_END_OF_LIST)
    {
        const std::string err_str = "Error" + std::to_string(err) + " with archieve parsing";
        throw std::runtime_error(err_str);
    }

    return retval;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    try {
        if (argc != 2)
        {
            std::cerr << "The format of run is \"zip_viewer ${PATH_TO_ZIP}\"\n";
            exit(1);
        }


        const fs::path zip_path(argv[1]);
        std::vector<TreeModel::ZipEntry> file_list = parse_zip(zip_path.string());

        QTreeView* tree_view = new QTreeView;
        TreeModel* tree_model = new TreeModel(file_list);

        tree_view->setModel(tree_model);

        tree_view->resizeColumnToContents(0);

        tree_view->show();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        return -1;
    }
    catch (...)
    {
        std::cerr << "unknown exception\n";
        return -1;
    }

    return a.exec();
}
