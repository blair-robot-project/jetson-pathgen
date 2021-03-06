//
// Created by noah on 11/24/17.
//
#include "../include/main.h"

double wheelbaseWidth = 2.168;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(){
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    string req_str;
    zmq::message_t request;
    string rep_str;
    zmq::message_t response;
    proto::PathRequest pathRequest = proto::PathRequest();
    proto::Path path;
    TrajectoryCandidate candidate;
    Segment *trajectory;
    int length;
    double deltaTime;

    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REP);

    cout << "Connecting to server" << endl;
    socket.connect ("tcp://10.4.49.2:5800");
    cout << "Connected to server!" << endl;

    while (true){
        path = proto::Path();
        socket.recv(&request);
        req_str = string(static_cast<char*>(request.data()), request.size());
        pathRequest.ParseFromString(req_str);
	cout << "Num waypoints: " << pathRequest.x_size() << endl;
	Waypoint points[pathRequest.x_size()+1];
	points[0] = {0, 0, 0};
        for (int c = 0; c < pathRequest.x_size(); c++) {
            points[c+1] = {pathRequest.x(c), pathRequest.y(c), pathRequest.theta(c)};
        }
        deltaTime = pathRequest.dt()/1000.;
        path.set_deltatime(deltaTime);
        pathfinder_prepare(points, pathRequest.x_size() + 1, FIT_HERMITE_CUBIC, PATHFINDER_SAMPLES_LOW, deltaTime, pathRequest.maxvel(),
                           pathRequest.maxaccel(), pathRequest.maxjerk(), &candidate);
        length = candidate.length;
        Segment leftTrajectory[length], rightTrajectory[length];
        trajectory = static_cast<Segment *>(malloc(length * sizeof(Segment)));
        pathfinder_generate(&candidate, trajectory);
        pathfinder_modify_tank(trajectory, length, leftTrajectory, rightTrajectory, wheelbaseWidth);
        for (int c = 0; c < length; c++){
            path.add_accelleft(leftTrajectory[c].acceleration);
            path.add_velleft(leftTrajectory[c].velocity);
            path.add_posleft(leftTrajectory[c].position);
            path.add_accelright(rightTrajectory[c].acceleration);
            path.add_velright(rightTrajectory[c].velocity);
            path.add_posright(rightTrajectory[c].position);
        }
        path.SerializeToString(&rep_str);
        response = zmq::message_t(rep_str.size());
        memcpy (response.data (), rep_str.c_str(), rep_str.size());
        socket.send (response);
    }
}
#pragma clang diagnostic pop
