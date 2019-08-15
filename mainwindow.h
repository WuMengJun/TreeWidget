#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include "treemodel.h"
#include "videotreewidget.h"
#include "wlvideoplayer.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void objectToItem(QJsonArray array, QTreeWidgetItem *item);
    void setTreeItemFromJson(QTreeWidget *tree, QByteArray bytesData);
    void videoInit();

private slots:
    void on_actionOpen_triggered();
    void palyer(const QModelIndex &index);

    void on_actionOpen2_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";
    TreeModel* model = nullptr;
    QTreeWidgetItem *mainItem;
    QTreeWidgetItem* subItem;
    VideoTreeWidget *videoTree = nullptr;
    WLVideoPlayer *WXJ;
};

#endif // MAINWINDOW_H
