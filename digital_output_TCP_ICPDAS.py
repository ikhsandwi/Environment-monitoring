# module ET-7002
# control I/O
from pymodbus.client.sync import ModbusTcpClient

client = ModbusTcpClient('192.168.3.16',port=502, debug=True)
client.connect()

data1 = client.write_coil(0,False,unit=1)
print (data1)
data2 = client.write_coil(1,False,unit=1)
print (data2)
data3 = client.write_coil(2,False,unit=1)
print (data3)

counter1 = client.read_inputs(2,1,unit=2)
print(counter1.bits[0])
client.close()

