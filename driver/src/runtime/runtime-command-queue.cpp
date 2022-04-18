#include <iostream>
#include "icd.h"

CL_API_ENTRY cl_int CL_API_CALL
clRetainCommandQueue(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call: clRetainCommandQueue"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clReleaseCommandQueue(cl_command_queue command_queue) {
    std::cerr << "Unimplemented OpenCL API call: clReleaseCommandQueue"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL
clGetCommandQueueInfo(cl_command_queue command_queue,
                      cl_command_queue_info param_name,
                      size_t param_value_size,
                      void* param_value,
                      size_t* param_value_size_ret) {
    std::cerr << "Unimplemented OpenCL API call: clGetCommandQueueInfo"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_int CL_API_CALL clSetCommandQueueProperty() {
    std::cerr << "Unimplemented OpenCL API call: clSetCommandQueueProperty"
              << std::endl;
    return CL_INVALID_PLATFORM;
}

CL_API_ENTRY cl_command_queue CL_API_CALL
clCreateCommandQueue(cl_context context,
                     cl_device_id device,
                     cl_command_queue_properties properties,
                     cl_int* errcode_ret) {
    std::cerr << "Unimplemented OpenCL API call: clCreateCommandQueue"
              << std::endl;
    return nullptr;
}
