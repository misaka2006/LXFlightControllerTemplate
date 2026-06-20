//
// Created by minec on 2026/6/21.
//

#ifndef ANO_H743_ITA_DRONE_H
#define ANO_H743_ITA_DRONE_H
#include "../../Decive/Inc/dvc_LXFlightController.h"

class Class_Drone {
    public:
    Class_Drone();
    void Init();
    Class_LXFlightController LXFlightController;
};

inline Class_Drone::Class_Drone() {

}

#endif //ANO_H743_ITA_DRONE_H