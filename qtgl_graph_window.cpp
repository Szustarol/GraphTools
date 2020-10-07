#include "qtgl_graph_window.h"
#include <QApplication>
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

    view = QMatrix4x4();
}

void qtgl_graph_window::resizeGL(int w, int h)
{
    screen_w = w;
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    shader_program.setUniformValue("view_matrix", view);
    graph_node::aspect_ratio = aspect;
}

void qtgl_graph_window::mousePressEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton){
        posx_begin = e->x();
        posy_begin = e->y();
        lastx = posx_begin;
        lasty = posy_begin;
    }
}

void qtgl_graph_window::mouseMoveEvent(QMouseEvent * e){
    if(posx_begin != -1 and posy_begin != -1){
        if(!grabbing){
            QVector2D beg(posx_begin, posy_begin);
            QVector2D now(e->x(), e->y());

            QVector2D d = now - beg;
            if(d.length() > 5){
                grabbing = true;
                QApplication::setOverrideCursor(Qt::OpenHandCursor);
            }
        }
        else{
            QVector2D now(e->x(), e->y());
            QVector2D last(lastx, lasty);
            QVector2D d = now - last;
            d /= screen_w/2*all_scale;

            d.setY(d.y()*-1*graph_node::aspect_ratio);
            screen_offset_x += d.x();
            screen_offset_y += d.y();
            graph_node::screen_offset_x = screen_offset_x;
            graph_node::screen_offset_y = screen_offset_y;
            update();
        }
        lastx = e->x();
        lasty = e->y();
    }
}

void qtgl_graph_window::wheelEvent(QWheelEvent *e){
    float delta = e->delta()/1000.0;
    all_scale += delta;
    if(all_scale < 0.2) all_scale = 0.2;
    if(all_scale > 3.0) all_scale = 3.0;
    update();
}

void qtgl_graph_window::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    posx_begin = -1;
    posy_begin = -1;
    grabbing = false;
    QApplication::setOverrideCursor(Qt::ArrowCursor);
    update();
}

void qtgl_graph_window::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    std::cout << all_scale << std::endl;
    shader_program.bind();
    view.setToIdentity();
    view.scale(all_scale);
    shader_program.setUniformValue("view_matrix", view);
    shader_program.release();
    n1.set_position(0.5f, 0.2f);
    n1.set_scale(0.05f);
    n1.draw(shader_program, this, &view);
}
