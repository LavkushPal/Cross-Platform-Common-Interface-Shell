#include "parentwindow.h"
#include "ui_parentwindow.h"
#include "responsewindow.h"

#include<QVBoxLayout>
#include<QMessageBox>
#include <QScrollArea>

#define MAX_TAB_OPENING 10
using namespace std;

ParentWindow::ParentWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParentWindow)
{
    ui->setupUi(this);

    //creates new tab with respoonse window on startup of app
    this->addResponseWindowToTab(ui->parentTabContainer->count());

}

ParentWindow::~ParentWindow()
{
    delete ui;
}

void ParentWindow::on_parentTabContainer_currentChanged(int index)
{
    // ui->parentTabContainer->removeTab(index);
}

void ParentWindow::on_parentTabContainer_tabCloseRequested(int index)
{
    QMessageBox msg;

    // if(index==0){ avoiding to remove all the tab
    //     msg.information(this,"Closing Windows","Base Window couldn't be closed",msg.Yes,msg.No);
    //     msg.exec();
    //     return;
    // }

    ui->parentTabContainer->removeTab(index);
    ui->parentTabContainer->update();
}


void ParentWindow::on_newWindow_triggered() {
    QMessageBox msg;

    if (ui->parentTabContainer->count() >= MAX_TAB_OPENING) {
        msg.about(this, "Tab Opening", "No More Tab Can be Open");
        msg.exec();
        return;
    }

    //creates new tab with respoonse window on request new window
    this->addResponseWindowToTab(ui->parentTabContainer->count());

}


void ParentWindow::addResponseWindowToTab(int tabIndex) {

    if (tabIndex < 0) {
        qDebug() << "Invalid tab index!" << tabIndex;
        return;
    }

    // Ensure the tab exists before accessing it
    if(tabIndex >= ui->parentTabContainer->count()) { //creatingn tab here
        QWidget *newTab = new QWidget();
        ui->parentTabContainer->addTab(newTab, QString("Tab %1").arg(ui->parentTabContainer->count()+1));
        //inititalize tab by adding 1 mandatory as requirment for creating not creating when come again
    }

    qDebug()<<"\n tabIndex passed"<<tabIndex<<" . tab index by function count "<<ui->parentTabContainer->count();

    QWidget *tabWidget = ui->parentTabContainer->widget(tabIndex);
    if (!tabWidget) {
        qDebug() << "Tab widget is NULL, something is wrong!";
        return;
    }

    // Ensure the tab has a layout
    if (!tabWidget->layout()) {
        QVBoxLayout *tabLayout = new QVBoxLayout();
        tabWidget->setLayout(tabLayout);
    }

    QScrollArea *qsa = tabWidget->findChild<QScrollArea *>();
    if (!qsa) {
        qsa = new QScrollArea(tabWidget);
        qsa->setWidgetResizable(true);

        QWidget *containerWidget = new QWidget();
        containerWidget->setLayout(new QVBoxLayout());

        qsa->setWidget(containerWidget);
        // qsa->setBackgroundRole(QPalette::Dark);
        // qsa->setForegroundRole(QPalette::Shadow);


        QVBoxLayout *tabLayout = qobject_cast<QVBoxLayout *>(tabWidget->layout());
        tabLayout->addWidget(qsa);
    }

    QWidget *containerWidget = qsa->widget();
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(containerWidget->layout());

    // Create and add new ResponseWindow
    ResponseWindow *res = new ResponseWindow(this, tabIndex);
    res->setCurrentDirectory();
    res->setFocusPolicy(Qt::TabFocus);


    QCursor cur;
    res->setCursor(cur);

    res->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    layout->addWidget(res); //adding response wndow
    layout->setAlignment(Qt::AlignTop);

    containerWidget->adjustSize(); //makes reponsive ui
    qsa->update(); //update all changes
    qApp->processEvents(); //force to update ui

    qDebug() << "Added new ResponseWindow to tab: " << tabIndex;
}




