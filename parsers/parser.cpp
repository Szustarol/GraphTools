#include "parser.h"
#include <algorithm>

parser::parser()
{

}

QString parser::to_list_rep(graph_lang lang, std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> data, bool weighted_graph, cpp_nopath cppno)
{
    Q_UNUSED(cppno);
    QString result;
    if(data.first->size() == 0) return "";

    size_t n_nodes = (--data.first->end())->first+1;
    std::map<unsigned, graph_node> & vertices= *data.first;
    std::vector<graph_edge> & edges = *data.second;
    std::vector<std::list<std::pair<QString, QString>>> raw_string_data;
    raw_string_data.resize(n_nodes);
    for(auto & vertex : vertices){
        std::list<std::pair<QString, QString>> vertex_data;
        for(auto & edge : edges){
            if(edge.directed){
                if(edge.node1_id == vertex.first){
                    vertex_data.push_back(
                        std::make_pair<QString, QString>(
                                    QString::number(edge.node2_id),
                                    QString::number(edge.weight)
                                    )
                     );
                }
            }
            else{
                if(edge.node1_id == vertex.first){
                    vertex_data.push_back(
                        std::make_pair<QString, QString>(
                                    QString::number(edge.node2_id),
                                    QString::number(edge.weight)
                                    )
                    );
                }
                else if(edge.node2_id == vertex.first){
                    vertex_data.push_back(
                        std::make_pair<QString, QString>(
                                    QString::number(edge.node1_id),
                                    QString::number(edge.weight)
                                    )
                    );
                }
            }
        }
        raw_string_data.at(vertex.first) = std::move(vertex_data);
    }

    if(lang == PYTHON){
        result = "G = [\n";
    }
    else if (lang == CPP){
        if(weighted_graph){
            result = "struct graph_data{\n \tunsigned target_node;\n \tfloat weight;\n};\n\n";
            result += "std::vector<std::vector<graph_data>> G = {\n";
        }
        else{
            result =  "std::vector<std::vector<float>> G = {\n";
        }
    }

    for(auto & raw : raw_string_data){
        if(lang == PYTHON){
            result += "\t[";
        }
        else if (lang == CPP){
            result += "\t{";
        }
        for(auto & pair : raw){
            if(lang == PYTHON){
                if(weighted_graph){
                    result += "(" + pair.first + "," + pair.second + "),";
                }
                else{
                    result += pair.first + ",";
                }

            }
            else if (lang == CPP){
                if(weighted_graph){
                    result += "{" + pair.first + "," + pair.second + "},";
                }
                else{
                    result += pair.first + ",";
                }
            }
        }
        if(lang == PYTHON){
            result += "],\n";
        }
        else if (lang == CPP){
            result += "},\n";
        }
    }

    if(lang == PYTHON){
        result += "]";
    }
    else if (lang == CPP){
        result += "};";
    }

    return result;
}

QString parser::to_matrix_rep(graph_lang lang, std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> data, bool weighted_graph, cpp_nopath cppno)
{
    QString result;

    if(data.first->size() == 0) return "";

    auto it = --data.first->end();
    size_t n_nodes = it->first + 1;



    std::vector<std::vector<QString>> m;
    m.resize(n_nodes);

    for(auto & vec : m){
        vec.resize(n_nodes);
        if(lang == CPP){
            switch(cppno){
                case INTMIN:
                    std::fill(vec.begin(), vec.end(), "INT_MIN");
                break;
                case INTMAX:
                    std::fill(vec.begin(), vec.end(), "INT_MAX");
                break;
                case MIN1:
                    std::fill(vec.begin(), vec.end(), "-1");
                break;
                case ZERO:
                    std::fill(vec.begin(), vec.end(), "0");
                break;
            }
        }
        else if(lang == PYTHON){
            std::fill(vec.begin(), vec.end(), "None");
        }
    }

    for(auto & edge : *data.second){
        if(weighted_graph)
            m.at(edge.node1_id).at(edge.node2_id) = QString::number(edge.weight);
        else
            m.at(edge.node1_id).at(edge.node2_id) = QString::number(1);
        if(!edge.directed){
            if(weighted_graph)
                m.at(edge.node2_id).at(edge.node1_id) = QString::number(edge.weight);
            else
                m.at(edge.node2_id).at(edge.node1_id) = QString::number(1);
        }
    }


    if(lang == PYTHON){
        result = "G = [\n";
    }
    else if (lang == CPP){
        result =  "std::vector<std::vector<float>> G = {\n";
    }

    for(auto & raw : m){
        if(lang == PYTHON){
            result += "\t[";
        }
        else if (lang == CPP){
            result += "\t{";
        }
        for(auto & val : raw){
            result += val + ", ";
        }
        if(lang == PYTHON){
            result += "],\n";
        }
        else if (lang == CPP){
            result += "},\n";
        }
    }

    if(lang == PYTHON){
        result += "]";
    }
    else if (lang == CPP){
        result += "};";
    }

    return result;
}
