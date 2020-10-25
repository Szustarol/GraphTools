#ifndef GRAPH_EDGE_EDITOR_H
#define GRAPH_EDGE_EDITOR_H

#include <QDialog>
#include <tuple>

namespace Ui {
class graph_edge_editor;
}

class graph_edge_editor : public QDialog
{
    Q_OBJECT

public:
    explicit graph_edge_editor(QWidget *parent = nullptr);
    ~graph_edge_editor();
    void set_state(bool directed, float weight);
    std::pair<bool, float> get_state();

private:
    Ui::graph_edge_editor *ui;
};

#endif // GRAPH_EDGE_EDITOR_H
