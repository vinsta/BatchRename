#ifndef BATCHRENAMER_H
#define BATCHRENAMER_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QDir>
#include <QtWidgets/QListWidget>

namespace Ui {
class BatchRename;
}

class BatchRename : public QMainWindow
{
    Q_OBJECT

public:
    QString dirPath;
    QDir dir;

    explicit BatchRename(QWidget *parent = 0);
    ~BatchRename();
signals:
    void Init();

private:
    Ui::BatchRename *ui;

protected slots:
    void Root();
    void Up();
    void Replace();
    void AddAll();
    void Add();
    void Remove();
    void Clear();
    void ItemClicked(QListWidgetItem*);
    void ItemDoubleClickedFile(QListWidgetItem*);
    void ItemDoubleClickedWorkFile(QListWidgetItem*);

protected:
    void ChangePath(QString path);
    void enterDir();
    void ShowFile();
    void showDefault();
    void renameFiles();
};

#endif // BATCHRENAMER_H
