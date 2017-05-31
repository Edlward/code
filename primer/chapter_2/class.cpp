#include <stdio.h>

// 声明
class Foo;  // forward declaration of a class
 
//  定义
class Bar { // definition of a class
  public:
    Bar(int i) : m_i(i) 
    {
        printf("bar:%d\n", i);
    }
  private:
    int m_i;
};
 
template <class T> // template argument
void qux() {
    T t;
}
 
int main()
{
    Bar Bar(1);
    class Bar Bar2(2); // elaborated type(详细类型)
}