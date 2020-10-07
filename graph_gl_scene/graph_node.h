#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "graph_window_data.h"
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QPainter>
#include <QOpenGLWidget>

class graph_node
{

    graph_window_data * graph_window;

    QMatrix4x4  translation_matrix;

    float posx, posy;

    float scale;

    void create_model();

public:

    unsigned index;

    QVector2D get_position() const;

    static float aspect_ratio;
    static float screen_offset_x, screen_offset_y;

    graph_node(graph_window_data * graph_window);

    ~graph_node();

    void set_position(float x, float y);

    void set_scale(float scale);

    void draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent, QMatrix4x4 * view, QMatrix4x4 * projection);
};

#endif // GRAPH_NODE_H
