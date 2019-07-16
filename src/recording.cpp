#include "../include/recording.hpp"


static void gaze_data_callback(TobiiResearchGazeData* gaze_data, void* user_data) {
    memcpy(user_data, gaze_data, sizeof(*gaze_data));
}

Recording::Recording(std::string serial, std::string new_list_channels) {
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

    TobiiResearchStatus status = tobii_research_get_gaze_output_frequency(eyetracker, &initial_gaze_output_frequency);

    settingConfig(new_list_channels);
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

        chanels_ET.resize(1);

        for(int i = 0; i < 1; i++) {
            chanels_ET[i].resize(nb_channels);
            chanels_ET[i][0] = gaze_data.left_eye.gaze_point.position_on_display_area.x;

            if(sig_LEFT && !sig_RIGHT) {
                chanels_ET[i][0] = gaze_data.left_eye.gaze_point.position_on_display_area.x;
                chanels_ET[i][1] = gaze_data.left_eye.gaze_point.position_on_display_area.y;
            } else if(!sig_LEFT && sig_RIGHT) {
                chanels_ET[i][0] = gaze_data.right_eye.gaze_point.position_on_display_area.x;
                chanels_ET[i][1] = gaze_data.right_eye.gaze_point.position_on_display_area.y;
            } else {
                chanels_ET[i][0] = gaze_data.left_eye.gaze_point.position_on_display_area.x;
                chanels_ET[i][1] = gaze_data.left_eye.gaze_point.position_on_display_area.y;
                chanels_ET[i][2] = gaze_data.right_eye.gaze_point.position_on_display_area.x;
                chanels_ET[i][3] = gaze_data.right_eye.gaze_point.position_on_display_area.y;
            }
        }

        if(lsl_sharing)
            outlet_ET->push_chunk(chanels_ET);
    }
}

void Recording::setRecording(bool value) {
    recording = value;
}

void Recording::setLSLSharing(bool value) {
    lsl_sharing = value;
}


void Recording::setupLSLSharing(bool value) {
    if(value)
        outlet_ET = new lsl::stream_outlet(*info_ET);
    else
       delete outlet_ET;
}

void Recording::settingConfig(std::string new_list_channels) {
    boost::algorithm::split(list_channels, new_list_channels, boost::algorithm::is_any_of(", "), boost::algorithm::token_compress_on);

    sig_LEFT = false;
    sig_RIGHT = false;
    nb_channels = 0;

    for (auto it : list_channels) {
        switch(resolveSignal(it)) {
            case _LEFT:
                nb_channels += 2;
                sig_LEFT = true;
                
                break;
            case _RIGHT:
                nb_channels += 2;
                sig_RIGHT = true;
                
                break;
            case _NONE:
                std::cout << "ERROR" << std::endl;
                
                break;
        }
    }
    
    std::cout << "freq: " << initial_gaze_output_frequency << std::endl;
    info_ET = new lsl::stream_info(serial_number, "Eye_tracker", nb_channels, initial_gaze_output_frequency, lsl::cf_float32, boost::asio::ip::host_name());
}

Signal Recording::resolveSignal(std::string input) {
    if(input == "LEFT") return _LEFT;
    if(input == "RIGHT") return _RIGHT;
    
    return _NONE;
}