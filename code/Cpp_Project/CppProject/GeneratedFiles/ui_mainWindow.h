/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_mainWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionClose;
    QAction *actionExit;
    QAction *actionAbout;
    QWidget *centralWidget;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menu0;
    QMenu *menu1;

    void setupUi(QMainWindow *mainWindowClass)
    {
        if (mainWindowClass->objectName().isEmpty())
            mainWindowClass->setObjectName(QStringLiteral("mainWindowClass"));
        mainWindowClass->setWindowModality(Qt::NonModal);
        mainWindowClass->setEnabled(true);
        mainWindowClass->resize(160, 190);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(mainWindowClass->sizePolicy().hasHeightForWidth());
        mainWindowClass->setSizePolicy(sizePolicy);
        mainWindowClass->setMinimumSize(QSize(160, 167));
        mainWindowClass->setMaximumSize(QSize(160, 167));
        actionOpen = new QAction(mainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen->setCheckable(false);
        QIcon icon;
        QString iconThemeName = QStringLiteral("None");
        if (QIcon::hasThemeIcon(iconThemeName)) {
            icon = QIcon::fromTheme(iconThemeName);
        } else {
            icon.addFile(QStringLiteral("."), QSize(), QIcon::Normal, QIcon::Off);
        }
        actionOpen->setIcon(icon);
        actionClose = new QAction(mainWindowClass);
        actionClose->setObjectName(QStringLiteral("actionClose"));
        actionExit = new QAction(mainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionAbout = new QAction(mainWindowClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        centralWidget = new QWidget(mainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        centralWidget->setMinimumSize(QSize(256, 0));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 160, 144));
        label->setMinimumSize(QSize(160, 144));
        label->setMaximumSize(QSize(160, 144));
        label->setAlignment(Qt::AlignBottom|Qt::AlignLeading|Qt::AlignLeft);
        mainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(mainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 160, 23));
        menu0 = new QMenu(menuBar);
        menu0->setObjectName(QStringLiteral("menu0"));
        menu1 = new QMenu(menuBar);
        menu1->setObjectName(QStringLiteral("menu1"));
        mainWindowClass->setMenuBar(menuBar);

        menuBar->addAction(menu0->menuAction());
        menuBar->addAction(menu1->menuAction());
        menu0->addAction(actionOpen);
        menu0->addSeparator();
        menu0->addAction(actionClose);
        menu0->addAction(actionExit);
        menu1->addAction(actionAbout);

        retranslateUi(mainWindowClass);

        QMetaObject::connectSlotsByName(mainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindowClass)
    {
        mainWindowClass->setWindowTitle(QApplication::translate("mainWindowClass", "GameBoy Simulator", nullptr));
        actionOpen->setText(QApplication::translate("mainWindowClass", "\346\211\223\345\274\200...", nullptr));
#ifndef QT_NO_SHORTCUT
        actionOpen->setShortcut(QApplication::translate("mainWindowClass", "Ctrl+O", nullptr));
#endif // QT_NO_SHORTCUT
        actionClose->setText(QApplication::translate("mainWindowClass", "\345\205\263\351\227\255ROM", nullptr));
        actionExit->setText(QApplication::translate("mainWindowClass", "\351\200\200\345\207\272", nullptr));
        actionAbout->setText(QApplication::translate("mainWindowClass", "\345\205\263\344\272\216...", nullptr));
        label->setText(QString());
        menu0->setTitle(QApplication::translate("mainWindowClass", "\346\226\207\344\273\266", nullptr));
        menu1->setTitle(QApplication::translate("mainWindowClass", "\345\270\256\345\212\251", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindowClass: public Ui_mainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
