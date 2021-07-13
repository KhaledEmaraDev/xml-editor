#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "codeeditor.h"
#include "highlighter.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void about();
    void newFile();
    void openFile(const QString &path = QString());
    bool saveFile(const QString &path = QString());
    bool checkSyntax();
    void minify();
    void prettify();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    void setupEditor();
    void setupFileMenu();
    void setupToolBar();
    void setupHelpMenu();
    bool maybeSave();

    CodeEditor *editor;
    Highlighter *highlighter;
};

#endif // MAINWINDOW_H
