#include "EmbeddedApplication.h"

HWND hwnd = nullptr;

struct HandleData {
    DWORD processID;
    HWND hwnd;
};

BOOL CALLBACK MyCallback(HWND handle, LPARAM lParam) {
    HandleData& data = *reinterpret_cast<HandleData*>(lParam);
    DWORD windowProcessID = 0;
    GetWindowThreadProcessId(handle, &windowProcessID);

    if (windowProcessID == data.processID && GetWindow(handle, GW_OWNER) == nullptr && IsWindowVisible(handle)) {
        data.hwnd = handle;

        return FALSE;
    }

    return TRUE;
};

HWND GetMainWindowHandle(DWORD processID) {
    HandleData data = {processID, nullptr};

    EnumWindows(MyCallback, reinterpret_cast<LPARAM>(&data));

    return data.hwnd;
}

EmbeddedApplication::EmbeddedApplication(QQuickItem *parent)
    : QQuickItem(parent)
{
}

void EmbeddedApplication::launchAndEmbed(const QString &programPath, const QStringList &arguments)
{
    Q_UNUSED(arguments);

    STARTUPINFO si = { sizeof(STARTUPINFO) };
    PROCESS_INFORMATION pi;

    if (!CreateProcess(programPath.toStdWString().c_str(), nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        qDebug() << "Failed to start process. Error: " << GetLastError() << Qt::endl;
        return;
    }

    QEventLoop loop;
    QTimer* timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [&]() mutable {
        HWND hwnd = GetMainWindowHandle(pi.dwProcessId);
        if (hwnd) {
            qDebug() << "Main window handle (HWND):" << hwnd;

            WId nativeWinId = reinterpret_cast<WId>(hwnd);
            QWindow *externalWindow = QWindow::fromWinId(nativeWinId);

            QQuickItem *parent = parentItem();
            externalWindow->setParent(parent->window());

            connect(parent, &QQuickItem::xChanged, externalWindow, [parent, externalWindow]() {
                externalWindow->setX(parent->x());
            });
            connect(parent, &QQuickItem::yChanged, externalWindow, [parent, externalWindow]() {
                externalWindow->setY(parent->y());
            });
            connect(parent, &QQuickItem::widthChanged, externalWindow, [parent, externalWindow]() {
                externalWindow->setWidth(parent->width());
            });
            connect(parent, &QQuickItem::heightChanged, externalWindow, [parent, externalWindow]() {
                externalWindow->setHeight(parent->height());
            });

            emit parent->xChanged();
            emit parent->yChanged();
            emit parent->widthChanged();
            emit parent->heightChanged();

            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);

            timer->stop();
            loop.exit();
        }
    });

    timer->start(100);
    loop.exec();
}
