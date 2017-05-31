#include <string>
#include <vector>
#include <iostream>
#include <memory>

using namespace std;

// shared_ptr的使用
// class declaration
class StrBlob;

// class definition
class StrBlob
{
public:    
    // Typedef names are only in effect in the scope where they are visible: 
    typedef vector<string>::size_type size_type;
    StrBlob():data(make_shared<vector<string> >()){};
    StrBlob(initializer_list<string> il):data(make_shared<vector<string> >(il)){};

    size_type size() const { return data->size();}
    bool empty() const { return data->empty(); }
    void push_back(const string &str) { data->push_back(str);}
    void pop_back();
    string& front() const;
    string& back();
    int get_count()
    {
        return data.use_count();
    }
private:
    shared_ptr<vector<string> > data;
    void check(size_type i, const string &msg) const;
};



string& StrBlob::front() const
{
    check(0, "front on empty vector\n");
    return data->front();
}
string& StrBlob::back()
{
    check(0, "back on empty vector\n");
    return data->back();
}
// 这里的size_type应该也是在StrBlob作用域内，因为StrBlob::
void StrBlob::check(size_type i, const string &msg) const
{
    if(i >= data->size())
        throw out_of_range(msg);
}

void StrBlob::pop_back()
{
    check(0, "pop_back on empty vector\n");
}


int main(int argc, char **argv)
{
    StrBlob strblob2;
    cout << "shared number: " << strblob2.get_count() << endl;
    
    {
        StrBlob strblob1({"123","abc"});
        strblob2 = strblob1;
        cout << strblob1.front() << endl;
        cout << strblob1.back() << endl;
        cout << "shared number: " << strblob1.get_count() << endl;
    }
    cout << "shared number: " << strblob2.get_count() << endl;
    
    cout << strblob2.front() << endl;

}
