#include "tinyxml2.h"
#include <string>

//DOM，搞清楚类的关系

using namespace tinyxml2;


XMLElement *queryElemByName(XMLElement *elem, const char *name)
{
    while(elem != nullptr)
    {
        if(strcmp(elem->Name(), name) == 0)
            break;
        elem = elem->NextSiblingElement();
        printf("while %s\n", elem->Name());
    }
    return elem;
}

void write(XMLDocument *xml, std::string name, int value)
{
    // 插入节点
    XMLElement *elem1 = xml->NewElement(name.c_str());
    elem1->SetName(name.c_str());   //相当于更改节点的名字
    elem1->SetText(value);        //给节点添加文本节点
    xml->InsertEndChild(elem1);
}

void read(XMLDocument *xml, std::string name, int &value)
{
    XMLElement *elem = xml->FirstChildElement(name.c_str());
    
    if(elem != nullptr)
    {
        elem->QueryIntText(&value);
    }
    else
    {
        printf("can not find %s\n", name.c_str());
    }
}

int main()
{
    XMLDocument *xml = new XMLDocument();
    std::string xmlname = "test.xml";

    if(xml->LoadFile(xmlname.c_str()))
    {
        XMLDeclaration *decla = xml->NewDeclaration();
        xml->InsertEndChild(decla);
        printf("load fault\n");
    }
    else
    {
        printf("load sucsessful\n");
    }
    
    // write(xml, "fps", 30);


    // 搜寻节点
    int value;
    std::string name = "fps";
    read(xml, name, value);
    printf("%s: %d\n", name.c_str(), value);

    xml->SaveFile(xmlname.c_str());
}

