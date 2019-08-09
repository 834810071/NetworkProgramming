#include <iostream>
#include <vector>
#include <algorithm>
#include <string.h>

using namespace std;

// 该文件为原始文件 具体实现在各个文件中

#define ERR_EXIF(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0);

char* trim(char* cmdline)
{
    char* s = cmdline;
    while (*s == ' ')
    {
        s++;
    }

    char* e = s;
    int len = 0;
    while ((strcmp(e, "") != 0) && *e != ' ')
    {
        len++;
        e++;
    }
    char* res = new char[len];
    memcpy(res, s, len);
    return res;
}

vector<char*> getcmd(char* cmdline) {
    vector<char*> res;
    char *s = trim(cmdline);
    if (strcmp(s, "") == 0) {
        return res;
    }
    int len = 0;
    char *temp;
    char *tmp ;
    int count = 3;
    while (count--) {
        s = trim(cmdline);
        if (strcmp(s, "") == 0) {
            return res;
        }
        len = 0;
        temp = s;
        tmp = new char[100];
        while ((strcmp(temp, "") != 0) && *temp != ' ') {
            len++;
            temp++;
        }
        memcpy(tmp, s, len);
        res.push_back(tmp);
        cmdline = cmdline + len;
        while ((strcmp(cmdline, "") != 0) && *cmdline == ' ')
        {
            cmdline++;
        }
    }
    return res;
}

int main() {
    char* p;
    char* cmdline = new char[100];
    while (cin.getline(cmdline, 100))
    {
        vector<char*> res = getcmd(cmdline);
        if (res.size() == 0 || res.size() == 2)
        {
            cout << "err" << endl;
        }
        else if (strcmp(res[0], "exit") == 0)
        {
            cout << "exit" << endl;
        }
        else if (strcmp(res[0], "send") == 0 && res.size() == 3)
        {
            cout << "send" << endl;
            cout << "username:\t" << res[1] << endl;
            cout << "msg:\t" << res[2] << endl;
        }
        else
        {
            cout << "err" << endl;
        }
    }

    return 0;
}