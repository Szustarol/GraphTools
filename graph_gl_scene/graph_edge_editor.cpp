#include "graph_edge_editor.h"
#include "ui_graph_edge_editor.h"
#include <QValidator>
#include <utility>

graph_edge_editor::graph_edge_editor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::graph_edge_editor)
{
    ui->setupUi(this);
    QRegExpValidator* rxv = new QRegExpValidator(QRegExp("[+-]?\\d*[\\.]?\\d+"), this);
    ui->weight_edit->setValidator(rxv);
}

graph_edge_editor::~graph_edge_editor()
{
    delete ui;
}

void graph_edge_editor::set_state(bool directed, float weight)
{
    ui->directed_checkbox->setChecked(directed);
    ui->weight_edit->setText(QString::number(weight));
}

std::pair<bool, float> graph_edge_editor::get_state()
{
    bool directed = ui->directed_checkbox->isChecked();
    float weight = ui->weight_edit->text().toFloat();
    return std::make_pair(directed, weight);
}
