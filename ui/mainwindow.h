#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "codeeditor.h"
#include "xml_highlighter.h"
#include "json_highlighter.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);

    void loadFile(const QString &fileName);

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void about();
    bool checkSyntax();
    void minify();
    void prettify();
    void convertToJson();
    void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
    void commitData(QSessionManager &);
#endif

private:
    void setupEditor();
    void setupActions();
    void setupStatusBar();
    bool maybeSave();
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);

    QTabWidget *tabber;
    CodeEditor *xmlEditor;
    XmlHighlighter *xmlHighlighter;
    CodeEditor *jsonEditor;
    JsonHighlighter *jsonHighlighter;
    QString curFile;
};

#endif // MAINWINDOW_H
