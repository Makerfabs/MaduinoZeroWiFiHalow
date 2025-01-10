/*
Arduino IDE v1.8.19
Board: arduino_zero_native v1.8.2
Library: DHT sensor library v1.4.6
 */
#include <Arduino.h>
#include <DHT.h>

#define DHTPIN 17 //PA04
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
float h,t;

#define ss_id  "ap_test1"
#define pass_id  "123123"

#define SerialMon SerialUSB
#define SerialTX_AH Serial1

#define RESPONE_OK 1
#define RESPONE_ERROR 2

int8_t At_Response(uint32_t timeouts,String &data)
{
    int index = 0;
    uint32_t start_tick = millis();
    const char *r1 = "OK";
    const char *r2 = "ERROR";
    const char *r3 = "+CONNECTED";
    const char *r4 = "+DISCONNECT";

    do {
        while (SerialTX_AH.available() > 0) 
        {
            int a = SerialTX_AH.read();
            if (a < 0)
                continue; // Skip 0x00 bytes, just in case

            data.reserve(1024);
            data += static_cast<char>(a);

            if(data.endsWith(r1) || data.endsWith(r3))
            {
                index = RESPONE_OK;
                SerialMon.println(data.c_str());
                goto finish;
            } 
            else if(data.endsWith(r2) || data.endsWith(r4))
            {
                index = RESPONE_ERROR;
                SerialMon.println(data.c_str());
                goto finish;
            }
        }
    } while (millis() - start_tick < timeouts);

finish:
    return index;
}

void sendAT(String s)
{
    s = "AT" + s;
    SerialTX_AH.write(s.c_str());
}

String AT_reponse_data = "";
bool TX_AH_STA_Mode_init(void)
{
    int at_cnt = 0;

    sendAT("+SYSDBG=LMAC,0");
    if (At_Response(1000,AT_reponse_data) == RESPONE_OK)
        SerialMon.println("AT+SYSDBG SUCCEED");
    else
    {
        at_cnt++;
        SerialMon.println("AT+SYSDBG ERROR");
    }

    sendAT("+CHAN_LIST=8600,8680,8760");
    if (At_Response(1000,AT_reponse_data) == RESPONE_OK)
        SerialMon.println("AT+CHAN_LIST SUCCEED");
    else
    {
        at_cnt++;
        SerialMon.println("AT+CHAN_LIST ERROR");
    }

    sendAT("+BSS_BW=8");
    if (At_Response(1000,AT_reponse_data) == RESPONE_OK)
        SerialMon.println("AT+BSS_BW SUCCEED");
    else
    {
        at_cnt++;
        SerialMon.println("AT+BSS_BW FAILD");
    }

    sendAT("+SSID=hgic_ah_test");
    if (At_Response(1000,AT_reponse_data) == RESPONE_OK)
        SerialMon.println("AT+SSID SUCCEED");
    else
    {
        at_cnt++;
        SerialMon.println("AT+SSID FAILD");
    }

    sendAT("+KEYMGMT=NONE");
    if (At_Response(1000,AT_reponse_data) == RESPONE_OK)
        SerialMon.println("AT+KEYMGMT SUCCEED");
    else
    {
        at_cnt++;
        SerialMon.println("AT+KEYMGMT FAILD");
    }

    sendAT("+MODE=STA");
    if (At_Response(1000,AT_reponse_data) == RESPONE_OK)
        SerialMon.println("AT+MODE=SAT SUCCEED");
    else
    {
        at_cnt++;
        SerialMon.println("AT+MODE=STA FAILD");
    }

    return (at_cnt == 0);
}

void setup(void)
{
    SerialMon.begin(115200);
    SerialTX_AH.begin(115200);
    dht.begin();
    delay(1000);
    SerialMon.println("\nMaduino Zero halow_v1.0_STA Test Begin!\r\n");    
    TX_AH_STA_Mode_init();
}

uint32_t rssi_tick = 0;
bool tx_ah_conn_status = false;
char rssi_buf[16];
String recv_str = "";
char recv_data[128] = {0};

void loop(void)
{
    if (millis() - rssi_tick > 3000)
    {   
        rssi_tick = millis();
        
        h = dht.readHumidity();
        t = dht.readTemperature();
        
        String data;
        sendAT("+CONN_STATE");
        if (At_Response(1000,AT_reponse_data) == RESPONE_OK) 
        {
            tx_ah_conn_status = true;
        }
        else 
        {
            tx_ah_conn_status = false;
        }
        
        AT_reponse_data = "";
        
        if(tx_ah_conn_status) 
        {
            String rssi_data;
            sendAT("+RSSI=1");
            if (At_Response(1000, rssi_data) == RESPONE_OK) 
            {
                int startIndex = rssi_data.indexOf(':');
                int endIndex = rssi_data.lastIndexOf('\n');
                String substr = rssi_data.substring(startIndex + 1, endIndex);
                strcpy(rssi_buf, substr.c_str());
            }
        }
        String send_data = "11111100000000";
        send_data += "\nT=" + String(t, 2) + ", H=" + String(h, 2) + "\n";
        int len = send_data.length();
        String cmd = "+TXDATA=" + String(len);

        sendAT(cmd);
        if (At_Response(1000,AT_reponse_data) == RESPONE_OK) 
        {
            SerialTX_AH.write(send_data.c_str());
        }
    }
    delay(1);
}