#include "graph_node.h"

#include <QTransform>
#include <cmath>
#include <QOpenGLFunctions>
#include <iostream>

float graph_node::aspect_ratio = 1.0f;

float graph_node::screen_offset_x = 0.0f;
float graph_node::screen_offset_y = 0.0f;


graph_node::graph_node(graph_window_data * graph_window)
{
    this->graph_window = graph_window;
    posx = 0;
    posy = 0;

    scale = 1.0f;

    translation_matrix.setToIdentity();
}

void graph_node::create_model(){
    translation_matrix.setToIdentity();
    translation_matrix.translate(posx, posy);
    translation_matrix.scale(scale);
    translation_matrix.scale(1.0f, aspect_ratio);

}

void graph_node::set_position(float x, float y)
{
    posx = x + screen_offset_x;
    posy = y + screen_offset_y;
    create_model();
}

void graph_node::set_scale(float scale)
{
    this->scale = scale;
    create_model();
}

void graph_node::draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent, QMatrix4x4 *view){
    shader.bind();
    auto & vao = graph_window->get_circle_vao();
    vao.bind();
    shader.setUniformValue("model_matrix", translation_matrix);
    glDrawElements(GL_TRIANGLES, graph_window->n_circle_elements, GL_UNSIGNED_INT, 0);
    vao.release();
    shader.release();
    QVector4D tr = QVector4D(posx, posy, 1.0f, 1.0f);
    tr = *view * tr;
    if(abs(tr.x()) < 1 and abs(tr.y()) < 1){
        float w = parent->width();
        float h = parent->height();
        w = (tr.x()+1)*w/2;
        h = (1-tr.y())*h/2;
        QPainter painter(parent);
        painter.setPen(Qt::white);
        painter.drawText(QRect(w-50, h-50, 100, 100), Qt::AlignCenter | Qt::AlignHCenter, "121");
        painter.end();
    }
}


graph_node::~graph_node(){
}
