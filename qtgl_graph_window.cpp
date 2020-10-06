#include "qtgl_graph_window.h"
#include <iostream>

qtgl_graph_window::qtgl_graph_window(QWidget *parent) : QOpenGLWidget(parent),
    shader_program(),
    graph_data(&shader_program),
    n1(&graph_data)
{

}

qtgl_graph_window::~qtgl_graph_window()
{

    makeCurrent();
    graph_data.cleanup();
    doneCurrent();

}

void qtgl_graph_window::set_shader_paths(const QString &fragment_shader_path, const QString &vertex_shader_path)
{
    this->fragment_shader_path = fragment_shader_path;
    this->vertex_shader_path = vertex_shader_path;
}


void qtgl_graph_window::initializeGL(){

    class shader_error{
        void error_notify(bool result){
            if (result == false){
                std::cerr << "There was an error during shader setup." << std::endl;
            }
        }
        public:
        shader_error(const bool & result){
            error_notify(result);
        };

        shader_error & operator = (bool result){
            error_notify(result);
            return *this;
        };

    };

    shader_error error_notifier = shader_program.create();
    error_notifier = shader_program.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           vertex_shader_path);
    error_notifier = shader_program.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           fragment_shader_path);

    error_notifier = shader_program.link();
    error_notifier = shader_program.bind();

    graph_data.build();

}

void qtgl_graph_window::resizeGL(int w, int h)
{

}

void qtgl_graph_window::mouse_press_event(QMouseEvent *e)
{

}

void qtgl_graph_window::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    n1.set_position(0.2f, 0.3f);
    n1.set_scale(0.2f);
    n1.set_scale(0.2f);
    n1.draw(shader_program);
}
