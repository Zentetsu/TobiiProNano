#include "../include/recording.hpp"


static void gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data) {
    memcpy(user_data, gaze_data, sizeof(*gaze_data));
}

Recording::Recording(std::string serial) {
    recording = false;

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

    for (int i = (int)eyetrackers->count-1; i == 0; i++) {
        tobii_research_get_address(eyetrackers->eyetrackers[i], &address);
        tobii_research_get_serial_number(eyetrackers->eyetrackers[i], &serial_number);
        tobii_research_get_device_name(eyetrackers->eyetrackers[i], &device_name);

        if(serial.compare(serial_number) == 0 || i == 0) {
            eyetracker = eyetrackers->eyetrackers[i];
        } else {
            tobii_research_free_string(address);
            tobii_research_free_string(serial_number);
            tobii_research_free_string(device_name);
        }

        std::cout << "INFO: " << address << '\t' << serial_number << '\t' << std::endl;
    }
}

Recording::~Recording() {
    tobii_research_unsubscribe_from_gaze_data(eyetracker, gaze_data_callback);
    tobii_research_free_string(address);
    tobii_research_free_string(serial_number);
    tobii_research_free_string(device_name);
    tobii_research_free_eyetrackers(eyetrackers);
}

void Recording::getData() {
    TobiiResearchStatus status = tobii_research_subscribe_to_gaze_data(eyetracker, gaze_data_callback, &gaze_data);

    if (status != TOBII_RESEARCH_STATUS_OK)
        throw std::runtime_error("\nERROR: Unable to subscribe to the eye-tracker.");
}

void Recording::recordData() {
    recording = true;
    
    while(recording) {
        std::cout << "LEFT: x=" << gaze_data.left_eye.gaze_point.position_on_display_area.x << ", y=" << gaze_data.left_eye.gaze_point.position_on_display_area.y << "; RIGHT: x=" << gaze_data.right_eye.gaze_point.position_on_display_area.x << ", y=" << gaze_data.right_eye.gaze_point.position_on_display_area.y << std::endl;
        // printf("Right eye 3d gaze origin in user coordinates (%f, %f, %f)\n",
        //         gaze_data.right_eye.gaze_origin.position_in_user_coordinates.x,
        //         gaze_data.right_eye.gaze_origin.position_in_user_coordinates.y,
        //         gaze_data.right_eye.gaze_origin.position_in_user_coordinates.z);
    }
}

void Recording::setRecording(bool value) {
    recording = value;
}