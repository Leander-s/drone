#include <drone.h>

int main(){
    Drone *drone = drone_start();
    while(1){
        drone_update(drone);
    }
    drone_end(drone);
    return 0;
}
