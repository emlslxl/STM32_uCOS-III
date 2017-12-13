#ifndef _BSP_GPS_H_
#include "bsp_gps.h"

//从buf里面得到第cx个逗号所在的位置
//返回值:0~0XFE,代表逗号所在位置的偏移.
//       0XFF,代表不存在第cx个逗号
u8 NMEA_Comma_Pos(u8 *buf, u8 cx)
{
    u8 *p = buf;
    while (cx)
    {
        if (*buf == '*' || *buf < ' ' || *buf > 'z')return 0XFF; //遇到'*'或者非法字符,则不存在第cx个逗号
        if (*buf == ',')cx--;
        buf++;
    }
    return buf - p;
}

void GPRMC_Parse(uint8_t *buff)
{
    uint16_t i = 0;
    uint16_t pos1, pos2;

    /* utc time */
    pos1 = NMEA_Comma_Pos(buff, 1);
    pos2 = NMEA_Comma_Pos(buff, 2);
    for (i = 0; i < 6; i++)
    {
        GpsInfo.UtcTime[i] = *(buff + pos1 + i);
    }
    UTC_Time.hour = (GpsInfo.UtcTime[0] - '0') * 10 + (GpsInfo.UtcTime[1] - '0');
    UTC_Time.min = (GpsInfo.UtcTime[2] - '0') * 10 + (GpsInfo.UtcTime[3] - '0');
    UTC_Time.sec = (GpsInfo.UtcTime[4] - '0') * 10 + (GpsInfo.UtcTime[5] - '0');

    /* fix status */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 3);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.Status = *(buff + pos1 + i);
    }
    if (GpsInfo.Status == 'A')
    {
        GpsData.Status = 'A'; // 有效定位
        LED_BLUE_ON;
    }
    else
    {
        GpsData.Status = 'V';
        LED_BLUE_OFF;
    }

    /* Latitude */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 4);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.Latitude[i] = *(buff + pos1 + i);
    }

    /* Latitude */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 5);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.LatitudeNS = *(buff + pos1 + i);
    }

    /* Longitude */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 6);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.Longitude[i] = *(buff + pos1 + i);
    }

    /* Longitude */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 7);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.LongitudeEW = *(buff + pos1 + i);
    }

    /* Speed */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 8);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.Speed[i] = *(buff + pos1 + i);
    }

    /* Azimuth */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 9);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.Azimuth[i] = *(buff + pos1 + i);
    }

    /* UTC Date */
    pos1 = pos2;
    pos2 = NMEA_Comma_Pos(buff, 10);
    for (i = 0; i < (pos2 - pos1 - 1); i++)
    {
        GpsInfo.UtcDate[i] = *(buff + pos1 + i);
    }
    UTC_Time.year = (GpsInfo.UtcDate[4] - '0') * 10 + (GpsInfo.UtcDate[5] - '0');
    UTC_Time.mon = (GpsInfo.UtcDate[2] - '0') * 10 + (GpsInfo.UtcDate[3] - '0');
    UTC_Time.day = (GpsInfo.UtcDate[0] - '0') * 10 + (GpsInfo.UtcDate[1] - '0');
}
#endif

