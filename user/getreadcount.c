#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void){
    printf(1, "Se ha llamada read %d veces\n", getreadcount());
    exit();
}