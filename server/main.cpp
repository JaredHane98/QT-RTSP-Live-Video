#include "GStreamerPipeline.hpp"
#include "ModuleController.hpp"
#include "BQ76952.hpp"
#include "TCPManager.hpp"
#include <gst/rtsp-server/rtsp-server.h>
#include <gst/controller/gstinterpolationcontrolsource.h>
#include <gst/controller/gstdirectcontrolbinding.h>
#include <thread>
#include <chrono>






static GstElement* rtspFactoryCreateElement(GstRTSPMediaFactory * factory, const GstRTSPUrl * url)
{
    GStreamPipeline pipeline("JETSON_PIPELINE", false, false);

    pipeline.addElement("v4l2src", "v4l2src0", "video/x-raw, format=(string)GRAY8, width=(int)1280, height=(int)720, interlace-mode=(string)progressive, framerate=(fraction)30/1"); 
    pipeline.addElement("rsdeinterlace", "rsdeinterlace0");
    pipeline.addElement("videoconvert", "videoconvert0", "video/x-raw, format=(string)I420, width=(int)2560, height=(int)720, framerate=(fraction)30/1");
    pipeline.addElement("nvvidconv", "nvvidconv0", "video/x-raw(memory:NVMM), format=(string)I420, width=(int)2560, height=(int)720, framerate=(fraction)30/1"); 
    pipeline.addElement("nvv4l2h264enc", "nvv4l2h264enc0");
    pipeline.addElement("h264parse", "h264parse0");
    pipeline.addElement("rtph264pay", "pay0");


    pipeline.setElementProperty("v4l2src0", "device", "/dev/video1");
    pipeline.setElementProperty("nvv4l2h264enc0", "bitrate", 8000000);
    pipeline.setElementProperty("nvv4l2h264enc0", "control-rate", 0);
    pipeline.setElementProperty("nvv4l2h264enc0", "preset-level", 3);
    pipeline.setElementProperty("nvv4l2h264enc0", "profile", 2);
    pipeline.setElementProperty("nvv4l2h264enc0", "insert-vui", true);
    pipeline.setElementProperty("nvv4l2h264enc0", "maxperf-enable", true);

    pipeline.linkElementsByName({"v4l2src0", "rsdeinterlace0", "videoconvert0", "nvvidconv0", "nvv4l2h264enc0", "h264parse0", "pay0"});

    return pipeline.attachToBin();
}

G_DECLARE_FINAL_TYPE (RTSPFactory, rtsp_factory, RTSP, FACTORY, GstRTSPMediaFactory);

struct _RTSPFactory
{
  GstRTSPOnvifMediaFactory parent;
};

G_DEFINE_TYPE (RTSPFactory, rtsp_factory, GST_TYPE_RTSP_MEDIA_FACTORY);

static void rtsp_factory_init (RTSPFactory * factory)
{}

static void rtsp_factory_class_init (RTSPFactoryClass * klass)
{
    GstRTSPMediaFactoryClass *mf_class = GST_RTSP_MEDIA_FACTORY_CLASS (klass);
    mf_class->create_element = rtspFactoryCreateElement;
}

static GstRTSPMediaFactory *rtspFactoryNew (void)
{
    GstRTSPMediaFactory *result = GST_RTSP_MEDIA_FACTORY (g_object_new (rtsp_factory_get_type (), NULL));
    return result;
}

static gboolean timeout (GstRTSPServer * server)
{
    GstRTSPSessionPool *pool;
    pool = gst_rtsp_server_get_session_pool (server);
    gst_rtsp_session_pool_cleanup (pool);
    g_object_unref (pool);
    return TRUE;
}

static void runRTSPServer()
{
    GMainLoop *loop;
    GstRTSPServer *server;
    GstRTSPMountPoints *mounts;
    GstRTSPMediaFactory *factory;

    gst_init(NULL, NULL);

    loop = g_main_loop_new(NULL, FALSE);
    server = gst_rtsp_onvif_server_new (); // creating server instance
    mounts = gst_rtsp_server_get_mount_points(server); // Get the mount points         
    factory = rtspFactoryNew (); // attach the factory 

    gst_rtsp_media_factory_set_media_gtype (factory, GST_TYPE_RTSP_ONVIF_MEDIA); // media type 
    gst_rtsp_media_factory_set_shared (factory, TRUE); 
    gst_rtsp_media_factory_set_protocols(factory, GST_RTSP_LOWER_TRANS_UDP); // transmission protocol


    gst_rtsp_mount_points_add_factory (mounts, "/test", factory);    // attach the test factory to the /test url 

    g_object_unref (mounts);

    assert(gst_rtsp_server_attach (server, NULL) != 0);

    g_timeout_add_seconds (2, (GSourceFunc) timeout, server);
    g_print ("stream ready at rtsp://IP_ADDRESS:8554/test\n");
    g_main_loop_run (loop); 
}

void runI2C()
{
    using namespace std::chrono_literals;

    BQ76952 bms("/dev/i2c-0", 0x08);
    std::shared_ptr<Module> bms_module = ModuleController::createModule("BMS");
    const uint16_t fw_version = bms.getFWVersion();
    const uint16_t hw_version = bms.getHWVersion();
    const uint16_t device_number = bms.getDeviceNumber();
    
    bms_module->addOutputValue(&fw_version, sizeof(uint16_t), "FW_Version");
    bms_module->addOutputValue(&hw_version, sizeof(uint16_t), "HW_Version");
    bms_module->addOutputValue(&device_number, sizeof(uint16_t), "Device_Number");

    while(true)
    {
        const std::vector<uint16_t> voltages = bms.getAllVoltages();
        const uint16_t ts1_temperature = bms.getThermistorTemp(TS1_TEMP);
        const uint16_t ts2_temperature = bms.getThermistorTemp(TS2_TEMP);
        const uint16_t int_temperature = bms.getThermistorTemp(INT_TEMP);

        bms_module->addOutputValue(&voltages[0], sizeof(uint16_t), "Cell_Voltage1");
        bms_module->addOutputValue(&voltages[1], sizeof(uint16_t), "Cell_Voltage2");
        bms_module->addOutputValue(&voltages[2], sizeof(uint16_t), "Cell_Voltage3");
        bms_module->addOutputValue(&voltages[3], sizeof(uint16_t), "Cell_Voltage4");
        bms_module->addOutputValue(&voltages[4], sizeof(uint16_t), "Cell_Voltage5");
        bms_module->addOutputValue(&voltages[5], sizeof(uint16_t), "Cell_Voltage6");
        bms_module->addOutputValue(&voltages[6], sizeof(uint16_t), "Cell_Voltage7");
        bms_module->addOutputValue(&voltages[7], sizeof(uint16_t), "Cell_Voltage8");
        bms_module->addOutputValue(&voltages[8], sizeof(uint16_t), "Cell_Voltage9");
        bms_module->addOutputValue(&voltages[9], sizeof(uint16_t), "Cell_Voltage10");
        bms_module->addOutputValue(&voltages[10], sizeof(uint16_t), "Cell_Voltage11");
        bms_module->addOutputValue(&voltages[11], sizeof(uint16_t), "Cell_Voltage12");
        bms_module->addOutputValue(&voltages[12], sizeof(uint16_t), "Cell_Voltage13");
        bms_module->addOutputValue(&voltages[13], sizeof(uint16_t), "Cell_Voltage14");
        bms_module->addOutputValue(&voltages[14], sizeof(uint16_t), "Cell_Voltage15");
        bms_module->addOutputValue(&voltages[15], sizeof(uint16_t), "Cell_Voltage16");
        bms_module->addOutputValue(&voltages[16], sizeof(uint16_t), "Stack_Voltage");
        bms_module->addOutputValue(&voltages[17], sizeof(uint16_t), "Pack_Voltage");
        bms_module->addOutputValue(&voltages[18], sizeof(uint16_t), "LD_Pin_Voltage");

        bms_module->addOutputValue(&ts1_temperature, sizeof(uint16_t), "TS1 Temperature");
        bms_module->addOutputValue(&ts2_temperature, sizeof(uint16_t), "TS2 Temperature");
        bms_module->addOutputValue(&int_temperature, sizeof(uint16_t), "INT Temperature");

        std::this_thread::sleep_for(5s);
    }
}



int main()
{
    std::thread i2c_thread(runI2C);

    TCPManagerServer::start(1234);

    runRTSPServer();

    return 0;
}








