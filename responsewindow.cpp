#include "responsewindow.h"
#include "ui_responsewindow.h"

#include<iostream>
#include<string>
#include<QProcess>
#include<QTextEdit>
#include<QLabel>
#include<QPoint>
#include<QDir>
#include<QMessageBox>


using namespace std;

#define DIRECTORY_SHOW_LABEL_LENGTH 10

//constructor
ResponseWindow::ResponseWindow(ParentWindow *pwq,int tabIndex,QWidget *parent)
    : QWidget(parent),pw(pwq),pwTabIndex(tabIndex)
    , ui(new Ui::ResponseWindow)
{
    ui->setupUi(this);
}

//destructor
ResponseWindow::~ResponseWindow()
{
    delete ui;
}

//it sets current directory of window
void ResponseWindow::setCurrentDirectory(){
    QString dir_str=QDir::currentPath(); //find current working directory path

    if(dir_str.isEmpty()){
        //current directory does not found so home path showing of user
        ui->directoryShowLabel->setText(QDir::homePath());
    }
    else{
        //show elided path on  minimized app only

        if(dir_str.size()>DIRECTORY_SHOW_LABEL_LENGTH){ //if length goes out of expected then it truncate the length
            QFontMetrics metrics(ui->directoryShowLabel->font()); //it truncats some words and makes smaller
            QString elidedText = metrics.elidedText(dir_str, Qt::ElideMiddle, ui->directoryShowLabel->width()+10);
            ui->directoryShowLabel->setText(elidedText);
            ui->directoryShowLabel->setToolTip(dir_str); // on hover it will show full directory label
            ui->directoryShowLabel->setToolTipDuration(5000);// for 5 second
        }
        else
            ui->directoryShowLabel->setText(dir_str);
    }
}


//it create new window of response window
void ResponseWindow::setNewResponseWindow() {
    if(!pw){
        qDebug()<<"pw parent window null ";
        return ;
    }

    if (pw) {
        qDebug() << "Adding response window to tab index:" << pwTabIndex;
        pw->addResponseWindowToTab(pwTabIndex);
    }
}


void ResponseWindow::on_lineEdit_editingFinished()
{
        string strInput;
        QString qstrInput;
        QString qstrOutput;
        string strOutput;

        //get the input from lineedit
        qstrInput=ui->lineEdit->text();
        ui->lineEdit->setDisabled(true);

        if(qstrInput.toLower()=="exit"){
            QMessageBox *msg=new QMessageBox();

            int return_msg =msg->critical(this,"Exit App","Do you really wanna close shell ?",QMessageBox::Yes,QMessageBox::No);

            if(return_msg==0x00004000){ //this is return value on Yes button
                QApplication::quit();
                return;
            }
        }

        if(qstrInput.isEmpty()) //if empty string just return
            return;

        QProcess *process=new QProcess(this);

        #ifdef Q_OS_WIN
            if(qstrInput.contains(" ")|| qstrInput.contains('|') || qstrInput.contains('>') || qstrInput.contains('<'))
                process->start("cmd.exe", QStringList() << "/C" << qstrInput); // Windows: Use cmd.exe to execute shell commands
            else
                process->start(qstrInput);// if input contains itself a executable line run.c ,run.py
        #else
            if(qstrInput.contains(" ")|| qstrInput.contains('|') || qstrInput.contains('>') || qstrInput.contains('<'))
                process->start("/bin/sh", QStringList() << "-c" << qstrInput);  // Linux/macOS: Use bash/sh to execute shell commands
            else
                process->start(qstrInput); // if input contains itself a executable line run.c ,run.py

        #endif

        if(!process->waitForStarted()){
            cout<<"Process Creation faild.";
            process->deleteLater();
            return;
        }

        if(!process->waitForFinished()){
            cout<<"Process is not yet Finished.";
            process->deleteLater();
            return;
        }

        QByteArray array=process->readAllStandardOutput();

        qstrOutput=QString::fromUtf8(array); //convert to human readable

        qstrOutput=qstrOutput.trimmed(); //trimming unnecessary chars
        strOutput=qstrOutput.toStdString(); //converts to standard string

        //set output to display label
        ui->outputDisplayLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding); //vertically can expand
        ui->outputDisplayLabel->setText(qstrOutput); //sets content to label
        ui->outputDisplayLabel->adjustSize(); //adjuct if contetnt has more lines
        ui->outputDisplayLabel->setWordWrap(true);
        ui->outputDisplayLabel->setAlignment(Qt::AlignLeft);

        qDebug()<<" called for next input line";
        setNewResponseWindow(); // now it create new response window beneath the current res window

        process->deleteLater();
}

