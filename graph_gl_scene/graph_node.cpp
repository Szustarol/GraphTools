#include "graph_node.h"

#include <QTransform>
#include <QOpenGLFunctions>

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

void graph_node::draw(QOpenGLShaderProgram & shader){
    auto & vao = graph_window->get_circle_vao();
    vao.bind();
    shader.setUniformValue("view_matrix", translation_matrix);
    glDrawElements(GL_TRIANGLES, graph_window->n_circle_elements, GL_UNSIGNED_INT, 0);
}


graph_node::~graph_node(){
}
