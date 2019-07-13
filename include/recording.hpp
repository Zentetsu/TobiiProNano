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
        Recording(std::string serial);
        ~Recording();

    private:
        TobiiResearchEyeTrackers *eyetrackers;
		TobiiResearchEyeTracker *eyetracker;
};

#endif