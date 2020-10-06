#include "graph_window_data.h"
#include <QtMath>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions>

void graph_window_data::gen_circle_indices()
{
    unsigned n_all = circle_vertices.size();

    if(n_all < 2) return;

    circle_element_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    circle_element_buffer.create();

    std::vector<GLuint> circle_indices;

    for(GLuint i = 0; i < n_all; i+=2){
        circle_indices.push_back(i);
    }

    circle_indices.push_back(0);
    circle_indices.push_back(1);

    for(GLuint i = 1+2; i < n_all; i+= 2){
        circle_indices.push_back(i);
    }

    circle_indices.push_back(1);

    circle_element_buffer.bind();
    circle_element_buffer.allocate(sizeof(GLuint)*circle_indices.size());

    circle_element_buffer.write(0,
                                circle_indices.data(),
                                sizeof(GLuint)*circle_indices.size());
}

void graph_window_data::build()
{
    gen_circle_vertices();
    gen_circle_indices();

    circle_VAO.create();
    circle_VAO.bind();
    circle_element_buffer.bind();
    circle_vertex_buffer.bind();

    parent_shader->setAttributeBuffer(0,
                                     GL_FLOAT,
                                     offsetof(circle_vertex, position),
                                     2,
                                     sizeof(circle_vertex));
    parent_shader->enableAttributeArray(0);
    parent_shader->setAttributeBuffer(1,
                                     GL_FLOAT,
                                     offsetof(circle_vertex, color),
                                     4,
                                     sizeof(circle_vertex));
    parent_shader->enableAttributeArray(1);
}

graph_window_data::graph_window_data(QOpenGLShaderProgram * parent_shader)
{
    this->parent_shader = parent_shader;

}

graph_window_data::~graph_window_data()
{

}

void graph_window_data::cleanup(){
    circle_VAO.destroy();
}


void graph_window_data::gen_circle_vertices(unsigned int n_vertices){
    for(float p = 0; p < n_vertices; p++){
        float c_part = p/n_vertices*2*M_PI;
        QVector2D pos;
        QVector2D pos_close;
        pos.setX(cosf(c_part));
        pos.setY(sinf(c_part));
        pos_close = pos;
        pos_close.setX(pos_close.x()*0.8);
        pos_close.setY(pos_close.y()*0.8);
        circle_vertex c_v;
        c_v.position = std::move(pos);
        c_v.color = QVector4D();
        circle_vertices.emplace_back(c_v);
        c_v.color = QVector4D();
        c_v.position = std::move(pos_close);
        circle_vertices.emplace_back(c_v);
    }

    circle_vertex_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    circle_vertex_buffer.create();
    circle_vertex_buffer.bind();
    circle_vertex_buffer.allocate(sizeof(circle_vertex)*circle_vertices.size());
    circle_vertex_buffer.write(0,
                               circle_vertices.data(),
                               sizeof(circle_vertex)*circle_vertices.size());
}
