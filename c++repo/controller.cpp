#include <iostream>
#include <fstream>
#include <stdexcept>
#include <array>
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <thread>
#include "uWebSockets/App.h"

#include "WaypointMissionList.cpp"

struct PerSocketData {};
std::unordered_set<uWS::WebSocket<false, true, PerSocketData>*> clients;

const char* pipe_path = "/tmp/testPipe";

// Function to create a pipe if it doesn't exist
std::ifstream CreatePipe(const char* pipe_path) {
    if (access(pipe_path, F_OK) == -1) {
        if (mkfifo(pipe_path, 0666) != 0) {
            perror("mkfifo");
            throw std::runtime_error("Could not create or access pipe");
        }
    }
    return std::ifstream(pipe_path);
}

// Function to run the Python script
void run_python_script(const std::string& command) {
    system(command.c_str());
}


void sendToAllClients(const std::string& msg) {
    //std::lock_guard<std::mutex> lock(clientsMutex);
    for (auto* client : clients) {
        client->send(msg, uWS::OpCode::TEXT);
    }
}

int main(int argc, char* argv[]) {
    uWS::App a = uWS::App()
        .ws<PerSocketData>("/*", {
            .open = [](auto* ws) {
                clients.insert(ws);
                std::cout << "Connection opened" << std::endl;
            },
            .message = [](auto* ws, std::string_view message, uWS::OpCode opCode) {
                ws->send(message, opCode);  // Echo back
                sendToAllClients("Someone Joined");
            },
            .close = [](auto* ws, int /*code*/, std::string_view /*message*/) {
                std::cout << "Connection closed" << std::endl;
            }
        })
        .listen(9001, [](auto* token) {
            if (token) {
                std::cout << "Server listening on port 9001" << std::endl;
            } else {
                std::cout << "Failed to listen on port 9001" << std::endl;
            }
        })
        .run();

    

    std::string trajgenPyScript = argv[1];
    try {
        //std::string command = "python3 /home/wired/dev/trajgenpy/examples/coverage_on_queried_data.py " + std::string(pipe_path);// + " > /dev/null 2>&1"; //Supresses print and errors from trajgen
        std::string command = "python3 " + trajgenPyScript + " " + std::string(pipe_path) + " > /dev/null 2>&1"; //Supresses print and errors from trajgen
        // Step 1: Open the pipe for reading (this will block until Python writes to it)
        std::thread python_thread(run_python_script, command);
        
        std::ifstream pipe = CreatePipe(pipe_path); // This blocks until python starts writing to pipe

        std::string result;
        std::string line;

        int i = 0;
        while (std::getline(pipe, line)) {
            result += line + "\n";
        }
        python_thread.join();

        WaypointMissionList* missions = new WaypointMissionList(result);

        std::cout << missions->toString() << std::endl;

        // Join the thread to ensure the Python script finishes

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}