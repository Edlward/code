#include "tinyxml2.h"

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

int main()
{
    XMLDocument xml;
    char *xmlname = "test.xml";

    if(xml.LoadFile("test.xml"))
    {
        XMLDeclaration *decla = xml.NewDeclaration();
        xml.InsertEndChild(decla);
        printf("load fault\n");
    }
    else
    {
        printf("load sucsessful\n");
    }
    

    // 插入节点
    XMLElement *elem1 = xml.NewElement("width");
    elem1->SetName("height");   //相当于更改节点的名字
    elem1->SetText(200);        //给节点添加文本节点
    xml.InsertEndChild(elem1);

    // 搜寻节点
    XMLElement *root = xml.FirstChildElement();
    XMLElement *result = queryElemByName(root, "width");
    if(result != nullptr)
    {
        printf("%s\n", result->Name());
        
        int tmp;
        result->QueryIntText(&tmp);
        printf("%d\n", tmp);
    }
    xml.SaveFile(xmlname);
}

