#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDoubleValidator>
#include <iostream>
#include "graph_gl_scene/graph_edge.h"
#include "graph_gl_scene/graph_node.h"
#include "parsers/parser.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->graph_main_gl_window->set_shader_paths("graph_gl_scene/fragment_shader.glsl",
                                               "graph_gl_scene/vertex_shader.glsl");

    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("[+-]?\\d*[\\.]?\\d+"), this);
    ui->weight_edit->setValidator(rxv);

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

    connect(ui->weighted_checkbox,
            SIGNAL(stateChanged(int)),
            ui->graph_main_gl_window,
            SLOT(set_graph_weighted(int)));

    connect(ui->graph_edit_button,
            SIGNAL(clicked()),
            ui->graph_main_gl_window,
            SLOT(edit_clicked()));

    connect(ui->weight_edit,
            &QLineEdit::editingFinished,
            this,
            [=](){
                float t = ui->weight_edit->text().toFloat();
                ui->graph_main_gl_window->set_new_edge_weight(t);
            });

    connect(ui->output_type_combo,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            [=](int state){
                if(state == 0){
                    ui->cpp_noedge_label->setVisible(false);
                    ui->cpp_noedge_type->setVisible(false);
                }
                else{
                    ui->cpp_noedge_label->setVisible(true);
                    ui->cpp_noedge_type->setVisible(true);
                }
            });

    connect(ui->generate_output_button,
            &QPushButton::pressed,
            this,
            [=](){
                graph_lang l;
                cpp_nopath n;
                if(ui->output_type_combo->currentIndex() == 0)
                    l = PYTHON;
                else
                    l = CPP;
                switch(ui->cpp_noedge_type->currentIndex()){
                case 0:
                    n = ZERO;
                    break;
                case 1:
                    n = INTMIN;
                    break;
                case 2:
                    n = MIN1;
                    break;
                case 3:
                    n = INTMAX;
                    break;
                }
                QString res;
                if(ui->output_format_combo->currentIndex() == 0)
                    res = parser::to_list_rep(l, ui->graph_main_gl_window->get_data_pair(), ui->graph_main_gl_window->isWeighted(), n);
                else if(ui->output_format_combo->currentIndex() == 1)
                    res = parser::to_matrix_rep(l, ui->graph_main_gl_window->get_data_pair(), ui->graph_main_gl_window->isWeighted(), n);
                ui->graph_result->setText(res);
            });
}


MainWindow::~MainWindow()
{
    delete ui;
}


