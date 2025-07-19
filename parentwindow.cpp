#include "parentwindow.h"
#include "ui_parentwindow.h"
#include "responsewindow.h"

#include<QVBoxLayout>
#include<QMessageBox>
#include <QScrollArea>
#include <QTabBar>
#include <QDir>

#define MAX_TAB_OPENING 10
using namespace std;

ParentWindow::ParentWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParentWindow)
{
    ui->setupUi(this); //pass calling reference object

    //creates new tab with respoonse window on startup of app

    ui->parentTabContainer->setStyleSheet(
        "QTabWidget::pane { "
        "   border: 1px solid #2C7DA0;"
        "   border-radius: 2px; }"

            // "QTabBar::tab { border: 1px solid black; }"

        "QTabBar::tab:selected {"
        "   background:#2C7DA0;"
        "   opacity:0.7; "
        "   padding-left:2px ; "
        "   padding-right:2px; }"
     );
    this->addResponseWindowToTab(ui->parentTabContainer->count());
}

//destructor
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
    ui->parentTabContainer->removeTab(index);

    int tabCount = ui->parentTabContainer->count();

    // Rename remaining tabs sequentially
    for (int i = index; i < tabCount; ++i) {
        QWidget *currentTab = ui->parentTabContainer->widget(i);
        if (currentTab) {
            QString newTitle = "Tab" + QString::number(i + 1); // Proper conversion
            ui->parentTabContainer->setTabText(i, newTitle);
        }
    }

    QTabBar *tabBar = ui->parentTabContainer->tabBar();

    // Check if tabs exceed available width
    int totalTabWidth = 0;
    for (int i = 0; i < tabBar->count(); ++i) {
        totalTabWidth += tabBar->tabRect(i).width();
    }

    int availableWidth = ui->parentTabContainer->width();

    // Enable scroll buttons only if tabs exceed available width
    tabBar->setUsesScrollButtons(totalTabWidth > availableWidth);

    qApp->processEvents(); //force to update ui
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

//to check whether tab exists for provided index
bool isTabExists(QTabWidget *tabWidget, int index) {
    return (index >= 0 && index < tabWidget->count() && tabWidget->widget(index) != nullptr);
}


void ParentWindow::addResponseWindowToTab(int tabIndex) {

    if(tabIndex < 0) {
        qDebug() << "Invalid tab index!" << tabIndex;
        return;
    }

    qDebug()<<"\n tabIndex passed"<<tabIndex<<" . tab index by function count "<<ui->parentTabContainer->count();

    if(!isTabExists(ui->parentTabContainer, tabIndex)) { //creatingn tab here
        QWidget *newTab = new QWidget();
        ui->parentTabContainer->addTab(newTab, QString("Tab %1").arg(ui->parentTabContainer->count()+1));


        QString execDir = QCoreApplication::applicationDirPath();  // or QDir::homePath()
        QDir startDir(execDir);
        current_directory_map[tabIndex] = startDir;


        /*QDir dir;//dir object
        dir.absolutePath();
        current_directory_map[ui->parentTabContainer->count()+1]=dir;*/ //storing one object for mentaning directory level

        //inititalize tab by adding 1 mandatory as requirment for creating not creating when come again
        qDebug()<<"new tab creadted for passed index :"<<tabIndex;
    }

    qDebug()<<"\n tabIndex passed"<<tabIndex<<" . tab index by function count "<<ui->parentTabContainer->count();

    QWidget *tabWidget = ui->parentTabContainer->widget(tabIndex);
    if (!tabWidget) {
        qDebug() << "Tab widget is NULL, something is wrong!";
        return;
    }

    // Ensure the tab has a layout
    if(!tabWidget->layout()) {
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
        qsa->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
        // qsa->setBackgroundRole(QPalette::Dark);
        // qsa->setForegroundRole(QPalette::Shadow);


        QVBoxLayout *tabLayout = qobject_cast<QVBoxLayout *>(tabWidget->layout());
        tabLayout->addWidget(qsa);
    }

    QWidget *containerWidget = qsa->widget();
    QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(containerWidget->layout());

    // Create and add new ResponseWindow
    ResponseWindow *res = new ResponseWindow(this, tabIndex);
    res->setCurrentDirectory(tabIndex);
    res->setFocusPolicy(Qt::TabFocus);


    QCursor cur;
    res->setCursor(cur);



    res->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    layout->addWidget(res); //adding response wndow
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(2);

    containerWidget->adjustSize(); //makes reponsive ui
    qsa->update(); //update all changes
    qApp->processEvents(); //force to update ui

    qDebug() << "Added new ResponseWindow to tab: " << tabIndex;
}




void ParentWindow::on_newWindow_hovered()
{

}

