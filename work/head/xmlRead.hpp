#if !defined(XMLREAD_H)
#define XMLREAD_H
#include <fstream>
#include <iostream>
#include <map>
#include <rapidxml/rapidxml.hpp>
#include <string>
// 确保xml的第一个值为int型作为ID
// 访问方法：mapData[idInt]["key"]或objName[idInt]["key"]
class xmlRead {
  public:
    std::map<int, std::map<std::string, std::string>> mapData;

  public:
    xmlRead(const char *path) { loadXmlToMap(path); };
    ~xmlRead() {};
    void loadXmlToMap(const char *path) {
        ;
        using namespace std;
        using namespace rapidxml;
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Error:" + std::string(__FILE__) +
                                     " at line " + std::to_string(__LINE__) +
                                     " in function " + std::string(__func__) +
                                     ": " + path+" open failed\n");
        }
        // try {

        // } catch (const std::exception &e) {
        //     std::cerr << "Error in " << __FILE__ << " at line " << __LINE__
        //               << " in function " << __func__ << ": " << e.what()
        //               << std::endl;
        // }
        string xmlString;
        string line;
        while (getline(file, line)) {
            xmlString += line;
        }
        file.close();

        rapidxml::xml_document<> doc;
        doc.parse<0>(&xmlString[0]);
        rapidxml::xml_node<> *rootNode = doc.first_node();
        for (rapidxml::xml_node<> *node = rootNode->first_node(); node;
             node = node->next_sibling()) {
            for (rapidxml::xml_node<> *childNode = node->first_node();
                 childNode; childNode = childNode->next_sibling()) {
                mapData[stoi(node->first_node()->value())][childNode->name()] =
                    childNode->first_node()?childNode->first_node()->value():"nofile";
            }
        }
        doc.remove_all_nodes();
        doc.clear();
    }

    std::map<std::string, std::string> &operator[](int id) {
        return mapData[id];
    }
};

#endif // XMLREAD_H
