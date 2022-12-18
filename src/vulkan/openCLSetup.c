#define OPENCLERROR(x) if(_OPENCL_DATA.err < 0){printf(x);exit(1);}

#include <stdio.h>
#include <stdlib.h>
#include <MKTAppEngine.h>
#include <math.h>

#define KERNEL_FUNC "add_numbers"
#define ARRAY_SIZE 64

void _VE_OPENCL_CREATE_PROGRAM(char * filename)
{
    float data[ARRAY_SIZE];
    float sum[2], total, actual_sum;
    cl_mem input_buffer, sum_buffer;
    cl_int num_groups;
    cl_int i, j;

    for(i=0; i<ARRAY_SIZE; i++) {
        data[i] = 1.0f*i;
    }

    cl_context context;
    cl_program program;
    cl_kernel kernel;
    cl_command_queue queueForOpenCL;
    size_t local_size, global_size;

    context = clCreateContext(NULL, 1, &_OPENCL_DATA.device, NULL, NULL, &_OPENCL_DATA.err);

    OPENCLERROR("Couldn't create a context")

    FILE *program_handle;
    char *program_buffer, *program_log;
    size_t program_size, log_size;

    program_handle = fopen(filename, "r");

    OPENCLERROR("Couldn't find the program file");

    fseek(program_handle, 0, SEEK_END);
    program_size = ftell(program_handle);
    rewind(program_handle);
    program_buffer = (char*)malloc(program_size + 1);
    program_buffer[program_size] = '\0';
    fread(program_buffer, sizeof(char), program_size, program_handle);
    fclose(program_handle);

    program = clCreateProgramWithSource(context, 1, 
    (const char**)&program_buffer, &program_size, &_OPENCL_DATA.err);

    OPENCLERROR("Couldn't create the program")

    free(program_buffer);

    _OPENCL_DATA.err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);

    if(_OPENCL_DATA.err < 0) {
            clGetProgramBuildInfo(program, _OPENCL_DATA.device, CL_PROGRAM_BUILD_LOG, 
                    0, NULL, &log_size);
            program_log = (char*) malloc(log_size + 1);
            program_log[log_size] = '\0';
            clGetProgramBuildInfo(program, _OPENCL_DATA.device, CL_PROGRAM_BUILD_LOG, 
                    log_size + 1, program_log, NULL);

            printf("%s\n", program_log);

            free(program_log);
            exit(1);
    }

    global_size = 8;
    local_size = 4; 
    num_groups = global_size/local_size;
    input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY |
            CL_MEM_COPY_HOST_PTR, ARRAY_SIZE * sizeof(float), data, &_OPENCL_DATA.err); // <=====INPUT

    sum_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE |
            CL_MEM_COPY_HOST_PTR, num_groups * sizeof(float), sum, &_OPENCL_DATA.err); // <=====OUTPUT
    
    OPENCLERROR("Couldn't create a buffer")
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wdeprecated-declarations"
    queueForOpenCL = clCreateCommandQueue(context, _OPENCL_DATA.device, 0, &_OPENCL_DATA.err);
    #pragma GCC diagnostic pop


    OPENCLERROR("Couldn't create a command queue")

    kernel = clCreateKernel(program, KERNEL_FUNC, &_OPENCL_DATA.err);

    OPENCLERROR("Couldn't create a kernel")

    _OPENCL_DATA.err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &input_buffer);
    _OPENCL_DATA.err |= clSetKernelArg(kernel, 1, local_size * sizeof(float), NULL);
    _OPENCL_DATA.err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), &sum_buffer);

    OPENCLERROR("Couldn't create a kernel argument");

    _OPENCL_DATA.err = clEnqueueNDRangeKernel(queueForOpenCL, kernel, 1, NULL, &global_size, 
            &local_size, 0, NULL, NULL);

    OPENCLERROR("Couldn't enqueue the kernel")

    _OPENCL_DATA.err = clEnqueueReadBuffer(queueForOpenCL, sum_buffer, CL_TRUE, 0, 
            sizeof(sum), sum, 0, NULL, NULL);

    OPENCLERROR("Couldn't read the buffer") 
    
    total = 0.0f; 
    for(j=0; j<num_groups; j++) {
        total += sum[j];
    }
    actual_sum = 1.0f * ARRAY_SIZE/2*(ARRAY_SIZE-1);
    printf("Computed sum = %.1f.\n", total);
    if(fabs(total - actual_sum) > 0.01*fabs(actual_sum))
        printf("Check failed.\n");
    else
        printf("Check passed.\n");

    for(j=0; j<num_groups; j++)
        printf("done: %f\n",sum[j]);

    clReleaseKernel(kernel);
   clReleaseMemObject(sum_buffer);
   clReleaseMemObject(input_buffer);
   clReleaseCommandQueue(queueForOpenCL);
   clReleaseProgram(program);
   clReleaseContext(context);
}

void _VE_INIT_OPENCL()
{
    _OPENCL_DATA.err = clGetPlatformIDs(1, &_OPENCL_DATA.platform, NULL);

    OPENCLERROR("Couldn't identify a platform")

    _OPENCL_DATA.err = clGetDeviceIDs(_OPENCL_DATA.platform, CL_DEVICE_TYPE_GPU, 1, &_OPENCL_DATA.device, NULL);
    if(_OPENCL_DATA.err == CL_DEVICE_NOT_FOUND)
        _OPENCL_DATA.err = clGetDeviceIDs(_OPENCL_DATA.platform, CL_DEVICE_TYPE_CPU, 1, &_OPENCL_DATA.device, NULL);

    OPENCLERROR("Couldn't access any devices")
    return;
}