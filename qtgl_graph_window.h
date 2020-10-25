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
#include "graph_gl_scene/graph_edge.h"
#include <QPainter>
#include <map>

enum click_type{
    NONE,
    DRAG,
    ADD_NODE,
    DELETE,
    ADD_CONNECTION_START,
    ADD_CONNECTION_END,
    EDIT
};


class qtgl_graph_window : public QOpenGLWidget
{
    Q_OBJECT


    void init_shaders();

    QString fragment_shader_path, vertex_shader_path;
    QOpenGLShaderProgram shader_program;

    graph_window_data graph_data;

    QMatrix4x4 view;

    float posx_begin = -1, posy_begin = -1;
    float lastx, lasty;

    float screen_offset_x = 0, screen_offset_y = 0;

    float all_scale = 1.0f;

    float aspect;

    bool adding_directed = false;

    int screen_w, screen_h;

    std::map<unsigned, graph_node> vertices;
    std::vector<graph_edge> edges;

    QVector2D edge_start_pos;
    QVector2D edge_end_pos;
    QMatrix4x4 projection;

    bool weighted = false;

    int start_node_id;

    float new_edge_weight = 1;

public:

    std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> get_data_pair();

    click_type click;

    int find_node_at_pos(int x, int y);

    bool isWeighted();

    const std::map<unsigned, graph_node> * get_vertices() const;
    const std::vector<graph_edge> * get_edges() const;

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

    void set_new_edge_weight(float weight);

public slots:
    void add_vertex_clicked();
    void remove_vertex_clicked();
    void add_edge_directed_clicked();
    void add_edge_undirected_clicked();
    void edit_clicked();
    void set_graph_weighted(int state);


signals:

};

#endif // QTGL_GRAPH_WINDOW_H
