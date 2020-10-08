#include "graph_edge.h"
#include <QtMath>
#include <iostream>
#include "graph_node.h"

graph_edge::graph_edge(graph_window_data * window_data)
{
    this->window_data = window_data;

    begin_pos = QVector2D(0.0f, 0.0f);
}

void graph_edge::set_position(const QVector2D & begin, const QVector2D & end){
    this->begin_pos = begin;
    this->end_pos = end;
}

void graph_edge::draw(QOpenGLShaderProgram & shader, QOpenGLWidget * parent, QMatrix4x4 * view, QMatrix4x4 * projection){
    shader.bind();
    QVector2D start_point = begin_pos;
    QVector2D diff = end_pos - begin_pos;
    float angle = atan2f(diff.y(), diff.x()) - M_PI/2;
    angle *= -180/M_PI;
    float len = diff.length();

    auto & vao = window_data->get_edge_vao(directed);
    vao.bind();

    start_point.setX(start_point.x() + graph_node::screen_offset_x);
    start_point.setY(start_point.y() + graph_node::screen_offset_y);

    model.setToIdentity();
    model.translate(start_point);
    model.translate(diff*0.5);
    model.scale(len/2);
    model.rotate(-angle, QVector3D(0, 0, 1.0f));
    model.scale(0.1/diff.length(), 1);

    shader.setUniformValue("model_matrix", model);
    if(directed)
        glDrawElements(GL_TRIANGLES, window_data->n_directed_elements*3, GL_UNSIGNED_INT, 0);
    else
        glDrawElements(GL_TRIANGLES, window_data->n_undirected_elements*3, GL_UNSIGNED_INT, 0);
    vao.release();
    shader.release();

    if(weight != 0){
        auto sig = [](float number){
            if(number >= 0) return 1;
            return -1;
        };


        QVector4D tr = QVector4D((5*begin_pos+end_pos)/6 + QVector2D(graph_node::screen_offset_x, graph_node::screen_offset_y), 1.0f, 1.0f);
        tr = *projection * *view  * tr;


        if(abs(tr.x()) < 1 and abs(tr.y()) < 1){
            float w = parent->width();
            float h = parent->height();
            w = (tr.x()+1)*w/2;
            h = (1-tr.y())*h/2;
            QPainter painter(parent);
            painter.setPen(Qt::white);
            QTransform t;
            QVector2D perp;
            perp.setX(-diff.y());
            perp.setY(diff.x());
            float rad = qDegreesToRadians(2*(angle-90)-90);
            auto d= perp.normalized()*20.f/sig(qSin(rad));
            std::cout << "PERP " <<  d.x() << "///" << d.y() << "ANGLE" << angle - 90 << std::endl;
            t.translate(w, h);
            t.translate(d.x(), d.y());
            t.rotate((angle-90));
            painter.setTransform(t);
            painter.drawText(QRect(-(INT_MAX / 2), -(INT_MAX / 2), INT_MAX, INT_MAX), Qt::AlignCenter | Qt::AlignHCenter, QString::number(weight));
            painter.end();
        }
    }

}
