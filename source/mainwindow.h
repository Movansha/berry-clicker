#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainwindow.h"
#include <QCloseEvent>

#include <string>
#include <QString>

using std::string;

//----------

extern string change_key;
extern QString default_stylesheet;
extern QString selected_stylesheet;

//----------

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    static MainWindow* instance;

    void update_key_label(const string& toggle_key_button, const string& label);
    void update_status_label();
    void change_checkbox(const string& button, const bool& status);

    void load_cfg_into_ui();

private slots:
    void change_toggle_key(const string& for_mouse_button);
    void change_CPS_value(const string& json_item, const int& value);
    void change_mouse_hold(const string& json_item, const bool& state);

    void change_mouse_button(const string& mouse_button);
    void change_toggle_sound(const bool& status);

    void developer_site();

private:
    Ui::MainWindowClass ui;

    void update_stylesheets();
    void connect_buttons();

protected:
    void closeEvent(QCloseEvent* event) override;
};
