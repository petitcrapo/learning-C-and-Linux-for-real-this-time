#include <math.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct object {
    double mass; // Kg
    double pos[2]; //pos[0] = x ,pos[1] = y // m
    double vel[2]; //vel[0] = x ,vel[1] = y // m.s-1
} object_s;

double* accel_calc(object_s* object, object_s* object_interacted, double* accel) {
    double o1o2[2] = {object_interacted->pos[0] - object->pos[0], object_interacted->pos[1] - object->pos[1]};
    double distance = sqrt(pow(o1o2[0], 2.0) + pow(o1o2[1], 2.0));
    double Force = (0.0000000000667 * (object->mass * object_interacted->mass) / pow(distance, 2.0));
    accel[0] += (o1o2[0]/distance) * (Force/object->mass);
    accel[1] += (o1o2[1]/distance) * (Force/object->mass);
    return accel;
}

void next_step(int delta_utime, object_s* object, double* accel) {
    object->pos[0] = object->pos[0] + (object->vel[0] * delta_utime/1000000);
    object->pos[1] = object->pos[1] + (object->vel[1] * delta_utime/1000000);
    object->vel[0] = object->vel[0] + (accel[0] * delta_utime/1000000);
    object->vel[1] = object->vel[1] + (accel[1] * delta_utime/1000000);
}

int main(void) {
    int dezoom = 1000000;
    const int object1col = 0x00ff0000;
    const int object2col = 0x0000ff00;
    const int object3col = 0x000000ff;
    object_s object1;
    object1.mass = 1.988e30; 
    object1.pos[0] = 960.0;
    object1.pos[1] = 540.0;
    object1.vel[0] = 0.0;
    object1.vel[1] = 0.0;
    double accel1[2] = {0, 0};
    object_s object2;
    object2.mass = 5.9722e24;
    object2.pos[0] = 960.0;
    object2.pos[1] = 1.5e14;
    object2.vel[0] = 3e4;
    object2.vel[1] = 0.0;
    double accel2[2] = {0, 0};
    object_s object3;
    object3.mass = 7.35e22;
    object3.pos[0] = 960.0;
    object3.pos[1] = 1.5e14 + 3.56410e8;
    object3.vel[0] = 4e4;
    object3.vel[1] = 0.0;
    double accel3[2] = {0, 0};
    int tick = 5000; // 5 millisecond
    int fbfd = open("/dev/fb0", O_WRONLY);
    while (1) {
        // printf("object1 position: x=%f, y=%f \t with a velocity of: vx=%f, vy=%f \n", object1.pos[0], object1.pos[1], object1.vel[0], object1.vel[1]);
        // printf("object2 position: x=%f, y=%f \t with a velocity of: vx=%f, vy=%f \n", object2.pos[0], object2.pos[1], object2.vel[0], object2.vel[1]);
        // printf("---------------------\n");
        
        accel_calc(&object1, &object2, accel1);
        accel_calc(&object1, &object3, accel1);
        next_step(tick, &object1, accel1);
        accel1[0] = 0;
        accel1[1] = 0;
        accel_calc(&object2, &object1, accel2);
        accel_calc(&object2, &object3, accel2);
        next_step(tick, &object2, accel2);
        accel2[0] = 0;
        accel2[1] = 0;
        accel_calc(&object3, &object1, accel3);
        accel_calc(&object3, &object2, accel3);
        next_step(tick, &object3, accel3);
        accel3[0] = 0;
        accel3[1] = 0;
        pwrite(fbfd, &object1col, sizeof(int), sizeof(int) * ((((int)(object1.pos[1])%1080) * 1920 + ((int)object1.pos[0])%1920)));
        pwrite(fbfd, &object2col, sizeof(int), sizeof(int) * ((((int)(object2.pos[1])%1080) * 1920 + ((int)object2.pos[0])%1920)));
        pwrite(fbfd, &object3col, sizeof(int), sizeof(int) * ((((int)(object3.pos[1])%1080) * 1920 + ((int)object3.pos[0])%1920)));
    }
    return 0;
}