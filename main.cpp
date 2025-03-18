#include "parentwindow.h"

#include <QApplication>
#include <QFontDatabase>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Register the font from the Qt Resource System
    int fontId = QFontDatabase::addApplicationFont(":/fonts/fonts/ttf/JetBrainsMono-Regular.ttf");

    if (fontId == -1) {
        qDebug() << "Failed to load font!";
    } else {
        QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
        // qDebug() << "Available font families:" << fontFamilies;

        if (!fontFamilies.isEmpty()) {
            QFont customFont(fontFamilies.at(0), 10);  // Set font size 12
            customFont.setLetterSpacing(QFont::PercentageSpacing, 110);
            a.setFont(customFont);  // Apply globally

        }
    }


    ParentWindow w;
    w.show();
    return a.exec();
}
