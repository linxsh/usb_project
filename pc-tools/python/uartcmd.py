#coding=utf-8
import serial

# commandd flag
CMD_MAGIC_FLAG = [0x66, 0x69, 0x6e, 0x67]
## command option
CMD_READ_OPT  = [0x00, 0x00, 0x00, 0x01]
CMD_WRITE_OPT = [0x00, 0x00, 0x00, 0x02]
CMD_ACK_OPT   = [0x00, 0x00, 0x00, 0x03]
CMD_READ1_OPT = [0x00, 0x00, 0x00, 0x04]##立刻读取数据
## command format
CMD_DATA_FMT = [0x00, 0x00, 0x00, 0x01]
CMD_CMD_FMT  = [0x00, 0x00, 0x00, 0x02]
## command length
CMD_LENGTH = 16

class UartCmd(object):
    def __init__(self, port, buand=115200):
        self.uart = serial.Serial(port, buand, timeout=0.1)

    def send_regs(self, reg):
        reg_len = [(len(reg)>>24)&0xff]+[(len(reg)>>16)&0xff]+[(len(reg)>>8)&0xff]+[(len(reg))&0xff]
        imsg = CMD_MAGIC_FLAG + CMD_WRITE_OPT + CMD_CMD_FMT + reg_len
        self.uart.write(imsg)
        omsg = list(self.uart.read(CMD_LENGTH))
        if (omsg[0:4] == CMD_MAGIC_FLAG):
            imsg = list(reg);
            self.uart.write(imsg)
            omsg = list(self.uart.read(CMD_LENGTH))
            if (omsg[0:4] == CMD_MAGIC_FLAG):
                return 0
            else:
                print "regs recv two step:"
                print omsg
                return -1
        print "regs recv one step:"
        print omsg
        return -1

    def recv_data(self, wait=1):
        imsg = CMD_MAGIC_FLAG + CMD_READ_OPT + CMD_CMD_FMT + [0x00, 0x00, 0x00, 0x00]
        self.uart.write(imsg)
        omsg = list(self.uart.read(CMD_LENGTH))
        if omsg[0:4] == CMD_MAGIC_FLAG:
            data_len = (omsg[12]<<24)+(omsg[13]<<16)+(omsg[14]<<8)+omsg[15]
            if data_len != 0:
                if wait :
                    imsg = CMD_MAGIC_FLAG + CMD_READ_OPT  + CMD_DATA_FMT + omsg[12:16]
                else :
                    imsg = CMD_MAGIC_FLAG + CMD_READ1_OPT + CMD_DATA_FMT + omsg[12:16]
                self.uart.write(imsg)
                omsg = list(self.uart.read(data_len))
                return omsg
            else:
                return []
        print "data recv one step:"
        print omsg
        return []

if __name__ == '__main__':
    cmd = UartCmd("/dev/ttyUSB0", 115200)
    reg = [1, 2, 3,4]
    cmd.send_regs(reg)
    cmd.recv_data()
