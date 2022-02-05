#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <regex>

struct CamInfo{
    std::string cameraName;
    std::string modelID;
    std::string vendorID;
    std::string deviceNumber;
};

FILE* GetDeviceList();
int GetTotalVideoDevices(FILE*);
void GetVideoDeviceDetails(std::vector<CamInfo> camVector);

int main(){
    FILE* deviceList;
    std::vector<CamInfo> camInfoVector;

    printf("Program Started...\n");
    deviceList = GetDeviceList();
    assert(GetTotalVideoDevices(deviceList) == 2);
    printf("Program Completed!\n");
    return 0;
}

FILE* GetDeviceList(){
    FILE* pFile;
    pFile = popen("ls /dev", "r");
    return pFile;
}

int GetTotalVideoDevices(FILE* deviceList){
    std::vector<std::string> folderNames;
    char s;
    std::string temp("");
    int totalVideoDevices = 0;

    while((s=fgetc(deviceList))!=EOF) {

      if(s == '\n'){
        if( std::regex_match(temp, std::regex("(.*)(video)(.*)"))){
            ++totalVideoDevices;
        }
        temp.clear();
      }

      else{
          
          temp.push_back(s);
      }
   }

   return totalVideoDevices;
}

void GetVideoDeviceDetails(std::vector<CamInfo> camVector){
    FILE* deviceList;
    int totalDevices;

    deviceList = GetDeviceList();
    totalDevices = GetTotalVideoDevices(deviceList);

    for(int i = 0; i < totalDevices; i++){
        
    }
}

