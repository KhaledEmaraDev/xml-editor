#ifndef JSON_HIGHLIGHTER_H
#define JSON_HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class JsonHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    JsonHighlighter(QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat bracketFormat;
    QTextCharFormat valueFormat;
    QTextCharFormat keyFormat;
};

#endif // JSON_HIGHLIGHTER_H
