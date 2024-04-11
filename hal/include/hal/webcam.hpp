/***
 * Handles the stream of the webcam and pushes the data to a socket
 * Use ffmpeg to stream the data
*/

#ifndef _WEBCAM_HPP_
#define _WEBCAM_HPP_

#include "../../app/include/socket.hpp"

#define CLEAR(x) memset(&(x), 0, sizeof(x))
class Webcam {
  private:
    enum io_method {
        IO_METHOD_READ,
        IO_METHOD_MMAP,
        IO_METHOD_USERPTR,
    };

    struct buffer {
        void* start;
        size_t length;
    };
    const char* dev_name = "/dev/video0";
    enum io_method io = IO_METHOD_MMAP;
    int fd = -1;
    struct buffer* buffers;
    unsigned int n_buffers = 0;
    int out_buf = 0;
    int force_format = 0;
    long frame_count = 100;
    Socket* streamSocket;
    std::thread streamThread;
    bool isRunning = false;
    void openConnectionT(void);
    void closeConnectionT(void);
    void sendResponseT(const void* str, int size);
    void process_image(const void* p, int size);
    int read_frame(void);
    void mainloop(void);
    void open_device(void);
    void errno_exit(const char* s);
    int xioctl(int fh, int request, void* arg);
    void start_capturing(void);
    void stop_capturing(void);
    void uninit_device(void);
    void init_read(unsigned int buffer_size);
    void init_mmap(void);
    void init_userp(unsigned int buffer_size);
    void init_device(void);
    void close_device(void);

  public:
    void startStream(void);
    void stopStream(void);
};

#endif