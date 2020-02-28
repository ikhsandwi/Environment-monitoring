# module M-7084
from pymodbus.client.sync import ModbusSerialClient as ModbusClient

client = ModbusClient(method='rtu', port='COM4',stopbits=1, bytesize=8, parity='N',baudrate=9600, timeout=8)
client.connect()

sensor1 = client.read_holding_registers(0,1,unit=1)  
datasensor1 = sensor1.registers[0]
print ("Sensor1", datasensor1)

sensor2 = client.read_holding_registers(1,1,unit=1)  
datasensor2 = sensor2.registers[0]
print ("Sensor2 ", datasensor2)

client.close()

