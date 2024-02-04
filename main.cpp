#include "mainwindow.h"

#include "treemodel.hpp"

#include <QtWidgets/QApplication>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QFileSystemModel>
#include <QtWidgets/QSplitter>
#include <QDir>
#include <QApplication>

#include <iostream>


#include <QStringList>

#include "mz.h"
#include "mz_strm.h"
#include "mz_zip.h"
#include "mz_zip_rw.h"

int32_t minizip_list(const char *path, std::vector<QStringList>& file_list) {
    mz_zip_file *file_info = NULL;
    uint32_t ratio = 0;
    int32_t err = MZ_OK;
    struct tm tmu_date;
    const char *method = NULL;
    char crypt = ' ';
    void *reader = NULL;


    reader = mz_zip_reader_create();
    if (!reader)
        return MZ_MEM_ERROR;

    err = mz_zip_reader_open_file(reader, path);
    if (err != MZ_OK) {
        printf("Error %" PRId32 " opening archive %s\n", err, path);
        mz_zip_reader_delete(&reader);
        return err;
    }

    err = mz_zip_reader_goto_first_entry(reader);

    if (err != MZ_OK && err != MZ_END_OF_LIST) {
        printf("Error %" PRId32 " going to first entry in archive\n", err);
        mz_zip_reader_delete(&reader);
        return err;
    }

    /* Enumerate all entries in the archive */
    do {
        err = mz_zip_reader_entry_get_info(reader, &file_info);

        if (err != MZ_OK) {
            printf("Error %" PRId32 " getting entry info in archive\n", err);
            break;
        }

        ratio = 0;
        if (file_info->uncompressed_size > 0)
            ratio = (uint32_t)((file_info->compressed_size * 100) / file_info->uncompressed_size);

        /* Display a '*' if the file is encrypted */
        if (file_info->flag & MZ_ZIP_FLAG_ENCRYPTED)
            crypt = '*';
        else
            crypt = ' ';

        method = mz_zip_get_compression_method_string(file_info->compression_method);
        mz_zip_time_t_to_tm(file_info->modified_date, &tmu_date);

        // Read all entries
        {
            QStringList entry;
            entry << file_info->filename;

            char str[1000];

            sprintf(str, "%d", file_info->compressed_size);
            entry << str;

            sprintf(str, "%d", file_info->uncompressed_size);
            entry << str;

            file_list.push_back(entry);
        }

        err = mz_zip_reader_goto_next_entry(reader);

        if (err != MZ_OK && err != MZ_END_OF_LIST) {
            printf("Error %" PRId32 " going to next entry in archive\n", err);
            break;
        }
    } while (err == MZ_OK);

    mz_zip_reader_delete(&reader);

    if (err == MZ_END_OF_LIST)
        return MZ_OK;

    return err;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if (argc != 2)
    {
        std::cerr << "The format of run is \"zip_viewer ${PATH_TO_ZIP}\"\n";
        exit(1);
    }
    const char* path = argv[1];

    std::vector<QStringList> file_list;
    minizip_list(path, file_list);

    QTreeView* tree_view = new QTreeView;
    TreeModel* tree_model = new TreeModel(file_list);

    tree_view->setModel(tree_model);
    tree_view->show();

    return a.exec();
}
