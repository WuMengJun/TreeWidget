#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QSettings>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    videoInit();
    ui->trwVideos->headerItem()->setText(0, "VideoList");
    QObject::connect(ui->trvVideos, SIGNAL(clicked(QModelIndex)), this, SLOT(palyer(QModelIndex)));
    connect(ui->trwVideos, &QTreeWidget::itemClicked, [=](QTreeWidgetItem *item, int column){
        qDebug() << item->text(column) << item->data(column, Qt::UserRole+1);
    });
    QFile file("H:\\jsonfile\\videoList.json");
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();
        videoTree = new VideoTreeWidget(data);
        videoTree->expandAll();
    }
    videoTree->setMinimumWidth(20);
    ui->gridLayout_4->addWidget(videoTree);
    connect(videoTree, &QTreeWidget::itemClicked, [=] (QTreeWidgetItem *item, int column){
        QString videourl = item->data(column, Qt::UserRole + 1).toString();
        QString caption = item->text(column);
        if(!ui->gridLayout_5->isEmpty()){
            WXJ->videoPlayer->close();
            ui->gridLayout_5->removeWidget(WXJ->videoPlayer);
        }
        WXJ->videoPlayer->show();
        ui->gridLayout_5->addWidget(WXJ->videoPlayer);
        WXJ->startPlay(videourl);
        ui->lbl_videoCaption->setText(caption);
    });
    ui->splitter->setStretchFactor(0, 3);
    ui->splitter->setStretchFactor(1, 7);


}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::objectToItem(QJsonArray array, QTreeWidgetItem *item)
{
    for (int i = 0; i < array.size(); ++i) {
        QJsonObject obj = array.at(i).toObject();
        subItem = new QTreeWidgetItem(item, QStringList(QString(obj.value("Caption").toString())));
        subItem->setData(0, Qt::UserRole+1, obj.value("VideoUrl").toString());
        if(!obj.value("Menus").isUndefined()){
            QJsonArray array = obj.value("Menus").toArray();
            objectToItem(array, subItem);
        }
    }
}
void MainWindow::setTreeItemFromJson(QTreeWidget *tree, QByteArray bytesData)
{
    tree->clear();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(bytesData, &error);
    if(error.error == QJsonParseError::NoError){
        QJsonArray array = doc.array();
        for(int i=0; i<array.size(); i++){
            QJsonObject obj = array.at(i).toObject();
            mainItem = new QTreeWidgetItem(tree, QStringList(QString(obj.value("Caption").toString())));
            mainItem->setData(0, Qt::UserRole+1, obj.value("VideoUrl").toString());
            if(!obj.value("Menus").isUndefined()){
                QJsonArray array = obj.value("Menus").toArray();
                objectToItem(array, mainItem);
            }
        }
    }
}

void MainWindow::videoInit()
{
    QSettings *pReg = new QSettings("HKEY_CURRENT_USER\\Environment", QSettings::NativeFormat);
    QVariant v = pReg->value("SDL_AUDIODRIVER");
    if(v.toString().isEmpty()) {
        pReg->setValue("SDL_AUDIODRIVER", "directsound");

        QProcess* myProcess = new QProcess(this);
        myProcess->start("cmd.exe", {"/c", "setx /m SDL_AUDIODRIVER directsound"});
    }
    delete pReg;
    WXJ = new WLVideoPlayer();
    WXJ->initStyle();
    WXJ->setVolume(60);
    WXJ->setContextMenuEnabled(false);
}

void MainWindow::palyer(const QModelIndex &index)
{
    qDebug() << "index" << index.data();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open json file", "/home/jsons", "*.json");
    if(fileName == "")
        return;
    currentFile = fileName;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        model = new TreeModel(doc);
        ui->trvVideos->setModel(model);
        file.close();
    }
    int allcolumnWidth = ui->trvVideos->width() - 100;
    int columnCount = ui->trvVideos->model()->columnCount();
    ui->trvVideos->setColumnWidth(columnCount - 1, 50);
    ui->trvVideos->setColumnWidth(columnCount - 2, 50);
    for (int i = 0; i < columnCount - 2; ++i) {
        ui->trvVideos->setColumnWidth(i, allcolumnWidth / (columnCount - 2));
    }
    setWindowTitle(fileName);
}


void MainWindow::on_actionOpen2_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open json file", "/home/jsons", "*.json");
    if(fileName == "")
        return;
    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        file.close();
        setTreeItemFromJson(ui->trwVideos, data);
    }
}
