#ifndef GRAPH_WINDOW_DATA_H
#define GRAPH_WINDOW_DATA_H

#include <QVector2D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

class graph_window_data
{
    struct circle_vertex{
        QVector2D position;
        QVector4D color;
    };

    std::vector<circle_vertex> circle_vertices;

    QOpenGLBuffer circle_element_buffer;
    QOpenGLBuffer circle_vertex_buffer;
    QOpenGLVertexArrayObject circle_VAO;

    QOpenGLShaderProgram * parent_shader;

    std::shared_ptr<QOpenGLShaderProgram> shader_prog;

    void gen_circle_vertices(unsigned n_vertices = 15);


    void gen_circle_indices();


public:

    unsigned n_circle_elements;

    void build();

    void cleanup();

    QOpenGLVertexArrayObject & get_circle_vao();

    graph_window_data(QOpenGLShaderProgram * parent_shader);
    ~graph_window_data();
};

#endif // GRAPH_WINDOW_DATA_H
