int i2cInit();
int i2cSetDev(int wr, int tries); 
int i2cWrite(int wr, int argcnt, ...);
char* i2cRead(int wr, int reg, int cnt);
int chars2int(char* s);
int chars2intLE(char* s);
void i2cStop();
