
typedef struct
{
	uint8_t UtcTime[6]; 
	uint8_t Status;   
	uint8_t Latitude[11];	
	uint8_t LatitudeNS; 
	uint8_t Longitude[12];	 
	uint8_t LongitudeEW;
	uint8_t Speed[6];	    
	uint8_t Azimuth[6];	
	uint8_t UtcDate[7]; 
	uint8_t Altitude[8];   
    uint8_t Hdop[5];
	uint8_t SatelliteNumS[3];
}GPSINFO;

typedef struct
{
	uint8_t SatelliteNumS; 
	uint8_t Status;  
	uint8_t Speed;	    
	uint8_t UTC_Time[7];  // UTC--->BTC
	uint8_t BTC_Time[7];  // UTC--->BTC
	uint8_t LatitudeNS; 
	uint8_t LongitudeEW;
	
	uint16_t Altitude;   
	uint16_t Azimuth;	  // 航向角
			
	double Latitude;	// 双精度
	double Longitude;	 
	
	uint32_t lat_upload; // 转换成度分秒
	uint32_t lon_upload; // 转换成度分秒

}GPSDATA;  // 上传数据

typedef struct
{
uint8_t sec;
uint8_t min;
uint8_t hour;
uint8_t day;
uint8_t mon;
uint8_t year;
uint8_t week;	
}TIME;  // 北京时间
