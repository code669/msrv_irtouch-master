/**
 *@brief  hht-irtouch
 *@author yangqian@honghe-tech.com
 *@date   2017/12/01
 */
#ifndef FFPROTOCOL_INFO
#define FFPROTOCOL_INFO

#include <string.h>
#include "stdint.h"

#define   MAX_TOUCH_POINT_COUNT   10 //最高支持10个点
#define   MAX_POINTS_PER_FRAME     6 //一帧6个点

typedef enum
{
    STATUS_NONE=0x00,//无触摸
    STATUS_HOVER=0x01,//悬空/无状态
    STATUS_UP=0x02,//抬起
    STATUS_DOWN=0x03,//按下
}INTERACTION_STATUS;//交互状态

typedef enum
{
    TYPE_TOUCH =0x01,//触摸事件
    TYPE_PEN   =0x02,//笔事件
}INTERACTION_TYPE;//交互类型

/*
 * 64 bytes per frame
02(report_id) 07(id) 01(status) 77 66(x) 55 44(y) 33 01(w) 44 02(h) 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
00 00 00 00 00 00 00 00 00 00 00 00 00 01(nr) 00 00
*/
typedef struct __attribute__((packed)) _Touch_point_info
{
       uint8_t  touch_status;  //down/up//none
       uint8_t  touch_id;
       uint16_t touch_xpos;
       uint16_t touch_ypos;
       uint16_t touch_width;
       uint16_t touch_height;
}Touch_point_info;

typedef struct __attribute__((packed)) _Pen_point_info
{
    uint8_t pen_status; //down/up
    uint8_t pen_id;
    uint16_t pen_xpos;
    uint16_t pen_ypos;
    uint16_t pen_width;
    uint16_t pen_height;
}Pen_point_info;

typedef struct __attribute__((packed)) _Report_touch_info
{
       uint8_t  report_id;//报文ID 0xfb
       Touch_point_info m_touch_points[MAX_POINTS_PER_FRAME];//触摸数据
       uint8_t  nr;       //有效触摸点个数
       uint8_t  reserved[2];
}Report_touch_info;

//混合类型
typedef struct __attribute__((packed)) _Interaction_touch_info
{

}Interaction_touch_info;


#endif // FFPROTOCOL_INFO

