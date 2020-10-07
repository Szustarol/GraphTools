#include "qtgl_graph_window.h"
#include <QApplication>
#include <iostream>
#include <utility>

qtgl_graph_window::qtgl_graph_window(QWidget *parent) : QOpenGLWidget(parent),
    shader_program(),
    graph_data(&shader_program)
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

int qtgl_graph_window::find_node_at_pos(int x, int y){
    float w = width();
    float h = height();
    w = x/w*2-1;
    h = (h-y)/h*2-1;
    w *= aspect;
    w = w/all_scale-screen_offset_x;
    h = h/all_scale-screen_offset_y;
    QVector2D target_pos = QVector2D(w, h);
    for(auto & n : vertices){
        //check if point is inside node
        auto pos = n.second.get_position();
        pos -= target_pos;
        if (pos.length() < 0.12*all_scale){
            //found vertex
            return n.first;
        }
    }
    return -1;
}

void qtgl_graph_window::resizeGL(int w, int h)
{
    screen_w = w;
    screen_h = h;


    qreal aspect = qreal(w) / qreal(h ? h : 1);

    this->aspect = aspect;

    shader_program.setUniformValue("view_matrix", view);
    graph_node::aspect_ratio = aspect;

    projection.setToIdentity();
    projection.ortho( -aspect, aspect, -1.0f, 1.0f, -10.0f, 10.0f );

    glViewport(0, 0, w, h);
}

void qtgl_graph_window::mousePressEvent(QMouseEvent *e)
{
    switch (click){
        case NONE:
            if(e->buttons() & Qt::LeftButton){
                posx_begin = e->x();
                posy_begin = e->y();
                lastx = posx_begin;
                lasty = posy_begin;
            }
        break;
        case ADD_NODE:{
                click = NONE;
                graph_node n(&graph_data);
                int posx, posy;
                posx = e->x();
                posy = e->y();
                float w = width();
                float h = height();
                w = posx/w*2-1;
                h = (h-posy)/h*2-1;
                w *= aspect;
                n.set_position((w/all_scale-screen_offset_x), (h/all_scale-screen_offset_y));
                n.set_scale(0.05f);
                unsigned i = 0;

                if(vertices.size() > 0 and ((vertices.end()--)->first == (vertices.size() -1))){
                    i = vertices.size();
                }
                else{
                    for(auto it = vertices.begin(); it != vertices.end(); it++){
                        if (it->first != i){
                            break;
                        }
                        i++;
                    }
                }
                n.index = i;
                vertices.emplace(i, n);
                update();
            }
        break;
        case DELETE:{
            click = NONE;
            auto node = find_node_at_pos(e->x(), e->y());
            vertices.erase(node);
            update();
        }
        break;
        case ADD_CONNECTION_START:{
            auto node = find_node_at_pos(e->x(), e->y());
            if(node != -1){
                click = ADD_CONNECTION_END;
                edge_start_pos = vertices.at(node).get_position();
            }
            else{
                click = NONE;
            }
            update();
        }
        break;
        case ADD_CONNECTION_END:{
            auto node = find_node_at_pos(e->x(), e->y());
            if(node != -1){
                edge_end_pos = vertices.at(node).get_position();
                graph_edge ed(&graph_data);
                ed.set_position(edge_start_pos, edge_end_pos);
                edges.emplace_back(ed);

            }
            click = NONE;
            update();
        }
        break;

    }

}

void qtgl_graph_window::mouseMoveEvent(QMouseEvent * e){
    if(posx_begin != -1 and posy_begin != -1 && (click == NONE or click == DRAG)){
        if(click != DRAG){
            QVector2D beg(posx_begin, posy_begin);
            QVector2D now(e->x(), e->y());

            QVector2D d = now - beg;
            if(d.length() > 5){
                click = DRAG;
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
    if(click == DRAG){
        click = NONE;
    }
    if(click == NONE)
        QApplication::setOverrideCursor(Qt::ArrowCursor);
    update();
}

void qtgl_graph_window::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    shader_program.bind();
    view.setToIdentity();
    view.scale(all_scale);
    shader_program.setUniformValue("projection_matrix", projection);
    shader_program.setUniformValue("view_matrix", view);
    shader_program.release();
    for(auto & edge : edges){
        edge.draw(shader_program, this, &view);
    }
    for(auto & vertex : vertices){
        vertex.second.draw(shader_program, this, &view, &projection);
    }


}


void qtgl_graph_window::add_vertex_clicked(){
    if(click != ADD_NODE){
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
        click = ADD_NODE;
    }
    else{
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
}

void qtgl_graph_window::remove_vertex_clicked(){
    if(click != DELETE){
        QApplication::setOverrideCursor(Qt::ForbiddenCursor);
        click = DELETE;
    }
    else{
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
}

void qtgl_graph_window::add_edge_directed_clicked(){
    if(click == ADD_CONNECTION_START or click == ADD_CONNECTION_END){
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
    else{
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        click = ADD_CONNECTION_START;
    }
}
