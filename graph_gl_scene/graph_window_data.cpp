#include "graph_window_data.h"
#include <QtMath>
#include <QOpenGLExtraFunctions>
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
    unsigned n_all = circle_vertices.size();

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
}
