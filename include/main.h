//
// Created by noah on 11/25/17.
//

#ifndef JETSON_PATHGEN_MAIN_H
#define JETSON_PATHGEN_MAIN_H
extern "C" {
    #include "../pathgen/src/generator.c"
    #include "../pathgen/src/mathutil.c"
    #include "../pathgen/src/fit.c"
    #include "../pathgen/src/modifiers/tank.c"
}
#include <zmq.hpp>
#include <pathRequestFile.pb.h>
#include <pathFile.pb.h>
#include <iostream>
using namespace std;

#endif //JETSON_PATHGEN_MAIN_H
