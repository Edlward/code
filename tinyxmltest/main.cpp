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
    XMLElement *elem = xml->FirstChildElement(name.c_str());
    
    if(elem != nullptr) // xml内已有这个节点，只需修改值
    {
        elem->SetText(value);
    }
    else // 插入节点
    {
        XMLElement *elem1 = xml->NewElement(name.c_str());
        elem1->SetName(name.c_str());   //相当于更改节点的名字
        elem1->SetText(value);        //给节点添加文本节点
        xml->InsertEndChild(elem1);
    }
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

void write(XMLDocument *xml, std::string name, double value)
{
    XMLElement *elem = xml->FirstChildElement(name.c_str());
    
    if(elem != nullptr) // xml内已有这个节点，只需修改值
    {
        elem->SetText(value);
    }
    else // 插入节点
    {
        XMLElement *elem1 = xml->NewElement(name.c_str());
        elem1->SetName(name.c_str());   //相当于更改节点的名字
        elem1->SetText(value);        //给节点添加文本节点
        xml->InsertEndChild(elem1);
    }
}

void read(XMLDocument *xml, std::string name, double &value)
{
    XMLElement *elem = xml->FirstChildElement(name.c_str());
    
    if(elem != nullptr)
    {
        elem->QueryDoubleText(&value);
    }
    else
    {
        printf("can not find %s\n", name.c_str());
    }
}

bool loadFile(XMLDocument **rxml, std::string xmlname)
{
    XMLDocument *xml = new XMLDocument();

    if(xml->LoadFile(xmlname.c_str()))
    {
        XMLDeclaration *decla = xml->NewDeclaration();
        xml->InsertEndChild(decla);
        printf("load fault\n");
        *rxml = xml;
        return false;
    }
    else
    {
        *rxml = xml;
        printf("load sucsessful\n");
        return true;
    }
}


int main()
{
    XMLDocument *xml;
    std::string xmlname = "test.xml";

    loadFile(&xml, xmlname);

    // write(xml, "fps", 30);
    // write(xml, "beta1", 0.2);

    // 搜寻节点
    // int value;
    // std::string name = "fps";
    // read(xml, name, value);
    // printf("%s: %d\n", name.c_str(), value);

    // double dvalue;
    // std::string dname = "beta1";
    // read(xml, dname, dvalue);
    // printf("%s: %f\n", dname.c_str(), dvalue);

    xml->SaveFile(xmlname.c_str());
}

