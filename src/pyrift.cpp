#include "LibOVR/Include/OVR.h"

#ifdef _MSC_VER
#   define DLLEXPORT __declspec(dllexport) 
#else
#   define DLLEXPORT
#endif

extern "C" DLLEXPORT void initialize(void);
extern "C" DLLEXPORT void get_orientation(float *, float *, float *);
extern "C" DLLEXPORT void get_orientation_quaternion(float *, float *, float *, float *);

using namespace OVR;

Ptr<DeviceManager> pManager;
Ptr<HMDDevice> pHMD;
Ptr<SensorDevice> pSensor;
SensorFusion *pSFusion;

extern "C" {

    DLLEXPORT void initialize(void) {
        System::Init(Log::ConfigureDefaultLog(LogMask_None));    
        pManager = *DeviceManager::Create();
        pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice(); 
        pSensor = *pHMD->GetSensor();

        pSFusion = new SensorFusion();
        pSFusion->AttachToSensor(pSensor);
    }

    DLLEXPORT void get_orientation(float *yaw, float *pitch, float *roll) {
        Quatf hmdOrient = pSFusion->GetPredictedOrientation();
        hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>(yaw, pitch, roll);
    }

    DLLEXPORT void get_orientation_quaternion(float *x, float *y, float *z, float *w) {
        Quatf hmdOrient = pSFusion->GetPredictedOrientation();
        *x = hmdOrient.x;
        *y = hmdOrient.y;
        *z = hmdOrient.z;
        *w = hmdOrient.w;
    }

}