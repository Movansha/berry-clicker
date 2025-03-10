#include "config.h"

#include "mainwindow.h"
#include <QtWidgets/QApplication>

#include "clicker.h"

//----------

int main(int argc, char *argv[])
{
    setup_json();

    QApplication app(argc, argv);
    app.setStyle("Fusion");

    MainWindow window;
    window.load_cfg_into_ui();
    window.show();

    left_clicker_Thread = thread(clicker::left);
    right_clicker_Thread = thread(clicker::right);
    msg_loop_Thread = thread(handle_Hooks);

    return app.exec();
}
