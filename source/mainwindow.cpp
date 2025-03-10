#include "config.h"
#include "mainwindow.h"
#include "clicker.h"

#include <string>
#include <QString>

#include <QCloseEvent>

using std::string;

//----------

string change_key = "false";

QString default_stylesheet =
"QPushButton {background: #406eff; color: #ffffff; border: 2 solid #4772f3; border-radius: 9;}"
"QPushButton::hover {background: #709bff; border: 2 solid #739dff;}"
"QPushButton::focus {background: #709bff; border: 2 solid #739dff;}"
"QPushButton::pressed {background: #7383ea; border: 2 solid #6476ce;}"
;

QString selected_stylesheet = 
"QPushButton {background: #963f66; color: #ffffff; border: 2 solid #933d61; border-radius: 9;}"
"QPushButton::hover {background: #a76784; border: 2 solid #a66482;}"
"QPushButton::focus {background: #a76784; border: 2 solid #a66482;}"
"QPushButton::pressed {background: #a55d6c; border: 2 solid #834956;}"
;


//----------

MainWindow* MainWindow::instance = nullptr;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    ui.setupUi(this);
    instance = this;

    MainWindow::connect_buttons();
}

MainWindow::~MainWindow() {}

//----------

void MainWindow::update_stylesheets() {
    ui.button_left->setStyleSheet(default_stylesheet);
    ui.button_right->setStyleSheet(default_stylesheet);
    ui.button_both->setStyleSheet(default_stylesheet);

    ui.button_ON->setStyleSheet(default_stylesheet);
    ui.button_OFF->setStyleSheet(default_stylesheet);

    if (config_json["mouse_button"] == "left") ui.button_left->setStyleSheet(selected_stylesheet);
    else if (config_json["mouse_button"] == "right") ui.button_right->setStyleSheet(selected_stylesheet);
    else if (config_json["mouse_button"] == "both") ui.button_both->setStyleSheet(selected_stylesheet);

    if (config_json["toggle_sound"] == true) ui.button_ON->setStyleSheet(selected_stylesheet);
    else ui.button_OFF->setStyleSheet(selected_stylesheet);
}

void MainWindow::update_key_label(const string& toggle_key_button, const string& label) {
    QString x = QString::fromStdString(label);

    if (toggle_key_button == "left") ui.button_tk_left->setText(x);
    if (toggle_key_button == "right") ui.button_tk_right->setText(x);
}

void MainWindow::update_status_label() {
    string left;
    string right;

    left = (is_active_L == true) ? "Active" : "Deactive";
    right = (is_active_R == true) ? "Active" : "Deactive";

    string x = "Left: " + left + "  |  " + "Right: " + right;

    ui.label_status->setText(QString::fromStdString(x));
}

//----------

void MainWindow::change_checkbox(const string& button, const bool& status) {
    if (button == "left") {
        config_json["mouse_hold_left"] = status;

        if (!write_json()) {
            MainWindow::load_cfg_into_ui();
            return;
        }

        ui.checkbox_hold_L->blockSignals(true);
        ui.checkbox_hold_L->setChecked(status);
        if (status != false) ui.checkbox_hold_L->blockSignals(false);
    }

    if (button == "right") {
        config_json["mouse_hold_right"] = status;

        if (!write_json()) {
            MainWindow::load_cfg_into_ui();
            return;
        }

        ui.checkbox_hold_R->blockSignals(true);
        ui.checkbox_hold_R->setChecked(status);
        if (status != false) ui.checkbox_hold_R->blockSignals(false);
    }
}

//----------
// ui interaction -->

void MainWindow::change_toggle_key(const string& for_mouse_button)  {
    if (change_key != "false") return;

    clicker::stop();

    MainWindow::update_status_label();
    MainWindow::update_key_label(for_mouse_button, "Press a key..");

    change_key = for_mouse_button;
}

void MainWindow::change_CPS_value(const string& json_item, const int& value) {
    config_json[json_item] = value;

    if (!write_json()) MainWindow::load_cfg_into_ui();
}

void MainWindow::change_mouse_hold(const string& json_item, const bool& state) {
    clicker::stop();

    MainWindow::update_status_label();

    config_json[json_item] = state;

    if (!write_json()) MainWindow::load_cfg_into_ui();
}

void MainWindow::change_mouse_button(const string& mouse_button) {
    clicker::stop();

    MainWindow::update_status_label();

    config_json["mouse_button"] = mouse_button;

    if (!write_json()) {
        MainWindow::load_cfg_into_ui();
        return;
    }

    MainWindow::update_stylesheets();
}

void MainWindow::change_toggle_sound(const bool& status) {
    config_json["toggle_sound"] = status;

    if (!write_json()) {
        MainWindow::load_cfg_into_ui();
        return;
    }

    MainWindow::update_stylesheets();
}

void MainWindow::developer_site() {
    ShellExecute(0, 0, L"https://github.com/Movansha", 0, 0, SW_SHOWMAXIMIZED);
}

// ui interaction <--
//----------

void MainWindow::connect_buttons() {
    connect(ui.button_tk_left, &QPushButton::clicked, this, [&]() {MainWindow::change_toggle_key("left"); });
    connect(ui.button_tk_right, &QPushButton::clicked, this, [&]() {MainWindow::change_toggle_key("right"); });
    
    connect(ui.spinbox_left, &QSpinBox::valueChanged, this, [&]() {MainWindow::change_CPS_value("left_cps", ui.spinbox_left->value()); });
    connect(ui.spinbox_right, &QSpinBox::valueChanged, this, [&]() {MainWindow::change_CPS_value("right_cps", ui.spinbox_right->value()); });

    connect(ui.checkbox_hold_L, &QCheckBox::stateChanged, this, [&]() {MainWindow::change_mouse_hold("mouse_hold_left", ui.checkbox_hold_L->isChecked()); });
    connect(ui.checkbox_hold_R, &QCheckBox::stateChanged, this, [&]() {MainWindow::change_mouse_hold("mouse_hold_right", ui.checkbox_hold_R->isChecked()); });

    connect(ui.button_left, &QPushButton::clicked, this, [&]() {MainWindow::change_mouse_button("left"); });
    connect(ui.button_right, &QPushButton::clicked, this, [&]() {MainWindow::change_mouse_button("right"); });
    connect(ui.button_both, &QPushButton::clicked, this, [&]() {MainWindow::change_mouse_button("both"); });

    connect(ui.button_ON, &QPushButton::clicked, this, [&]() {MainWindow::change_toggle_sound(true); });
    connect(ui.button_OFF, &QPushButton::clicked, this, [&]() {MainWindow::change_toggle_sound(false); });

    connect(ui.button_developer, &QPushButton::clicked, this, &MainWindow::developer_site);
}

void MainWindow::load_cfg_into_ui() {
    ui.button_tk_left->setText(QString::fromStdString(config_json["toggle_key_left"]));
    ui.button_tk_right->setText(QString::fromStdString(config_json["toggle_key_right"]));

    ui.spinbox_left->setValue(config_json["left_cps"]);
    ui.spinbox_right->setValue(config_json["right_cps"]);

    ui.checkbox_hold_L->setChecked(config_json["mouse_hold_left"]);
    ui.checkbox_hold_R->setChecked(config_json["mouse_hold_right"]);

    MainWindow::update_stylesheets();
}

//----------

void MainWindow::closeEvent(QCloseEvent* event) {
    PostThreadMessage(msg_loop_ID, WM_QUIT, 0, 0);

    UnhookWindowsHookEx(keyboard_hook);
    UnhookWindowsHookEx(mouse_hook);

    if (msg_loop_Thread.joinable()) msg_loop_Thread.join();

    clicker::stop();

    config_json["left_cps"] = 0;
    config_json["right_cps"] = 0;

    clicker::program = false;

    if (left_clicker_Thread.joinable()) left_clicker_Thread.join();
    if (right_clicker_Thread.joinable()) right_clicker_Thread.join();

    instance = nullptr;
    event->accept();
}
