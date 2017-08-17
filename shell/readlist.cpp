#include <string>
#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>

using namespace std;

int main(int argc, char **argv)
{

    string path = "/home/lxg/codedata/agegender/data/";

    ofstream out_female;
    out_female.open((path + "female.txt").c_str(), ios_base::ate);
    ofstream out_male;
    out_male.open((path + "male.txt" ).c_str(), ios_base::ate);
    
    string files[5] = {"fold_frontal_0_data.txt", "fold_frontal_1_data.txt", "fold_frontal_2_data.txt",
                        "fold_frontal_3_data.txt", "fold_frontal_4_data.txt"};
    
    for(int i = 0; i < 5; ++i)
    {
        string file_name = files[i];

        cout << file_name << endl;
        
        ifstream in((path + file_name).c_str());

        if(!in.is_open())
        {
            cout << "can not open fiel " << argv[1] << endl;
        }

        string s1, s2, s3;
        char str[1024];
        // 首行无数据
        in.getline(str, 500);

        while(in.getline(str, 500))
        {
            cout << str << endl;

            istringstream s(str);

            s >> s1 >> s2 >> s3;

            cout << s1 << endl;
            cout << s2 << endl;
            cout << s3 << endl;

            if(string(str).find("f\t") != string::npos)
            {
                cout << "female" << endl;
                out_female << path + "aligned/" + s1 + "/landmark_aligned_face." + s3 + "." + s2 << endl;
            }
            else if(string(str).find("m\t") != string::npos)
            {
                cout << "male" << endl;
                out_male << path + "aligned/" + s1 + "/landmark_aligned_face." + s3 + "." + s2 << endl;
            }
            else
            {
                cout << "not found" << endl;
            }
        }
    }
}