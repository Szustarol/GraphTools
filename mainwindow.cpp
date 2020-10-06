#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph_main_gl_window->set_shader_paths("graph_gl_scene/fragment_shader.glsl",
                                               "graph_gl_scene/vertex_shader.glsl");
}

MainWindow::~MainWindow()
{
    delete ui;
}

