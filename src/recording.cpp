#include "../include/recording.hpp"


Recording::Recording() {
    eyetrackers = NULL;
    TobiiResearchStatus result;
    result = tobii_research_find_all_eyetrackers(&eyetrackers);

    if(result != TOBII_RESEARCH_STATUS_OK)
        throw std::runtime_error("\nERROR: Finding tracker failed.");

    if((int)eyetrackers->count == 0)
        throw std::runtime_error("\nERROR: No tracker found.");
    else if ((int)eyetrackers->count > 1)
        std::cout << "WARNING: Too many device, the first one will be selected by default." << std::endl;
    else
        std::cout << "INFO: Found " << (int)eyetrackers->count << " Eye Trackers." << std::endl;

    eyetracker = eyetrackers->eyetrackers[0];
    char *address;
    char *serial_number;
    char *device_name;
    tobii_research_get_address(eyetracker, &address);
    tobii_research_get_serial_number(eyetracker, &serial_number);
    tobii_research_get_device_name(eyetracker, &device_name);
    std::cout << "INFO: " << address << '\t' << serial_number << '\t' << device_name << '\t' << std::endl;
    tobii_research_free_string(address);
    tobii_research_free_string(serial_number);
    tobii_research_free_string(device_name);
}

Recording::~Recording() {
    tobii_research_free_eyetrackers(eyetrackers);
}