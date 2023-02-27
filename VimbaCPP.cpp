/**
*
*	(c) 2022 - Lars Dittrich
*
**/

#include <iostream>

//Includes for OpenCV
#include<opencv2/opencv.hpp>

//Includes for Vimba
#include "VimbaCPP/Include/VimbaCPP.h"
#include "VimbaCPP/Include/VimbaSystem.h"

using namespace std;
using namespace cv;
using namespace AVT::VmbAPI;

Mat VimbaGrabPicture() {
    cout << "[Vimba] Starting Vimba SDK... " << endl;
    VimbaSystem& sys = VimbaSystem::GetInstance();
    VmbErrorType    err = sys.Startup();
    CameraPtrVector cameras;

    cout << "[Vimba] Found these cameras:" << endl;
    err = sys.GetCameras(cameras);
    for (int i = 0; i < cameras.size(); i++) {
        string name, model;
        cameras[i]->GetName(name);
        cameras[i]->GetModel(model);
        cout << std::to_string(i) + ": " + name + "; " + model << endl;
    }

    cout << "Choose Camera-ID: ";
    cin >> s;
    CameraPtr camera = cameras[0];
    FramePtr frame;
    VmbErrorType res;
    res = camera->Open(VmbAccessModeFull);

    // !!! This has to be linked to an GenICam-XML File created with Vimba Viewer
    res = camera->LoadCameraSettings("Mako.xml");


    if (res == VmbErrorSuccess) {
        res = camera->AcquireSingleImage(frame, 500);
        if (res == VmbErrorSuccess) {

            VmbUchar_t* buffer = NULL;
            VmbUint32_t nWidth = 0;
            VmbUint32_t nHeight = 0;

            res = frame->GetWidth(nWidth);
            res = frame->GetHeight(nHeight);
            res = frame->GetImage(buffer);

            image_bgr = Mat(nHeight, nWidth, CV_8UC1, buffer);

	    //Somehow you need to convert to RGB instead of BGR with OpenCV, if you record your picture in the Bayer-Colorspaces?
            cvtColor(image_bgr, image_bgr, COLOR_BayerRG2RGB);
            cout << "[Vimba] Succsess";
	    return image_bgr;
        }
        else {
            cout << "[Vimba] Error";
        }
    }
    else {
        cout << "[Vimba] Error";

    }
}
