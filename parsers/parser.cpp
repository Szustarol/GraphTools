#include "parser.h"

parser::parser()
{

}

QString parser::to_list_rep(graph_lang lang, std::pair<std::map<unsigned, graph_node> *, std::vector<graph_edge> *> data, cpp_nopath cppno)
{
    QString result;
    std::map<unsigned, graph_node> & vertices= *data.first;
    std::vector<graph_edge> & edges = *data.second;
    std::list<std::list<std::pair<QString, QString>>> raw_string_data;
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
        raw_string_data.emplace_back(vertex_data);
    }

    if(lang == PYTHON){
        result = "G = [\n";
    }
    else if (lang == CPP){
        result = "struct graph_data{\n \tunsigned target_node;\n \tfloat weight;\n};\n\n";
        result += "std::vector<std::vector<graph_data>> G = {\n";
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
                result += "(" + pair.first + "," + pair.second + "),";
            }
            else if (lang == CPP){
                result += "{" + pair.first + "," + pair.second + "},";
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
