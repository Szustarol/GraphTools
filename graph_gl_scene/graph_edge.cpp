#include "graph_edge.h"
#include <QtMath>
#include <iostream>
#include "graph_node.h"

graph_edge::graph_edge(graph_window_data * window_data)
{
    this->window_data = window_data;

    begin_pos = QVector2D(0.0f, 0.0f);
}

void graph_edge::set_position(const QVector2D & begin, const QVector2D & end){
    this->begin_pos = begin;
    this->end_pos = end;
}

void graph_edge::draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent, QMatrix4x4 * view){
    shader.bind();
    QVector2D start_point = begin_pos;
    QVector2D diff = end_pos - begin_pos;
    float angle = atan2f(diff.y(), diff.x()) - M_PI/2;
    angle *= -180/M_PI;
    float len = diff.length();

    auto & vao = window_data->get_edge_vao(directed);
    vao.bind();

    start_point.setX(start_point.x() + graph_node::screen_offset_x);
    start_point.setY(start_point.y() + graph_node::screen_offset_y);

    model.setToIdentity();
    model.translate(start_point);
    model.translate(diff*0.5);
    model.scale(len/2);
    model.rotate(-angle, QVector3D(0, 0, 1.0f));
    model.scale(0.1/diff.length(), 1);

    shader.setUniformValue("model_matrix", model);
    if(directed)
        glDrawElements(GL_TRIANGLES, window_data->n_directed_elements*3, GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_TRIANGLES, window_data->n_undirected_elements*3, GL_UNSIGNED_INT, 0);
    vao.release();
    shader.release();
}
