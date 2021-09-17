from bluepy import btle
import time
import re

btctl_file = "/tmp/btctl_output"

with open(btctl_file) as file:
    btctl_list = file.readlines()

MACs = []

for line in btctl_list:
    if "Device" in line:
        device = line
        if "calroom.MCP9808" in device:
            substrs = device.split(" ")
            for substr in substrs:
                if re.match("[0-9a-f]{2}([-:]?)[0-9a-f]{2}(\\1[0-9a-f]{2}){4}$", substr.lower()) and substr not in MACs:
                    MACs.append(substr)

for MAC in MACs:

    # MAC =  "69:4E:57:33:FD:38"
    SERVICE_UUID = 0x1800
    CHARACTERISTIC_UUID = 0xCCC1

    print("Connect to:" + MAC)
    dev = btle.Peripheral(MAC)
    print("\n--- dev ----------------------------")
    print(type(dev))
    print(dev)

    print("\n--- dev.services -------------------")
    for svc in dev.services:
        print(str(svc))
        
    print("\n------------------------------------")
    print("Get Serice By UUID: " + str(SERVICE_UUID))
    service_uuid = btle.UUID(SERVICE_UUID)
    service = dev.getServiceByUUID(service_uuid)

    print(service)
    print("\n--- service.getCharacteristics() ---")
    print(type(service.getCharacteristics()))
    print(service.getCharacteristics())

    #----------------------------------------------
    characteristics = dev.getCharacteristics()
    print("\n--- dev.getCharacteristics() -------")
    print(type(characteristics))
    print(characteristics)

    for char in characteristics:
        print("-----------------")
        print(type(char))
        print(char)
        print(char.uuid)
        if (char.uuid == CHARACTERISTIC_UUID):
            print("=== !CHARACTERISTIC_UUID matched! ===")
            nano33BLE_Char = char
            print(char)
            print(dir(char))
            #print(char.getDescriptors)
            #print(char.propNames)
            #print(char.properties)
            #print(type(char.read()))
            print(char.read())

if nano33BLE_Char != None:
    print("\nnano33BLE_Char found")
    print(nano33BLE_Char)
    print(int.from_bytes(nano33BLE_Char.read(), byteorder='little'))
else:
    print("\nnano33BLE_Char NOT found!")
#=============================================
dev.disconnect()
print("\n--- bye ---\n")