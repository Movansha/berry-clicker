#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_interface.h"

#include <QString>
#include <QCloseEvent>

#include <string>

using std::string;

extern string change_Key;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    static MainWindow* instance;

    void update_status_label();
    void update_key_label(const string& button, const string& key);
    void change_hold_Checkbox(const string& button, const bool& status);

    void load_cfg_into_ui();
    void connect_buttons();

private slots:
    void set_SS();

    void change_TK_L();
    void change_TK_R();

    void change_L_CPS(const int& value);
    void change_R_CPS(const int& value);

    void change_MH_L(const bool& state);
    void change_MH_R(const bool& state);

    void change_MB_Left();
    void change_MB_Right();
    void change_MB_Both();

    void change_TS_ON();
    void change_TS_OFF();

    void developer_Site();
private:
    Ui::MainWindowClass ui;

protected:
    void closeEvent(QCloseEvent* event) override;
};