import serial
import binascii
import matplotlib.pyplot as plt

import random

ser = None

# 显示可用串口
def show_serial() -> str:
    sl = []
    for i in range(256):
        try:
            ser = serial.Serial('COM'+str(i))
            sl.append('COM'+str(i))
            ser.close()
        except serial.SerialException:
            pass
    return sl

# 读取串口数据 熵
def read_serial_e(ser: serial.Serial) -> bytes:
    data = b''
    i = 0
    # print('RAW: ',end='')
    data += ser.read(5)
    if data[-1:] == b'\0':
        # print(len(data),data)
        return data[:-1]

# 读取串口数据 电压
def read_serial_v(ser: serial.Serial) -> bytes:
    data = b''
    i = 0
    # print('RAW: ',end='')
    data += ser.read(9)
    if data[-1:] == b'\n':
        # print(len(data),data)
        return data[:-1]

def SendFrame(ser:serial.Serial, frame:bytes):
    ser.write(frame)



def plotBars(data:list):
    plt.clf()
    print([d/sum(data) for d in data])
    plt.bar(range(len(data)), [d/sum(data) for d in data])
    plt.title('Data lenth ='+str(sum(data)))
    plt.pause(0.001)

def plothistogram(data:list):
    plt.clf()
    plt.hist(data, bins=16)
    plt.title('Data lenth ='+str(len(data)))
    plt.pause(0.001)

# 读入处理数据e
def readAndSolve_e(ser:serial.Serial):
    sumData = [0]*16
    data = b''
    with open('Edata6.txt','ab') as f:
        while True:
            ser.write(b'1\n')
            data += read_serial_e(ser)
            if len(data) == 128*16:
                # data1 = binascii.b2a_hex(data).decode('utf-8')
                
                # for i in data1:
                #     sumData[int(i,16)] += 1
                # plotBars(sumData)
                f.write(data)
                data = b''

def readAndSolve_v(ser:serial.Serial):
    data_v = []
    with open('data_v.txt','wb') as f:
        for i in range(2**1024):
            ser.write(b'1\n')
            for _ in range(8):
                data_v.append(int(read_serial_v(ser).decode('utf-8'),16)/4095*3.3)
                # data_v.append(random.gauss(1.65,0.1))
            # f.write(data)
            if i % 100 == 0:
                plothistogram(data_v)

def readAndSolve_(ser:serial.Serial):
    data_v = []
    with open('data_.txt','wb') as f:
        for i in range(2**1024):
            ser.write(b'1\n')
            d = read_serial_v(ser).decode('utf-8')
            data_v.append(int(d,16) if d[0] == '0' else int(d,16)-2**32)
            if i % 100 == 0:
                plothistogram(data_v)
            # f.write(data)
            if i % 50 == 0:
                plothistogram(data_v)

# 退出函数
import atexit
@atexit.register
def exit_func():
    print('exit')
    ser.close()


def main():
    global ser
    plt.ion()
    sl = show_serial()
    port = 'COM' + input('找到串口：'+str(sl)+' 请输入串口号：COM') if len(sl) != 1 else sl[0]
    print('打开串口：'+port)
    baudrate = 115200
    ser = serial.Serial(port, baudrate, timeout=10)
    readAndSolve_e(ser)
    plt.ioff()
    plt.show()
    

if __name__ == '__main__':
    main()

