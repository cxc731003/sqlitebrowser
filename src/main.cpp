#include "Application.h"
#include "sqlite.h"

void db4sMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();

    // Emulate the default Qt treatment. This might not work in some OS, like Windows.
    fprintf(stderr, "%s\n", localMsg.constData());

    const char *file = context.file ? context.file : "";
    const char *function = context.function ? context.function : "";
    localMsg = QString("%1 (%2, %3:%4)\n").arg(msg).arg(function).arg(file).arg(context.line).toLocal8Bit();

    // Log using the SQLite log mechanism, so it gets the same treatment than messages
    // reported by SQLite itself. This will allow these messages to be seen in our Log window.
    // Error code will be the type
    sqlite3_log(static_cast<int>(type), localMsg.constData());
}

int main( int argc, char ** argv )
{
    qInstallMessageHandler(db4sMessageOutput);

    // Create application object. All the initialisation stuff happens in there
    Application a(argc, argv);

    // Quit application now if user doesn't want to see the UI
    if(a.dontShowMainWindow())
        return 0;

    // Run application
    return a.exec();
}
