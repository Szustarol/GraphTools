#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QPair>
#include "../graph_gl_scene/graph_edge.h"
#include "../graph_gl_scene/graph_node.h"

enum graph_lang{
    PYTHON,
    CPP,
};

enum cpp_nopath{
    ZERO,
    INTMAX,
    INTMIN,
    MIN1,
};

class parser
{
public:
    parser();

    static QString to_list_rep(graph_lang lang, std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> data, bool weighted_graph = false, cpp_nopath cppno = ZERO);
    static QString to_matrix_rep(graph_lang lang, std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> data, bool weighted_graph = false, cpp_nopath cppno = ZERO);
};

#endif // PARSER_H
