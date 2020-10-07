#ifndef QTGL_GRAPH_WINDOW_H
#define QTGL_GRAPH_WINDOW_H


#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include "graph_gl_scene/graph_window_data.h"
#include "graph_gl_scene/graph_node.h"
#include <QPainter>

class qtgl_graph_window : public QOpenGLWidget
{
    Q_OBJECT


    void init_shaders();

    QString fragment_shader_path, vertex_shader_path;
    QOpenGLShaderProgram shader_program;

    graph_window_data graph_data;

    graph_node n1;

    QMatrix4x4 view;

    float posx_begin = -1, posy_begin = -1;
    float lastx, lasty;
    float grabbing = false;

    float screen_offset_x = 0, screen_offset_y = 0;

    float all_scale = 1.0f;

    int screen_w;


public:
    explicit qtgl_graph_window(QWidget *parent = nullptr);
    ~qtgl_graph_window();

    void set_shader_paths(const QString & fragment_shader_path,
                          const QString & vertex_shader_path);

    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;

    void mousePressEvent(QMouseEvent * e) override;
    void mouseReleaseEvent(QMouseEvent * e) override;
    void mouseMoveEvent(QMouseEvent * e) override;
    void wheelEvent(QWheelEvent * e) override;


signals:

};

#endif // QTGL_GRAPH_WINDOW_H
