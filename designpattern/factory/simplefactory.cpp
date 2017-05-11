#include "simplefactory.h"


//主函数测试简单工厂  
int main(int argc, char **argv)  
{  
    Factory* factory = new Factory();  

    factory->CreateSingleCore(COREA)->Show();  
    factory->CreateSingleCore(COREB)->Show();  
}  