#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(void){
    printf(1, "Se ha llamado read %d veces\n", getreadcount());
    exit();
}