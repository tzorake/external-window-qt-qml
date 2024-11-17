#ifndef EMBEDDEDAPPLICATION_H
#define EMBEDDEDAPPLICATION_H

#include <QQuickItem>
#include <QProcess>
#include <QWindow>
#include <QWidget>
#include <QQuickWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <windows.h>

class EmbeddedApplication
    : public QQuickItem
{
    Q_OBJECT
public:
    explicit EmbeddedApplication(QQuickItem *parent = nullptr);

    Q_INVOKABLE void launchAndEmbed(const QString &programPath, const QStringList &arguments = {});

private:
    QWindow *externalWindow = nullptr;
};

#endif // EMBEDDEDAPPLICATION_H
