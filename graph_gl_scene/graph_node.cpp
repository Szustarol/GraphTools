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
    translation_matrix.translate(posx+screen_offset_x, posy+screen_offset_y);
    translation_matrix.scale(scale);

}

void graph_node::set_position(float x, float y)
{
    posx = x;
    posy = y;
    //create_model();
}

void graph_node::set_scale(float scale)
{
    this->scale = scale;
    //create_model();
}

QVector2D graph_node::get_position() const {
    QVector2D r;
    r.setX(posx);
    r.setY(posy);
    return r;
};


void graph_node::draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent, QMatrix4x4 *view, QMatrix4x4 * projection){
    shader.bind();
    create_model();
    auto & vao = graph_window->get_circle_vao();
    vao.bind();
    shader.setUniformValue("model_matrix", translation_matrix);
    glDrawElements(GL_TRIANGLES, graph_window->n_circle_elements, GL_UNSIGNED_INT, 0);
    vao.release();
    shader.release();
    QVector4D tr = QVector4D(posx+screen_offset_x, posy+screen_offset_y, 1.0f, 1.0f);
    tr = *projection * *view * tr;
    if(abs(tr.x()) < 1 and abs(tr.y()) < 1){
        float w = parent->width();
        float h = parent->height();
        w = (tr.x()+1)*w/2;
        h = (1-tr.y())*h/2;
        QPainter painter(parent);
        painter.setPen(Qt::white);
        painter.drawText(QRect(w-50, h-50, 100, 100), Qt::AlignCenter | Qt::AlignHCenter, QString::number(index));
        painter.end();
    }
}


graph_node::~graph_node(){
}
