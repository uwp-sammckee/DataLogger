import argparse



if __name__ == "__main__":
    args = parser.parse_args()

    device = args.device
    baud = args.baud

    conn = serial.Serial(port=device, baudrate=baud)

    if not os.path.exists("data/"):
        os.makedirs("data/")

    print("Serial Connection Open")
    i = 0
    while True:
        try:
            if conn.in_waiting:
                data = conn.readline().decode()

                print(data, end="")

                if "NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN,  NAN"  in data:
                    if i == 1:
                        break
                    else: i += 1

                if "DATA DONE" in data:
                    break
                    
                loggedData.append(data)

        except KeyboardInterrupt:
            print("Quiting")
            break

    
    print("Done Processing Data")
    processData()