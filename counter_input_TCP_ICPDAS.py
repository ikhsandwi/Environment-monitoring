# module ET-7002
from pymodbus.client.sync import ModbusTcpClient

client = ModbusTcpClient('192.168.255.1',port=502, debug=True)
client.connect()

TCP_count1 = client.write_coils (35, [True]*5, unit=1)
# dataCount1 = TCP_count1.registers[0]
print ('DI0',TCP_count1)
# TCP_count2 = client.read_input_registers(34, 1, unit=1)
# dataCount2 = TCP_count2.registers[0]
# print ('DI1',dataCount2)
# TCP_count3 = client.read_input_registers(36, 1, unit=1)
# dataCount3 = TCP_count3.registers[0]
# print ('DI2',dataCount3)
# TCP_count4 = client.read_input_registers(38, 1, unit=1)
# dataCount4 = TCP_count4.registers[0]
# print ('DI3',dataCount4)

client.close()

