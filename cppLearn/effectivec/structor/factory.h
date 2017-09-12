#ifndef _FACTORY_H_
#define _FACTORY_H_

// 抽象基类，定义所有派生类共有行为的抽象单元
// 抽象基类的具体实现类，public继承抽象基类
// 抽象工厂基类，创建具体实现类对象的接口类
// 抽象工厂基类的具体实现类，创建具体实现类的抽象工厂基类的实现类


// 抽象基类，定义所有派生类共有行为的抽象单元
class Human
{
public:
    IHuman(void){}
    ~IHuman(){}
    // 纯虚函数，派生类中必须重新声明，定义自己的版本
    virtual void Laugh()=0;
    virtual void Cry()=0;
    virtual void Talk()=0; 
};

// 抽象基类的具体实现类，public继承抽象基类
class WhiteHuman:public IHuman
{
public:
    WhiteHuman(void){}
    ~WhiteHuman(void){}
    void Laugh()
    {
        std::cout << "whiteHuman laugh\n";
    }
    void Cry()
    {
        std::cout << "whiteHuman cry\n";
    }
}
class YellowHuman:public IHuman
{
public:
    YellowHuman(void){}
    ~YellowHuman(void){}
    void Laugh()
    {
        std::cout << "yellowHuman laugh\n";
    }
    void Cry()
    {
        std::cout << "yellowHuman cry\n";
    }
};

// 抽象工厂基类，创建具体实现类对象的接口类
class IHumanFactory
{
public:
    IHumanFactory(){}
    ~IHumanFactory(){}
    // 抽象基类的创建函数，内部一般含有new创建的对象
    virtual IHumanFactory(){}
};

// 抽象工厂基类的具体实现类，创建具体实现类的抽象工厂基类的实现类
class WhiteHumanFactory:public IHumanFactory
{
public:
    WhiteHumanFactory(void){}
    ~WhiteHumanFactory(){}
    // 具体实现类的穿件函数，用于创建具体的对象
    IHuman *CreateHuman()
    {
        return new WhiteHuman;
    }
};

class YellowHumanFactory:public IHumanFactory
{
public:
    YellowHumanFactory(void){}
    ~YellowHumanFactory(){}
    // 具体实现类的穿件函数，用于创建具体的对象
    IHuman *CreateHuman()
    {
        return new YellowHuman;
    }
};
#endif