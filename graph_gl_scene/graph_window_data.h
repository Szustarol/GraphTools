#ifndef GRAPH_WINDOW_DATA_H
#define GRAPH_WINDOW_DATA_H

#include <QVector2D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

struct tight_vector2d{
    float dat[2];
    tight_vector2d() = default;
    tight_vector2d(QVector2D & vec);
    float & x();
    float & y();
};

struct tight_vector4d{
    float dat[4];
    tight_vector4d() = default;
    tight_vector4d(QVector4D & vec);
    float & x();
    float & y();
    float & z();
    float & w();
};

class graph_window_data
{
    struct circle_vertex{
        tight_vector2d position;
        tight_vector4d color;
    };


    QOpenGLBuffer circle_element_buffer;
    QOpenGLBuffer circle_vertex_buffer;
    QOpenGLVertexArrayObject circle_VAO;

    QOpenGLBuffer directed_edge_element_buffer;
    QOpenGLBuffer undirected_edge_element_buffer;
    QOpenGLBuffer edge_vertex_buffer;
    QOpenGLVertexArrayObject directed_edge_VAO;
    QOpenGLVertexArrayObject undirected_edge_VAO;


    QOpenGLShaderProgram * parent_shader;

    std::shared_ptr<QOpenGLShaderProgram> shader_prog;

    void gen_circle_vertices(unsigned n_vertices = 15);
    void gen_circle_indices();

    void gen_edge_all();
    unsigned n_all;


public:

    unsigned n_circle_elements;

    unsigned n_directed_elements;
    unsigned n_undirected_elements;

    void build();

    void cleanup();

    QOpenGLVertexArrayObject & get_circle_vao();
    QOpenGLVertexArrayObject & get_edge_vao(bool directed);


    graph_window_data(QOpenGLShaderProgram * parent_shader);
    ~graph_window_data();
};

#endif // GRAPH_WINDOW_DATA_H
