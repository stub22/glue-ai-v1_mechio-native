#include <sstream>
#include <stdio.h>
#include <stdarg.h>
extern "C" {
    #include <mpsse.h>
}
#include "mechio/common/logger.h"
using namespace std;

struct mpsse_context *i2c = NULL;
stringstream err;

int i2cInit() {
    i2c = Open(0x0403, 0x6011, I2C, FOUR_HUNDRED_KHZ, MSB, IFACE_B, NULL);
    if(i2c == NULL || !i2c->open){
        err.str("");
        err << "Failed to initialize MPSSE: " << ErrorString(i2c);
        mechio::common::error(err.str());
        return -1;
    }
    return 0;
}

int i2cSetDev(int wr, int tries) 
{
    int i, ack;
    char* s = (char*) malloc(1);
    s[0] = (char)wr;
    for(i=0; i<3; i++)
    {
        Stop(i2c);
        Start(i2c);
        Write(i2c, s, 1);
        ack = GetAck(i2c);
        if (ack==0) break;
        if (i==2) {
            mechio::common::error("Error in i2cSetDev");
            return -1;
        }
    }
    free(s);
    return ack;
}

int i2cWrite(int wr, int argcnt, ...)          // yes I did!
{
    int i;
    va_list args;
    int ack;
    char *s = (char*) malloc(argcnt+1);
    s[0] = (char) wr;
    va_start(args, argcnt);
    for(i=0; i<argcnt; i++)
    {
        s[i+1] = (char) va_arg(args, int);
    }
    va_end(args);
    Start(i2c);
    Write(i2c, s, argcnt+1);
    free(s);
    return GetAck(i2c);
}

char* i2cRead(int wr, int reg, int cnt)
{
    int ack;
    bool err=false;
    char* s = (char*) malloc(2);
    char* ret;
    Start(i2c);
    s[0] = (char) wr;
    s[1] = (char) reg;
    Write(i2c, s, 2);
    ack = GetAck(i2c);
    if (ack) 
    {
        err=true;
        mechio::common::error("ERROR: i2cRead ack failure (1)");
    }
    Start(i2c);
    s[0] = (char) wr+1;
    Write(i2c, s, 1);
    ret = Read(i2c, cnt);
    ack = GetAck(i2c);
    if (ack)
    {
        err=true;
        mechio::common::error("ERROR: i2cRead ack failure (2)");
    }
    free(s);
    if (err)
    {
        return NULL;
    } else {
        return ret;
    }
}

int chars2int(char* s)
{
    unsigned int lsb, msb;
    int ret;
    msb = s[0] & 0xff;
    lsb = s[1] & 0xff;
    ret = (msb << 8) + lsb;
    if (ret > 0x8000)
        ret -= 0x10000;
    return ret;
}

int chars2intLE(char* s)
{
    unsigned int lsb, msb;
    int ret;
    lsb = s[0] & 0xff;
    msb = s[1] & 0xff;
    ret = (msb << 8) + lsb;
    if (ret > 0x8000)
        ret -= 0x10000;
    return ret;
}

void i2cStop()
{
    Stop(i2c);
}

