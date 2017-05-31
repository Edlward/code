#include "abstractfactory.h"


//主函数测试抽象工厂  
int main(void)  
{  
    FactoryA* factorya = new FactoryA();  
    factorya->CreateSingleCore()->Show();  
    factorya->CreateMultiCore()->Show();  
    
    FactoryB* factoryb = new FactoryB();  
    factoryb->CreateSingleCore()->Show();  
    factoryb->CreateMultiCore()->Show();  
  
    return 0;  
}  