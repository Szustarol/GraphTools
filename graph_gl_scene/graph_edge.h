#ifndef GRAPH_EDGE_H
#define GRAPH_EDGE_H

#include <QVector2D>
#include <QMatrix>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include "graph_window_data.h"

class graph_edge
{
    QVector2D begin_pos;
    QVector2D end_pos;

    QMatrix4x4 model;

    graph_window_data * window_data;

    QPolygonF collision_rect;

public:
    static float all_scale;
    static float screen_offset_x, screen_offset_y;

    unsigned node1_id, node2_id;

    bool directed = false;

    float weight;

    bool is_clicked(QPointF click_position);

    void set_position(const QVector2D & begin, const QVector2D & end);

    void draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent, QMatrix4x4 * view, QMatrix4x4 * projection, bool weighted = false);

    graph_edge(graph_window_data * window_data);
};

#endif // GRAPH_EDGE_H
