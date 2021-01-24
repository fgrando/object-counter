#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include <string>
#include <iostream>
#include <INIReader.h>

using namespace std;

class DB{
    public:
        static DB& get() { static DB instance; return instance; }

        string captureSource;
        float captureResize;
        int captureBlur;

        int processTextX;
        int processTextY;
        int processThresholdAdaptBlob;
        int processDilationSize;
        int processDilationElement;
        int processErosionSize;
        int processErosionElement;
        int processThreshold;
        bool processThresholdVerbose;
        bool processBgSubVerbose;
        bool processErosionVerbose;
        bool processDilationVerbose;
        bool processBlurVerbose;
        string processSubtractor;
        string processErosion;
        string processDilation;
        bool mainVerbose;

        int detectionLifetimeMs;
        int detectionAgeToCheckDistanceFrames;
        int detectionPathDistancePx;
        int detectionMinPathLenToBeDetected;

    private:
        DB() { load("configs.ini"); }

        inline void load(string name)
        {
            INIReader reader(name);

            if (reader.ParseError() != 0) {
                std::cout << "failed to load " << name << "\n";
            }

            string SOURCE;

            SOURCE = "capture";
            captureSource = reader.Get(SOURCE, "source", "?");
            captureResize = reader.GetReal(SOURCE, "resize", -1);
            captureBlur   = reader.GetInteger(SOURCE, "blur", -1);

            SOURCE = "process";
            processTextX   = reader.GetInteger(SOURCE, "textX", -1);
            processTextY   = reader.GetInteger(SOURCE, "textY", -1);
            processThresholdAdaptBlob   = reader.GetInteger(SOURCE, "treshAdapBlob", -1);
            processDilationSize = reader.GetInteger(SOURCE, "dilationSize", -1);
            processDilationElement = reader.GetInteger(SOURCE, "dilationElem", -1);
            processErosionSize = reader.GetInteger(SOURCE, "erosionSize", -1);
            processErosionElement = reader.GetInteger(SOURCE, "erosionElem", -1);
            processThreshold = reader.GetInteger(SOURCE, "threshold", -1);
            processThresholdVerbose = reader.GetBoolean(SOURCE, "thresholdVerbose", true);
            processBgSubVerbose = reader.GetBoolean(SOURCE, "bgSubVerbose", true);
            processErosionVerbose = reader.GetBoolean(SOURCE, "erosionVerbose", true);
            processDilationVerbose = reader.GetBoolean(SOURCE, "dilationVerbose", true);
            processBlurVerbose = reader.GetBoolean(SOURCE, "blurVerbose", true);
            processSubtractor = reader.Get(SOURCE, "subtractor", "?");
            processErosion = reader.Get(SOURCE, "erosion", "?");
            processDilation = reader.Get(SOURCE, "dilation", "?");
            mainVerbose = reader.GetBoolean(SOURCE, "mainVerbose", true);

            SOURCE = "detection";
            detectionLifetimeMs = reader.GetInteger(SOURCE, "lifetimeMs", -1);
            detectionAgeToCheckDistanceFrames = reader.GetInteger(SOURCE, "ageToCheckDistanceFrames", -1);
            detectionPathDistancePx = reader.GetInteger(SOURCE, "pathDistancePx", -1);
            detectionMinPathLenToBeDetected = reader.GetInteger(SOURCE, "minPathLenToBeDetected", -1);


            std::cout   << "source " << captureSource << endl
                        << "resize " << captureResize << endl
                        << "blur " << captureBlur << endl
                        << "textX " << processTextX << endl
                        << "textY " << processTextY << endl
                        << "treshAdapBlob " << processThresholdAdaptBlob << endl
                        << "dilationSize " << processDilationSize << endl
                        << "dilationElem " << processDilationElement << endl
                        << "erosionSize " << processErosionSize << endl
                        << "erosionElem " << processErosionElement << endl
                        << "threshold " << processThreshold << endl
                        << "thresholdVerbose " << processThresholdVerbose << endl
                        << "bgSubVerbose " << processBgSubVerbose << endl
                        << "erosionVerbose " << processErosionVerbose << endl
                        << "dilationVerbose " << processDilationVerbose << endl
                        << "blurVerbose " << processBlurVerbose << endl
                        << "subtractor " << processSubtractor << endl
                        << "erosion " << processErosion << endl
                        << "dilation " << processDilation << endl
                        << "lifetimeMs " << detectionLifetimeMs << endl
                        << "ageToCheckDistanceFrames " << detectionAgeToCheckDistanceFrames << endl
                        << "pathDistancePx " << detectionPathDistancePx << endl
                        << "minPathLenToBeDetected " << detectionMinPathLenToBeDetected << endl
                        << "mainVerbose " << mainVerbose << endl
                    << endl;
        }
};

#endif