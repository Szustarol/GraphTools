#ifndef GRAPH_NODE_H
#define GRAPH_NODE_H

#include "graph_window_data.h"
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

class graph_node
{

    graph_window_data * graph_window;

    QMatrix4x4  translation_matrix;

    float posx, posy;

public:
    graph_node(graph_window_data * graph_window);

    ~graph_node();

    void set_position(float x, float y);

    void set_scale(float scale);

    void draw(QOpenGLShaderProgram & shader);
};

#endif // GRAPH_NODE_H
