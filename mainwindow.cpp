#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph_main_gl_window->set_shader_paths("graph_gl_scene/fragment_shader.glsl",
                                               "graph_gl_scene/vertex_shader.glsl");
    connect(ui->graph_add_vertex,
            SIGNAL(clicked()),
            ui->graph_main_gl_window,
            SLOT(add_vertex_clicked()));

    connect(ui->remove_vertex,
            SIGNAL(clicked()),
            ui->graph_main_gl_window,
            SLOT(remove_vertex_clicked()));

    connect(ui->graph_add_connection,
            SIGNAL(clicked()),
            ui->graph_main_gl_window,
            SLOT(add_edge_undirected_clicked()));

    connect(ui->graph_add_directed_connection,
            SIGNAL(clicked()),
            ui->graph_main_gl_window,
            SLOT(add_edge_directed_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

