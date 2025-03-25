#include "responsewindow.h"
#include "ui_responsewindow.h"


#include<cstdlib>
#include<string>
#include <unordered_map>


#include <QProcess>
#include <QTextEdit>
#include <QLabel>
#include <QPoint>
#include <QDir>
#include <QMessageBox>
#include <QRegularExpression>
#include <QString>
#include <QDateTime>
#include <QFile>

using namespace std;

#define DIRECTORY_SHOW_LABEL_LENGTH 30

//constructor
ResponseWindow::ResponseWindow(ParentWindow *pwq,int tabIndex,QWidget *parent)
    : QWidget(parent),pw(pwq),pwTabIndex(tabIndex)
    , ui(new Ui::ResponseWindow)
{

    ui->setupUi(this);

    windows_shell_cmd_map["cd"] = std::bind(&ResponseWindow::changeDirectory, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    windows_shell_cmd_map["clear"]=std::bind(&ResponseWindow::clearCurrentTab, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    windows_shell_cmd_map["copy"]=std::bind(&ResponseWindow::copyFileToFileData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    windows_shell_cmd_map["date"]=std::bind(&ResponseWindow::dateCurrent, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    windows_shell_cmd_map["del"]=std::bind(&ResponseWindow::deleteFiles, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    windows_shell_cmd_map["move"]=std::bind(&ResponseWindow::moveFiles, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);


    //later write for these aslo
    // windows_shell_cmd_map["MD"];
    // windows_shell_cmd_map["RD"];
    // windows_shell_cmd_map["REN"];
    // windows_shell_cmd_map["PATH"];


    // //LINUX
    linux_shell_cmd_map["cd"] = std::bind(&ResponseWindow::changeDirectory, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

    // //later implement these also
    // linux_shell_cmd_map["bg"];
    // linux_shell_cmd_map["fg"];
    // linux_shell_cmd_map["source"];



    //connect for getting input when user press enter after writing input
    connect(ui->lineEdit, &QLineEdit::returnPressed, this, &ResponseWindow::onInputEntered);
    ui->lineEdit->setPlaceholderText("Type here");
    ui->lineEdit->setToolTip("Type your command here");
}

//destructor
ResponseWindow::~ResponseWindow()
{
    delete ui;
}

//it sets current directory of window
void ResponseWindow::setCurrentDirectory(int tabIndex){
    QDir dir=pw->current_directory_map[tabIndex]; //find current working directory path

    QString dir_str=dir.absolutePath();

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


// // Check if a command is a Windows shell built-in
// bool isShellBuiltinWindows(const std::string& command) {
//     std::string cmd = "cmd /c \"ver >nul && " + command + " 2>nul\"";
//     return (system(cmd.c_str()) == 0);
// }

// // Check if a command is an executable file in Windows
// bool isExecutableFileWindows(const std::string& command) {
//     std::string cmd = "where " + command + " >nul 2>&1";
//     return (system(cmd.c_str()) == 0);
// }

// // Check if a command is a Linux shell built-in
// bool isShellBuiltinLinux(const std::string& command) {
//     std::string cmd = "type " + command + " 2>/dev/null | grep -q 'builtin'";
//     return (system(cmd.c_str()) == 0);
// }

// // Check if a command is an executable file in Linux
// bool isExecutableFileLinux(const std::string& command) {
//     std::string cmd = "which " + command + " > /dev/null 2>&1";
//     return (system(cmd.c_str()) == 0);
// }






// void pathFilesDirectory(const QStringList &tokens,QString qstrInput,QProcess *proces){

// }

void ResponseWindow::moveFiles(const QStringList &tokens,QString qstrInput,QProcess *proces){

    try {
        if(tokens.size()>=3){ //expected: copy srcfile destfile
            QFile::rename(tokens[1],tokens[2]); //renaming as move : source file - dest file
            ui->outputDisplayLabel->setText("File moved successfully!");
        }
    } catch (...) {
        ui->outputDisplayLabel->setText("Sorry,File move failed!");
    }
}

void ResponseWindow::deleteFiles(const QStringList &tokens,QString qstrInput,QProcess *proces){

    try {
        if(tokens.size()>=2){}
        QFile::remove(tokens[1]); //expected: remove file_to_be_removed
        ui->outputDisplayLabel->setText("File deleted successfully!");
    } catch (...) {
        ui->outputDisplayLabel->setText("File deletion failed!");
    }
}

void ResponseWindow::dateCurrent(const QStringList &tokens,QString qstrInput,QProcess *proces){

    // QDateTime qdt=QDateTime::date();
    QDateTime qdt=QDateTime::currentDateTime();
    QString dt,tt;

    try {
        dt="Date(DD-MM-YYYY): "+qdt.toString("dd-MM-yyyy");
        ui->outputDisplayLabel->setText(dt);
        // qDebug()<<"Date: "<<qdt.toString("dd-MM-yyyy");

        tt="Time(HH:MM:SS): "+qdt.toString("HH:mm:ss");
        ui->outputDisplayLabel->setText(dt+"\n"+tt);
        // qDebug()<<"Time: "<<qdt.toString("HH:mm:ss");

    } catch (...) {
        ui->outputDisplayLabel->setText("Sorry, date is not found!");
    }

}

void ResponseWindow::copyFileToFileData(const QStringList &tokens,QString qstrInput,QProcess *proces){
    try {
        if(tokens.size()>=3){ //expected: copy srcfile destfile
            QFile::copy(tokens[1],tokens[2]); //source file - dest file
            ui->outputDisplayLabel->setText("File copied successfully!");
        }
    } catch (...) {
        ui->outputDisplayLabel->setText("Sorry,File copy failed!");
    }
}


void ResponseWindow::clearCurrentTab(const QStringList &tokens, QString qstrInput, QProcess *proces) {

    qDebug()<<"clear tab function ";

    // Get the parent widget
    QWidget *parentWidget = this->window(); //find top level parent window directly
    if (!parentWidget) {
        qDebug()<<"clear tab :parent windows does not found ";
        return;
    }

    // Find the QTabWidget inside the parent
    QTabWidget *tabWidget = parentWidget->findChild<QTabWidget*>();
    if (!tabWidget){
        qDebug()<<"clear tab :tab widget does not found ";
        return; // Check if found
    }

    //find the current tab
    QWidget *currentTab = tabWidget->widget(pwTabIndex);

    // Find all child widgets inside the current tab
    // QList<QWidget*> childWidgets = currentTab->findChildren<QWidget*>();

    // find all ResponseWindows widgets from tab widget
    QList<ResponseWindow*> responseWindows = currentTab->findChildren<ResponseWindow*>();

    for (ResponseWindow *rw : responseWindows) {
        delete rw; //delete response windows of current tab
    }

    qDebug()<<"cleared all tabs";

}


void ResponseWindow::changeDirectory(const QStringList &tokens, QString qstrInput, QProcess *proces) {

    if(tokens.size() < 2) {
        ui->outputDisplayLabel->setText("Error: No directory specified");
        qDebug() << "Error: No directory specified";
        return;
    }

    QDir dir(pw->current_directory_map[pwTabIndex]);
    QString targetPath = dir.absoluteFilePath(tokens[1]);
    // qDebug() << "Resolved Target Directory: " << targetPath;

    if(!QDir(targetPath).exists()) {
        ui->outputDisplayLabel->setText("directory path does not exist"+targetPath);
        return;
    }

    if(dir.cd(targetPath)) {
        pw->current_directory_map[pwTabIndex] = dir.canonicalPath();
        ui->outputDisplayLabel->setText("Directory : "+dir.canonicalPath());
    }
    else{
        ui->outputDisplayLabel->setText(targetPath);
    }
}


// Dummy built-in command handler
void ResponseWindow::executeManualImplementedCommand(const QStringList &tokens, QString qstrInput, QProcess *process) {

    qDebug() << "execute manual shell function called";
    std::string str = tokens[0].toStdString();
    qDebug() << "Entered cmd by user: " << QString::fromStdString(str);

    #ifdef Q_OS_WIN
        // Check if the command exists before accessing it
        if (this->windows_shell_cmd_map.find(str) != this->windows_shell_cmd_map.end()) {
            this->windows_shell_cmd_map.at(str)(tokens, qstrInput, process); // Call appropriate function
        } else {
            ui->outputDisplayLabel->setText("Error: Command not found in windows!");
        }
    #else
        if (linux_shell_cmd_map.find(str) != linux_shell_cmd_map.end()) {
            linux_shell_cmd_map[str](tokens, qstrInput, process); // Call appropriate function
        } else {
            ui->outputDisplayLabel->setText("Error: Command not found in linux!");
        }
    #endif


    //Directly Executable via cmd.exe:             DIR, ECHO, EXIT, FOR, PAUSE, PROMPT, SET, TIME, TITLE
    //Requires Manual Handling in Your Shell:         CD, CLS, COPY, DATE, DEL, MD, MOVE, PATH, RD, REN

}

// Run a command via the system shell if it is not recognized
void runViaSystemShell(const QString &input, QProcess *process) {

    #ifdef Q_OS_WIN
        process->setProgram("cmd.exe");
        process->setArguments(QStringList() << "/c" << input);
    #else
        process->setProgram("/bin/sh");
        process->setArguments(QStringList() << "-c" << input);
    #endif

    process->start(); // Run the command asynchronously

    // if (!process->waitForStarted()) {
    //     qDebug() << "Failed to start process!";
    //     return;
    // }

    // process->waitForFinished();  // Optional: Wait for the command to finish

    // qDebug() << "Command Output:\n" << process->readAllStandardOutput();
    // qDebug() << "Command Error Output:\n" << process->readAllStandardError();




    //pass directly with cmd.exe
    /*
     *
    windows DIR
    windows ECHO
    windows EXIT
    windows FOR
    windows SET
    windows TIME
    windows TITLE
    windows PAUSE
    windowsPROMPT
     *
     *
    linux echo
    linux exit
    linux history
    linux pwd
    linux set
    linux umask
    linux unalias
     *
     * */

}


// Function to tokenize input (simple whitespace splitting)
QStringList tokenizeInput(const QString &input) {
    return input.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
}

// Check if the given path exists
bool ResponseWindow::fileExists(const QString &filePath) {
    QFileInfo fileInfo(filePath);

    QDir currentTabDir= pw->current_directory_map[pwTabIndex];

    // If file path is absolute, check directly
    if (fileInfo.isAbsolute()) {
        return fileInfo.exists();
    }

    // If file path is relative, check inside the tab's directory
    QString fullFilePath = currentTabDir.filePath(filePath);

    return QFileInfo::exists(fullFilePath);
}


// Check if the file is executable
bool isExecutable(const QString &filePath) {
    QFileInfo fileInfo(filePath);

    #ifdef Q_OS_WIN
            // Windows: Check common executable extensions
        static const QStringList executableExtensions = {
            ".exe", ".bat", ".cmd", ".com", ".scr", ".msi", ".dll", ".ps1"
        };

        QString fileExt = fileInfo.suffix().toLower();
        return executableExtensions.contains("." + fileExt);
    #else
        // Linux/macOS: Check if file has execute permission
        return fileInfo.isExecutable(); //builtin to check executable linux/macos file
    #endif
}


// Check if a file is a source code file
bool isSourceFile(const QString &filePath) {
    return filePath.endsWith(".c", Qt::CaseInsensitive) || //support of c
           filePath.endsWith(".cpp", Qt::CaseInsensitive) || //support of c++
           filePath.endsWith(".py", Qt::CaseInsensitive) || //support of python
           filePath.endsWith(".java", Qt::CaseInsensitive); //support of java
}

// Determine if a source code file requires compilation
bool needsCompilation(const QString &filePath) {
    return filePath.endsWith(".c", Qt::CaseInsensitive) ||
           filePath.endsWith(".cpp", Qt::CaseInsensitive);
}

// Launch an executable process
void launchProcess(const QString command, const QStringList arguments, QProcess *process) {

    process->setProgram(command);
    process->setArguments(arguments);
    process->start(); //start process and wait at from where this function has called

    // if (!process.waitForStarted()) {
    //     qDebug() << "Failed to start process:" << command;
    //     return;
    // }
    // process.waitForFinished();
    // qDebug() << "Process output:" << process.readAllStandardOutput();
}

// Compile the source file and run it (for C/C++)
void compileAndRun(const QString sourceFile, const QStringList arguments, QProcess *process) {

    QString outputExecutable = "tempExecutable";

    #ifdef Q_OS_WIN
        outputExecutable.append(".exe");
    #endif
        QString compiler = sourceFile.endsWith(".c", Qt::CaseInsensitive) ? "gcc" : "g++";

        QStringList compileArgs;
        compileArgs << sourceFile << "-o" << outputExecutable;

        process->start(compiler, compileArgs);
        if (!process->waitForFinished() || process->exitCode() != 0) {
            qDebug() << "Compilation failed for:" << sourceFile;
            qDebug() << "Compiler output:" << process->readAllStandardError();
            return;
        }
        qDebug() << "Compilation successful. Running executable...";

        QStringList execArgs = arguments.mid(1);
        launchProcess(outputExecutable, execArgs, process);
}

// Compile and Run Java source file
void compileAndRunJava(const QString sourceFile, const QStringList arguments, QProcess *process) {

    if (!sourceFile.endsWith(".java", Qt::CaseInsensitive)) {
        qDebug() << "Not a valid Java source file.";
        return;
    }

    // Step 1: Compile Java file with javac
    process->start("javac", QStringList() << sourceFile);
    if (!process->waitForFinished() || process->exitCode() != 0) {
        qDebug() << "Compilation failed for:" << sourceFile;
        qDebug() << "Compiler output:" << process->readAllStandardError();
        return;
    }
    qDebug() << "Compilation successful.";

    // Extract class name from file (remove path & extension)
    QString className = QFileInfo(sourceFile).baseName();

    // Step 2: Run the Java class
    QStringList execArgs;
    execArgs << className;
    execArgs.append(arguments.mid(1)); // Extra arguments

    process->start("java", execArgs);

    // if (!process.waitForStarted()) {
    //     qDebug() << "Failed to start Java process for:" << className;
    //     return;
    // }
    // process.waitForFinished();
    // qDebug() << "Java Program Output:\n" << process.readAllStandardOutput();
}



// Run a source file with an interpreter (for Python)
void runWithInterpreter(const QString &sourceFile, const QStringList &arguments, QProcess *process) {

    // Determine available Python interpreter
    QString pythonInterpreter = "python";

    #ifdef Q_OS_UNIX
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        if (env.contains("PYTHON")) {
            pythonInterpreter = env.value("PYTHON");
        } else if (QFileInfo("/usr/bin/python3").exists()) {
            pythonInterpreter = "python3";
        }
    #endif

        QStringList args;
        args << sourceFile;
        args << arguments.mid(1);

        process->setProgram(pythonInterpreter);
        process->setArguments(args);
        process->start();

        // if (!process.waitForStarted()) {
        //     qDebug() << "Failed to start interpreter for:" << sourceFile;
        //     return;
        // }
        // process.waitForFinished();
        // qDebug() << "Interpreter output:" << process.readAllStandardOutput();
}


void ResponseWindow::onInputEntered()
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

        if(qstrInput.isEmpty()){
            ui->outputDisplayLabel->setText("Please enter some input");
            setNewResponseWindow(); //setting fresh input window
            return;
        }

        strInput=qstrInput.toStdString();

        //user input processing for correct execution
        QStringList tokens = tokenizeInput(qstrInput);
        QString command = tokens[0];
        QProcess *process=new QProcess(this);

        qDebug()<<"entered cmd: "<<tokens[0];


        //veryfy running of .cpp .exe

        //handling of executable-source-manaual-shell file and comands
        if(fileExists(command)){ //is file exists executable or source file

            if(isExecutable(command)) {
                launchProcess(command, tokens,process); //executable file
                qDebug()<<"exec file";
            }
            else if (isSourceFile(command)) {

                if(command.endsWith(".java", Qt::CaseInsensitive))
                    compileAndRun(command, tokens,process); //compliler source file : .java
                else if(needsCompilation(command))
                    compileAndRun(command, tokens,process); //compliler source file : .c .cpp
                else
                    runWithInterpreter(command, tokens,process); //interpreter src file : .py
            }
            else{
                qDebug() << "File exists but is not recognized as executable or source code:" << command;
            }
        }
        else {

            if(windows_shell_cmd_map.find(tokens[0].toStdString())!= windows_shell_cmd_map.end() ){
                qDebug()<<"cd found in shell cmd map";
                executeManualImplementedCommand(tokens,qstrInput,process);
                setNewResponseWindow(); //set the new response window after clearing all old
                return;
                // Built-in command was executed
            }
            else{
                // Fallback: run via system shell
                runViaSystemShell(qstrInput,process);
            }
        }


        if(!process->waitForStarted()){
            ui->outputDisplayLabel->setText("Process Creation faild.");
            process->deleteLater();
            return;
        }

        if(!process->waitForFinished()){
            ui->outputDisplayLabel->setText("Wait to be finished process!");
            process->deleteLater();
            return;
        }

        QByteArray array=process->readAllStandardOutput();

        qstrOutput=QString::fromUtf8(array); //convert to human readable

        // qstrOutput=qstrOutput.trimmed(); //trimming unnecessary chars
        strOutput=qstrOutput.toStdString(); //converts to standard string

        //set output to display label
        ui->outputDisplayLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum); //vertically can expand

        if(qstrOutput.isEmpty())
            ui->outputDisplayLabel->setText("Sorry,output does not found");
        else
            ui->outputDisplayLabel->setText(qstrOutput); //sets content to label

        ui->outputDisplayLabel->adjustSize(); //adjuct if contetnt has more lines
        ui->outputDisplayLabel->setWordWrap(true);
        ui->outputDisplayLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        ui->outputDisplayLabel->setMinimumSize(QSize(0, 0));

        qDebug()<<" called for next input line";
        setNewResponseWindow(); // now it create new response window beneath the current res window

        process->deleteLater(); //deletes process after executing it and showing output
}

