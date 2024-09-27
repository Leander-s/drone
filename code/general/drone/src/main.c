#include <drone.h>

int main(){
    gpio_put(LED_PIN, 1);
    Drone *drone = drone_start();
    gpio_put(LED_PIN, 0);
    while(1){
        drone_update(drone);
    }
    drone_end(drone);
    return 0;
}
