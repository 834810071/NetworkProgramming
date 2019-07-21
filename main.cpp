#include <iostream>

using namespace std;

#define ERR_EXIF(m) \
        do \
        { \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while(0);

int main() {
    ERR_EXIF("main");
    return 0;
}