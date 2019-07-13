#include "../include/recording.hpp"


Recording::Recording(std::string serial) {
    eyetrackers = NULL;
    TobiiResearchStatus result;
    result = tobii_research_find_all_eyetrackers(&eyetrackers);

    if(result != TOBII_RESEARCH_STATUS_OK)
        throw std::runtime_error("\nERROR: Finding tracker failed.");

    if((int)eyetrackers->count == 0)
        throw std::runtime_error("\nERROR: No tracker found.");
    else if ((int)eyetrackers->count > 1)
        std::cout << "INFO: Some device are connected, the first one will be selected by default if the serial number isn't not correct." << std::endl;
    else
        std::cout << "INFO: Found " << (int)eyetrackers->count << " Eye Trackers." << std::endl;

    for (int i = 0; i < (int)eyetrackers->count; i++) {
        char *address;
        char *serial_number;
        char *device_name;

        tobii_research_get_address(eyetrackers->eyetrackers[i], &address);
        tobii_research_get_serial_number(eyetrackers->eyetrackers[i], &serial_number);
        tobii_research_get_device_name(eyetrackers->eyetrackers[i], &device_name);

        if(serial.compare(serial_number) == 0)
            eyetracker = eyetrackers->eyetrackers[i];

        std::cout << "INFO: " << address << '\t' << serial_number << '\t' << std::endl;

        tobii_research_free_string(address);
        tobii_research_free_string(serial_number);
        tobii_research_free_string(device_name);
    }
}

Recording::~Recording() {
    tobii_research_free_eyetrackers(eyetrackers);
}