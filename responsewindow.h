#ifndef RESPONSEWINDOW_H
#define RESPONSEWINDOW_H

#include <QWidget>
#include "parentwindow.h"
#include <string>
#include<functional>
#include<unordered_map>
#include <QProcess>
#include <QDir>

using namespace std;

namespace Ui {
class ResponseWindow;
}


class ResponseWindow : public QWidget {
    Q_OBJECT


    public:
    std::unordered_map<std::string, std::function<void(const QStringList&, QString, QProcess*)>> windows_shell_cmd_map;
    std::unordered_map<std::string,std::function<void(const QStringList &,QString,QProcess *)>> linux_shell_cmd_map;

    public:
        explicit ResponseWindow(ParentWindow *pw,int tabIndex,QWidget *parent = nullptr);
        ~ResponseWindow();

    private slots:
        // void on_lineEdit_editingFinished();


    public:
        void setCurrentDirectory(int);//it is the member of this class now
        void setNewResponseWindow();
        void onInputEntered();
        void executeManualImplementedCommand(const QStringList &,QString,QProcess *);
        void changeDirectory(const QStringList &,QString,QProcess *);
        void clearCurrentTab(const QStringList &,QString,QProcess *);
        void dateCurrent(const QStringList &,QString,QProcess *);
        void copyFileToFileData(const QStringList &,QString,QProcess *);
        void deleteFiles(const QStringList &,QString,QProcess *);
        void moveFiles(const QStringList &,QString,QProcess *);
        bool fileExists(const QString &);


    private:
        Ui::ResponseWindow *ui;

        ParentWindow *pw; //an instance of parent window to add new response window
        int pwTabIndex; // current index of parent window
};

#endif // RESPONSEWINDOW_H
