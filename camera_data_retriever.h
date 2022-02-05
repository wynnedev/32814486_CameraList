#include <iostream>
#include <stdio.h>
#include <vector>
#include <assert.h>
#include <regex>

struct cam_info{
    std::string camera_name;
    std::string model_id;
    std::string vendor_id;
    std::string device_number;
};

struct V4L2_record{
    std::string name;
    std::string port1;
    std::string port2;
};

void GetCamList(std::vector<cam_info>&);
void ShowFileData(FILE* file);
FILE* GetDeviceRecord(V4L2_record& record);
std::vector<V4L2_record> ProcessV4L2Data(FILE*);
V4L2_record GetRecordFromString(std::vector<std::string> s);
cam_info GetCamInfo(FILE* camData);
FILE* GetV4L2Data();
void PrintCamInfo(cam_info&);


void GetCamList(std::vector<cam_info>& cam_list){
    FILE* pFile;
    FILE* deviceRecord;
    std::vector<V4L2_record> v4L2_records;
    V4L2_record v4L2_record;     
    
    printf("Program Started...\n");
    pFile = GetV4L2Data();
    v4L2_records = ProcessV4L2Data(pFile);

    for(auto &record : v4L2_records){
        deviceRecord = GetDeviceRecord(record);
        cam_list.push_back(GetCamInfo(deviceRecord));
    }
}

V4L2_record GetRecordFromString(std::vector<std::string> recordString){
    V4L2_record record;

    record.name = recordString[0].substr(0, recordString[0].find_first_of('('));
    record.port1 = recordString[1];
    record.port2 = recordString[2];

    return record;
}

std::vector<V4L2_record> ProcessV4L2Data(FILE* v4L2_data){
    std::vector<V4L2_record> record;
    std::vector<std::string> record_strings;
    std::string line;
    char s;

    while((s=fgetc(v4L2_data))!=EOF){
        if(s == '\n'){
            record_strings.push_back(line);
            line.clear();
            if(s = fgetc(v4L2_data) == '\n'){
                record.push_back(GetRecordFromString(record_strings));
            }

            else{
                ungetc(s , v4L2_data);
            }
        }

        else if(s == '\000') {
            //skip character
        }

        else{
            line.push_back(s);
        }   
    }
    return record;
}

FILE* GetDeviceRecord(V4L2_record& record){
    FILE* file = nullptr;
    char query[128];
    sprintf(query, "sudo udevadm info --query=all %s",record.port1.c_str());
    file = popen(query, "r");
    return file;
}

cam_info GetCamInfo(FILE* camData){
    cam_info camInfo;
    std::string line;
    char s;

    while((s=fgetc(camData))!=EOF){
        if(s == '\n'){
            unsigned long found = std::string::npos;
            found = line.find('=') + 1;
            if(line.find("ID_SERIAL_SHORT") != std::string::npos){
                camInfo.device_number = line.substr(found, line.length() - found);
            }

            else if(line.find("ID_MODEL_ID") != std::string::npos){
                camInfo.model_id = line.substr(found, line.length() - found); 
            }

            else if(line.find("ID_VENDOR_ID") != std::string::npos){
                camInfo.vendor_id = line.substr(found, line.length() - found);
            }

            else if(line.find("ID_V4L_PRODUCT") != std::string::npos){
                camInfo.camera_name = line.substr(found, line.length() - found);
            }
            line.clear();
        }

        else{
            line.push_back(s);
        }
    }
        return camInfo;
}

FILE* GetV4L2Data(){
    FILE* file;
    file = popen("v4l2-ctl --list-devices", "r");
    return file;
}

void ShowFileData(FILE* data){
    char s;
    while((s=fgetc(data))!=EOF){
        printf("%c", s);
    }
}

void PrintCamInfo(cam_info& camInfo){
    printf(
        "Camera Name: %s\nModel Id: %s\nVendor Id: %s\nDevice Number: %s\n",
        camInfo.camera_name.c_str(),
        camInfo.model_id.c_str(),
        camInfo.vendor_id.c_str(),
        camInfo.device_number.c_str());
}

