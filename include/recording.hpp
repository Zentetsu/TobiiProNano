#ifndef RECORDING_HPP
#define RECORDING_HPP


#include <ctime>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <stdbool.h>
#include <inttypes.h>

#include <lsl_cpp.h>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include <tobii_research.h>
#include <tobii_research_streams.h>
#include <tobii_research_eyetracker.h>
#include <tobii_research_calibration.h>

#include "../include/signal_type.hpp"


class Recording {
    public:
        Recording(std::string serial, std::string new_list_channels);
        ~Recording();

        void getData();
        void recordData();
        void setRecording(bool value);
        void setLSLSharing(bool value);
        void setupLSLSharing(bool value);

        lsl::stream_info *info_ET;
        lsl::stream_outlet *outlet_ET;

      private:
        void settingConfig(std::string new_list_channels);
        Signal resolveSignal(std::string input);
    
        TobiiResearchEyeTrackers *eyetrackers;
		TobiiResearchEyeTracker *eyetracker;

        TobiiResearchGazeData gaze_data;

        char *address;
        char *serial_number;
        char *device_name;

        bool recording;
        bool lsl_sharing;
        bool sig_LEFT;
        bool sig_RIGHT;

        int nb_channels;

        std::vector<std::string> list_channels;
        std::vector<std::vector<float>> chanels_ET;
};

#endif