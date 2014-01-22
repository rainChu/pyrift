#include "LibOVR/Include/OVR.h"

#ifdef _MSC_VER
#   define DLLEXPORT __declspec(dllexport) 
#else
#   define DLLEXPORT
#endif

extern "C" DLLEXPORT void prepare( void );

extern "C" DLLEXPORT int initialize( void );
extern "C" DLLEXPORT int get_orientation( float *, float *, float * );
extern "C" DLLEXPORT int get_orientation_quaternion( float *, float *, float *, float * );

using namespace OVR;

Ptr<DeviceManager> manager;
Ptr<HMDDevice> hmd;
Ptr<SensorDevice> sensor;
SensorFusion *fusion;

enum ErrorCode
{
    SUCCESS         = 0, //
    NO_DEVICE       = 1, // Could not detect an Oculus Rift
    NOT_INITIALIZED = 2  // Device not initialized; call initialize() first.
};


extern "C"
{
    DLLEXPORT
    void prepare( void )
    {
        fusion = NULL;
    }


    DLLEXPORT
    int initialize( void )
    {
        System::Init( Log::ConfigureDefaultLog( LogMask_None ) );
        manager = *DeviceManager::Create();
        hmd = *manager->EnumerateDevices<HMDDevice>().CreateDevice();
        if ( !hmd )
            return ErrorCode::NO_DEVICE;

        sensor = *hmd->GetSensor();

        fusion = new SensorFusion();
        fusion->AttachToSensor( sensor );

        return SUCCESS;
    }


    DLLEXPORT
    int get_orientation( float *yaw, float *pitch, float *roll )
    {
        if ( !fusion )
            return NOT_INITIALIZED;

        Quatf hmdOrient = fusion->GetPredictedOrientation();
        hmdOrient.GetEulerAngles<Axis_Y, Axis_X, Axis_Z>( yaw, pitch, roll );

        return SUCCESS;
    }


    DLLEXPORT
    int get_orientation_quaternion( float *x, float *y, float *z, float *w )
    {
        if ( !fusion )
            return NOT_INITIALIZED;

        Quatf hmdOrient = fusion->GetPredictedOrientation();
        *x = hmdOrient.x;
        *y = hmdOrient.y;
        *z = hmdOrient.z;
        *w = hmdOrient.w;

        return SUCCESS;
    }
}