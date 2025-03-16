#ifndef PARENTWINDOW_H
#define PARENTWINDOW_H

#include <QMainWindow>
// #include "basewindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ParentWindow;
}
QT_END_NAMESPACE

class ParentWindow : public QMainWindow
{
    Q_OBJECT

public:
    ParentWindow(QWidget *parent = nullptr);
    ~ParentWindow();

private slots:
    void on_parentTabContainer_currentChanged(int index);

    void on_parentTabContainer_tabCloseRequested(int index);

    void on_newWindow_triggered();


public:
    void addScrollArea(QWidget *qw);

    void addResponseWindowToTab(int tabIndex);


private:
    Ui::ParentWindow *ui;
    // BaseWindow *bas_wind;
};
#endif // PARENTWINDOW_H
