#include "BatchRename.h"
#include "ui_BatchRename.h"
#include <QDebug>
BatchRename::BatchRename(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BatchRename)
{
    ui->setupUi(this);
    connect(this,SIGNAL(Init()),this,SLOT(Root()));
    connect(ui->lwt_Dir,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(ItemClicked(QListWidgetItem*)));
    connect(ui->lwt_File,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(ItemDoubleClickedFile(QListWidgetItem*)));
    connect(ui->lwt_Work_File,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(ItemDoubleClickedWorkFile(QListWidgetItem*)));
    connect(ui->btn_Root,SIGNAL(clicked()),this,SLOT(Root()));
    connect(ui->btn_Replace,SIGNAL(clicked()),this,SLOT(Replace()));
    connect(ui->btn_Up,SIGNAL(clicked()),this,SLOT(Up()));
    connect(ui->btn_AddAll,SIGNAL(clicked()),this,SLOT(AddAll()));
    connect(ui->btn_Add,SIGNAL(clicked()),this,SLOT(Add()));
    connect(ui->btn_Remove,SIGNAL(clicked()),this,SLOT(Remove()));
    connect(ui->btn_Clear,SIGNAL(clicked()),this,SLOT(Clear()));

    emit Init();
}
BatchRename::~BatchRename()
{
    delete ui;
}
void BatchRename::showDefault()
{
    ui->lwt_Dir->clear();
    QFileInfoList driveList = dir.drives();
    for (int i = 0; i < driveList.count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(driveList.at(i).filePath());
        ui->lwt_Dir->addItem(item);
    }
}
void BatchRename::ShowFile()
{
    QListWidgetItem *item;
    ui->lwt_Dir->clear();
    ui->lwt_File->clear();
    if (dir.isRoot())
    {
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Pic/Dir"), QSize(), QIcon::Normal, QIcon::Off);

        item = new QListWidgetItem(".");
        item->setIcon(icon);
        ui->lwt_Dir->addItem(item);

        item = new QListWidgetItem("..");
        item->setIcon(icon);
        ui->lwt_Dir->addItem(item);
    }
    QFileInfoList fileList=dir.entryInfoList();
    for (unsigned int i=0;i<dir.count();i++)
    {
        QIcon icon;
        item = new QListWidgetItem (fileList.at(i).fileName());
        if (fileList.at(i).isDir())
        {
            icon.addFile(QString::fromUtf8(":/Pic/Dir"), QSize(), QIcon::Normal, QIcon::Off);
            item->setIcon(icon);
            ui->lwt_Dir->addItem(item);
        }
        else
        {
            icon.addFile(QString::fromUtf8(":/Pic/File"), QSize(), QIcon::Normal, QIcon::Off);
            item->setIcon(icon);
            ui->lwt_File->addItem(item);
        }
    }
}
void BatchRename::ChangePath(QString path)
{
    if (path != dir.path())
    {
        ui->lwt_File->clear();
        ui->lwt_Work_File->clear();
    }
    dir.setPath(path);
    dir.makeAbsolute();
    dirPath=dir.absolutePath();
    ui->let_Addr->setText(dirPath);
}
void BatchRename::enterDir()
{
    QFileInfo file;
    if (ui->lwt_Dir->selectedItems().count() > 0)
    {
        QString name = ui->lwt_Dir->selectedItems().first()->text();
        if (dir.isRoot() && name.contains(".."))
        {
            ChangePath("/");
            showDefault();
        }
        else if (dir.path() != name)
        {
            file.setFile(dir, name);
            ChangePath(file.filePath());
            ShowFile();
        }
    }
}
void BatchRename::Root()
{
    ChangePath("/");
    showDefault();
}
void BatchRename::Replace()
{
    renameFiles();
}
void BatchRename::renameFiles()
{
    QString strFind = ui->let_Find->text();
    QString strReplace = ui->let_Replace->text();
    QString oldFileName, newFileName;
    QRegExp re(strFind);
    re.setPatternSyntax(QRegExp::Wildcard);
    for (int i = 0; i < ui->lwt_Work_File->count(); i++)
    {
        oldFileName = newFileName = ui->lwt_Work_File->item(i)->text();
        newFileName.replace(re, strReplace);
        ui->lwt_Work_File->item(i)->setText(newFileName);
        oldFileName = dirPath + oldFileName;
        newFileName = dirPath + newFileName;
        QFile::rename(oldFileName, newFileName);
    }
    dir.refresh();
    ShowFile();
}
void BatchRename::Up()
{
    if (dir.isRoot())
    {
        ChangePath("/");
        showDefault();
    }
    else
    {
        dir.cd("..");
        ChangePath(dir.path());
        ShowFile();
    }
}
void BatchRename::AddAll()
{
    for (int i = 0; i < ui->lwt_File->count(); i++)
    {
        QListWidgetItem *item = new QListWidgetItem(*(ui->lwt_File->item(i)));
        ui->lwt_Work_File->addItem(item);
    }
}
void BatchRename::Add()
{
    QListWidgetItem *item = NULL;
    for (int i = 0; i < ui->lwt_File->selectedItems().count(); i++)
    {
        item = new QListWidgetItem(*(ui->lwt_File->selectedItems().at(i)));
        ui->lwt_Work_File->addItem(item);
    }
}
void BatchRename::Remove()
{
    QList<QListWidgetItem *> tmpList = ui->lwt_Work_File->selectedItems();
    for (int i = 0; i < tmpList.count(); i++)
    {
        QListWidgetItem *item = tmpList.at(i);
        //ui->lwt_Work_File->takeItem(ui->lwt_Work_File->row(item));
        //ui->lwt_Work_File->removeItemWidget(item);
        delete item;
    }
}
void BatchRename::Clear()
{
    ui->lwt_Work_File->clear();
}
void BatchRename::ItemClicked(QListWidgetItem*)
{
    enterDir();
}
void BatchRename::ItemDoubleClickedFile(QListWidgetItem*)
{
    Add();
}
void BatchRename::ItemDoubleClickedWorkFile(QListWidgetItem*)
{
    Remove();
}
