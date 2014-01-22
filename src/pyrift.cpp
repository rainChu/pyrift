#include "LibOVR/Include/OVR.h"

#ifdef _MSC_VER
#   define DLLEXPORT __declspec(dllexport) 
#else
#   define DLLEXPORT
#endif

extern "C" DLLEXPORT int initialize( void );
extern "C" DLLEXPORT void get_orientation( float *, float *, float * );
extern "C" DLLEXPORT void get_orientation_quaternion( float *, float *, float *, float * );

using namespace OVR;

Ptr<DeviceManager> manager;
Ptr<HMDDevice> hmd;
Ptr<SensorDevice> sensor;
SensorFusion *fusion = NULL;

extern "C"
{
    DLLEXPORT
    int initialize( void )
    {
        System::Init( Log::ConfigureDefaultLog( LogMask_None ) );
        manager = *DeviceManager::Create();
        hmd = *manager->EnumerateDevices<HMDDevice>().CreateDevice();
        if ( !hmd )
            return 1; // Could not find Oculus Rift

        sensor = *hmd->GetSensor();

        fusion = new SensorFusion();
        fusion->AttachToSensor( sensor );

        return 0;
    }


    DLLEXPORT
    void get_orientation( float *yaw, float *pitch, float *roll )
    {
        Quatf hmdOrient = fusion->GetPredictedOrientation();
        hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>( yaw, pitch, roll );
    }


    DLLEXPORT
    void get_orientation_quaternion( float *x, float *y, float *z, float *w )
    {
        Quatf hmdOrient = fusion->GetPredictedOrientation();
        *x = hmdOrient.x;
        *y = hmdOrient.y;
        *z = hmdOrient.z;
        *w = hmdOrient.w;
    }
}