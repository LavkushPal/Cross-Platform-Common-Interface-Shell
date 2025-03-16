#ifndef RESPONSEWINDOW_H
#define RESPONSEWINDOW_H

#include <QWidget>
#include "parentwindow.h"

namespace Ui {
class ResponseWindow;
}



class ResponseWindow : public QWidget {
    Q_OBJECT

    public:
        explicit ResponseWindow(ParentWindow *pw,int tabIndex,QWidget *parent = nullptr);
        ~ResponseWindow();

    private slots:
        void on_lineEdit_editingFinished();

    public:
        void setCurrentDirectory();//it is the member of this class now
        // so it can use private members of this class also in cpp file
        void setNewResponseWindow();

    private:
        Ui::ResponseWindow *ui;

        ParentWindow *pw; //an instance of parent window to add new response window
        int pwTabIndex; // current index of parent window
};

#endif // RESPONSEWINDOW_H
