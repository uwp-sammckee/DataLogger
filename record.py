from datetime import datetime
import argparse
import serial
import os
import re

# record.py
# 
# Will also record the data from the arduino
# Will also save to a .csv file
# 

parser = argparse.ArgumentParser(prog="Data recorder",
                                 description="This script uses Serial to record datalogged from our custom flight computer")

parser.add_argument('device', help="The Serial Device")

parser.add_argument('--baud', default=19200,
                    help='The baud rate for the Serial Connection')


packetSize = 16
baud = 0
device = ""
conn = None

fileFormat = "data/%m-%Y-%H-%M-data.csv"
searchPattern = fr"(-?\d+(\.\d+)?|NAN)(, (-?\d+(\.\d+)?|NAN)){packetSize-1}"
loggedData = []

def processData():
    file = open(datetime.now().strftime(fileFormat), "w")

    for datapoint in loggedData:
        datapoint = datapoint.replace("  ", " ")
        match = re.match(searchPattern, datapoint)

        if match:
            file.write(datapoint)

if __name__ == "__main__":
    args = parser.parse_args()

    device = args.device
    baud = args.baud

    conn = serial.Serial(port=device, baudrate=baud)

    if not os.path.exists("data/"):
        os.makedirs("data/")

    print("Serial Connection Open")
    while True:
        try:
            if conn.in_waiting:
                data = conn.readline().decode()

                print(data, end="")

                if "NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN, NAN"  in data:
                    break

                if "DATA DONE" in data:
                    break
                    
                loggedData.append(data)

        except KeyboardInterrupt:
            print("Quiting")
            break

    
    print("Done Processing Data")
    processData()