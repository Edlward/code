#include "factorymethod.h"

//主函数测试工厂方法  
int main(int argc, char **argv)
{
    // 可以只指定抽象工厂基类，
    Factory* factorya = new FactoryA;
    factorya->CreateSingleCore()->Show();
        
    FactoryB* factoryb = new FactoryB;
    factoryb->CreateSingleCore()->Show();
}