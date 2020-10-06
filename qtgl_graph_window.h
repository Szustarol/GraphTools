#ifndef QTGL_GRAPH_WINDOW_H
#define QTGL_GRAPH_WINDOW_H


#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>
#include "graph_gl_scene/graph_window_data.h"

class qtgl_graph_window : public QOpenGLWidget
{
    Q_OBJECT


    void init_shaders();

    QString fragment_shader_path, vertex_shader_path;
    QOpenGLShaderProgram shader_program;

    graph_window_data graph_data;


public:
    explicit qtgl_graph_window(QWidget *parent = nullptr);
    ~qtgl_graph_window();

    void set_shader_paths(const QString & fragment_shader_path,
                          const QString & vertex_shader_path);

    void paintGL() override;
    void initializeGL() override;
    void resizeGL(int w, int h) override;

    void mouse_press_event(QMouseEvent * e);


signals:

};

#endif // QTGL_GRAPH_WINDOW_H
