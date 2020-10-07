#include "graph_window_data.h"
#include <QtMath>
#include <QOpenGLExtraFunctions>
#include <iostream>
#include <QOpenGLFunctions>

tight_vector2d::tight_vector2d(QVector2D & rec){
    dat[0] = rec.x();
    dat[1] = rec.y();
}

tight_vector4d::tight_vector4d(QVector4D & rec){
    dat[0] = rec.x();
    dat[1] = rec.y();
    dat[2] = rec.z();
    dat[3] = rec.w();
}

float & tight_vector2d::x(){
    return dat[0];
}

float & tight_vector2d::y(){
    return dat[1];
}

float & tight_vector4d::x(){
    return dat[0];
}

float & tight_vector4d::y(){
    return dat[1];
}

float & tight_vector4d::z(){
    return dat[2];
}

float & tight_vector4d::w(){
    return dat[3];
}


void graph_window_data::gen_circle_indices()
{

    std::vector<GLuint> circle_indices;


    if(n_all < 2) return;

    circle_element_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    circle_element_buffer.create();

    for(unsigned i = 0; i < n_all-2; i++){
        circle_indices.push_back(i);
        circle_indices.push_back(i+1);
        circle_indices.push_back(i+2);
    }

    circle_indices.push_back(0);
    circle_indices.push_back(n_all-1);
    circle_indices.push_back(n_all-2);

    circle_indices.push_back(0);
    circle_indices.push_back(1);
    circle_indices.push_back(n_all-1);

    circle_element_buffer.bind();
    circle_element_buffer.allocate(circle_indices.data(),
                                   sizeof(GLuint)*circle_indices.size());
    circle_element_buffer.release();
}

void graph_window_data::build()
{
    gen_circle_vertices();
    gen_circle_indices();

    circle_VAO.create();
    circle_VAO.bind();
    circle_vertex_buffer.bind();
    circle_element_buffer.bind();


    parent_shader->setAttributeBuffer(0,
                                     GL_FLOAT,
                                     offsetof(circle_vertex, position) + offsetof(tight_vector2d, dat),
                                     2,
                                     sizeof(circle_vertex));
    parent_shader->enableAttributeArray(0);
    parent_shader->setAttributeBuffer(1,
                                     GL_FLOAT,
                                     offsetof(circle_vertex, color) + offsetof(tight_vector4d, dat),
                                     4,
                                     sizeof(circle_vertex));
    parent_shader->enableAttributeArray(1);
    circle_VAO.release();

    gen_edge_all();

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
    circle_element_buffer.destroy();
    circle_vertex_buffer.destroy();

}

QOpenGLVertexArrayObject &graph_window_data::get_circle_vao()
{
    return circle_VAO;
}

QOpenGLVertexArrayObject &graph_window_data::get_edge_vao(bool directed)
{
    if(directed)
            return directed_edge_VAO;
    return undirected_edge_VAO;
}


void graph_window_data::gen_edge_all(){
    float vertices[] = {
        0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 0.7f,
        -0.2f, -1.0f, 0.0f, 0.0f, 1.0f, 0.7f,
        -0.2f, 0.8f, 0.0f, 0.0f, 1.0f, 0.7f,
        -0.3f, 0.7f, 0.0f, 0.0f, 1.0f, 0.7f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.7f,
        0.3f, 0.7f, 0.0f, 0.0f, 1.0f, 0.7f,
        0.2f, 0.8f, 0.0f, 0.0f, 1.0f, 0.7f,
        0.2f, 1.0f, 0.0f, 0.0f, 1.0f, 0.7f,
        -0.2f, 1.0f, 0.0f, 0.0f, 1.0f, 0.7f
    };

    GLuint indices_directed[] = {
        0, 1, 6,
        1, 2, 6,
        2, 3, 4,
        4, 6, 5,
        2, 6, 4
    };

    GLuint indices_undirected[] = {
        1, 7, 8,
        1, 7, 0
    };

    directed_edge_element_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    undirected_edge_element_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::IndexBuffer);
    edge_vertex_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);

    edge_vertex_buffer.create();
    edge_vertex_buffer.bind();
    edge_vertex_buffer.allocate(vertices, sizeof(vertices));

    directed_edge_element_buffer.create();
    directed_edge_element_buffer.bind();
    directed_edge_element_buffer.allocate(indices_directed, sizeof(indices_directed));

    undirected_edge_element_buffer.create();
    undirected_edge_element_buffer.bind();
    undirected_edge_element_buffer.allocate(indices_undirected, sizeof(indices_undirected));

    directed_edge_VAO.create();
    directed_edge_VAO.bind();
    directed_edge_element_buffer.bind();
    edge_vertex_buffer.bind();

    parent_shader->setAttributeBuffer(0, GL_FLOAT, 0, 2, 6*sizeof(float));
    parent_shader->setAttributeBuffer(1, GL_FLOAT, 2*sizeof(float), 4, 6*sizeof(float));
    parent_shader->enableAttributeArray(0);
    parent_shader->enableAttributeArray(1);
    directed_edge_VAO.release();

    undirected_edge_VAO.create();
    undirected_edge_VAO.bind();
    undirected_edge_element_buffer.bind();
    edge_vertex_buffer.bind();

    parent_shader->setAttributeBuffer(0, GL_FLOAT, 0, 2, 6*sizeof(float));
    parent_shader->setAttributeBuffer(1, GL_FLOAT, 2*sizeof(float), 4, 6*sizeof(float));
    parent_shader->enableAttributeArray(0);
    parent_shader->enableAttributeArray(1);
    undirected_edge_VAO.release();

    n_undirected_elements = sizeof(indices_undirected)/ sizeof(GLuint) / 3;
    n_directed_elements = sizeof(indices_directed) / sizeof(GLuint) / 3;

    directed_edge_element_buffer.release();
    undirected_edge_element_buffer.release();
    edge_vertex_buffer.release();

}

void graph_window_data::gen_circle_vertices(unsigned int n_vertices){
    std::vector<circle_vertex> circle_vertices;

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
        c_v.position = tight_vector2d(pos);
        c_v.color = tight_vector4d();
        c_v.color.x() = 1.0f;
        c_v.color.y() = 0.5f;
        c_v.color.z() = 0.5f;
        c_v.color.w() = 1.0f;
        circle_vertices.push_back(c_v);
        c_v.position = tight_vector2d(pos_close);
        circle_vertices.emplace_back(c_v);
    }

    circle_vertex_buffer = QOpenGLBuffer(QOpenGLBuffer::Type::VertexBuffer);
    circle_vertex_buffer.create();
    circle_vertex_buffer.bind();
    circle_vertex_buffer.allocate(circle_vertices.data(),
                                  sizeof(circle_vertex)*circle_vertices.size());

    n_circle_elements = n_vertices * 6;
    n_all = circle_vertices.size();

}
