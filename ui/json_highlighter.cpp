#include "json_highlighter.h"

JsonHighlighter::JsonHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    tagFormat.setForeground(Qt::darkBlue);
    tagFormat.setFontWeight(QFont::Bold);
    rule.format = tagFormat;
    rule.pattern = QRegularExpression(QStringLiteral("[{}\\[\\],]"));
    highlightingRules.append(rule);
    rule.pattern = QRegularExpression(QStringLiteral("[/\\?]?>"));
    highlightingRules.append(rule);

    commentFormat.setForeground(Qt::red);

    valueFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression(QStringLiteral("\"[^\"]*\""));
    rule.format = valueFormat;
    highlightingRules.append(rule);

    keyFormat.setFontWeight(QFont::Bold);
    keyFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression(QStringLiteral("[#@]?\\w+:"));
    rule.format = keyFormat;
    highlightingRules.append(rule);
}

void JsonHighlighter::highlightBlock(const QString &text)
{
    for (const HighlightingRule &rule : qAsConst(highlightingRules)) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
