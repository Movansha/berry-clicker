#include "settings.h"
#include "clicker.h"

#include "main_window.h"
#include <QString>
#include <QCloseEvent>

#include <iostream>
#include <string>
#include <Windows.h>

using std::string;

string change_Key = "no";


MainWindow* MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    instance = this;
}

MainWindow::~MainWindow() {}

void MainWindow::set_SS() {
    QString default_SS = "QPushButton {font: 11pt \"Corbel\"; color: #ffffff; background: #406eff; border: 2 solid #4a77ff; border-radius: 8;}"
        "QPushButton::hover {background: #6991ff; border: 2 solid #7398ff;}"
        "QPushButton::focus {background: #6991ff; border: 2 solid #7398ff;}"
        "QPushButton::pressed {background: #406eff; border: 2 solid #4a77ff;}";

    QString selected_SS = "QPushButton {font: 11pt \"Corbel\"; color: #ffffff; background: #8c3a5c; border: 2 solid #933d61; border-radius: 8;}"
        "QPushButton::hover {background: #9d5f7a; border: 2 solid #a66482;}"
        "QPushButton::focus {background: #9d5f7a; border: 2 solid #a66482;}"
        "QPushButton::pressed {background: #8c3a5c; border: 2 solid #8c3a5c;}";

    ui.left_Button->setStyleSheet(default_SS);
    ui.right_Button->setStyleSheet(default_SS);
    ui.both_Button->setStyleSheet(default_SS);

    ui.on_Button->setStyleSheet(default_SS);
    ui.off_Button->setStyleSheet(default_SS);

    if (config::mouse_Button == "left") ui.left_Button->setStyleSheet(selected_SS);
    else if (config::mouse_Button == "right") ui.right_Button->setStyleSheet(selected_SS);
    else ui.both_Button->setStyleSheet(selected_SS);

    if (config::toggle_Sound == true) ui.on_Button->setStyleSheet(selected_SS);
    else ui.off_Button->setStyleSheet(selected_SS);
}

void MainWindow::update_key_label(const string& button, const string& key) {
    QString key_text = QString::fromStdString(key);

    if (button == "left") ui.tk_Left_Button->setText(key_text);
    else ui.tk_Right_Button->setText(key_text);

    change_Key = "no";
}

void MainWindow::change_hold_Checkbox(const string& button, const bool& status) {
    if (button == "left") {
        ui.hold_L_CheckBox->blockSignals(true);
        ui.hold_L_CheckBox->setChecked(status);
        ui.hold_L_CheckBox->blockSignals(false);

        config::mouse_hold_L = status;
        write_cfg(line_MH_L, std::to_string(status));
    }

    else {
        ui.hold_R_CheckBox->blockSignals(true);
        ui.hold_R_CheckBox->setChecked(status);
        ui.hold_R_CheckBox->blockSignals(false);

        config::mouse_hold_R = status;
        write_cfg(line_MH_R, std::to_string(status));
    }
}

void MainWindow::update_status_label() {
    string left, right;

    if (is_active_L == true) left = "Active";
    else left = "Deactive";

    if (is_active_R == true) right = "Active";
    else right = "Deactive";

    string label = "Left: " + left + "  |  " + "Right: " + right;
    ui.status_Label->setText(QString::fromStdString(label));
}

// >> ui interaction

void MainWindow::change_TK_L() {
    clicker::stop();
    MainWindow::update_status_label();
    MainWindow::update_key_label("left", "Press a key");

    change_Key = "left";
}

void MainWindow::change_TK_R() {
    clicker::stop();
    MainWindow::update_status_label();
    MainWindow::update_key_label("right", "Press a key");

    change_Key = "right";
}

void MainWindow::change_L_CPS(const int& value) {
    try {
        config::left_CPS = value;
        write_cfg(line_L_CPS, std::to_string(value));
    }
    catch (...) {
        config::left_CPS = 15;
        write_cfg(line_L_CPS, "15");
    }
}

void MainWindow::change_R_CPS(const int& value) {
    try {
        config::right_CPS = value;
        write_cfg(line_R_CPS, std::to_string(value));
    }
    catch (...) {
        config::right_CPS = 20;
        write_cfg(line_R_CPS, "20");
    }
}

void MainWindow::change_MH_L(const bool& state) {
    clicker::stop();
    MainWindow::update_status_label();

    config::mouse_hold_L = state;
    write_cfg(line_MH_L, std::to_string(state));
}

void MainWindow::change_MH_R(const bool& state) {
    clicker::stop();
    MainWindow::update_status_label();

    config::mouse_hold_R = state;
    write_cfg(line_MH_R, std::to_string(state));
}

void MainWindow::change_MB_Left() {
    clicker::stop();
    MainWindow::update_status_label();

    config::mouse_Button = "left";
    write_cfg(line_MB, "left");

    MainWindow::set_SS();
}

void MainWindow::change_MB_Right() {
    clicker::stop();
    MainWindow::update_status_label();

    config::mouse_Button = "right";
    write_cfg(line_MB, "right");

    MainWindow::set_SS();
}

void MainWindow::change_MB_Both() {
    clicker::stop();
    MainWindow::update_status_label();

    config::mouse_Button = "both";
    write_cfg(line_MB, "both");

    MainWindow::set_SS();
}

void MainWindow::change_TS_ON() {
    config::toggle_Sound = true;
    write_cfg(line_TS, "1");
    set_SS();
}

void MainWindow::change_TS_OFF() {
    config::toggle_Sound = false;
    write_cfg(line_TS, "0");
    set_SS();
}

void MainWindow::developer_Site() {
    ShellExecute(0, 0, L"https://github.com/Movansha", 0, 0, SW_SHOWMAXIMIZED);
}

// ui interaction <<

void MainWindow::load_cfg_into_ui() {
    QString selected_SS = "QPushButton {font: 11pt \"Corbel\"; color: #ffffff; background: #8c3a5c; border: 2 solid #933d61; border-radius: 8;}"
        "QPushButton::hover {background: #9d5f7a; border: 2 solid #a66482;}"
        "QPushButton::focus {background: #9d5f7a; border: 2 solid #a66482;}"
        "QPushButton::pressed {background: #8c3a5c; border: 2 solid #8c3a5c;}";

    QString toggle_Key_L = QString::fromStdString(config::toggle_Key_L);
    QString toggle_Key_R = QString::fromStdString(config::toggle_Key_R);

    ui.tk_Left_Button->setText(toggle_Key_L);
    ui.tk_Right_Button->setText(toggle_Key_R);

    ui.left_cps_SpinBox->setValue(config::left_CPS);
    ui.right_cps_SpinBox->setValue(config::right_CPS);

    ui.hold_L_CheckBox->setChecked(config::mouse_hold_L);
    ui.hold_R_CheckBox->setChecked(config::mouse_hold_R);

    if (config::mouse_Button == "left") ui.left_Button->setStyleSheet(selected_SS);
    else if (config::mouse_Button == "right") ui.right_Button->setStyleSheet(selected_SS);
    else if (config::mouse_Button == "both") ui.both_Button->setStyleSheet(selected_SS);

    if (config::toggle_Sound == true) ui.on_Button->setStyleSheet(selected_SS);
    else if (config::toggle_Sound == false) ui.off_Button->setStyleSheet(selected_SS);
}

void MainWindow::connect_buttons() {
    connect(ui.tk_Left_Button, &QPushButton::clicked, this, &MainWindow::change_TK_L);
    connect(ui.tk_Right_Button, &QPushButton::clicked, this, &MainWindow::change_TK_R);
    connect(ui.left_cps_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(change_L_CPS(int)));
    connect(ui.right_cps_SpinBox, SIGNAL(valueChanged(int)), this, SLOT(change_R_CPS(int)));
    connect(ui.hold_L_CheckBox, &QCheckBox::stateChanged, this, &MainWindow::change_MH_L);
    connect(ui.hold_R_CheckBox, &QCheckBox::stateChanged, this, &MainWindow::change_MH_R);
    connect(ui.left_Button, &QPushButton::clicked, this, &MainWindow::change_MB_Left);
    connect(ui.right_Button, &QPushButton::clicked, this, &MainWindow::change_MB_Right);
    connect(ui.both_Button, &QPushButton::clicked, this, &MainWindow::change_MB_Both);
    connect(ui.on_Button, &QPushButton::clicked, this, &MainWindow::change_TS_ON);
    connect(ui.off_Button, &QPushButton::clicked, this, &MainWindow::change_TS_OFF);
    connect(ui.developer_Button, &QPushButton::clicked, this, &MainWindow::developer_Site);
}

void MainWindow::closeEvent(QCloseEvent* event) {
    PostThreadMessage(msg_loop_ID, WM_QUIT, 0, 0);

    UnhookWindowsHookEx(keyboard_Hook);
    UnhookWindowsHookEx(mouse_Hook);

    if (msg_loop_Thread.joinable()) msg_loop_Thread.join();

    clicker::stop();
    clicker::program = false;

    if (left_Clicker_Thread.joinable()) left_Clicker_Thread.join();
    if (right_Clicker_Thread.joinable()) right_Clicker_Thread.join();

    instance = nullptr;
    event->accept();
}