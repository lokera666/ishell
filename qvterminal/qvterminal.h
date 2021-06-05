#ifndef QVTERMINAL_H
#define QVTERMINAL_H

#include <QAbstractScrollArea>
#include <QAction>
#include <QTimer>

#include "qvtlayout.h"
#include "sshclient.h"

class QVTerminal : public QAbstractScrollArea
{
    Q_OBJECT
public:
    explicit QVTerminal(QWidget *parent = nullptr);
    ~QVTerminal();

    // style
    const QVTCharFormat &format() const;
    void setFormat(const QVTCharFormat &format);

    // mode
    bool echo() const;
    void setEcho(bool echo);

    bool crlf() const;
    void setCrlf(bool crlf);
    void appendData(QByteArray data);
signals:

public slots:
    void writeData(QByteArray data);

    void paste();

protected slots:

//    void appendData(QByteArray data);
    void appendString(QString str);
    void toggleCursor();

private:
    SSHClient* sshClient;
    // parser
    enum State {
        Text,
        Escape,
        Format,
        ResetFont
    };
    State _state;
    int _formatValue;

    // cursor
    QVTCharFormat _format;
    QVTCharFormat _curentFormat;
    int _cw;
    int _ch;
    QPoint _cursorPos;
    QTimer _cursorTimer;
    bool _cvisible;

    // data
    QVTLayout *_layout;

    // mode
    bool _echo;
    bool _crlf;

    // QWidget interface
protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    virtual void mousePressEvent(QMouseEvent* event);
#ifndef QT_NO_CONTEXTMENU
    virtual void contextMenuEvent(QContextMenuEvent *event);
#endif // QT_NO_CONTEXTMENU

    // QAbstractScrollArea interface
protected:
    virtual bool viewportEvent(QEvent *event);
    QColor vt100color(char c);

    QAction *_pasteAction;
};

#endif // QVTERMINAL_H
