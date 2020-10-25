#include "qtgl_graph_window.h"
#include <QApplication>
#include <iostream>
#include <utility>

qtgl_graph_window::qtgl_graph_window(QWidget *parent) : QOpenGLWidget(parent),
    shader_program(),
    graph_data(&shader_program)
{
    click = NONE;
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

std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> qtgl_graph_window::get_data_pair()
{
    return std::make_pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> (&vertices, &edges);
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
        if (pos.length() < 0.05){
            //found vertex
            return n.first;
        }
    }
    return -1;
}

bool qtgl_graph_window::isWeighted()
{
    return weighted;
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
                QApplication::setOverrideCursor(Qt::ArrowCursor);
            }
        break;
        case DELETE:{
            click = NONE;
            auto node = find_node_at_pos(e->x(), e->y());
            if(node != -1){
                vertices.erase(node);

                for(int i = edges.size() -1; i >= 0; i--){
                    if((int)edges.at(i).node1_id == node or (int)edges.at(i).node2_id == node){
                        edges.erase(edges.begin() + i);
                    }
                }


                update();
                return;
            }
            for(unsigned i = 0; i < edges.size(); i++){
                if(edges.at(i).is_clicked(QPointF(e->x(), e->y()))){
                    edges.erase(edges.begin()+i);
                    update();
                    return;
                }
            }
            QApplication::setOverrideCursor(Qt::ArrowCursor);

        }
        break;
        case EDIT:{
            click = NONE;
            QApplication::setOverrideCursor(Qt::ArrowCursor);
            for(unsigned i = 0; i < edges.size(); i++){
                if(edges.at(i).is_clicked(QPointF(e->x(), e->y()))){
                    edges.at(i).edit();
                    update();
                    return;
                }
            }

        }
        break;
        case ADD_CONNECTION_START:{
            auto node = find_node_at_pos(e->x(), e->y());
            if(node != -1){
                click = ADD_CONNECTION_END;
                edge_start_pos = vertices.at(node).get_position();
                start_node_id = node;
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
                ed.directed = adding_directed;
                ed.set_position(edge_start_pos, edge_end_pos);
                ed.weight = new_edge_weight;
                ed.node1_id = start_node_id;
                ed.node2_id = node;
                edges.emplace_back(ed);

            }
            click = NONE;
            QApplication::setOverrideCursor(Qt::ArrowCursor);

            update();
        }
        break;
        case DRAG://shouldn't happen
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
            graph_edge::screen_offset_x = screen_offset_x;
            graph_edge::screen_offset_y = screen_offset_y;
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
    graph_edge::all_scale = all_scale;
    update();
}

void qtgl_graph_window::set_new_edge_weight(float weight)
{
    std::cout << weight << std::endl;
    new_edge_weight = weight;
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
        edge.draw(shader_program, this, &view, &projection, weighted);
    }
    for(auto & vertex : vertices){
        vertex.second.draw(shader_program, this, &view, &projection);
    }
    if(weighted){
        for(auto & edge : edges){
            edge.drawText(this);
        }
    }
    if(click != NONE and click != DRAG){
        QPainter comm_painter(this);
        comm_painter.setPen(Qt::green);
        QString t;
        switch(click){
        case EDIT:
            t = tr("graph_edit_edge");
            break;
        case DELETE:
            t = tr("graph_delete");
            break;
        case ADD_CONNECTION_START:
            t = tr("graph_add_start");
            break;
        case ADD_CONNECTION_END:
            t = tr("graph_add_end");
            break;
        case ADD_NODE:
            t = tr("graph_add_node");
            break;
        default://shouldn't happen
            break;
        }

        comm_painter.drawText(QRect(0, screen_h-40, screen_w, 40), Qt::AlignHCenter, t);
    }

}

const std::map<unsigned, graph_node> * qtgl_graph_window::get_vertices() const{
    return &vertices;
};

const std::vector<graph_edge> * qtgl_graph_window::get_edges() const{
    return &edges;
};


void qtgl_graph_window::add_vertex_clicked(){
    if(click != ADD_NODE){
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
        click = ADD_NODE;
    }
    else{
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
    update();
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
    update();
}

void qtgl_graph_window::add_edge_directed_clicked(){
    if(click == ADD_CONNECTION_START or click == ADD_CONNECTION_END){
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
    else{
        adding_directed = true;
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        click = ADD_CONNECTION_START;
    }
    update();
}


void qtgl_graph_window::add_edge_undirected_clicked(){
    if(click == ADD_CONNECTION_START or click == ADD_CONNECTION_END){
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
    else{
        adding_directed = false;
        QApplication::setOverrideCursor(Qt::ClosedHandCursor);
        click = ADD_CONNECTION_START;
    }
    update();
}

void qtgl_graph_window::edit_clicked()
{
    if(click != EDIT){
        QApplication::setOverrideCursor(Qt::PointingHandCursor);
        click = EDIT;
    }
    else{
        QApplication::setOverrideCursor(Qt::ArrowCursor);
        click = NONE;
    }
    update();
}

void qtgl_graph_window::set_graph_weighted(int state)
{
    switch (state){
    case Qt::Unchecked:
        weighted = false;
        break;
    case Qt::PartiallyChecked:
    case Qt::Checked:
        weighted = true;
        break;
    }
    update();
}
