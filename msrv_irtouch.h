/**
 *@brief  hht-irtouch
 *@author yangqian@honghe-tech.com
 *@date   2017/12/01
 */
#ifndef MSRV_IRTOUCH_H
#define MSRV_IRTOUCH_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <linux/types.h>
#include <linux/input.h>  
#include <linux/uinput.h>  
#include <linux/hidraw.h>

/* According to earlier standards */
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>

#include <termios.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <string.h>
#include <ctime>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <iostream>
using namespace std;
#include "ffprotocol_info.h"
#include "commands_def.h"
#include "hid-tp-coords-ops.h"

#define CUSTOM_CALIBRATION {4096,0,0, 0,4123,-34}  
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed long long s64;
//************************************************************************************************
#define DBG_UART_PRT   1

#if DBG_UART_PRT

#define HHT_LOG_DEBUG(msg...)\
    do{\
        { \
            printf(msg);\
        } \
    }while(0)

#else
    #define HHT_LOG_DEBUG(fmt, ...)
#endif

#define DBG_UART_ERROR   1

#if DBG_UART_ERROR

#define HHT_LOG_ERROR(msg...)\
    do{\
        { \
            printf("\n>>{%s}>>>[%s()]>>>(%d);\n",__FILE__,__func__,__LINE__);\
            printf(msg);\
        } \
    }while(0)

#else
    #define HHT_LOG_ERROR(fmt, ...)
#endif

#if 0
#define DEFAULT_DEVICE   "/dev/ttyUSB0"
#define DEFAULT_BAUD_RATE   921600
#else
#define DEFAULT_DEVICE     "/dev/ttyS2"
#define DEFAULT_BAUD_RATE   115200
#endif

#define HID_TOUCH_DEVICE  "/dev/hidraw1"

#define UNUSED(expr) do { (void)(expr); } while (0)

//************************************************************************************************
static uint8_t ENABLE_HID_RAW_DATA[64] = {0xfc, 0x30, 0x01, 0x01, 0x00, 0x01};
//reply(Success): IRTouch --> Android: (64B) 0xfc 0xfe 0x81 0x02 0x00 0x30 0x01 ...
static uint8_t DISABLE_HID_RAW_DATA[64] = {0xfc, 0x30, 0x01, 0x01, 0x00, 0x00};
//reply(Success): IRTouch --> Android: (64B) 0xfc 0xfe 0x81 0x02 0x00 0x30 0x01 ...
static uint8_t ENABLE_HID_WITH_DATA[64] = {0xfc, 0x30, 0x06, 0x00, 0x00, 0x01};
//reply(Success): IRTouch --> Android: (64B) 0xfc 0xfe 0x81 0x02 0x00 0x30 0x06 ...
static uint8_t DISABLE_HID_WITH_DATA[64] = {0xfc, 0x30, 0x06, 0x00, 0x00,0x00};
//reply(Success): IRTouch --> Android: (64B) 0xfc 0xfe 0x81 0x02 0x00 0x30 0x06 ...
//************************************************************************************************
class MSRV_IRTOUCH
{
public:
    MSRV_IRTOUCH();
    ~MSRV_IRTOUCH();
    static  MSRV_IRTOUCH* GetInstance();
    static  void DestoryInstance();
    //公共函数接口
    char*convert_hex_to_str(uint8_t *pBuf, const int nLen,const bool isHex);
    void sleep_ms(unsigned int msec);
    int m_strlen(uint8_t *s);
    static void signal_handler(int sig);
    u8 DoCalibration(u16 *xvalue, u16 *yvalue);
    // 创建虚拟输入设备
    int create_virtual_input_device(void);
    //***********************************************************
    //termios操作
    void show_termios(const struct termios *s);

    int get_termios(int fd, struct termios *s);
    int set_termios(int fd, const struct termios *s);

    int baud_to_speed(int baud);
    int get_speed(const struct termios *s);
    int get_ispeed(const struct termios *s);
    int get_ospeed(const struct termios *s);
    int get_bsize(const struct termios *s);
    char get_parity(const struct termios *s);
    int get_stop(const struct termios *s);

    int speed_to_baud(int speed);
    int set_speed(struct termios *s, int speed);
    int set_ispeed(struct termios *s, int speed);
    int set_ospeed(struct termios *s, int speed);
    int set_bsize(struct termios *s, int bsize);
    int set_parity(struct termios *s, char parity);
    int set_stop(struct termios *s, int stop);
    int enable_read(struct termios *s);
    int disable_read(struct termios *s);
    int enable_flow_control(struct termios *s);
    int disable_flow_control(struct termios *s);
    //串口操作
    int uart_open(const char *path);
    int uart_config(int fd, int baud, char parity, int bsize, int stop);
    int uart_read(int fd, char *buf, int len);
    int uart_write(int fd, const char *data, int len);
    int uart_close(int fd);
    //******************************************************************
    //读写HID驱动设备
    int hid_device_open();
    void hid_device_close(int hid_device);
    int hid_device_write(int device_fd,uint8_t *cmd);
    int hid_device_read(int device_fd,uint8_t *rvbuf);
    //校验解析hid触摸数据
    int check_incoming_data(uint8_t* rvbuf,int len);
    void handle_incoming_data(uint8_t*rvbuf,int len);
    Report_touch_info *get_report_info(uint8_t *rvbuf,int len);
    void transfer_report_info_to_uart(Report_touch_info *info);
    //******************************************************************
    void write_sndbuf_to_uart(const char *Byte, int num);
    bool trans_hht_touch_w_h_pos(trans_point_data frame);
    //******************************************************************
    void report_info_to_android(Touch_point_info *info,bool *touch_down_flag);//上报触摸数据至Android

     //触摸输入
    #if 1
    //按键
    void send_mt_abs_touch_key_down_event(int pos_id, int xpos, int ypos);
    void send_mt_abs_touch_key_up_event(int pos_id, int xpos, int ypos);
    //手指
    void send_mt_abs_touch_figner_down_event(int pos_id, int xpos, int ypos);
    void send_mt_abs_touch_figner_up_event(int pos_id, int xpos, int ypos);
    //笔
    void send_mt_abs_touch_pen_down_event(int pos_id, int xpos, int ypos);
    void send_mt_abs_touch_pen_up_event(int pos_id,int xpos,int ypos);
	//STYLUS
    void send_mt_abs_stylus_down_event(int pos_id,int xpos,int ypos);
    void send_mt_abs_stylus_up_event(int pos_id,int xpos,int ypos);
    //橡皮擦
    void send_mt_abs_touch_rubber_down_event(int pos_id, int xpos, int ypos);
    void send_mt_abs_touch_rubber_up_event(int pos_id,int xpos,int ypos);

    void send_mt_abs_event(int pos_id, int abs_x, int abs_y);
    int report_key_event(int fd, unsigned short code, int pressed, timeval *time);
    int report_rel_event(int fd, unsigned short code, int value, timeval *time);
    int report_abs_event(int fd, unsigned short code, int value, timeval *time);
    int report_sync_event(int fd, int code, timeval *time);
#endif
    //外部调用启动接口
    void start();
private:
    static  MSRV_IRTOUCH *m_pInstance;
    static  void *Run(void*arg);
    int m_touch_panel_type;// 触摸面板类型
    int nseqlocal;//
    int nptlocal; //
    pthread_t m_pthread;
};

#endif // MSRV_IRTOUCH_H
