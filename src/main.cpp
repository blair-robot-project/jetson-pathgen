//
// Created by noah on 11/24/17.
//
#include "../include/main.h"

double deltaTime = 0.05, maxVel = 5, maxAccel = 4.5, maxJerk = 9, wheelbaseWidth = 26.6536/12.;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    Waypoint points[2];
    points[0] = {0,0,0};
    string req_str;
    zmq::message_t request;
    string rep_str;
    zmq::message_t response;
    proto::PathRequest pathRequest = proto::PathRequest();
    proto::Path path;
    path.set_deltatime(deltaTime);
    TrajectoryCandidate candidate;
    Segment *trajectory;
    int length;

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);

    cout << "Connecting to server" << endl;
    socket.connect ("tcp://10.4.49.2:5556");

    while (true){
        socket.recv(&request);
        req_str = string(static_cast<char*>(request.data()), request.size());
        pathRequest.ParseFromString(req_str);
        points[1] = {pathRequest.x(), pathRequest.y(), pathRequest.theta()};
        pathfinder_prepare(points, 2, FIT_HERMITE_CUBIC, PATHFINDER_SAMPLES_LOW, deltaTime, maxVel, maxAccel, maxJerk, &candidate);
        length = candidate.length;
        Segment leftTrajectory[length], rightTrajectory[length];
        trajectory = static_cast<Segment *>(malloc(length * sizeof(Segment)));
        pathfinder_generate(&candidate, trajectory);
        pathfinder_modify_tank(trajectory, length, leftTrajectory, rightTrajectory, wheelbaseWidth);
        for (int c = 0; c < length; c++){
            path.set_accelleft(c, leftTrajectory[c].acceleration);
            path.set_velleft(c, leftTrajectory[c].velocity);
            path.set_posleft(c, leftTrajectory[c].position);
            path.set_accelright(c, rightTrajectory[c].acceleration);
            path.set_velright(c, rightTrajectory[c].velocity);
            path.set_posright(c, rightTrajectory[c].position);
        }
        path.SerializeToString(&rep_str);
        response = zmq::message_t(rep_str.size());
        memcpy (response.data (), rep_str.c_str(), rep_str.size());
        std::cout << "Sending path data ..." << std::endl;
        socket.send (response);
    }
}
#pragma clang diagnostic pop