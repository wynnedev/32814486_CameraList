#include "camera_data_retriever.h"

int main(){
    std::vector<cam_info> cam_list;
    GetCamList(cam_list);

    for(auto& cam_info : cam_list){
        PrintCamInfo(cam_info);
    }

    printf("Program Completed!\n");
    return 0;
}
