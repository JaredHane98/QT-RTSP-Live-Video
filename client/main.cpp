
#include "MainWindow.hpp"
#include "GStreamerPipeline.hpp"
#include "TCPManager.hpp"
#include "StreamInterface.hpp"
#include <QApplication>




static void onPadAdded(GstElement* element, GstPad* pad, GstElement* data)
{
    // Link two Element with named pad
	GstPad *sink_pad = gst_element_get_static_pad (GST_ELEMENT(data), "sink");
	if(gst_pad_is_linked(sink_pad)) 
    {
		g_print("rtspsrc and depay are already linked. Ignoring\n");
		return;
	}
    gst_element_link_pads(element, gst_pad_get_name(pad), GST_ELEMENT(data), "sink");
}

static GstFlowReturn newFrame(GstElement* sink, StreamInterface* interface)
{
    GstSample *sample; GstMapInfo info;
    g_signal_emit_by_name (sink, "pull-sample", &sample);   // Retrieve the buffer 
    if(sample)
    {
        GstBuffer* image_buffer = gst_sample_get_buffer(sample);
        if(image_buffer)
           interface->updateImage(image_buffer);
        gst_sample_unref (sample);
        return GST_FLOW_OK;
    }
    return GST_FLOW_ERROR;
} 


void runSimplePipeline(const std::string& location, StreamInterface* interface)
{
    GStreamPipeline pipeline("RTSP_CLIENT");
    pipeline.addElement("rtspsrc","rtspsrc");
    pipeline.addElement("rtph264depay", "videodepay");
    pipeline.addElement("h264parse", "h264parse");
    pipeline.addElement("avdec_h264", "videodecode");
    pipeline.addElement("videoscale", "videoscale");
    pipeline.addElement("videorate", "videorate");
    pipeline.addElement("videoconvert", "videoconvert", "video/x-raw, format=(string)GRAY8");
    pipeline.addElement("appsink", "appsink");

    pipeline.setElementProperty("rtspsrc", "location", location.c_str());          // location of the stream
    pipeline.setElementProperty("rtspsrc", "protocols", GST_RTSP_LOWER_TRANS_UDP); // set the protocol
    pipeline.setElementProperty("rtspsrc", "latency", 0);                          // latency set to min

    pipeline.setElementProperty("appsink", "emit-signals", TRUE);

    pipeline.linkElementsByName({"videodepay", "h264parse", "videodecode", "videoscale", "videorate", "videoconvert", "appsink"}); 

    pipeline.setElementSignal("appsink", "new-sample", G_CALLBACK(newFrame), interface);
    pipeline.setElementSignal("rtspsrc", "pad-added", G_CALLBACK(onPadAdded), pipeline.getElementByName("videodepay"));
    
    pipeline.setPipelineState(GST_STATE_PLAYING);
    pipeline.runMainLoop(); 
}


void runGStreamer(const std::string& location, StreamInterface* interface)
{
    std::thread gstream_thread(runSimplePipeline, location, interface); 
    gstream_thread.detach(); 
}


Q_DECLARE_METATYPE(cv::Mat)

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qRegisterMetaType<cv::Mat>();

    StreamInterface interface;
    runGStreamer("rtsp://192.168.68.55:8554/test", &interface);
    TCPManagerClient::start("192.168.68.55", "1234");


    MainWindow window;
    window.startWorkerThread(&interface);
    window.show();
    return app.exec();
}



