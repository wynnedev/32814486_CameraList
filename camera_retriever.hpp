#ifndef camera_retriever_hpp
#define camera_retriever_hpp

#include <vector>
#include <string>
#include <stdio.h>

struct CamInfo{
    std::string cameraName;
    std::string modelID;
    std::string vendorID;
    int deviceNumber;
};

class CameraRetriever{
    
    public:
    explicit CameraRetriever(std::vector<CamInfo>& camList) : _camList(camList){};
    std::vector<CamInfo>& GetCamList();
    
    private:
    FILE* GetCameraDataFromOS();
    void ShowCameraData();
    void ParseCameraData();
    void UpdateCamList();
    FILE* _cameraData;
    std::vector<CamInfo> _camList;
};


#endif