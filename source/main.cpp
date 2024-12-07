#include "settings.h"

#include <QtWidgets/QApplication>
#include "main_window.h"

#include "clicker.h"
#include <thread>


using std::thread;

int main(int argc, char* argv[]) {
    setup_cfg();

    QApplication app(argc, argv);
    app.setStyle("Fusion");

    left_Clicker_Thread = thread(clicker::left);
    right_Clicker_Thread = thread(clicker::right);
    msg_loop_Thread = thread(handle_Hooks);

    MainWindow window;
    window.load_cfg_into_ui();
    window.connect_buttons();
    window.show();

    return app.exec();
}