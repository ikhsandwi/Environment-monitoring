# module M-7084
# clear value address
from pymodbus.client.sync import ModbusSerialClient as ModbusClient

client = ModbusClient(method='rtu', port='COM8',stopbits=1, bytesize=8, parity='N',baudrate=9600, timeout=5)
client.connect()

clearsensor1 = client.write_coil(512,1,unit=3) #Register no 513 in datasheet size 1 words Device ID 3
print (clearsensor1)

clearsensor2 = client.write_coil(513,1,unit=3) #Register no 514 in datasheet size 1 words Device ID 3
print (clearsensor2)

clearsensor3 = client.write_coil(514,1,unit=3) #Register no 513 in datasheet size 1 words Device ID 3
print (clearsensor3)

clearsensor4 = client.write_coil(515,1,unit=3) #Register no 514 in datasheet size 1 words Device ID 3
print (clearsensor4)

clearsensor5 = client.write_coil(516,1,unit=3) #Register no 513 in datasheet size 1 words Device ID 3
print (clearsensor5)

clearsensor6 = client.write_coil(517,1,unit=3) #Register no 514 in datasheet size 1 words Device ID 3
print (clearsensor6)

clearsensor7 = client.write_coil(518,1,unit=3) #Register no 513 in datasheet size 1 words Device ID 3
print (clearsensor7)

clearsensor8 = client.write_coil(519,1,unit=3) #Register no 514 in datasheet size 1 words Device ID 3
print (clearsensor8)
client.close()
