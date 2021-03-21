#include "json/json.h"
#include <unistd.h>



int main(){
    char buf[255];
    getcwd(buf, sizeof(buf));
    slrTable();
    return 0;
}