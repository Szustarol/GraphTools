#include "graph_node.h"

#include <QTransform>
#include <cmath>
#include <QOpenGLFunctions>
#include <iostream>

graph_node::graph_node(graph_window_data * graph_window)
{
    this->graph_window = graph_window;
    posx = 0;
    posy = 0;

    translation_matrix.setToIdentity();
}

void graph_node::set_position(float x, float y)
{
    translation_matrix.setToIdentity();
    translation_matrix.translate(x, y);
    posx = x;
    posy = y;
}

void graph_node::set_scale(float scale)
{
    translation_matrix.setToIdentity();
    set_position(posx, posy);
    translation_matrix.scale(scale);
}

void graph_node::draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent){
    shader.bind();
    auto & vao = graph_window->get_circle_vao();
    vao.bind();
    shader.setUniformValue("view_matrix", translation_matrix);
    glDrawElements(GL_TRIANGLES, graph_window->n_circle_elements, GL_UNSIGNED_INT, 0);
    vao.release();
    shader.release();
    if(abs(posx) < 1 and abs(posy) < 1){
        std::cout << "HERE" << std::endl;
        float w = parent->width();
        float h = parent->height();
        w = (posx+1)*w/2;
        h = (1-posy)*h/2;
        std::cout << "------------" << std::endl;
        std::cout << parent->width() << " " <<  parent->height() << std::endl;
        std::cout << posx << " " <<  posy << std::endl;
        std::cout << w << " " <<  h << std::endl;
        QPainter painter(parent);
        painter.setPen(Qt::white);
        painter.drawText(QRect(w-50, h-50, 100, 100), Qt::AlignCenter | Qt::AlignHCenter, "121");
        painter.end();
    }
}


graph_node::~graph_node(){
}
